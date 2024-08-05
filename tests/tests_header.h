// Copyright [2023] <Burr Etienne>

#ifndef TESTS_TESTS_HEADER_H_
#define TESTS_TESTS_HEADER_H_

#include <check.h>

#include "../src/calc/headers/calc_calculation.h"
#include "../src/calc/headers/calc_help_functions.h"
#include "../src/calc/headers/calc_stack.h"

#define TOL (1e-7)

Suite *suite_annuitet_credit_calculation(void);
Suite *suite_diff_credit_calculation(void);
Suite *suite_calculation(void);
Suite *suite_input_validation(void);
Suite *suite_stack(void);

#endif  // TESTS_TESTS_HEADER_H_
