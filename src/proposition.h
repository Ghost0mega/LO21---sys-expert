#pragma once
#include <stdlib.h>
#include <string.h>

typedef struct Proposition {
    char *name;
    int negated; // 0: false, 1: true (represents ¬)
} Proposition;

/**
 * Crée une proposition.
 * @param name Nom de la proposition.
 * @param negated 1 si négation (¬), 0 sinon.
 * @return Proposition initialisée.
 */
static inline Proposition proposition_make(const char *name, int negated) {
    Proposition p;
    p.name = (char*)malloc(strlen(name) + 1);
    strcpy(p.name, name);
    p.negated = negated ? 1 : 0;
    return p;
}

/**
 * Libère les ressources d'une proposition.
 * @param p Proposition à libérer.
 * @return Aucun.
 */
static inline void proposition_free(Proposition *p) {
    if (p && p->name) {
        free(p->name);
        p->name = NULL;
    }
}

/**
 * Compare deux propositions (nom et négation).
 * @param a Première proposition.
 * @param b Deuxième proposition.
 * @return 1 si égales, 0 sinon.
 */
static inline int proposition_equals(const Proposition *a, const Proposition *b) {
    if (!a || !b) return 0;
    if (a->negated != b->negated) return 0;
    if (!a->name || !b->name) return 0;
    return strcmp(a->name, b->name) == 0;
}
