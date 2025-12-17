#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"
#include "inference.h"

typedef struct StrNode { char *s; struct StrNode *next; } StrNode;

static int strlist_contains(StrNode *h, const char *s) {
    for (; h; h = h->next) { if (strcmp(h->s, s) == 0) return 1; }
    return 0;
}
static void strlist_add_sorted_unique(StrNode **h, const char *s) {
    StrNode *prev = NULL, *cur = *h;
    while (cur && strcmp(cur->s, s) < 0) { prev = cur; cur = cur->next; }
    if (cur && strcmp(cur->s, s) == 0) return;
    StrNode *n = (StrNode*)malloc(sizeof(StrNode)); n->s = strdup(s); n->next = cur;
    if (prev) prev->next = n; else *h = n;
}
static void strlist_free(StrNode *h) { while (h) { StrNode *n = h->next; free(h->s); free(h); h = n; } }
static int strlist_len(StrNode *h) { int c=0; for (;h;h=h->next) c++; return c; }
static int strlist_maxlen(StrNode *h){ int m=0; for(;h;h=h->next){int L=(int)strlen(h->s); if(L>m)m=L;} return m;}

// Build variables: all premise names not appearing as any conclusion name
static void build_variables(const BC *bc, StrNode **vars_out) {
    StrNode *concls = NULL;
    for (const ListRegleNode *rn = bc->regles.head; rn; rn = rn->next) {
        if (regle_has_conclusion(&rn->value)) {
            Proposition c = regle_get_conclusion(&rn->value);
            strlist_add_sorted_unique(&concls, c.name);
        }
    }
    for (const ListRegleNode *rn = bc->regles.head; rn; rn = rn->next) {
        for (const ListPropositionNode *pn = rn->value.premises.head; pn; pn = pn->next) {
            if (!strlist_contains(concls, pn->value.name)) {
                strlist_add_sorted_unique(vars_out, pn->value.name);
            }
        }
    }
    strlist_free(concls);
}

// Rebuild derived facts from base toggles
static void rebuild_facts(const BC *bc, StrNode *vars, int *base_states, BaseFaits *out) {
    *out = facts_create();
    int idx = 0;
    for (StrNode *v = vars; v; v = v->next, ++idx) {
        if (base_states[idx]) facts_add(out, proposition_make(v->s, 0));
    }
    inference_forward_chain(bc, out);
}

// Check if a fact is true
static int facts_has_name(const BaseFaits *bf, const char *name) {
    Proposition p = proposition_make(name, 0);
    int has = facts_contains(bf, &p);
    proposition_free(&p);
    return has;
}

// Simple name->line map for building rule layout
typedef struct Map { const char *name; int line; struct Map *next;} Map;
static void map_set_local(Map **pm, const char*name,int line){ Map *n=(Map*)malloc(sizeof(Map)); n->name=name; n->line=line; n->next=*pm; *pm=n; }
static int map_get_local(Map *m,const char*name,int *line){ for(Map *x=m;x;x=x->next) if(strcmp(x->name,name)==0){*line=x->line; return 1;} return 0; }

