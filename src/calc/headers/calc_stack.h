// Copyright [2023] <Burr Etienne>

#ifndef SRC_CALC_HEADERS_CALC_STACK_H_
#define SRC_CALC_HEADERS_CALC_STACK_H_

#include "../headers/calc_token.h"

#define CALC_STACK_MAX_SIZE (256)

typedef struct calc_token_stack {
  int size;
  int top;
  calc_token *data;
} calc_token_stack;

// Functions for work with stack
void calc_stack_init(calc_token_stack *stack, int size);
void calc_stack_push(calc_token_stack *stack, calc_token token);
calc_token calc_stack_pop(calc_token_stack *stack);
void calc_stack_free(calc_token_stack *stack);
calc_token calc_stack_get_token_from_top(calc_token_stack *stack);

#endif  // SRC_CALC_HEADERS_CALC_STACK_H_
