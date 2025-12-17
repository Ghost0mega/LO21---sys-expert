#include <stdlib.h>
#include "list_regle.h"

/**
 * Crée une liste de règles vide.
 * @return Liste initialisée, vide.
 */
ListRegle listr_create() {
    ListRegle l; l.head = l.tail = NULL; l.size = 0; return l;
}

/**
 * Libère une liste de règles et son contenu.
 * @param list Pointeur vers la liste à libérer.
 * @return Aucun.
 */
void listr_free(ListRegle *list) {
    if (!list) return;
    ListRegleNode *cur = list->head;
    while (cur) {
        ListRegleNode *next = cur->next;
        regle_free(&cur->value);
        free(cur);
        cur = next;
    }
    list->head = list->tail = NULL; list->size = 0;
}

/**
 * Indique si la liste de règles est vide.
 * @param list Liste à tester.
 * @return 1 si vide, 0 sinon.
 */
int listr_is_empty(const ListRegle *list) {
    return !list || list->size == 0;
}

/**
 * Ajoute une règle en queue de liste.
 * @param list Liste cible.
 * @param value Règle à ajouter (copiée par valeur).
 * @return Aucun.
 */
void listr_push_back(ListRegle *list, Regle value) {
    if (!list) return;
    ListRegleNode *node = (ListRegleNode*)malloc(sizeof(ListRegleNode));
    node->value = value;
    node->next = NULL;
    if (!list->tail) {
        list->head = list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
    list->size++;
}

/**
 * Récupère la règle en tête de liste.
 * @param list Liste cible.
 * @param out Sortie: règle en tête.
 * @return 1 si succès, 0 sinon.
 */
int listr_head(const ListRegle *list, Regle *out) {
    if (!list || !list->head || !out) return 0;
    *out = list->head->value;
    return 1;
}
