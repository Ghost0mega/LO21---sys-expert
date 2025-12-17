#pragma once
#include "list_regle.h"

typedef struct BC {
    ListRegle regles;
} BC;

/**
 * Crée une base de connaissances vide.
 * @return Base de connaissances initialisée.
 */
BC bc_create();

/**
 * Libère une base de connaissances.
 * @param bc Pointeur vers la base.
 * @return Aucun.
 */
void bc_free(BC *bc);

/**
 * Ajoute une règle à la base (en queue).
 * @param bc Base de connaissances.
 * @param r Règle à ajouter.
 * @return Aucun.
 */
void bc_add_regle(BC *bc, Regle r);

/**
 * Accède à la règle en tête de la base.
 * @param bc Base de connaissances.
 * @param out Sortie: règle en tête.
 * @return 1 si succès, 0 sinon.
 */
int bc_head_regle(const BC *bc, Regle *out);

/**
 * Supprime la première règle dont la conclusion a le nom 'label'.
 * @param bc Base de connaissances.
 * @param label Nom de la conclusion de la règle à supprimer.
 * @return 1 si supprimée, 0 sinon.
 */
int bc_remove_rule_by_label(BC *bc, const char *label);
