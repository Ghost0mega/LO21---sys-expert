#include <assert.h>
#include <stdio.h>
#include "list_proposition.h"

static Proposition P(const char *name, int neg) { return proposition_make(name, neg); }

int main(void) {
    ListProposition lp = listp_create();
    assert(listp_is_empty(&lp) == 1);

    Proposition a = P("A", 0);
    listp_push_back(&lp, a); // list owns a
    assert(listp_is_empty(&lp) == 0);
    Proposition head;
    assert(listp_head(&lp, &head) == 1);
    assert(head.name && strcmp(head.name, "A") == 0 && head.negated == 0);

    // contains
    Proposition a2 = P("A", 0);
    assert(listp_contains(&lp, &a2) == 1);
    proposition_free(&a2);

    // push more and remove first
    Proposition b = P("B", 0);
    Proposition ax = P("A", 1);
    listp_push_back(&lp, b);
    listp_push_back(&lp, ax);
    Proposition a_rm = P("A", 0);
    assert(listp_remove_first(&lp, &a_rm) == 1);
    proposition_free(&a_rm);

    // remove all by name
    listp_push_back(&lp, P("X", 0));
    listp_push_back(&lp, P("X", 1));
    listp_push_back(&lp, P("Y", 0));
    int removed = listp_remove_all_by_name(&lp, "X");
    assert(removed == 2);
    Proposition x = P("X", 0); assert(listp_contains(&lp, &x) == 0); proposition_free(&x);
    Proposition nx = P("X", 1); assert(listp_contains(&lp, &nx) == 0); proposition_free(&nx);

    listp_free(&lp);
    printf("test_list_proposition: OK\n");
    return 0;
}
