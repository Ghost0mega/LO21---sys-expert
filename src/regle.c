#include <stdlib.h>
#include "regle.h"

/**
 * Crée une règle vide (sans conclusion, prémisse vide).
 * @return Règle initialisée.
 */
Regle regle_create() {
    Regle r; r.premises = listp_create(); r.has_conclusion = 0; r.conclusion = proposition_make("", 0); proposition_free(&r.conclusion); return r;
}

/**
 * Libère une règle (prémisse et conclusion éventuelle).
 * @param r Pointeur vers la règle.
 * @return Aucun.
 */
void regle_free(Regle *r) {
    if (!r) return;
    listp_free(&r->premises);
    if (r->has_conclusion) proposition_free(&r->conclusion);
    r->has_conclusion = 0;
}

/**
 * Ajoute une proposition à la prémisse en queue.
 * @param r Règle cible.
 * @param p Proposition à ajouter.
 * @return Aucun.
 */
void regle_add_premise(Regle *r, Proposition p) {
    if (!r) return;
    listp_push_back(&r->premises, p);
}

/**
 * Définit la conclusion de la règle.
 * @param r Règle cible.
 * @param p Proposition conclusion.
 * @return Aucun.
 */
void regle_set_conclusion(Regle *r, Proposition p) {
    if (!r) return;
    if (r->has_conclusion) proposition_free(&r->conclusion);
    r->conclusion = p;
    r->has_conclusion = 1;
}

/**
 * Teste récursivement l'appartenance d'une proposition à la prémisse.
 * @param r Règle à inspecter.
 * @param p Proposition recherchée.
 * @return 1 si trouvé, 0 sinon.
 */
int regle_premise_contains_recursive(const Regle *r, const Proposition *p) {
    if (!r) return 0;
    return listp_contains_recursive(r->premises.head, p);
}

/**
 * Supprime la première occurrence d'une proposition de la prémisse.
 * @param r Règle cible.
 * @param p Proposition à supprimer.
 * @return 1 si supprimé, 0 sinon.
 */
int regle_remove_premise(Regle *r, const Proposition *p) {
    if (!r) return 0;
    return listp_remove_first(&r->premises, p);
}

/**
 * Vérifie si la prémisse d'une règle est vide.
 * @param r Règle cible.
 * @return 1 si vide, 0 sinon.
 */
int regle_premise_is_empty(const Regle *r) {
    if (!r) return 1;
    return listp_is_empty(&r->premises);
}

/**
 * Accède à la proposition en tête de la prémisse.
 * @param r Règle cible.
 * @param out Sortie: proposition en tête.
 * @return 1 si succès, 0 sinon.
 */
int regle_premise_head(const Regle *r, Proposition *out) {
    if (!r) return 0;
    return listp_head(&r->premises, out);
}

/**
 * Indique si une règle possède une conclusion.
 * @param r Règle cible.
 * @return 1 si oui, 0 sinon.
 */
int regle_has_conclusion(const Regle *r) {
    return r && r->has_conclusion;
}

/**
 * Récupère la conclusion d'une règle.
 * @param r Règle cible.
 * @return La proposition conclusion (valeur).
 */
Proposition regle_get_conclusion(const Regle *r) {
    if (!r || !r->has_conclusion) {
        return proposition_make("", 0);
    }
    return r->conclusion;
}

void regle_remove_premises_by_name(Regle *r, const char *name) {
    if (!r || !name) return;
    (void)listp_remove_all_by_name(&r->premises, name);
}
