//
// Created by ДНС on 31.03.2020.
//
//#include "../problem_st/problem_st.h"
#include "../problem_lx/problem_lx.h"
#include <ctype.h>
#include "new_pr_st.h"

int is_end (const char* str, int i) { // return 1 if we get end of file, else 0
    if ((str[i] == '0' && str[i + 1] == '0') || i == 1024 || str[i] == '\0') {
        return 1;
    }
    return 0;
}

int is_add_sub (struct lexem_t l, int* i) {
    if (l.kind == OP) {
        if (l.lex.op == ADD || l.lex.op == SUB)
            return 1;
    }
    (*i)--;
    return 0;
}

int is_mul_div (struct lexem_t l, int* i) {
    if (l.kind == OP) {
        if (l.lex.op == MUL || l.lex.op == DIV)
            return 1;
    }
    (*i)--;
    return 0;
}

int is_left_brace (struct lexem_t l, int i) {
    if (l.kind == BRACE) {
        if (l.lex.b == LBRAC)
            return 1;
        printf ("ERROR: expected '(' on %d position", i);
        exit(0);
    }
    return 0;
}

int is_right_brace (struct lexem_t l) {
    if (l.kind == BRACE){
        if (l.lex.b == RBRAC)
            return 1;
    }
    return 0;
}

struct lexem_t get_cur_lexem (const char* str, int* i) {
    struct lexem_t lexem;
    int number = 0, flag = 0;

    while (isspace(str[*i]) != 0)
        ++(*i);


    if (is_end(str, *i) == 1) {
        lexem.kind = END;
        lexem.lex.num = 0;
        return lexem;
    }

    while (isdigit(str[*i]) == 1) {
        number = 10 * number + (int)(str[*i] - '0');
        flag = 1;
        ++(*i);
    }

    if (flag == 1) {
        lexem.kind = NUM;
        lexem.lex.num = number;
        (*i)--;
        return lexem;
    }

    switch (str[*i]) {
        case '+':
            lexem.kind = OP;
            lexem.lex.op = ADD;
            break;
        case '-':
            lexem.kind = OP;
            lexem.lex.op = SUB;
            break;
        case '*':
            lexem.kind = OP;
            lexem.lex.op = MUL;
            break;
        case '/':
            lexem.kind = OP;
            lexem.lex.op = DIV;
            break;
        case '(':
            lexem.kind = BRACE;
            lexem.lex.b = LBRAC;
            break;
        case ')':
            lexem.kind = BRACE;
            lexem.lex.b = RBRAC;
            break;
        default:
            printf("%d: %d(%c) is unknown symbol\n", *i, str[*i], str[*i]);
            exit(1);
    }
    return lexem;
}



