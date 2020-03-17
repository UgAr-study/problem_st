#include "C:\clineProjects\hw_prog\problem_lx\problem_lx.h"
#include "problem_st.h"

int test_dump() {
    struct lexem_t lxs[5] = {
            {BRACE, .lex.b = LBRAC}, {NUM, .lex.num = 2}, {OP, MUL}, {NUM, .lex.num = 2}, {BRACE, .lex.b = RBRAC}
    };

    struct lex_array_t larr = {lxs, 5, 5};
    dump_lexarray(larr);
    //printf ("\ntest_dump has successfully finished\n\n");
    return 1;
}

int main() {
    int res;
    struct lex_array_t larr;
    struct node_t* top;
    char inp[MAXLEN] = {0};

    res = scanf("%1023c", inp);
    assert(res == 1);

    larr = lex_string(inp);

    if (larr.lexems == NULL) {
        printf("ERROR\n");
        return 0;
    }

    top = build_syntax_tree (larr);
    printf ("result is: %d\n", calc_result(top));
    dump_lexarray(larr);

    free(larr.lexems);
    return 0;
}
