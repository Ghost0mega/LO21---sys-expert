#include <assert.h>
#include <stdio.h>
#include "bc.h"

static Proposition P(const char *n, int neg) { return proposition_make(n, neg); }

int main(void) {
    BC bc = bc_create();
    Regle r1 = regle_create();
    regle_add_premise(&r1, P("A", 0));
    regle_set_conclusion(&r1, P("R1", 0));
    bc_add_regle(&bc, r1);

    Regle r2 = regle_create();
    regle_add_premise(&r2, P("B", 0));
    regle_set_conclusion(&r2, P("R2", 0));
    bc_add_regle(&bc, r2);

    Regle out;
    assert(bc_head_regle(&bc, &out) == 1);

    assert(bc_remove_rule_by_label(&bc, "R1") == 1);
    assert(bc_remove_rule_by_label(&bc, "R1") == 0);

    bc_free(&bc);
    printf("test_bc: OK\n");
    return 0;
}
