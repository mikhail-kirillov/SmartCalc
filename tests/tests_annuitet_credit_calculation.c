// Copyright [2023] <Burr Etienne>

#include "../tests/tests_header.h"

START_TEST(normal) {
  double const total = 200000;
  double const term = 12;
  double const interest = 10;

  double month = 0;
  double overpay = 0;
  double sum = 0;

  annuitet_credit_calculation(total, term, interest, &month, &overpay, &sum);
  ck_assert_double_eq_tol(month, 17583.1774460, TOL);
  ck_assert_double_eq_tol(overpay, 10998.1293520, TOL);
  ck_assert_double_eq_tol(sum, 210998.1293520, TOL);
}
END_TEST

START_TEST(minimal) {
  double const total = 0.01;
  double const term = 1;
  double const interest = 0.01;

  double month = 0;
  double overpay = 0;
  double sum = 0;

  annuitet_credit_calculation(total, term, interest, &month, &overpay, &sum);
  ck_assert_double_eq_tol(month, 0.0100001, TOL);
  ck_assert_double_eq_tol(overpay, 0.0000001, TOL);
  ck_assert_double_eq_tol(sum, 0.0100001, TOL);
}
END_TEST

START_TEST(maximum) {
  double const total = 999999999999;
  double const term = 600;
  double const interest = 999;

  double month = 0;
  double overpay = 0;
  double sum = 0;

  annuitet_credit_calculation(total, term, interest, &month, &overpay, &sum);
  ck_assert_double_eq_tol(month, 832499999999.1673584, TOL);
  ck_assert_double_eq_tol(overpay, 498499999999501.4375000, TOL);
  ck_assert_double_eq_tol(sum, 499499999999500.4375000, TOL);
}
END_TEST

Suite *suite_annuitet_credit_calculation(void) {
  Suite *suite = suite_create("Annuitet credit calculation");

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
