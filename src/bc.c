#include "bc.h"

/**
 * Crée une base de connaissances vide.
 * @return Base de connaissances initialisée.
 */
BC bc_create() {
    BC bc; bc.regles = listr_create(); return bc;
}

/**
 * Libère une base de connaissances.
 * @param bc Pointeur vers la base.
 * @return Aucun.
 */
void bc_free(BC *bc) {
    if (!bc) return;
    listr_free(&bc->regles);
}

/**
 * Ajoute une règle à la base (en queue).
 * @param bc Base de connaissances.
 * @param r Règle à ajouter.
 * @return Aucun.
 */
void bc_add_regle(BC *bc, Regle r) {
    if (!bc) return;
    listr_push_back(&bc->regles, r);
}

/**
 * Accède à la règle en tête de la base.
 * @param bc Base de connaissances.
 * @param out Sortie: règle en tête.
 * @return 1 si succès, 0 sinon.
 */
int bc_head_regle(const BC *bc, Regle *out) {
    if (!bc) return 0;
    return listr_head(&bc->regles, out);
}

int bc_remove_rule_by_label(BC *bc, const char *label) {
    if (!bc || !label) return 0;
    ListRegleNode *prev = NULL, *cur = bc->regles.head;
    while (cur) {
        const Regle *r = &cur->value;
        if (regle_has_conclusion(r)) {
            Proposition c = regle_get_conclusion(r);
            if (c.name && strcmp(c.name, label) == 0) {
                ListRegleNode *next = cur->next;
                if (prev) prev->next = next; else bc->regles.head = next;
                if (cur == bc->regles.tail) bc->regles.tail = prev;
                regle_free(&cur->value);
                free(cur);
                bc->regles.size--;
                return 1;
            }
        }
        prev = cur; cur = cur->next;
    }
    return 0;
}
