// Copyright [2023] <Burr Etienne>

#ifndef SRC_CALC_HEADERS_CALC_CALCULATION_H_
#define SRC_CALC_HEADERS_CALC_CALCULATION_H_

#include <ctype.h>
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Annuitet credit calculation functions
void annuitet_credit_calculation(double total, int term, double interest,
                                 double *month, double *overpay, double *sum);
// Differentiated credit calculation functions
void diff_credit_calculation(double total, int term, double interest,
                             double *month_max, double *mounth_min,
                             double *overpay, double *sum);

// Calculation function
double calculation(char const *input, double const *x);

#endif  // SRC_CALC_HEADERS_CALC_CALCULATION_H_
