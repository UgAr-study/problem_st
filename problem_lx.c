//
// Created by ДНС on 09.03.2020.
//

#include "problem_lx.h"
#include <ctype.h>

struct lex_array_t lex_string(const char *str) {
    int i = 0, number = 0, flag = 0;

    assert(str != NULL);

    struct lex_array_t larr = { malloc(ICAP * sizeof(struct lexem_t)),
                                0,
                                ICAP };
    assert(larr.lexems != NULL);

    // TODO:
    // for each char in str
    //   if char reminds space, skip it
    //   if char reminds number, lex number
    //   if char reminds brace, lex brace
    //   if char reminds operation, lex operation
    //   otherwise free lex array, return NULL

    while (1) {
        while (isspace(str[i]) != 0)
            ++i;


        if ((str[i] == '0' && str[i + 1] == '0') || i == 1024)
            break;

        if (str[i] == '\0' || i == 1024)
            break;

        if (larr.size  == larr.capacity) {
            larr.lexems = (struct lexem_t*) realloc (larr.lexems, (larr.capacity + ICAP) * sizeof (struct lexem_t));
            assert (larr.lexems);
            larr.capacity += ICAP;
        }

        while (isdigit(str[i]) == 1) {
            number = 10 * number + (int)(str[i] - '0');
            flag = 1;
            ++i;
        }

        if (flag == 1) {
            larr.lexems[larr.size].kind = NUM;
            larr.lexems[larr.size].lex.num = number;
            flag = 0;
            number = 0;
            larr.size++;
            continue;
        }

        switch (str[i]) {
            case 43:        //  +
                larr.lexems[larr.size].kind = OP;
                larr.lexems[larr.size].lex.op = ADD;
                larr.size++;
                break;
            case 45:        //  -
                larr.lexems[larr.size].kind = OP;
                larr.lexems[larr.size].lex.op = SUB;
                larr.size++;
                break;
            case 42:        //  *
                larr.lexems[larr.size].kind = OP;
                larr.lexems[larr.size].lex.op = MUL;
                larr.size++;
                break;
            case 47:        //  /
                larr.lexems[larr.size].kind = OP;
                larr.lexems[larr.size].lex.op = DIV;
                larr.size++;
                break;
            case 40:        //  (
                larr.lexems[larr.size].kind = BRACE;
                larr.lexems[larr.size].lex.b = LBRAC;
                larr.size++;
                break;
            case 41:        //  )
                larr.lexems[larr.size].kind = BRACE;
                larr.lexems[larr.size].lex.b = RBRAC;
                larr.size++;
                break;
            default:
                printf("%d: %d is unknown symbol\n", i, str[i]);
                exit(1);
        }
        ++i;
    }

    return larr;
}

static void print_op(enum operation_t opcode) {
    switch(opcode) {
        case ADD: printf(" PLUS"); break;
        case SUB: printf(" MINUS"); break;
        case MUL: printf(" MUL"); break;
        case DIV: printf(" DIV"); break;
        default: assert(0 && "unknown opcode");
    }
}

static void
print_brace(enum braces_t bracetype) {
    switch(bracetype) {
        case LBRAC: printf(" LBRAC"); break;
        case RBRAC: printf(" RBRAC"); break;
        default: assert(0 && "unknown bracket");
    }
}

static void
print_num(int n) {
    printf(" NUMBER:%d", n);
}

void
print_lexem(struct lexem_t lxm) {
    switch(lxm.kind) {
        case OP: print_op(lxm.lex.op); break;
        case BRACE: print_brace(lxm.lex.b); break;
        case NUM: print_num(lxm.lex.num); break;
        default: assert(0 && "unknown lexem");
    }
}

void dump_lexarray(struct lex_array_t pl) {
    int i;
    assert(pl.lexems != NULL);
    for (i = 0; i < pl.size; ++i)
        print_lexem(pl.lexems[i]);
}
