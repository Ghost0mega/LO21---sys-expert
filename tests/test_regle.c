#include <assert.h>
#include <stdio.h>
#include "regle.h"

static Proposition P(const char *n, int neg) { return proposition_make(n, neg); }

int main(void) {
    Regle r = regle_create();
    assert(regle_premise_is_empty(&r) == 1);
    assert(regle_has_conclusion(&r) == 0);

    regle_add_premise(&r, P("A", 0));
    assert(regle_premise_is_empty(&r) == 0);
    Proposition h;
    assert(regle_premise_head(&r, &h) == 1);
    assert(h.name && strcmp(h.name, "A") == 0 && h.negated == 0);
    Proposition a = P("A", 0);
    assert(regle_premise_contains_recursive(&r, &a) == 1);
    proposition_free(&a);

    regle_set_conclusion(&r, P("R1", 0));
    assert(regle_has_conclusion(&r) == 1);
    Proposition c = regle_get_conclusion(&r);
    assert(c.name && strcmp(c.name, "R1") == 0);

    Proposition a_rm = P("A", 0);
    assert(regle_remove_premise(&r, &a_rm) == 1);
    proposition_free(&a_rm);
    assert(regle_premise_is_empty(&r) == 1);

    regle_free(&r);
    printf("test_regle: OK\n");
    return 0;
}
