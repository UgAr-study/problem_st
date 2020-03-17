#include "problem_st.h"

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

struct node_t* mult (struct lex_array_t lexarr, int *i) {
    struct node_t *mult;

    mult = (struct node_t*) calloc (1, sizeof (struct node_t));
    mult->left = numb (lexarr, *i);
    *i = *i + 1;

    if (*i == lexarr.size)
        return mult->left;
    if ()
}

struct node_t* expr (struct lex_array_t lexarr, int i, struct node_t** ptr_top) {
    assert (ptr_top != NULL);

    struct node_t *expression, *tmp;
    if (i == lexarr.size)
        return NULL;

    expression = (struct node_t*) calloc (1, sizeof (struct node_t));
    expression->left = numb (lexarr, i);
    //++i;

    if (i == lexarr.size)
        return expression->left;

    if (lexarr.lexems[i].kind != OP) {
        printf ("Wrong format\n");
        exit (2);
    }

    expression->lex = lexarr.lexems[i];
    ++i;

    expression->right = expr (lexarr, i, ptr_top);

    if (expression->right->left != NULL) {
        tmp = expression->right;
        expression->right = expression->right->left;
        tmp->left = expression;

        if (*ptr_top == NULL)
            *ptr_top = tmp;
    }

    return expression;
}

struct node_t* build_syntax_tree(struct lex_array_t lexarr) {
    struct node_t* tree = NULL;
    expr (lexarr, 0, &tree);
    return tree;
}

int calc_result(struct node_t *top) { // 22 - 3 + 5 - 6 ответ 20, а верный 18!
    int result, l, r;
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
            break;
        case SUB:
            result = l - r;
            break;
        case MUL:
            result = l * r;
            break;
        case DIV:
            result = l / r;
            break;
        default:
            printf ("Operation is not found, sorry (\n");
            exit (3);
    }
    return result;
}

void free_syntax_tree(struct node_t * top) {
    // TODO: your code here
}
