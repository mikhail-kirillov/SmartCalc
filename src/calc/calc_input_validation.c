// Copyright [2023] <Burr Etienne>

#include "headers/calc_calculation.h"
#include "headers/calc_help_functions.h"

// Enums for outputs
typedef enum is_x_codes { X_NOT_IN_EXP_CODE, X_IN_EXP_CODE } is_x_codes;

typedef enum is_exp_ok_codes {
  IS_EXP_ERROR_CODE,
  IS_EXP_OK_CODE
} is_exp_ok_codes;

typedef enum is_x_ok_codes { IS_X_ERROR_CODE, IS_X_OK_CODE } is_x_ok_codes;

// Local functions
int is_x(char const symbol);
int is_point(char const symbol);
int is_function(char const *str);
int is_left_bracket(char const symbol);
int is_right_bracket(char const symbol);
void is_correct_input(char const *exp, char const *x, int *is_x_in_exp,
                      int *is_exp_ok, int *is_x_ok);

// X, point and digit
int is_x(char const symbol) { return symbol == 'x'; }
int is_point(char const symbol) { return symbol == '.'; }

// Functions
int is_ln(char const *str) { return 2 * !strncmp(str, "ln", 2); }
int is_log(char const *str) { return 3 * !strncmp(str, "log", 3); }
int is_cos(char const *str) { return 3 * !strncmp(str, "cos", 3); }
int is_sin(char const *str) { return 3 * !strncmp(str, "sin", 3); }
int is_tan(char const *str) { return 3 * !strncmp(str, "tan", 3); }
int is_sqrt(char const *str) { return 4 * !strncmp(str, "sqrt", 4); }
int is_acos(char const *str) { return 4 * !strncmp(str, "acos", 4); }
int is_asin(char const *str) { return 4 * !strncmp(str, "asin", 4); }
int is_atan(char const *str) { return 4 * !strncmp(str, "atan", 4); }
int is_function(char const *str) {
  int code = 0;
  if ((code = is_ln(str))) {
  } else if ((code = is_log(str))) {
  } else if ((code = is_cos(str))) {
  } else if ((code = is_sin(str))) {
  } else if ((code = is_tan(str))) {
  } else if ((code = is_sqrt(str))) {
  } else if ((code = is_acos(str))) {
  } else if ((code = is_asin(str))) {
  } else if ((code = is_atan(str))) {
  } else {
    code = 0;
  }
  return code;
}
int is_function_char(char const symbol) {
  return symbol == 'L' || symbol == 'l' || symbol == 'S' || symbol == 's' ||
         symbol == 'C' || symbol == 'c' || symbol == 'T' || symbol == 't' ||
         symbol == 'Q';
}

// Operators
int is_operator(char const symbol) {
  return symbol == '+' || symbol == '-' || symbol == '/' || symbol == '*' ||
         symbol == '^' || symbol == '%';
}

// Brackets
int is_left_bracket(char const symbol) { return symbol == '('; }
int is_right_bracket(char const symbol) { return symbol == ')'; }

void is_correct_input(char const *exp, char const *x, int *is_x_in_exp,
                      int *is_exp_ok, int *is_x_ok) {
  int right_bracket_counter = 0;
  int left_bracket_counter = 0;

  int error_flag_exp = 0;
  for (size_t i = 0; i < strlen(exp) && !error_flag_exp; i++) {
    if (isdigit(exp[i])) {
    } else if (is_x(exp[i])) {
      *is_x_in_exp = X_IN_EXP_CODE;
    } else if (is_function(exp + i)) {
      i = i + is_function(exp + i) - 1;
    } else if (is_operator(exp[i])) {
    } else if (is_left_bracket(exp[i])) {
      left_bracket_counter++;
    } else if (is_right_bracket(exp[i])) {
      right_bracket_counter++;
    } else if (is_point(exp[i])) {
    } else {
      error_flag_exp = 1;
    }
  }

  if ((right_bracket_counter == left_bracket_counter) && !error_flag_exp) {
    *is_exp_ok = IS_EXP_OK_CODE;
  }

  int error_flag_x = 0;
  for (size_t i = 0; i < strlen(x) && !error_flag_x; i++) {
    if (isdigit(x[i])) {
    } else if (is_point(x[i])) {
    } else {
      error_flag_x = 1;
    }
  }

  if (strlen(x) == 0 && *is_x_in_exp == X_IN_EXP_CODE) {
    error_flag_x = 1;
  }

  if (!error_flag_x) {
    *is_x_ok = IS_X_OK_CODE;
  }
}

void input_validation(char const *exp, char const *x, int *is_x, int *is_exp_ok,
                      int *is_x_ok) {
  *is_x = X_NOT_IN_EXP_CODE;
  *is_exp_ok = IS_EXP_ERROR_CODE;
  *is_x_ok = IS_X_ERROR_CODE;

  if (exp && x) {
    is_correct_input(exp, x, is_x, is_exp_ok, is_x_ok);
  }
}
