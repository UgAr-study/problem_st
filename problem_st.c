#include "problem_st.h"
#include "../problem_lx/problem_lx.h"

// Grammar:
// expr ::= mult {+, -} expr | mult
// mult ::= term {*, /} mult | term
// term ::= ( expr ) | number

struct node_t* expr (struct lex_array_t lexarr, int* i, struct node_t** ptr_top);

struct node_t *create (struct lexem_t lex) {
    struct node_t* res;
    res = (struct node_t*) calloc (1, sizeof (struct node_t));
    res->lex = lex;
    return res;
}

struct node_t* numb (struct lex_array_t lexarr, int i) {
    struct node_t* num;
    if (lexarr.lexems[i].kind != NUM){
        printf ("N: Wrong format\n");
        exit(1);
    }
    num = create(lexarr.lexems[i]);
    return num;
}

struct node_t* term (struct lex_array_t lexarr, int *i, struct node_t **ptr_top) {
    printf ("T:\n");
    struct node_t* term = NULL;
    if (lexarr.lexems[*i].kind == NUM) {
        printf ("term = num\n");
        term = numb(lexarr, *i);
        //*i = *i + 1;
        return term;
    }
    if (lexarr.lexems[*i].kind == BRACE) {
        printf ("term = brace\n");
        if (lexarr.lexems[*i].lex.b == LBRAC) {
            *i = *i + 1;
            term = expr (lexarr, i, ptr_top);
            return term;
        }
        if (lexarr.lexems[*i].lex.b == RBRAC) {
            *i = *i + 1;
            return term;
        }
    }
    return term;
}

struct node_t* mult (struct lex_array_t lexarr, int *i, struct node_t** ptr_top) {
    assert(ptr_top != NULL);
    printf ("M:\n");
    struct node_t *multy, *tmp, *m_left;

    multy = (struct node_t*) calloc (1, sizeof (struct node_t));

    m_left = term (lexarr, i, &multy->left);
    printf ("after term i = %d\n", *i);

    if (multy->left == NULL)
        multy->left = m_left;

    printf ("m->l = %p\n", multy->left);
    //*i = *i + 1;
    if (multy->left == NULL)
        return NULL;

    if (*i == lexarr.size) {
        printf ("i = lexarr.size\n");
        return multy->left;
    }

    *i = *i + 1;
    printf ("M: check kind; i = %d\n", *i);

    if (lexarr.lexems[*i].kind != OP){
        //printf ("M: Wrong format\n");
        //exit (4);
        return multy->left;
    }

    if ((lexarr.lexems[*i].lex.op != MUL) && (lexarr.lexems[*i].lex.op != DIV))
        return multy->left;

    multy->lex = lexarr.lexems[*i];

    *i = *i + 1;
    assert(*i != lexarr.size);
    multy->right = mult(lexarr, i, ptr_top);

    if (multy->right->left != NULL && abs (multy->lex.lex.op - multy->right->lex.lex.op) <= 1) {
        tmp = multy->right;
        multy->right = multy->right->left;
        tmp->left = multy;

        if (*ptr_top == NULL)
            *ptr_top = tmp;
    }

    return multy;
}

struct node_t* expr (struct lex_array_t lexarr, int *i, struct node_t** ptr_top) {
    static int ent_nub = 0;
    if (ent_nub == 0) {
        printf("lexarr.size = %d\n", lexarr.size);
    }
    ent_nub++;

    assert (ptr_top != NULL);

    printf ("E:\n");

    struct node_t *expression, *tmp, *e_left;
    if (*i == lexarr.size)
        return NULL;

    expression = (struct node_t*) calloc (1, sizeof (struct node_t));

    e_left = mult (lexarr, i, &expression->left);

    if (expression->left == NULL && e_left != NULL)
        expression->left = e_left;

    if (*i == lexarr.size)
        return expression->left;

    if (lexarr.lexems[*i].kind != OP) {
        //printf ("E: Wrong format\n");
        //exit (2);
        return expression->left;
    }

    expression->lex = lexarr.lexems[*i];
    *i = *i + 1;

    assert(*i != lexarr.size);

    expression->right = expr (lexarr, i, ptr_top);
    printf ("e->r = %p\n", expression->right);

    if (expression->right->left != NULL && abs(expression->lex.lex.num - expression->right->lex.lex.num) <= 1) {
        tmp = expression->right;
        expression->right = expression->right->left;
        tmp->left = expression;

        if (*ptr_top == NULL)
            *ptr_top = tmp;
    }

    return expression;
}

struct node_t* build_syntax_tree(struct lex_array_t lexarr) {
    struct node_t* tree = NULL, *e;
    int i = 0;
    e = expr (lexarr, &i, &tree);
    assert (e != NULL);
    if (tree == NULL)
        tree = e;
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
            exit (3);
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
            exit(2);
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
    // TODO: your code here
}

