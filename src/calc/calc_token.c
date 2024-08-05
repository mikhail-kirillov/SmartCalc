// Copyright [2023] <Burr Etienne>

#include "headers/calc_token.h"

void calc_token_set_number(calc_token *token, double number) {
  token->type = CALC_TOKEN_NUMBER_TYPE_CODE;
  token->calc_data.number = number;
}

void calc_token_set_operator(calc_token *token, char symbol) {
  token->type = CALC_TOKEN_NOT_NUMBER_TYPE_CODE;
  switch (symbol) {
    case '(':
      token->calc_data.calc_other.data = CALC_TOKEN_LEFT_BRACKET_CODE;
      token->calc_data.calc_other.priority = CALC_TOKEN_BRACKET_CODE;
      break;

    case '+':
      token->calc_data.calc_other.data = CALC_TOKEN_PLUS_CODE;
      token->calc_data.calc_other.priority = CALC_TOKEN_PLUS_MINUS_CODE;
      break;
    case '-':
      token->calc_data.calc_other.data = CALC_TOKEN_MINUS_CODE;
      token->calc_data.calc_other.priority = CALC_TOKEN_PLUS_MINUS_CODE;
      break;

    case '*':
      token->calc_data.calc_other.data = CALC_TOKEN_MULT_CODE;
      token->calc_data.calc_other.priority = CALC_TOKEN_MULT_DIV_MOD_CODE;
      break;
    case '/':
      token->calc_data.calc_other.data = CALC_TOKEN_DIV_CODE;
      token->calc_data.calc_other.priority = CALC_TOKEN_MULT_DIV_MOD_CODE;
      break;
    case '%':
      token->calc_data.calc_other.data = CALC_TOKEN_MOD_CODE;
      token->calc_data.calc_other.priority = CALC_TOKEN_MULT_DIV_MOD_CODE;
      break;

    case '^':
      token->calc_data.calc_other.data = CALC_TOKEN_POW_CODE;
      token->calc_data.calc_other.priority = CALC_TOKEN_POW_PRIORITY_CODE;
      break;

    case 'L':
      token->calc_data.calc_other.data = CALC_TOKEN_LOG_CODE;
      token->calc_data.calc_other.priority = CALC_TOKEN_FUNCTION_CODE;
      break;
    case 'l':
      token->calc_data.calc_other.data = CALC_TOKEN_LN_CODE;
      token->calc_data.calc_other.priority = CALC_TOKEN_FUNCTION_CODE;
      break;
    case 'S':
      token->calc_data.calc_other.data = CALC_TOKEN_SIN_CODE;
      token->calc_data.calc_other.priority = CALC_TOKEN_FUNCTION_CODE;
      break;
    case 's':
      token->calc_data.calc_other.data = CALC_TOKEN_ASIN_CODE;
      token->calc_data.calc_other.priority = CALC_TOKEN_FUNCTION_CODE;
      break;
    case 'C':
      token->calc_data.calc_other.data = CALC_TOKEN_COS_CODE;
      token->calc_data.calc_other.priority = CALC_TOKEN_FUNCTION_CODE;
      break;
    case 'c':
      token->calc_data.calc_other.data = CALC_TOKEN_ACOS_CODE;
      token->calc_data.calc_other.priority = CALC_TOKEN_FUNCTION_CODE;
      break;
    case 'T':
      token->calc_data.calc_other.data = CALC_TOKEN_TAN_CODE;
      token->calc_data.calc_other.priority = CALC_TOKEN_FUNCTION_CODE;
      break;
    case 't':
      token->calc_data.calc_other.data = CALC_TOKEN_ATAN_CODE;
      token->calc_data.calc_other.priority = CALC_TOKEN_FUNCTION_CODE;
      break;
    case 'Q':
      token->calc_data.calc_other.data = CALC_TOKEN_SQRT_CODE;
      token->calc_data.calc_other.priority = CALC_TOKEN_FUNCTION_CODE;
      break;
  }
}