// Draw ASCII similar to print.c but with highlighting for facts
static void draw_ascii(const BC *bc, StrNode *vars, const BaseFaits *facts, int cursor_row, int y_offset) {
    // Build name->line for variables (even lines)
    int var_count = strlist_len(vars);
    int total_lines = var_count ? (2*var_count - 1) : 0;
    int left_name_w = strlist_maxlen(vars);
    int col_names  = 2; // space after cursor '>'
    int left_w = col_names + left_name_w; // total left space before connectors

    // Map name->line arrays
    // To simplify, copy to arrays for O(1) index mapping
    char **var_names = (char**)malloc(sizeof(char*)*var_count);
    int i=0; for (StrNode *v=vars; v; v=v->next) var_names[i++]=v->s;

    // Precompute rules draw info (top,bottom,label line, premises lines)
    typedef struct PremI { int line; int neg; struct PremI *next; } PremI;
    typedef struct RuleI { const char *label; int top,bottom,label_line; PremI *p; struct RuleI *next; } RuleI;
    RuleI *rules=NULL, *rtail=NULL;

    // temp map for rule outputs to line (label lines), enable chaining
    Map *m=NULL;

    // Seed map with variables
    for (int k=0;k<var_count;++k) map_set_local(&m, var_names[k], 2*k);

    for (const ListRegleNode *rn = bc->regles.head; rn; rn = rn->next) {
        const Regle *r = &rn->value; int top= 1e9, bottom= -1e9; PremI *p=NULL,*pt=NULL;
        for (const ListPropositionNode *pn = r->premises.head; pn; pn = pn->next) {
            int line = -1;
            if (!map_get_local(m, pn->value.name, &line)) continue; // unknown yet
            PremI *pi=(PremI*)malloc(sizeof(PremI)); pi->line=line; pi->neg=pn->value.negated?1:0; pi->next=NULL; if(!p)p=pt=pi; else {pt->next=pi; pt=pi;}
            if (line<top) top=line;
            if (line>bottom) bottom=line;
        }
        int label_line;
        if (!p) { label_line = total_lines; if (label_line>=total_lines) total_lines=label_line+1; }
        else { label_line = (top+bottom)/2; if ((label_line%2)==0) label_line = (label_line+1<=bottom)?label_line+1:((top+1<=bottom)?top+1:top); if ((label_line%2)==0){ label_line=bottom+1; if(label_line>=total_lines) total_lines=label_line+1; } }
        RuleI *ri=(RuleI*)malloc(sizeof(RuleI)); ri->label = regle_has_conclusion(r)? regle_get_conclusion(r).name : ""; ri->top=top; ri->bottom=bottom; ri->label_line=label_line; ri->p=p; ri->next=NULL; if(!rules) rules=rtail=ri; else {rtail->next=ri; rtail=ri;}
        if (regle_has_conclusion(r)) map_set_local(&m, regle_get_conclusion(r).name, label_line);
    }

    // Draw rows
    for (int row=0; row<total_lines; ++row) {
        move(y_offset + row, 0);
        clrtoeol();
        // Cursor + variable names on even lines
        if (row % 2 == 0) {
            int vindex = row/2;
            // Cursor
            if (vindex == cursor_row) addch('>'); else addch(' ');
            addch(' ');
            // Name with highlight if fact true (use same highlighting as rules: reverse)
            const char *nm = var_names[vindex];
            int is_true = facts_has_name(facts, nm);
            if (is_true) attron(A_REVERSE);
            addnstr(nm, left_name_w);
            if ((int)strlen(nm) < left_name_w) for (int s=(int)strlen(nm); s<left_name_w; ++s) addch(' ');
            if (is_true) attroff(A_REVERSE);
        } else {
            // Non-variable separator line: just spaces up to left_w
            for (int s=0; s<left_w; ++s) addch(' ');
        }

        // For each rule column, draw connectors and labels
        int use_utf8 = (MB_CUR_MAX > 1);
        const char *ch_vert = use_utf8 ? "│" : "|";
        const char *ch_tee  = use_utf8 ? "┤" : "+";
        const char *ch_top  = use_utf8 ? "┐" : "+";
        const char *ch_bot  = use_utf8 ? "┘" : "+";

        for (RuleI *ri=rules; ri; ri=ri->next) {
            addch(' '); // spacer before connectors
            // connector cell: two chars width like print.c (leading space or '!') + glyph
            int is_segment = (ri->top>=0 && row>=ri->top && row<=ri->bottom);
            int is_prem = 0, neg = 0;
            if (is_segment) {
                for (PremI *pi=ri->p; pi; pi=pi->next) if (pi->line==row){ is_prem=1; neg=pi->neg; break; }
                addch(is_prem && neg ? '!' : ' ');
                const char *glyph = ch_vert;
                if (ri->top == ri->bottom) glyph = ch_tee;
                else if (row == ri->top) glyph = ch_top;
                else if (row == ri->bottom) glyph = ch_bot;
                else if (is_prem) glyph = ch_tee;
                addstr(glyph);
            } else {
                addstr("  ");
            }
            addch(' '); // space between connector and label
            // Label column
            int labw = (int)strlen(ri->label);
            if (row == ri->label_line && labw > 0) {
                int true_label = facts_has_name(facts, ri->label);
                if (true_label) attron(A_REVERSE);
                addstr(ri->label);
                if (true_label) attroff(A_REVERSE);
            } else {
                for (int s=0; s<labw; ++s) addch(' ');
            }
        }
    }

    free(var_names);
    // free RuleI structures
    for (RuleI *ri=rules; ri;) { RuleI *nx=ri->next; PremI *p=ri->p; while(p){PremI *pn=p->next; free(p); p=pn;} free(ri); ri=nx; }
    // free map
    for (Map *x=m; x;) { Map *nx=x->next; free(x); x=nx; }
}

