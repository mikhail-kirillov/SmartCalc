// Copyright [2023] <Burr Etienne>

#ifndef SRC_CALC_HEADERS_CALC_HELP_FUNCTIONS_H_
#define SRC_CALC_HEADERS_CALC_HELP_FUNCTIONS_H_

// Function at string
int is_ln(char const *str);
int is_log(char const *str);
int is_cos(char const *str);
int is_sin(char const *str);
int is_tan(char const *str);
int is_sqrt(char const *str);
int is_acos(char const *str);
int is_asin(char const *str);
int is_atan(char const *str);

// This char is operator
int is_operator(char const symbol);

// This char is function
int is_function_char(char const symbol);

// Input validation function
void input_validation(char const *exp, char const *x, int *is_x, int *is_exp_ok,
                      int *is_x_ok);

#endif  // SRC_CALC_HEADERS_CALC_HELP_FUNCTIONS_H_
