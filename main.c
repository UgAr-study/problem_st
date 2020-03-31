#include "..\problem_lx\problem_lx.h"
#include "new_pr_st.h"

int main() {
    int res;
    struct lex_array_t larr;
    struct node_t* top;
    char inp[MAXLEN] = {0};

    res = scanf("%1023c", inp);
    if (res != 1) {
        printf ("ERROR: file is empty\n");
        exit (0);
    }

    top = build_syntax_tree_(inp);
    print_tree (top);

    printf ("\nresult is: %d\n", calc_result(top));


    free_syntax_tree (top);
    free(larr.lexems);
    return 0;
}