// Simple start menu, returns 0: Base exemple, 1: Base personnalisée, 2: Quiter
static int show_start_menu(void) {
    const char *items[3] = { "Base exemple", "Base personalisée", "Quiter" };
    int n = 3;
    int sel = 0;
    while (1) {
        erase();
        attron(A_BOLD);
        mvprintw(0, 0, "Select a mode: ENTER confirm, ↑/↓ move, q to quit");
        attroff(A_BOLD);
        for (int i = 0; i < n; ++i) {
            move(2 + i, 0);
            clrtoeol();
            addch((i == sel) ? '>' : ' ');
            addch(' ');
            if (i == sel) attron(A_REVERSE);
            addstr(items[i]);
            if (i == sel) attroff(A_REVERSE);
        }
        refresh();
        int ch = getch();
        if (ch == 'q' || ch == 'Q') return 2;
        else if (ch == KEY_UP) { if (sel > 0) sel--; }
        else if (ch == KEY_DOWN) { if (sel < n - 1) sel++; }
        else if (ch == '\n' || ch == '\r' || ch == KEY_ENTER) { return sel; }
    }
}

void run_ui(const BC *bc) {
    setlocale(LC_ALL, "");
    // Init curses
    initscr(); cbreak(); noecho(); keypad(stdscr, TRUE); curs_set(0);

    while (1) {
        // Start menu loop
        int choice = show_start_menu();
        if (choice == 2) { endwin(); return; }

        // Build variable list
        StrNode *vars=NULL; build_variables(bc, &vars);
        int var_count = strlist_len(vars);
        int *base_states = (int*)calloc((size_t)var_count, sizeof(int));

        // Apply initial state based on menu
        if (choice == 0) {
            for (int i = 0; i < var_count; ++i) base_states[i] = 1; // Base exemple: all ON
        } else {
            for (int i = 0; i < var_count; ++i) base_states[i] = 0; // Base personnalisée: all OFF
        }

        // Initial derived facts
        BaseFaits facts = facts_create();
        rebuild_facts(bc, vars, base_states, &facts);

        int selected = 0; int ch;
        while (1) {
            erase();
            // Help header
            attron(A_BOLD);
            mvprintw(0, 0, "↑/↓ move  •  x toggle  •  q menu");
            attroff(A_BOLD);
            // Draw graph starting one line below header
            draw_ascii(bc, vars, &facts, selected, 1);
            refresh();
            ch = getch();
            if (ch == 'q' || ch == 'Q') break; // return to main menu
            else if (ch == KEY_UP) { if (selected>0) selected--; }
            else if (ch == KEY_DOWN) { if (selected < var_count-1) selected++; }
            else if (ch == 'x' || ch == 'X') {
                if (var_count>0) {
                    base_states[selected] = !base_states[selected];
                    facts_free(&facts);
                    rebuild_facts(bc, vars, base_states, &facts);
                }
            }
        }

        // Cleanup this session and return to menu
        facts_free(&facts);
        strlist_free(vars);
        free(base_states);
    }
}
