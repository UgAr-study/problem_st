#include "new_pr_st.h"

// Grammar:
// expr ::= mult {+, -} expr | mult
// mult ::= term {*, /} mult | term
// term ::= ( expr ) | number

struct node_t* expr (const char* str, int* i);

struct node_t *create (struct lexem_t lex) {
    struct node_t* res;
    res = (struct node_t*) calloc (1, sizeof (struct node_t));
    if (res == NULL) {
        printf ("ERROR: calloc() couldn't provide memory\n");
        exit(0);
    }
    res->lex = lex;
    return res;
}

struct node_t* term (const char* str, int *i) {
#ifdef VISUALIZE
    printf("in term\n");
#endif
    struct node_t* term = NULL;
    struct lexem_t cur_lex;

    cur_lex = get_cur_lexem(str, i);
    if (cur_lex.kind == END)
        return term;

    if (cur_lex.kind == NUM) {
        term = create(cur_lex);
        return term;
    }

    if (is_left_brace(cur_lex, *i) == 1) {
        (*i)++;
        term = expr (str, i);
        (*i)++;
        cur_lex = get_cur_lexem(str, i);
        if (is_right_brace(cur_lex) == 0) {
            printf("ERROR: expected ')' on %d position", *i);
            exit(0);
        }
        return term;
    } else {
        printf ("Wrong format: expected number or brace on %d position\n", *i + 1);
        exit (1);
    }
}

struct node_t* mult (const char* str, int *i) {
#ifdef VISUALIZE
    printf ("in mult\n");
#endif

    struct node_t *multy, *m_left;
    struct lexem_t cur_lex;

    m_left = term (str, i);

#ifdef VISUALIZE
    printf ("M: i = %d\n", *i);
#endif

    if (m_left != NULL) {
        (*i)++;
        cur_lex = get_cur_lexem(str, i);

        while (is_mul_div(cur_lex, i) == 1) {
            multy = create(cur_lex);
            multy->left = m_left;
            (*i)++;
            multy->right = term (str, i);

            if (multy->right == NULL) {
                printf ("Wrong format: expected a number or expression on %d position", *i);
                exit(0);
            }

            m_left = multy;
            (*i)++;
            cur_lex = get_cur_lexem(str, i);
        }
    }

    return m_left;
}

struct node_t* expr (const char* str, int *i) {
#ifdef VISUALIZE
    printf ("in expr\n");
#endif

    struct node_t *expression, *e_left;
    struct lexem_t cur_lex;

    e_left = mult(str, i);

#ifdef VISUALIZE
    printf ("E: i = %d\n", *i);
#endif

    if (e_left != NULL) {
        (*i)++;
        cur_lex = get_cur_lexem(str, i);

        while (is_add_sub(cur_lex, i) == 1) {
            expression = create(cur_lex);
            expression->left = e_left;
            (*i)++;
            expression->right = mult (str, i);

            if (expression->right == NULL) {
                printf ("Wrong format: expected a number or expression on %d position", *i);
                exit(0);
            }

            e_left = expression;
            (*i)++;
            cur_lex = get_cur_lexem(str, i);
        }
    }

    return e_left;
}

struct node_t* build_syntax_tree_(const char* str) {
    struct node_t* tree;
    int i = 0;
    tree = expr (str, &i);

    if (tree == NULL) {
        printf ("Nothing to be calculated\n");
        exit (0);
    }

    return tree;
}

int calc_result(struct node_t *top) {
    int result, l, r;
    assert (top != NULL);
    //left
    //right
    //visit
    if (top->lex.kind == NUM)
        return top->lex.lex.num;

    l = calc_result (top->left);
    r = calc_result (top->right);
    switch (top->lex.lex.op) {
        case ADD:
            result = l + r;
            printf ("%d + %d   ", l, r);
            break;
        case SUB:
            result = l - r;
            printf ("%d - %d   ", l, r);
            break;
        case MUL:
            result = l * r;
            printf ("%d * %d   ", l, r);
            break;
        case DIV:
            result = l / r;
            printf ("%d / %d   ", l, r);
            break;
        default:
            printf ("Operation is not found, sorry (\n");
            exit (1);
    }
    return result;
}

void print_node (struct lexem_t lex) {
    switch (lex.kind) {
        case OP:
            switch (lex.lex.op) {
                case ADD:
                    printf ("ADD ");
                    break;
                case SUB:
                    printf ("SUB ");
                    break;
                case MUL:
                    printf ("MUL ");
                    break;
                case DIV:
                    printf ("DIV ");
                    break;
                default:
                    exit(1);
            }
            break;
        case NUM:
            printf ("%d ", lex.lex.num);
            break;
        default:
            exit(1);
    }
}

void print_tree (struct node_t* top) {
    if (top == NULL){
        printf ("Error: top is NULL\n");
        return;
    }
    if (top->left == NULL && top->right == NULL)
        return;

    print_node (top->lex);

    if (top->left == NULL) {
        printf ("\n");
        return;
    }
    print_node (top->left->lex);

    if (top->right == NULL)
        return;
    print_node (top->right->lex);

    printf ("\n");

    print_tree (top->left);
    print_tree (top->right);
}

void free_syntax_tree(struct node_t * top) {

    if (top->left == NULL && top->right == NULL) {
        free (top);
        return;
    }

    if (top->left != NULL)
        free_syntax_tree (top->left);
    if (top->right != NULL)
        free_syntax_tree (top->right);

    free (top);
}
