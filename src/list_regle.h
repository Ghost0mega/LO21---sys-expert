#pragma once
#include "regle.h"

typedef struct ListRegleNode {
    Regle value;
    struct ListRegleNode *next;
} ListRegleNode;

typedef struct ListRegle {
    ListRegleNode *head;
    ListRegleNode *tail;
    size_t size;
} ListRegle;

/**
 * Crée une liste de règles vide.
 * @return Liste initialisée, vide.
 */
ListRegle listr_create();

/**
 * Libère une liste de règles et son contenu.
 * @param list Pointeur vers la liste à libérer.
 * @return Aucun.
 */
void listr_free(ListRegle *list);

/**
 * Indique si la liste de règles est vide.
 * @param list Liste à tester.
 * @return 1 si vide, 0 sinon.
 */
int listr_is_empty(const ListRegle *list);

/**
 * Ajoute une règle en queue de liste.
 * @param list Liste cible.
 * @param value Règle à ajouter (copiée par valeur).
 * @return Aucun.
 */
void listr_push_back(ListRegle *list, Regle value);

/**
 * Récupère la règle en tête de liste.
 * @param list Liste cible.
 * @param out Sortie: règle en tête.
 * @return 1 si succès, 0 sinon.
 */
int listr_head(const ListRegle *list, Regle *out);
