#pragma once
#include "proposition.h"

typedef struct ListPropositionNode {
    Proposition value;
    struct ListPropositionNode *next;
} ListPropositionNode;

typedef struct ListProposition {
    ListPropositionNode *head;
    ListPropositionNode *tail;
    size_t size;
} ListProposition;

ListProposition listp_create();
/**
 * Crée une liste de propositions vide.
 * @return Liste initialisée, vide.
 */
void listp_free(ListProposition *list);
/**
 * Libère une liste de propositions et son contenu.
 * @param list Pointeur vers la liste à libérer.
 * @return Aucun.
 */
int listp_is_empty(const ListProposition *list);
/**
 * Indique si la liste de propositions est vide.
 * @param list Liste à tester.
 * @return 1 si vide, 0 sinon.
 */
void listp_push_back(ListProposition *list, Proposition value);
/**
 * Ajoute une proposition en queue de liste.
 * @param list Liste dans laquelle ajouter.
 * @param value Proposition à ajouter (copiée par valeur).
 * @return Aucun.
 */
int listp_head(const ListProposition *list, Proposition *out);
/**
 * Récupère la proposition en tête de liste.
 * @param list Liste cible.
 * @param out Sortie: proposition en tête.
 * @return 1 si succès, 0 sinon.
 */
int listp_remove_first(ListProposition *list, const Proposition *value);
/**
 * Supprime la première occurrence d'une proposition dans la liste.
 * @param list Liste cible.
 * @param value Proposition à supprimer (comparée par nom et négation).
 * @return 1 si supprimé, 0 si non trouvé.
 */
int listp_contains_recursive(const ListPropositionNode *node, const Proposition *value);
/**
 * Test récursif d'appartenance d'une proposition dans une liste (à partir d'un nœud).
 * @param node Nœud de départ.
 * @param value Proposition recherchée.
 * @return 1 si trouvé, 0 sinon.
 */
int listp_contains(const ListProposition *list, const Proposition *value);
/**
 * Teste si une proposition appartient à la liste.
 * @param list Liste cible.
 * @param value Proposition recherchée.
 * @return 1 si trouvé, 0 sinon.
 */
