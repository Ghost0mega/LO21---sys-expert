#include <stdlib.h>
#include "list_proposition.h"

/**
 * Crée une liste de propositions vide.
 * @return Liste initialisée, vide.
 */
ListProposition listp_create() {
    ListProposition l; l.head = l.tail = NULL; l.size = 0; return l;
}

/**
 * Libère une liste de propositions et son contenu.
 * @param list Pointeur vers la liste à libérer.
 * @return Aucun.
 */
void listp_free(ListProposition *list) {
    if (!list) return;
    ListPropositionNode *cur = list->head;
    while (cur) {
        ListPropositionNode *next = cur->next;
        proposition_free(&cur->value);
        free(cur);
        cur = next;
    }
    list->head = list->tail = NULL; list->size = 0;
}

/**
 * Indique si la liste de propositions est vide.
 * @param list Liste à tester.
 * @return 1 si vide, 0 sinon.
 */
int listp_is_empty(const ListProposition *list) {
    return !list || list->size == 0;
}

/**
 * Ajoute une proposition en queue de liste.
 * @param list Liste dans laquelle ajouter.
 * @param value Proposition à ajouter (copiée par valeur).
 * @return Aucun.
 */
void listp_push_back(ListProposition *list, Proposition value) {
    if (!list) return;
    ListPropositionNode *node = (ListPropositionNode*)malloc(sizeof(ListPropositionNode));
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
 * Récupère la proposition en tête de liste.
 * @param list Liste cible.
 * @param out Sortie: proposition en tête.
 * @return 1 si succès, 0 sinon.
 */
int listp_head(const ListProposition *list, Proposition *out) {
    if (!list || !list->head || !out) return 0;
    *out = list->head->value;
    return 1;
}

/**
 * Supprime la première occurrence d'une proposition dans la liste.
 * @param list Liste cible.
 * @param value Proposition à supprimer (comparée par nom et négation).
 * @return 1 si supprimé, 0 si non trouvé.
 */
int listp_remove_first(ListProposition *list, const Proposition *value) {
    if (!list || !value) return 0;
    ListPropositionNode *prev = NULL, *cur = list->head;
    while (cur) {
        if (proposition_equals(&cur->value, value)) {
            if (prev) prev->next = cur->next; else list->head = cur->next;
            if (cur == list->tail) list->tail = prev;
            proposition_free(&cur->value);
            free(cur);
            list->size--;
            return 1;
        }
        prev = cur; cur = cur->next;
    }
    return 0;
}

/**
 * Test récursif d'appartenance d'une proposition dans une liste (à partir d'un nœud).
 * @param node Nœud de départ.
 * @param value Proposition recherchée.
 * @return 1 si trouvé, 0 sinon.
 */
int listp_contains_recursive(const ListPropositionNode *node, const Proposition *value) {
    if (!node) return 0;
    if (proposition_equals(&node->value, value)) return 1;
    return listp_contains_recursive(node->next, value);
}

/**
 * Teste si une proposition appartient à la liste.
 * @param list Liste cible.
 * @param value Proposition recherchée.
 * @return 1 si trouvé, 0 sinon.
 */
int listp_contains(const ListProposition *list, const Proposition *value) {
    if (!list) return 0;
    return listp_contains_recursive(list->head, value);
}

int listp_remove_all_by_name(ListProposition *list, const char *name) {
    if (!list || !name) return 0;
    int removed = 0;
    ListPropositionNode *prev = NULL, *cur = list->head;
    while (cur) {
        if (cur->value.name && strcmp(cur->value.name, name) == 0) {
            ListPropositionNode *next = cur->next;
            if (prev) prev->next = next; else list->head = next;
            if (cur == list->tail) list->tail = prev;
            proposition_free(&cur->value);
            free(cur);
            cur = next;
            list->size--;
            removed++;
            continue;
        }
        prev = cur; cur = cur->next;
    }
    return removed;
}
