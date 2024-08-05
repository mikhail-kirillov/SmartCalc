// Copyright [2023] <Burr Etienne>

#include "headers/calc_calculation.h"
#include "headers/calc_help_functions.h"
#include "headers/calc_queue.h"
#include "headers/calc_stack.h"
#include "headers/calc_token.h"

void infix_to_postfix(char const *infix, calc_token_stack *stack,
                      calc_token_queue *queue, double x) {
  int i = 0;  // iterator
  while (infix[i] != '\0') {
    if (is_operator(infix[i])) {
      // If unar
      if ((i == 0 || infix[i - 1] == '(') &&
          (infix[i] == '-' || infix[i] == '+')) {
        calc_token token = {};
        calc_token_set_number(&token, 0);
        calc_queue_add(queue, token);
      }

      // Set op
      calc_token op = {};
      calc_token_set_operator(&op, infix[i]);

      // If not first
      if (stack->top != -1) {
        // If just push
        if (calc_stack_get_token_from_top(stack).calc_data.calc_other.priority <
            op.calc_data.calc_other.priority) {
          calc_stack_push(stack, op);

          // Or pop to queue
        } else {
          while (1) {
            if (stack->top == -1) {
              break;
            }
            calc_token token = calc_stack_pop(stack);

            if (token.calc_data.calc_other.priority >=
                op.calc_data.calc_other.priority) {
              calc_queue_add(queue, token);

            } else {
              calc_stack_push(stack, token);

              break;
            }
          }
          // Push new
          calc_stack_push(stack, op);
        }

        // If first
      } else {
        calc_stack_push(stack, op);
      }

    } else if (infix[i] == 'x') {
      // Add to queue number instead of x
      calc_token token = {};
      calc_token_set_number(&token, x);
      calc_queue_add(queue, token);

    } else if (isdigit(infix[i])) {
      // Tmp vars for get num and n-symbols
      int tmp_n = 0;
      double tmp_number = 0;

      // Read
      setlocale(LC_NUMERIC, "C");
      sscanf(infix + i, "%lf%n", &tmp_number, &tmp_n);

      // Add n-symbols to iterator and - 1 iteration
      i = i + tmp_n - 1;
      calc_token token = {};

      // Add to queue
      calc_token_set_number(&token, tmp_number);
      calc_queue_add(queue, token);

    } else if (infix[i] == '(') {
      // Just add to stack
      calc_token op = {};
      calc_token_set_operator(&op, infix[i]);
      calc_stack_push(stack, op);

    } else if (infix[i] == ')') {
      // Get token from stack
      calc_token token = calc_stack_pop(stack);

      // While stack is not empty and new token from stack not equal '('
      while (stack->top > -1 && token.calc_data.calc_other.data != '(') {
        // Add token to queue
        calc_queue_add(queue, token);
        // And get new token from stack
        token = calc_stack_pop(stack);
      }

      // If in top of stack token is function
      if (is_function_char(
              calc_stack_get_token_from_top(stack).calc_data.calc_other.data)) {
        // Get it
        token = calc_stack_pop(stack);
        // And add to queue
        calc_queue_add(queue, token);
      }

    } else if (is_ln(infix + i)) {   // is function
      i = i + is_ln(infix + i) - 1;  // add to iterator function len
      calc_token op = {};            // make token
      calc_token_set_operator(&op, CALC_TOKEN_LN_CODE);  // set function data
      calc_stack_push(stack, op);  // add token to stack (because max priority)

    } else if (is_log(infix + i)) {
      i = i + is_log(infix + i) - 1;
      calc_token op = {};
      calc_token_set_operator(&op, CALC_TOKEN_LOG_CODE);
      calc_stack_push(stack, op);

    } else if (is_sin(infix + i)) {
      i = i + is_sin(infix + i) - 1;
      calc_token op = {};
      calc_token_set_operator(&op, CALC_TOKEN_SIN_CODE);
      calc_stack_push(stack, op);

    } else if (is_asin(infix + i)) {
      i = i + is_asin(infix + i) - 1;
      calc_token op = {};
      calc_token_set_operator(&op, CALC_TOKEN_ASIN_CODE);
      calc_stack_push(stack, op);

    } else if (is_cos(infix + i)) {
      i = i + is_cos(infix + i) - 1;
      calc_token op = {};
      calc_token_set_operator(&op, CALC_TOKEN_COS_CODE);
      calc_stack_push(stack, op);

    } else if (is_acos(infix + i)) {
      i = i + is_acos(infix + i) - 1;
      calc_token op = {};
      calc_token_set_operator(&op, CALC_TOKEN_ACOS_CODE);
      calc_stack_push(stack, op);

    } else if (is_tan(infix + i)) {
      i = i + is_tan(infix + i) - 1;
      calc_token op = {};
      calc_token_set_operator(&op, CALC_TOKEN_TAN_CODE);
      calc_stack_push(stack, op);

    } else if (is_atan(infix + i)) {
      i = i + is_atan(infix + i) - 1;
      calc_token op = {};
      calc_token_set_operator(&op, CALC_TOKEN_ATAN_CODE);
      calc_stack_push(stack, op);

    } else if (is_sqrt(infix + i)) {
      i = i + is_sqrt(infix + i) - 1;
      calc_token op = {};
      calc_token_set_operator(&op, CALC_TOKEN_SQRT_CODE);
      calc_stack_push(stack, op);
    }

    i = i + 1;  // add 1 to iterator
  }
  // While stack is not empty
  while (stack->top >= 0) {
    // Add token from stack to queue
    calc_token token = calc_stack_pop(stack);
    calc_queue_add(queue, token);
  }
}

