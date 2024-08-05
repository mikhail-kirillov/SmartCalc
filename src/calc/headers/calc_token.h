// Copyright [2023] <Burr Etienne>

#ifndef SRC_CALC_HEADERS_CALC_TOKEN_H_
#define SRC_CALC_HEADERS_CALC_TOKEN_H_

#include "../headers/calc_calculation.h"

typedef struct calc_token {
  int type;  // Number or not
  union calc_data {
    double number;  // If number set number
    struct calc_other {
      int data;      // Else set operator
      int priority;  // And priority
    } calc_other;
  } calc_data;
} calc_token;

typedef enum calc_token_type_codes {
  CALC_TOKEN_NUMBER_TYPE_CODE,
  CALC_TOKEN_NOT_NUMBER_TYPE_CODE
} calc_token_type_codes;

typedef enum calc_token_data_codes {
  CALC_TOKEN_LEFT_BRACKET_CODE = '(',
  CALC_TOKEN_RIGHT_BRACKET_CODE = ')',

  CALC_TOKEN_PLUS_CODE = '+',
  CALC_TOKEN_MINUS_CODE = '-',

  CALC_TOKEN_MULT_CODE = '*',
  CALC_TOKEN_DIV_CODE = '/',
  CALC_TOKEN_MOD_CODE = '%',

  CALC_TOKEN_POW_CODE = '^',

  CALC_TOKEN_LOG_CODE = 'L',
  CALC_TOKEN_LN_CODE = 'l',
  CALC_TOKEN_SIN_CODE = 'S',
  CALC_TOKEN_ASIN_CODE = 's',
  CALC_TOKEN_COS_CODE = 'C',
  CALC_TOKEN_ACOS_CODE = 'c',
  CALC_TOKEN_TAN_CODE = 'T',
  CALC_TOKEN_ATAN_CODE = 't',
  CALC_TOKEN_SQRT_CODE = 'Q'
} calc_token_data_codes;

typedef enum calc_token_priority_codes {
  CALC_TOKEN_BRACKET_CODE,
  CALC_TOKEN_PLUS_MINUS_CODE,
  CALC_TOKEN_MULT_DIV_MOD_CODE,
  CALC_TOKEN_POW_PRIORITY_CODE,
  CALC_TOKEN_FUNCTION_CODE
} calc_token_priority_codes;

// Functions for set data into token
void calc_token_set_number(calc_token *token, double number);
void calc_token_set_operator(calc_token *token, char symbol);

#endif  // SRC_CALC_HEADERS_CALC_TOKEN_H_
