// Copyright [2023] <Burr Etienne>

#include "../tests/tests_header.h"

START_TEST(all_tests) {
  char const *string =
      "+1-2.22*(-3/"
      "1%x)^0*ln(1)+log(10)-sin(2)+cos(3)-asin(1)+acos(1)*tan(1)-atan(1)/"
      "sqrt(4)-10";
  double const x = 10;
  double result = calculation(string, &x);
  ck_assert_double_eq_tol(result, -11.8627853, TOL);
}
END_TEST

START_TEST(plus_tests) {
  char const *string = "1+x";
  double const x = 10;
  double result = calculation(string, &x);
  ck_assert_double_eq_tol(result, 11, TOL);

  char const *str1 = "+x";
  result = calculation(str1, &x);
  ck_assert_double_eq_tol(result, 10, TOL);

  char const *str2 = "+1";
  result = calculation(str2, NULL);
  ck_assert_double_eq_tol(result, 1, TOL);

  char const *str3 = "1+1";
  result = calculation(str3, NULL);
  ck_assert_double_eq_tol(result, 2, TOL);
}
END_TEST

START_TEST(minus_tests) {
  char const *string = "1-x";
  double x = 10;
  double result = calculation(string, &x);
  ck_assert_double_eq_tol(result, -9, TOL);

  char const *str1 = "-x";
  result = calculation(str1, &x);
  ck_assert_double_eq_tol(result, -10, TOL);

  char const *str2 = "-1";
  result = calculation(str2, NULL);
  ck_assert_double_eq_tol(result, -1, TOL);

  char const *str3 = "1-1";
  result = calculation(str3, NULL);
  ck_assert_double_eq_tol(result, 0, TOL);
}
END_TEST

START_TEST(brackets_tests) {
  char const *string = "(1+x)";
  double const x = 10;
  double result = calculation(string, &x);
  ck_assert_double_eq_tol(result, 11, TOL);

  char const *str1 = "(+x)+1";
  result = calculation(str1, &x);
  ck_assert_double_eq_tol(result, 11, TOL);

  char const *str2 = "1+(-x)";
  result = calculation(str2, &x);
  ck_assert_double_eq_tol(result, -9, TOL);

  char const *str3 = "())))";
  result = calculation(str3, NULL);
  ck_assert_double_nan(result);
}
END_TEST

START_TEST(mult_tests) {
  char const *string = "1*x";
  double const x = 10;
  double result = calculation(string, &x);
  ck_assert_double_eq_tol(result, 10, TOL);

  char const *str1 = "1*1";
  result = calculation(str1, NULL);
  ck_assert_double_eq_tol(result, 1, TOL);
}
END_TEST

START_TEST(div_tests) {
  char const *string = "x/1";
  double const x = 10;
  double result = calculation(string, &x);
  ck_assert_double_eq_tol(result, 10, TOL);

  char const *str1 = "1/1";
  result = calculation(str1, NULL);
  ck_assert_double_eq_tol(result, 1, TOL);
}
END_TEST

START_TEST(mod_tests) {
  char const *string = "x%100";
  double const x = 10;
  double result = calculation(string, &x);
  ck_assert_double_eq_tol(result, 10, TOL);

  char const *str1 = "2%3";
  result = calculation(str1, NULL);
  ck_assert_double_eq_tol(result, 2, TOL);

  char const *str2 = "3%2";
  result = calculation(str2, NULL);
  ck_assert_double_eq_tol(result, 1, TOL);

  char const *str3 = "11.0%0.0";
  result = calculation(str3, NULL);
  ck_assert_double_nan(result);
}
END_TEST

START_TEST(pow_tests) {
  char const *string = "x^0";
  double const x = 10;
  double result = calculation(string, &x);
  ck_assert_double_eq_tol(result, 1, TOL);

  char const *str1 = "1^1";
  result = calculation(str1, NULL);
  ck_assert_double_eq_tol(result, 1, TOL);
}
END_TEST

START_TEST(log_tests) {
  char const *string = "log(x)";
  double const x = 10;
  double result = calculation(string, &x);
  ck_assert_double_eq_tol(result, 1, TOL);

  char const *str1 = "log(0)";
  result = calculation(str1, NULL);
  ck_assert_double_nan(result);
}
END_TEST

START_TEST(ln_tests) {
  char const *string = "ln(x)";
  double const x = 10;
  double result = calculation(string, &x);
  ck_assert_double_eq_tol(result, 2.3025851, TOL);

  char const *str1 = "ln(0)";
  result = calculation(str1, NULL);
  ck_assert_double_nan(result);
}
END_TEST

START_TEST(sin_tests) {
  char const *string = "sin(x)";
  double const x = 10;
  double result = calculation(string, &x);
  ck_assert_double_eq_tol(result, -0.5440211108893698, TOL);

  char const *str1 = "sin(0)";
  result = calculation(str1, NULL);
  ck_assert_double_eq_tol(result, 0.0, TOL);
}
END_TEST

START_TEST(asin_tests) {
  char const *string = "asin(x)";
  double const x = 10;
  double result = calculation(string, &x);
  ck_assert_double_nan(result);

  char const *str1 = "asin(0)";
  result = calculation(str1, NULL);
  ck_assert_double_eq_tol(result, 0.0, TOL);
}
END_TEST

