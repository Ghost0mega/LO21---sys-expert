#include <stdio.h>
#include "inference.h"

/**
 * Crée une base de faits vide.
 * @return Base de faits initialisée.
 */
BaseFaits facts_create() {
    BaseFaits bf; bf.facts = listp_create(); return bf;
}

/**
 * Libère la base de faits.
 * @param bf Pointeur vers la base de faits.
 * @return Aucun.
 */
void facts_free(BaseFaits *bf) {
    if (!bf) return;
    listp_free(&bf->facts);
}

/**
 * Ajoute un fait (proposition) à la base s'il n'existe pas déjà.
 * @param bf Base de faits.
 * @param p Proposition à ajouter.
 * @return Aucun.
 */
void facts_add(BaseFaits *bf, Proposition p) {
    if (!bf) return;
    if (!facts_contains(bf, &p)) {
        listp_push_back(&bf->facts, p);
    } else {
        // Already present; free duplicate
        proposition_free(&p);
    }
}

/**
 * Teste si un fait appartient à la base.
 * @param bf Base de faits.
 * @param p Proposition recherchée.
 * @return 1 si trouvé, 0 sinon.
 */
int facts_contains(const BaseFaits *bf, const Proposition *p) {
    if (!bf) return 0;
    return listp_contains(&bf->facts, p);
}

/**
 * Vérifie si la prémisse d'une règle est satisfaite par la base de faits.
 * @param r Règle cible.
 * @param bf Base de faits.
 * @return 1 si toutes les propositions de la prémisse sont présentes, 0 sinon.
 */
static int premises_satisfied(const Regle *r, const BaseFaits *bf) {
    const ListPropositionNode *cur = r->premises.head;
    while (cur) {
        const Proposition *p = &cur->value;
        if (!p->negated) {
            if (!facts_contains(bf, p)) return 0;
        } else {
            // Negated premise: satisfied if the positive counterpart is NOT present
            Proposition pos = proposition_make(p->name, 0);
            int pos_present = facts_contains(bf, &pos);
            proposition_free(&pos);
            if (pos_present) return 0;
        }
        cur = cur->next;
    }
    return 1;
}

/**
 * Moteur d'inférence par chaînage avant.
 * Ajoute des conclusions à la base de faits tant qu'il y a des changements.
 * @param bc Base de connaissances.
 * @param bf Base de faits (modifiée en place).
 * @return Aucun.
 */
void inference_forward_chain(const BC *bc, BaseFaits *bf) {
    if (!bc || !bf) return;
    int changed;
    do {
        changed = 0;
        const ListRegleNode *cur = bc->regles.head;
        while (cur) {
            const Regle *r = &cur->value;
            if (regle_has_conclusion(r) && premises_satisfied(r, bf)) {
                Proposition c = regle_get_conclusion(r);
                if (!facts_contains(bf, &c)) {
                    // copy to avoid freeing original from rule
                    Proposition nc = proposition_make(c.name, c.negated);
                    facts_add(bf, nc);
                    changed = 1;
                }
            }
            cur = cur->next;
        }
    } while (changed);
}
