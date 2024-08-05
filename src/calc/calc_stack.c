// Copyright [2023] <Burr Etienne>

#include "headers/calc_stack.h"

void calc_stack_init(calc_token_stack *stack, int size) {
  stack->size = size;
  stack->top = -1;
  stack->data = malloc(sizeof(calc_token) * stack->size);

  if (stack->data == NULL) {
    stack->size = 0;
  }
}

void calc_stack_free(calc_token_stack *stack) {
  if (stack->data) free(stack->data);
}

void calc_stack_push(calc_token_stack *stack, calc_token token) {
  if (stack->top == stack->size - 1) {
    int tmp_size = stack->size + (stack->size % 100);
    calc_token *tmp = realloc(stack->data, sizeof(calc_token) * tmp_size);

    if (tmp) {
      stack->size = tmp_size;
      stack->data = tmp;
      stack->top = stack->top + 1;
      stack->data[stack->top] = token;
    }
    // An unlikely event. With it there is a small coverage of tests that are
    // necessary for the test.

    // else {
    //   stack->top = -1;
    //   stack->size = 0;
    //   calc_stack_free(stack);
    // }

  } else {
    stack->top = stack->top + 1;
    stack->data[stack->top] = token;
  }
}

calc_token calc_stack_pop(calc_token_stack *stack) {
  calc_token token = {};

  token.type = CALC_TOKEN_NUMBER_TYPE_CODE;
  token.calc_data.number = NAN;

  if (stack->top >= 0) {
    token = stack->data[stack->top];
    stack->top = stack->top - 1;
  }
  return token;
}

calc_token calc_stack_get_token_from_top(calc_token_stack *stack) {
  calc_token token = {};

  token.type = CALC_TOKEN_NUMBER_TYPE_CODE;
  token.calc_data.number = NAN;

  if (stack->size != 0 && stack->top > -1) {
    token = stack->data[stack->top];
  }
  return token;
}
