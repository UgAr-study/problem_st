#include "../problem_st/problem_st.h"

// Grammar:
// expr ::= mult {+, -} expr | mult
// mult ::= term {*, /} mult | term
// term ::= ( expr ) | number

struct node_t* expr (struct lex_array_t lexarr, int* i);

struct node_t* return_left (struct node_t* top) {
    struct node_t* left = top->left;
    free (top);
    return left;
}

struct node_t *create (struct lexem_t lex) {
    struct node_t* res;
    res = (struct node_t*) calloc (1, sizeof (struct node_t));
    res->lex = lex;
    return res;
}

struct node_t* numb (struct lex_array_t lexarr, int i) {
    printf ("in numb\n");
    struct node_t* num;
    if (lexarr.lexems[i].kind != NUM){
        printf ("N: Wrong format\n");
        exit(1);
    }
    num = create(lexarr.lexems[i]);
    return num;
}

struct node_t* term (struct lex_array_t lexarr, int *i) {
    printf("in term\n");

    if (lexarr.lexems[*i].kind == NUM)
        return numb(lexarr, *i);

    if (lexarr.lexems[*i].kind == BRACE) {
        if (lexarr.lexems[*i].lex.b == LBRAC) {
            *i = *i + 1;
            return expr (lexarr, i);
        }
        if (lexarr.lexems[*i].lex.b == RBRAC) {
            printf("RBRACE\n");
            *i = *i + 1;
            return NULL;
        }
    } else {
        printf ("Wrong format: expected number or brace on %d position\n", *i + 1);
        exit (1);
    }
    return NULL;
}

struct node_t* mult (struct lex_array_t lexarr, int *i) {
    printf ("in mult\n");
    struct node_t *multy, *m_left;


    m_left = term (lexarr, i);
    printf ("M: i = %d\n", *i);
    if (*i != lexarr.size) {
        (*i)++;
        if (lexarr.lexems[*i].kind == OP) {
            if (lexarr.lexems[*i].lex.op == MUL || lexarr.lexems[*i].lex.op == DIV) {
                multy = (struct node_t *) calloc(1, sizeof(struct node_t));
                multy->lex.lex = lexarr.lexems[*i].lex;
                multy->left = m_left;
                (*i)++;
                multy->right = mult(lexarr, i);
                (*i)++;
                return multy;
            }
            (*i)++;
        }
        (*i)--;
    }

    return m_left;
}

struct node_t* expr (struct lex_array_t lexarr, int *i) {
    printf ("in expr\n");

    struct node_t *expression, *e_left;
    if (*i == lexarr.size)
        return NULL;

    e_left = mult(lexarr, i);
    printf ("E: i = %d\n", *i);
    if (*i != lexarr.size) {
        if (lexarr.lexems[*i].kind == OP) {
            if (lexarr.lexems[*i].lex.op == ADD || lexarr.lexems[*i].lex.op == SUB) {
                expression = (struct node_t *) calloc(1, sizeof(struct node_t));
                expression->lex.lex = lexarr.lexems[*i].lex;
                expression->left = e_left;
                (*i)++;
                expression->right = expr(lexarr, i);
                (*i)++;
                return expression;
            }
        }
    }

    return e_left;
}

struct node_t* build_syntax_tree(struct lex_array_t lexarr) {
    struct node_t* tree;
    int i = 0;
    tree = expr (lexarr, &i);

    if (tree == NULL) {
        printf ("Nothing to be calculated\n");
        exit (1);
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
