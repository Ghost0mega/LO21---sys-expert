#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "print.h"

typedef struct NameLine {
    char *name;
    int line;
    struct NameLine *next;
} NameLine;

typedef struct PremInfo {
    int line;
    int neg;
    struct PremInfo *next;
} PremInfo;

typedef struct RuleDraw {
    const char *name;
    int name_len;
    PremInfo *premises;
    int top;
    int bottom;
    int label_line;
    struct RuleDraw *next;
} RuleDraw;

// Helper functions to append strings/spaces to lines
static void append_to_line(char **dst, int *len, const char *s) {
    int cur = *len;
    int add = (int)strlen(s);
    *dst = (char*)realloc(*dst, (size_t)(cur + add + 1));
    memcpy(*dst + cur, s, (size_t)add);
    (*dst)[cur + add] = '\0';
    *len = cur + add;
}

static void append_spaces(char **dst, int *len, int count) {
    if (count <= 0) return;
    int cur = *len;
    *dst = (char*)realloc(*dst, (size_t)(cur + count + 1));
    memset(*dst + cur, ' ', (size_t)count);
    (*dst)[cur + count] = '\0';
    *len = cur + count;
}

static int name_in_map(NameLine *map, const char *name, int *out_line) {
    for (; map; map = map->next) {
        if (strcmp(map->name, name) == 0) {
            if (out_line) *out_line = map->line;
            return 1;
        }
    }
    return 0;
}

static void name_map_set(NameLine **map, const char *name, int line) {
    NameLine *n = (NameLine*)malloc(sizeof(NameLine));
    n->name = (char*)malloc(strlen(name) + 1);
    strcpy(n->name, name);
    n->line = line;
    n->next = *map;
    *map = n;
}

static void name_map_free(NameLine *map) {
    while (map) {
        NameLine *next = map->next;
        free(map->name);
        free(map);
        map = next;
    }
}

static int nameset_contains(NameLine *set, const char *name) {
    return name_in_map(set, name, NULL);
}

static void nameset_add_sorted(NameLine **set, const char *name) {
    // Insert if not exists, keep lexicographic order
    NameLine *prev = NULL, *cur = *set;
    while (cur && strcmp(cur->name, name) < 0) { prev = cur; cur = cur->next; }
    if (cur && strcmp(cur->name, name) == 0) return; // already
    NameLine *n = (NameLine*)malloc(sizeof(NameLine));
    n->name = (char*)malloc(strlen(name) + 1);
    strcpy(n->name, name);
    n->line = -1;
    n->next = cur;
    if (prev) prev->next = n; else *set = n;
}

static int nameset_maxlen(NameLine *set) {
    int m = 0;
    for (; set; set = set->next) {
        int L = (int)strlen(set->name);
        if (L > m) m = L;
    }
    return m;
}

static int premises_contains_line(PremInfo *p, int line, int *neg_out) {
    for (; p; p = p->next) {
        if (p->line == line) { if (neg_out) *neg_out = p->neg; return 1; }
    }
    return 0;
}

