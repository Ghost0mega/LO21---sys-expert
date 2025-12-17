#pragma once
#include "bc.h"
#include "list_proposition.h"

typedef struct BaseFaits {
    ListProposition facts;
} BaseFaits;

/**
 * Crée une base de faits vide.
 * @return Base de faits initialisée.
 */
BaseFaits facts_create();

/**
 * Libère la base de faits.
 * @param bf Pointeur vers la base de faits.
 * @return Aucun.
 */
void facts_free(BaseFaits *bf);

/**
 * Ajoute un fait (proposition) à la base s'il n'existe pas déjà.
 * @param bf Base de faits.
 * @param p Proposition à ajouter.
 * @return Aucun.
 */
void facts_add(BaseFaits *bf, Proposition p);

/**
 * Teste si un fait appartient à la base.
 * @param bf Base de faits.
 * @param p Proposition recherchée.
 * @return 1 si trouvé, 0 sinon.
 */
int facts_contains(const BaseFaits *bf, const Proposition *p);

/**
 * Moteur d'inférence par chaînage avant.
 * Ajoute des conclusions à la base de faits tant qu'il y a des changements.
 * @param bc Base de connaissances.
 * @param bf Base de faits (modifiée en place).
 * @return Aucun.
 */
void inference_forward_chain(const BC *bc, BaseFaits *bf);
