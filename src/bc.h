#pragma once
#include "list_regle.h"

typedef struct BC {
    ListRegle regles;
} BC;

BC bc_create();
/**
 * Crée une base de connaissances vide.
 * @return Base de connaissances initialisée.
 */
void bc_free(BC *bc);
/**
 * Libère une base de connaissances.
 * @param bc Pointeur vers la base.
 * @return Aucun.
 */
void bc_add_regle(BC *bc, Regle r);
/**
 * Ajoute une règle à la base (en queue).
 * @param bc Base de connaissances.
 * @param r Règle à ajouter.
 * @return Aucun.
 */
int bc_head_regle(const BC *bc, Regle *out);
/**
 * Accède à la règle en tête de la base.
 * @param bc Base de connaissances.
 * @param out Sortie: règle en tête.
 * @return 1 si succès, 0 sinon.
 */
