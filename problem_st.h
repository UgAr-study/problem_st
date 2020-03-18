#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../problem_lx/problem_lx.h"

//if you already have it in problem_lx.h, you should delete this part of code
//<<<<<<<<<<<<<<<<<<<<<<<<

//max string length
enum { MAXLEN = 1024 };

// lexem is operation | brace | number
enum lexem_kind_t { OP, BRACE, NUM };

// operation is: +, -, *, /
enum operation_t { ADD = 0, SUB = 1, MUL = 3, DIV = 4 };

// braces are: (, )
enum braces_t { LBRAC, RBRAC };

// lexem is one of lexem kind entities
// if (l.kind == BRACE)
//   assert(l.lex.b == LBRAC || l.lex.b == RBRAC)
struct lexem_t {
    enum lexem_kind_t kind;
    union {
        enum operation_t op;
        enum braces_t b;
        int num;
    } lex;
};

// array of lexems
// size: actual number of elements
// capacity: number of additions before realloc
struct lex_array_t {
    struct lexem_t *lexems;
    int size, capacity;
};

// initial capacity
enum { ICAP = 10 };

// string to lexem array: see Problem LX on slides
struct lex_array_t lex_string(const char *str);

// printing
void print_lexem(struct lexem_t lxm);
void dump_lexarray(struct lex_array_t pl);

//>>>>>>>>>>>>>>>>>>>>


struct node_t {
    struct lexem_t lex;
    struct node_t *left, *right;
};

struct node_t* build_syntax_tree(struct lex_array_t lexarr);

//calculating the result
int calc_result(struct node_t *top);

//print tree
void print_tree (struct node_t* top);
//print node
void print_node (struct lexem_t lex);
//free tree
void free_syntax_tree(struct node_t * top);