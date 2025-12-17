// Demo program building the example knowledge base and running inference
#include <stdio.h>
#include <stdlib.h>
#include "proposition.h"
#include "regle.h"
#include "bc.h"
#include "inference.h"
#include "print.h"

/**
 * Affiche les faits de la base.
 * @param bf Base de faits à afficher.
 * @return Aucun.
 */
static void print_facts(const BaseFaits *bf) {
  const ListProposition *lp = &bf->facts;
  const ListPropositionNode *cur = lp->head;
  printf("Faits connus:\n");
  while (cur) {
    const Proposition *p = &cur->value;
    printf(" - %s%s\n", p->negated ? "¬" : "", p->name);
    cur = cur->next;
  }
}

int main(int argc, char *argv[]) {
  (void)argc; (void)argv;

  BC bc = bc_create();

  // Nouvelles règles selon la spécification:
  // Entrées: A, B, C, D, E (présents comme faits initiaux)
  // R1: A et B et C => R1
  Regle r1 = regle_create();
  regle_add_premise(&r1, proposition_make("A", 0));
  regle_add_premise(&r1, proposition_make("B", 0));
  regle_add_premise(&r1, proposition_make("C", 0));
  regle_set_conclusion(&r1, proposition_make("R1", 0));
  bc_add_regle(&bc, r1);

  // R2: ¬C et D et E => R2
  Regle r2 = regle_create();
  regle_add_premise(&r2, proposition_make("C", 1));
  regle_add_premise(&r2, proposition_make("D", 0));
  regle_add_premise(&r2, proposition_make("E", 0));
  regle_set_conclusion(&r2, proposition_make("R2", 0));
  bc_add_regle(&bc, r2);

  // R3: A et C => R3
  Regle r3 = regle_create();
  regle_add_premise(&r3, proposition_make("A", 0));
  regle_add_premise(&r3, proposition_make("C", 0));
  regle_set_conclusion(&r3, proposition_make("R3", 0));
  bc_add_regle(&bc, r3);

  // R4: R1 et D et E => R4
  Regle r4 = regle_create();
  regle_add_premise(&r4, proposition_make("R1", 0));
  regle_add_premise(&r4, proposition_make("D", 0));
  regle_add_premise(&r4, proposition_make("E", 0));
  regle_set_conclusion(&r4, proposition_make("R4", 0));
  bc_add_regle(&bc, r4);

  // R5: ¬R3 et B => R5
  Regle r5 = regle_create();
  regle_add_premise(&r5, proposition_make("R3", 1));
  regle_add_premise(&r5, proposition_make("B", 0));
  regle_set_conclusion(&r5, proposition_make("R5", 0));
  bc_add_regle(&bc, r5);

  // Base de faits initiale: A, B, C, D, E
  BaseFaits bf = facts_create();
  facts_add(&bf, proposition_make("A", 0));
  facts_add(&bf, proposition_make("B", 0));
  facts_add(&bf, proposition_make("C", 0));
  facts_add(&bf, proposition_make("D", 0));
  facts_add(&bf, proposition_make("E", 0));

  printf("\nReprésentation ASCII de la base de connaissances:\n");
  bc_print_ascii(&bc);

  printf("Avant inférence:\n");
  print_facts(&bf);

  inference_forward_chain(&bc, &bf);

  printf("\nAprès inférence:\n");
  print_facts(&bf);

  // Cleanup
  bc_free(&bc);
  facts_free(&bf);
  return 0;
}
