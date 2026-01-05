#include <assert.h>
#include <stdio.h>
#include "proposition.h"

int main(void) {
    Proposition a = proposition_make("A", 0);
    Proposition a2 = proposition_make("A", 0);
    Proposition not_a = proposition_make("A", 1);
    Proposition b = proposition_make("B", 0);

    assert(proposition_equals(&a, &a2) == 1);
    assert(proposition_equals(&a, &not_a) == 0);
    assert(proposition_equals(&a, &b) == 0);

    proposition_free(&a);
    proposition_free(&a2);
    proposition_free(&not_a);
    proposition_free(&b);

    printf("test_proposition: OK\n");
    return 0;
}
