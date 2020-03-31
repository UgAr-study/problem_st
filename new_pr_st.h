//
// Created by ДНС on 31.03.2020.
//
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


struct lexem_t get_cur_lexem (const char* str, int* i);
int is_end (const char* str, int i);
int is_add_sub (struct lexem_t l, int* i);
int is_mul_div (struct lexem_t l, int* i);
int is_left_brace (struct lexem_t l, int i);
int is_right_brace (struct lexem_t l);
struct node_t* build_syntax_tree_(const char* str);

//calculating the result
int calc_result(struct node_t *top);

//print tree
void print_tree (struct node_t* top);
//print node
void print_node (struct lexem_t lex);
//free tree
void free_syntax_tree(struct node_t * top);