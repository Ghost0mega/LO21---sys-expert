#pragma once
#include "list_proposition.h"

typedef struct Regle {
    ListProposition premises; // list of propositions; tail-insertion
    int has_conclusion;
    Proposition conclusion;
} Regle;

Regle regle_create();
/**
 * Crée une règle vide (sans conclusion, prémisse vide).
 * @return Règle initialisée.
 */
void regle_free(Regle *r);
/**
 * Libère une règle (prémisse et conclusion éventuelle).
 * @param r Pointeur vers la règle.
 * @return Aucun.
 */
void regle_add_premise(Regle *r, Proposition p);
/**
 * Ajoute une proposition à la prémisse en queue.
 * @param r Règle cible.
 * @param p Proposition à ajouter.
 * @return Aucun.
 */
void regle_set_conclusion(Regle *r, Proposition p);
/**
 * Définit la conclusion de la règle.
 * @param r Règle cible.
 * @param p Proposition conclusion.
 * @return Aucun.
 */
int regle_premise_contains_recursive(const Regle *r, const Proposition *p);
/**
 * Teste récursivement l'appartenance d'une proposition à la prémisse.
 * @param r Règle à inspecter.
 * @param p Proposition recherchée.
 * @return 1 si trouvé, 0 sinon.
 */
int regle_remove_premise(Regle *r, const Proposition *p);
/**
 * Supprime la première occurrence d'une proposition de la prémisse.
 * @param r Règle cible.
 * @param p Proposition à supprimer.
 * @return 1 si supprimé, 0 sinon.
 */
int regle_premise_is_empty(const Regle *r);
/**
 * Vérifie si la prémisse d'une règle est vide.
 * @param r Règle cible.
 * @return 1 si vide, 0 sinon.
 */
int regle_premise_head(const Regle *r, Proposition *out);
/**
 * Accède à la proposition en tête de la prémisse.
 * @param r Règle cible.
 * @param out Sortie: proposition en tête.
 * @return 1 si succès, 0 sinon.
 */
int regle_has_conclusion(const Regle *r);
/**
 * Indique si une règle possède une conclusion.
 * @param r Règle cible.
 * @return 1 si oui, 0 sinon.
 */
Proposition regle_get_conclusion(const Regle *r);
/**
 * Récupère la conclusion d'une règle.
 * @param r Règle cible.
 * @return La proposition conclusion (valeur).
 */