double postfix_calculation(calc_token_queue *queue) {
  double result = NAN;  // Error result set
  int error_flag = 0;

  // Make stack and init it
  calc_token_stack stack = {};
  calc_stack_init(&stack, queue->end + 2);

  while (queue->start <= queue->end && !error_flag) {
    calc_token token = calc_queue_get(queue);

    if (token.type == CALC_TOKEN_NUMBER_TYPE_CODE) {
      calc_stack_push(&stack, token);

    } else {
      calc_token token_two = {};
      calc_token token_one = {};
      double two = 0;
      double one = 0;
      double tmp_result = 0;
      switch (token.calc_data.calc_other.data) {
        case CALC_TOKEN_PLUS_CODE:
          token_two = calc_stack_pop(&stack);
          token_one = calc_stack_pop(&stack);
          two = token_two.calc_data.number;
          one = token_one.calc_data.number;
          tmp_result = one + two;
          calc_token token_tmp_result = {};
          calc_token_set_number(&token_tmp_result, tmp_result);
          calc_stack_push(&stack, token_tmp_result);
          break;

        case CALC_TOKEN_MINUS_CODE:
          token_two = calc_stack_pop(&stack);
          token_one = calc_stack_pop(&stack);
          two = token_two.calc_data.number;
          one = token_one.calc_data.number;
          tmp_result = one - two;
          calc_token_set_number(&token_tmp_result, tmp_result);
          calc_stack_push(&stack, token_tmp_result);
          break;

        case CALC_TOKEN_MULT_CODE:
          token_two = calc_stack_pop(&stack);
          token_one = calc_stack_pop(&stack);
          two = token_two.calc_data.number;
          one = token_one.calc_data.number;
          tmp_result = one * two;
          calc_token_set_number(&token_tmp_result, tmp_result);
          calc_stack_push(&stack, token_tmp_result);
          break;

        case CALC_TOKEN_DIV_CODE:
          token_two = calc_stack_pop(&stack);
          token_one = calc_stack_pop(&stack);
          two = token_two.calc_data.number;
          one = token_one.calc_data.number;
          tmp_result = one / two;
          calc_token_set_number(&token_tmp_result, tmp_result);
          calc_stack_push(&stack, token_tmp_result);
          break;

        case CALC_TOKEN_MOD_CODE:
          token_two = calc_stack_pop(&stack);
          token_one = calc_stack_pop(&stack);
          two = token_two.calc_data.number;
          one = token_one.calc_data.number;
          if ((int)two == 0) {
            error_flag = 1;
            result = NAN;
            break;
          }
          tmp_result = (int)one % (int)two;
          calc_token_set_number(&token_tmp_result, tmp_result);
          calc_stack_push(&stack, token_tmp_result);
          break;

        case CALC_TOKEN_POW_CODE:
          token_two = calc_stack_pop(&stack);
          token_one = calc_stack_pop(&stack);
          two = token_two.calc_data.number;
          one = token_one.calc_data.number;
          tmp_result = pow(one, two);
          calc_token_set_number(&token_tmp_result, tmp_result);
          calc_stack_push(&stack, token_tmp_result);
          break;

        case CALC_TOKEN_LN_CODE:
          token_one = calc_stack_pop(&stack);
          one = token_one.calc_data.number;
          if (one == 0.0) {
            error_flag = 1;
            result = NAN;
            break;
          }
          tmp_result = log(one);
          calc_token_set_number(&token_tmp_result, tmp_result);
          calc_stack_push(&stack, token_tmp_result);
          break;

        case CALC_TOKEN_LOG_CODE:
          token_one = calc_stack_pop(&stack);
          one = token_one.calc_data.number;
          if (one == 0.0) {
            error_flag = 1;
            result = NAN;
            break;
          }
          tmp_result = log10(one);
          calc_token_set_number(&token_tmp_result, tmp_result);
          calc_stack_push(&stack, token_tmp_result);
          break;

        case CALC_TOKEN_SIN_CODE:
          token_one = calc_stack_pop(&stack);
          one = token_one.calc_data.number;
          tmp_result = sin(one);
          calc_token_set_number(&token_tmp_result, tmp_result);
          calc_stack_push(&stack, token_tmp_result);
          break;

        case CALC_TOKEN_ASIN_CODE:
          token_one = calc_stack_pop(&stack);
          one = token_one.calc_data.number;
          tmp_result = asin(one);
          calc_token_set_number(&token_tmp_result, tmp_result);
          calc_stack_push(&stack, token_tmp_result);
          break;

        case CALC_TOKEN_COS_CODE:
          token_one = calc_stack_pop(&stack);
          one = token_one.calc_data.number;
          tmp_result = cos(one);
          calc_token_set_number(&token_tmp_result, tmp_result);
          calc_stack_push(&stack, token_tmp_result);
          break;

        case CALC_TOKEN_ACOS_CODE:
          token_one = calc_stack_pop(&stack);
          one = token_one.calc_data.number;
          tmp_result = acos(one);
          calc_token_set_number(&token_tmp_result, tmp_result);
          calc_stack_push(&stack, token_tmp_result);
          break;

        case CALC_TOKEN_TAN_CODE:
          token_one = calc_stack_pop(&stack);
          one = token_one.calc_data.number;
          tmp_result = tan(one);
          calc_token_set_number(&token_tmp_result, tmp_result);
          calc_stack_push(&stack, token_tmp_result);
          break;

        case CALC_TOKEN_ATAN_CODE:
          token_one = calc_stack_pop(&stack);
          one = token_one.calc_data.number;
          tmp_result = atan(one);
          calc_token_set_number(&token_tmp_result, tmp_result);
          calc_stack_push(&stack, token_tmp_result);
          break;

        case CALC_TOKEN_SQRT_CODE:
          token_one = calc_stack_pop(&stack);
          one = token_one.calc_data.number;
          tmp_result = sqrt(one);
          calc_token_set_number(&token_tmp_result, tmp_result);
          calc_stack_push(&stack, token_tmp_result);
          break;
      }
    }
  }
  if (stack.top == 0) {
    calc_token token_res = calc_stack_pop(&stack);

    if (token_res.type == CALC_TOKEN_NUMBER_TYPE_CODE) {
      result = token_res.calc_data.number;
    }
  }
  calc_stack_free(&stack);
  return result;
}

double calculation(char const *input, double const *x) {
  double result = NAN;  // Error result set
  double null_x = 0;

  calc_token_stack stack = {};
  calc_stack_init(&stack, CALC_STACK_MAX_SIZE);

  calc_token_queue queue = {};
  calc_queue_init(&queue);

  // Set x 0 if input x is NULL
  if (x == NULL) {
    null_x = 0;
  } else {
    null_x = *x;
  }

  infix_to_postfix(input, &stack, &queue, null_x);

  // Get result from postfix queue
  result = postfix_calculation(&queue);

  calc_stack_free(&stack);

  return result;
}