START_TEST(cos_tests) {
  char const *string = "cos(x)";
  double const x = 10;
  double result = calculation(string, &x);
  ck_assert_double_eq_tol(result, -0.8390715290764524, TOL);

  char const *str1 = "cos(0)";
  result = calculation(str1, NULL);
  ck_assert_double_eq_tol(result, 1.0, TOL);
}
END_TEST

START_TEST(acos_tests) {
  char const *string = "acos(x)";
  double const x = 10;
  double result = calculation(string, &x);
  ck_assert_double_nan(result);

  char const *str1 = "acos(0)";
  result = calculation(str1, NULL);
  ck_assert_double_eq_tol(result, 1.5707963267948966, TOL);
}
END_TEST

START_TEST(tan_tests) {
  char const *string = "tan(x)";
  double const x = 10;
  double result = calculation(string, &x);
  ck_assert_double_eq_tol(result, 0.6483608274590866, TOL);

  char const *str1 = "tan(-10)";
  result = calculation(str1, NULL);
  ck_assert_double_eq_tol(result, -0.6483608274590866, TOL);

  char const *str2 = "tan(0)";
  result = calculation(str2, NULL);
  ck_assert_double_eq_tol(result, 0, TOL);
}
END_TEST

START_TEST(atan_tests) {
  char const *string = "atan(x)";
  double const x = 10;
  double result = calculation(string, &x);
  ck_assert_double_eq_tol(result, 1.4711276743037347, TOL);

  char const *str1 = "atan(-10)";
  result = calculation(str1, NULL);
  ck_assert_double_eq_tol(result, -1.4711276743037347, TOL);

  char const *str2 = "atan(0)";
  result = calculation(str2, NULL);
  ck_assert_double_eq_tol(result, 0, TOL);
}
END_TEST

START_TEST(sqrt_tests) {
  char const *string = "sqrt(x)";
  double const x = 10;
  double result = calculation(string, &x);
  ck_assert_double_eq_tol(result, 3.1622776601683795, TOL);

  char const *str1 = "sqrt(-10)";
  result = calculation(str1, NULL);
  ck_assert_double_nan(result);

  char const *str2 = "sqrt(0)";
  result = calculation(str2, NULL);
  ck_assert_double_eq_tol(result, 0, TOL);
}
END_TEST

Suite *suite_calculation(void) {
  Suite *suite = suite_create("Calculation");

  TCase *tests_one = tcase_create("Test all");
  tcase_add_test(tests_one, all_tests);
  suite_add_tcase(suite, tests_one);

  TCase *tests_two = tcase_create("Test plus");
  tcase_add_test(tests_two, plus_tests);
  suite_add_tcase(suite, tests_two);

  TCase *tests_three = tcase_create("Test minus");
  tcase_add_test(tests_three, minus_tests);
  suite_add_tcase(suite, tests_three);

  TCase *tests_four = tcase_create("Test brackets");
  tcase_add_test(tests_four, brackets_tests);
  suite_add_tcase(suite, tests_four);

  TCase *tests_five = tcase_create("Test mult");
  tcase_add_test(tests_five, mult_tests);
  suite_add_tcase(suite, tests_five);

  TCase *tests_six = tcase_create("Test div");
  tcase_add_test(tests_six, div_tests);
  suite_add_tcase(suite, tests_six);

  TCase *tests_seven = tcase_create("Test mod");
  tcase_add_test(tests_seven, mod_tests);
  suite_add_tcase(suite, tests_seven);

  TCase *tests_eight = tcase_create("Test pow");
  tcase_add_test(tests_eight, pow_tests);
  suite_add_tcase(suite, tests_eight);

  TCase *tests_nine = tcase_create("Test log");
  tcase_add_test(tests_nine, log_tests);
  suite_add_tcase(suite, tests_nine);

  TCase *tests_ten = tcase_create("Test ln");
  tcase_add_test(tests_ten, ln_tests);
  suite_add_tcase(suite, tests_ten);

  TCase *tests_eleven = tcase_create("Test sin");
  tcase_add_test(tests_eleven, sin_tests);
  suite_add_tcase(suite, tests_eleven);

  TCase *tests_tvelve = tcase_create("Test asin");
  tcase_add_test(tests_tvelve, asin_tests);
  suite_add_tcase(suite, tests_tvelve);

  TCase *tests_third = tcase_create("Test cos");
  tcase_add_test(tests_third, cos_tests);
  suite_add_tcase(suite, tests_third);

  TCase *tests_fourteen = tcase_create("Test acos");
  tcase_add_test(tests_fourteen, acos_tests);
  suite_add_tcase(suite, tests_fourteen);

  TCase *tests_fifteen = tcase_create("Test tan");
  tcase_add_test(tests_fifteen, tan_tests);
  suite_add_tcase(suite, tests_fifteen);

  TCase *tests_sixteen = tcase_create("Test atan");
  tcase_add_test(tests_sixteen, atan_tests);
  suite_add_tcase(suite, tests_sixteen);

  TCase *tests_seventeen = tcase_create("Test sqrt");
  tcase_add_test(tests_seventeen, sqrt_tests);
  suite_add_tcase(suite, tests_seventeen);

  return suite;
}