void bc_print_ascii(const BC *bc) {
    if (!bc) return;

    // Collect rule conclusions set and max rule name length
    NameLine *conclusions = NULL;
    int max_rule_name_len = 0;
    for (const ListRegleNode *n = bc->regles.head; n; n = n->next) {
        if (regle_has_conclusion(&n->value)) {
            const Proposition c = regle_get_conclusion(&n->value);
            nameset_add_sorted(&conclusions, c.name); // order doesn't matter here
            int L = (int)strlen(c.name);
            if (L > max_rule_name_len) max_rule_name_len = L;
        }
    }

    // Collect variable names (premises that are not rule conclusions)
    NameLine *vars = NULL;
    for (const ListRegleNode *n = bc->regles.head; n; n = n->next) {
        const Regle *r = &n->value;
        for (const ListPropositionNode *p = r->premises.head; p; p = p->next) {
            const char *nm = p->value.name;
            if (!nameset_contains(conclusions, nm)) {
                nameset_add_sorted(&vars, nm);
            }
        }
    }
    if (!vars) return; // nothing to show

    // Assign line indices to variables (even indices)
    int var_count = 0;
    for (NameLine *v = vars; v; v = v->next) {
        v->line = 2 * var_count; // 0,2,4,...
        var_count++;
    }
    int total_lines = var_count > 0 ? (2 * var_count - 1) : 0; // includes interstitial lines

    // Prepare rule drawings pass: map for names to lines (start with variables)
    NameLine *name_to_line = NULL;
    for (NameLine *v = vars; v; v = v->next) name_map_set(&name_to_line, v->name, v->line);

    RuleDraw *rules = NULL, *rules_tail = NULL;

    for (const ListRegleNode *n = bc->regles.head; n; n = n->next) {
        const Regle *r = &n->value;
        // Build premises info using current mapping (variables and previous rules)
        PremInfo *prem = NULL, *prem_tail = NULL;
        int top = 1e9, bottom = -1e9;
        for (const ListPropositionNode *p = r->premises.head; p; p = p->next) {
            int line;
            if (!name_in_map(name_to_line, p->value.name, &line)) {
                // Unknown reference (rule defined later?) skip it
                continue;
            }
            PremInfo *pi = (PremInfo*)malloc(sizeof(PremInfo));
            pi->line = line;
            pi->neg = p->value.negated ? 1 : 0;
            pi->next = NULL;
            if (!prem) prem = prem_tail = pi; else { prem_tail->next = pi; prem_tail = pi; }
            if (line < top) top = line;
            if (line > bottom) bottom = line;
        }
        if (!prem) {
            // No mappable premises; still register rule name with label under last line
            int label_line = total_lines; // below last
            RuleDraw *rd = (RuleDraw*)malloc(sizeof(RuleDraw));
            rd->name = regle_has_conclusion(r) ? regle_get_conclusion(r).name : "";
            rd->name_len = (int)strlen(rd->name);
            rd->premises = NULL;
            rd->top = rd->bottom = -1;
            rd->label_line = label_line;
            rd->next = NULL;
            if (!rules) rules = rules_tail = rd; else { rules_tail->next = rd; rules_tail = rd; }
            // Extend lines if needed
            if (label_line >= total_lines) total_lines = label_line + 1;
            // Map rule output to this label line for later rules
            if (regle_has_conclusion(r)) name_map_set(&name_to_line, regle_get_conclusion(r).name, label_line);
            continue;
        }

        // Choose label line: nearest odd line between top and bottom
        int label_line = (top + bottom) / 2;
        if ((label_line % 2) == 0) {
            if (label_line + 1 <= bottom) label_line += 1;
            else if (label_line - 1 >= top) label_line -= 1;
        }
        if (label_line < top || label_line > bottom) label_line = top + 1 <= bottom ? top + 1 : top; // fallback
        if ((label_line % 2) == 0) {
            // no odd line in range (single-line case), add one line at bottom
            label_line = bottom + 1;
            if (label_line >= total_lines) total_lines = label_line + 1;
        }

        RuleDraw *rd = (RuleDraw*)malloc(sizeof(RuleDraw));
        rd->name = regle_has_conclusion(r) ? regle_get_conclusion(r).name : "";
        rd->name_len = (int)strlen(rd->name);
        rd->premises = prem;
        rd->top = top;
        rd->bottom = bottom;
        rd->label_line = label_line;
        rd->next = NULL;
        if (!rules) rules = rules_tail = rd; else { rules_tail->next = rd; rules_tail = rd; }

        // Map rule output name to its label line for downstream rules
        if (regle_has_conclusion(r)) name_map_set(&name_to_line, regle_get_conclusion(r).name, label_line);
    }

    // Build and print canvas
    int left_width = nameset_maxlen(vars);
    if (left_width < 1) left_width = 1;

    // Prepare lines
    char **lines = (char**)calloc((size_t)total_lines, sizeof(char*));
    int *line_len = (int*)calloc((size_t)total_lines, sizeof(int));
    for (int i = 0; i < total_lines; ++i) {
        int cap = left_width + 1; // initial capacity
        lines[i] = (char*)malloc((size_t)cap);
        lines[i][0] = '\0';
        line_len[i] = 0;
    }


    // Write left column (variable names)
    // Traverse vars in order and write names at even lines
    int var_index = 0;
    for (NameLine *v = vars; v; v = v->next, ++var_index) {
        int l = 2 * var_index;
        // ensure capacity
        int cap = left_width + 1;
        lines[l] = (char*)realloc(lines[l], (size_t)cap);
        memset(lines[l], ' ', (size_t)left_width);
        lines[l][left_width] = '\0';
        // place name left-aligned
        memcpy(lines[l], v->name, strlen(v->name));
        line_len[l] = left_width;
    }
    // Fill interstitial lines with spaces
    for (int l = 0; l < total_lines; ++l) {
        if (line_len[l] == 0) {
            lines[l] = (char*)realloc(lines[l], (size_t)(left_width + 1));
            memset(lines[l], ' ', (size_t)left_width);
            lines[l][left_width] = '\0';
            line_len[l] = left_width;
        }
    }

    // Helper to append to a line
    // Render each rule column
    for (RuleDraw *rd = rules; rd; rd = rd->next) {
        // For consistent spacing between columns
        for (int l = 0; l < total_lines; ++l) append_to_line(&lines[l], &line_len[l], " ");

        // For each line, draw connector
        for (int l = 0; l < total_lines; ++l) {
            const char *ch_vert = "│";
            const char *ch_tee  = "┤";
            const char *ch_top  = "┐";
            const char *ch_bot  = "┘";
            if (rd->top >= 0 && l >= rd->top && l <= rd->bottom) {
                int is_prem = 0, neg = 0;
                if (premises_contains_line(rd->premises, l, &neg)) is_prem = 1;
                const char *glyph = ch_vert;
                if (rd->top == rd->bottom) {
                    glyph = ch_tee;
                } else if (l == rd->top) {
                    glyph = ch_top;
                } else if (l == rd->bottom) {
                    glyph = ch_bot;
                } else if (is_prem) {
                    glyph = ch_tee;
                } else {
                    glyph = ch_vert;
                }
                if (is_prem && neg) {
                    append_to_line(&lines[l], &line_len[l], "!");
                } else {
                    append_to_line(&lines[l], &line_len[l], " ");
                }
                append_to_line(&lines[l], &line_len[l], glyph);
            } else {
                append_to_line(&lines[l], &line_len[l], "  ");
            }
        }

        // Space between connector and label
        for (int l = 0; l < total_lines; ++l) append_to_line(&lines[l], &line_len[l], " ");

        // Label column with rule name on its label line
        for (int l = 0; l < total_lines; ++l) {
            if (l == rd->label_line && rd->name && rd->name_len > 0) {
                append_to_line(&lines[l], &line_len[l], rd->name);
            } else {
                append_spaces(&lines[l], &line_len[l], rd->name_len);
            }
        }
    }

    // Print
    for (int l = 0; l < total_lines; ++l) {
        printf("%s\n", lines[l]);
        free(lines[l]);
    }
    free(lines);
    free(line_len);

    // Free helper structures
    for (RuleDraw *rd = rules; rd;) {
        RuleDraw *next = rd->next;
        PremInfo *p = rd->premises;
        while (p) { PremInfo *pn = p->next; free(p); p = pn; }
        free(rd);
        rd = next;
    }
    name_map_free(name_to_line);
    name_map_free(vars);
    name_map_free(conclusions);
}
