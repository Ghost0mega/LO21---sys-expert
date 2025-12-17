#pragma once
#include "bc.h"
#include "list_proposition.h"

typedef struct BaseFaits {
    ListProposition facts;
} BaseFaits;

BaseFaits facts_create();
/**
 * Crée une base de faits vide.
 * @return Base de faits initialisée.
 */
void facts_free(BaseFaits *bf);
/**
 * Libère la base de faits.
 * @param bf Pointeur vers la base de faits.
 * @return Aucun.
 */
void facts_add(BaseFaits *bf, Proposition p);
/**
 * Ajoute un fait (proposition) à la base s'il n'existe pas déjà.
 * @param bf Base de faits.
 * @param p Proposition à ajouter.
 * @return Aucun.
 */
int facts_contains(const BaseFaits *bf, const Proposition *p);
/**
 * Teste si un fait appartient à la base.
 * @param bf Base de faits.
 * @param p Proposition recherchée.
 * @return 1 si trouvé, 0 sinon.
 */

void inference_forward_chain(const BC *bc, BaseFaits *bf);
/**
 * Moteur d'inférence par chaînage avant.
 * Ajoute des conclusions à la base de faits tant qu'il y a des changements.
 * @param bc Base de connaissances.
 * @param bf Base de faits (modifiée en place).
 * @return Aucun.
 */
