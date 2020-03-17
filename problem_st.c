#include "problem_st.h"
#include "../problem_lx/problem_lx.h"

// Grammar:
// expr ::= mult {+, -} expr | mult
// mult ::= term {*, /} mult | term
// term ::= ( expr ) | number

struct node_t *create (struct lexem_t lex) {
    struct node_t* res;
    res = (struct node_t*) calloc (1, sizeof (struct node_t));
    res->lex = lex;
    return res;
}

/*struct node_t* term (struct lex_array_t lexarr, int i) {

}

void mult (struct lex_array_t lexarr, int i) {

}

void expr (struct lex_array_t lexarr, int i) {

}*/

struct node_t* numb (struct lex_array_t lexarr, int i) {
    struct node_t* num;
    if (lexarr.lexems[i].kind != NUM){
        printf ("Wrong format\n");
        exit(1);
    }
    num = create(lexarr.lexems[i]);
    return num;
}

struct node_t* mult (struct lex_array_t lexarr, int *i, struct node_t** ptr_top) {
    assert(ptr_top != NULL);

    struct node_t *multy, *tmp;

    multy = (struct node_t*) calloc (1, sizeof (struct node_t));
    multy->left = numb (lexarr, *i);

    *i = *i + 1;

    if (*i == lexarr.size)
        return multy->left;

    if (lexarr.lexems[*i].kind != OP){
        printf ("Wrong format\n");
        exit (4);
    }

    if ((lexarr.lexems[*i].lex.op != MUL) && (lexarr.lexems[*i].lex.op != DIV))
        return multy->left;

    multy->lex = lexarr.lexems[*i];

    *i = *i + 1;
    assert(*i != lexarr.size);
    multy->right = mult(lexarr, i, ptr_top);

    if (multy->right->left != NULL) {
        tmp = multy->right;
        multy->right = multy->right->left;
        tmp->left = multy;

        if (*ptr_top == NULL)
            *ptr_top = tmp;
    }

    return multy;
}

struct node_t* expr (struct lex_array_t lexarr, int i, struct node_t** ptr_top) {
    assert (ptr_top != NULL);

    struct node_t *expression, *tmp, *e_left, *e_right;
    if (i == lexarr.size)
        return NULL;

    expression = (struct node_t*) calloc (1, sizeof (struct node_t));

    e_left = mult (lexarr, &i, &expression->left);

    if (expression->left == NULL && e_left != NULL)
        expression->left = e_left;

    if (i == lexarr.size)
        return expression->left;

    if (lexarr.lexems[i].kind != OP) {
        printf ("Wrong format\n");
        exit (2);
    }

    expression->lex = lexarr.lexems[i];
    ++i;

    assert(i != lexarr.size);

    expression->right = expr (lexarr, i, ptr_top);
    /*e_right = expr (lexarr, i, &expression->right);

    if (expression->right == NULL && e_right != NULL)
        expression->right = e_right;*/

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
    e = expr (lexarr, 0, &tree);
    assert (e != NULL);
    if (tree == NULL)
        tree = e;
    return tree;
}

int calc_result(struct node_t *top) { // 22 - 3 + 5 - 6 ответ 20, а верный 18!
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
    return;
}

void free_syntax_tree(struct node_t * top) {
    // TODO: your code here
}

