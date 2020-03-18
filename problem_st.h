#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../problem_lx/problem_lx.h"

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