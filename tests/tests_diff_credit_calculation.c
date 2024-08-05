// Copyright [2023] <Burr Etienne>

#include "../tests/tests_header.h"

START_TEST(normal) {
  double const total = 200000;
  double const term = 12;
  double const interest = 10;

  double month_min = 0;
  double month_max = 0;
  double overpay = 0;
  double sum = 0;

  diff_credit_calculation(total, term, interest, &month_max, &month_min,
                          &overpay, &sum);
  ck_assert_double_eq_tol(month_max, 18333.3333333, TOL);
  ck_assert_double_eq_tol(month_min, 16805.5555556, TOL);
  ck_assert_double_eq_tol(overpay, 10833.3333333, TOL);
  ck_assert_double_eq_tol(sum, 210833.3333333, TOL);
}
END_TEST

START_TEST(minimal) {
  double const total = 0.01;
  double const term = 1;
  double const interest = 0.01;

  double month_min = 0;
  double month_max = 0;
  double overpay = 0;
  double sum = 0;

  diff_credit_calculation(total, term, interest, &month_max, &month_min,
                          &overpay, &sum);
  ck_assert_double_eq_tol(month_max, 0.0100001, TOL);
  ck_assert_double_eq_tol(month_min, 0.0000000, TOL);
  ck_assert_double_eq_tol(overpay, 0.0000001, TOL);
  ck_assert_double_eq_tol(sum, 0.0100001, TOL);
}
END_TEST

START_TEST(maximum) {
  double const total = 999999999999;
  double const term = 600;
  double const interest = 999;

  double month_min = 0;
  double month_max = 0;
  double overpay = 0;
  double sum = 0;

  diff_credit_calculation(total, term, interest, &month_max, &month_min,
                          &overpay, &sum);
  ck_assert_double_eq_tol(month_max, 834166666665.8325195, TOL);
  ck_assert_double_eq_tol(month_min, 3054166666.6574688, TOL);
  ck_assert_double_eq_tol(overpay, 250166249999746.5625000, TOL);
  ck_assert_double_eq_tol(sum, 251166249999745.5625000, TOL);
}
END_TEST

Suite *suite_diff_credit_calculation(void) {
  Suite *suite = suite_create("Diff credit calculation");

  TCase *tests_one = tcase_create("Test normal");
  tcase_add_test(tests_one, normal);
  suite_add_tcase(suite, tests_one);

  TCase *tests_two = tcase_create("Test minimal");
  tcase_add_test(tests_two, minimal);
  suite_add_tcase(suite, tests_two);

  TCase *tests_three = tcase_create("Test maximum");
  tcase_add_test(tests_three, maximum);
  suite_add_tcase(suite, tests_three);

  return suite;
}
