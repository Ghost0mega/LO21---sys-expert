#include <assert.h>
#include <stdio.h>
#include "inference.h"

static Proposition P(const char *n, int neg) { return proposition_make(n, neg); }

static int has(BaseFaits *bf, const char *name) {
    Proposition t = P(name, 0);
    int ok = facts_contains(bf, &t);
    proposition_free(&t);
    return ok;
}

int main(void) {
    // Build simple KB: A & B => R, R & C => S, !T & A => U
    BC bc = bc_create();

    Regle r1 = regle_create();
    regle_add_premise(&r1, P("A", 0));
    regle_add_premise(&r1, P("B", 0));
    regle_set_conclusion(&r1, P("R", 0));
    bc_add_regle(&bc, r1);

    Regle r2 = regle_create();
    regle_add_premise(&r2, P("R", 0));
    regle_add_premise(&r2, P("C", 0));
    regle_set_conclusion(&r2, P("S", 0));
    bc_add_regle(&bc, r2);

    Regle r3 = regle_create();
    regle_add_premise(&r3, P("T", 1));
    regle_add_premise(&r3, P("A", 0));
    regle_set_conclusion(&r3, P("U", 0));
    bc_add_regle(&bc, r3);

    BaseFaits bf = facts_create();
    facts_add(&bf, P("A", 0));
    facts_add(&bf, P("B", 0));
    facts_add(&bf, P("C", 0));

    inference_forward_chain(&bc, &bf);
    assert(has(&bf, "R") == 1);
    assert(has(&bf, "S") == 1);
    assert(has(&bf, "U") == 1); // since T is absent

    facts_free(&bf);
    bc_free(&bc);
    printf("test_inference: OK\n");
    return 0;
}
