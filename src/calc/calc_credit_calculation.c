// Copyright [2023] <Burr Etienne>

#include "headers/calc_calculation.h"

// Annuitet inner functions
double annuitet_credit_calculation_month(double total, double interest,
                                         int term);
double annuitet_credit_calculation_month_sum(double month, int term);
double annuitet_credit_calculation_month_overpay(double sum, double total);

// Diff inner functions
double diff_credit_calculation_overpay(double sum, double total);
void diff_credit_calculation_months_and_sum(double total, int term,
                                            double interest, double *month_max,
                                            double *month_min, double *sum);

// Annuitet calculation
void annuitet_credit_calculation(double total, int term, double interest,
                                 double *month, double *overpay, double *sum) {
  interest = interest / 100;

  *month = annuitet_credit_calculation_month(total, interest, term);
  *sum = annuitet_credit_calculation_month_sum(*month, term);
  *overpay = annuitet_credit_calculation_month_overpay(*sum, total);
}

// Diff calculation
void diff_credit_calculation(double total, int term, double interest,
                             double *month_max, double *month_min,
                             double *overpay, double *sum) {
  interest = interest / 100;
  diff_credit_calculation_months_and_sum(total, term, interest, month_max,
                                         month_min, sum);
  *overpay = diff_credit_calculation_overpay(*sum, total);
}

double annuitet_credit_calculation_month(double total, double interest,
                                         int term) {
  double res = 0;
  res = total * (((interest / 12) * pow((1 + (interest / 12)), term)) /
                 (pow((1 + (interest / 12)), term) - 1));
  return res;
}

double annuitet_credit_calculation_month_sum(double month, int term) {
  double res = 0;
  res = month * term;
  return res;
}

double annuitet_credit_calculation_month_overpay(double sum, double total) {
  double res = 0;
  res = sum - total;
  return res;
}

void diff_credit_calculation_months_and_sum(double total, int term,
                                            double interest, double *month_max,
                                            double *month_min, double *sum) {
  double equality_share_of_total = total / term;
  double tmp_total = total;
  for (int i = 0; i < term; i++) {
    double interest_now = tmp_total * (interest / 12);
    double sum_now = equality_share_of_total + interest_now;
    tmp_total = tmp_total - equality_share_of_total;
    if (i == 0) {
      *month_max = sum_now;
    } else if (i == term - 1) {
      *month_min = sum_now;
    }
    *sum += sum_now;
  }
}

double diff_credit_calculation_overpay(double sum, double total) {
  double res = 0;
  res = sum - total;
  return res;
}
