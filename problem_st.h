//
// Created by ДНС on 14.03.2020.
//

#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../problem_lx/problem_lx.h"

// lexem is operation | brace | number
////enum lexem_kind_t { OP, BRACE, NUM };

// operation is: +, -, *, /
////enum operation_t { ADD, SUB, MUL, DIV };

// braces are: (, )
////enum braces_t { LBRAC, RBRAC };

// lexem is one of lexem kind entities
// if (l.kind == BRACE)
//   assert(l.lex.b == LBRAC || l.lex.b == RBRAC)
/*struct lexem_t {
    enum lexem_kind_t kind;
    union {
        enum operation_t op;
        enum braces_t b;
        int num;
    } lex;
};*/

// array of lexems
// size: actual number of elements
// capacity: number of additions before realloc
/*struct lex_array_t {
    struct lexem_t *lexems;
    int size, capacity;
};*/

// initial capacity
////enum { ICAP = 10 };

struct node_t {
    struct lexem_t lex;
    struct node_t *left, *right;
};

// string to lexem array: see Problem LX on slides
struct lex_array_t lex_string(const char *str);

//build tree
struct node_t* build_syntax_tree(struct lex_array_t lexarr);

//calculating the result
int calc_result(struct node_t *top);

// printing
void print_lexem(struct lexem_t lxm);
//void dump_lexarray(struct lex_array_t pl);

