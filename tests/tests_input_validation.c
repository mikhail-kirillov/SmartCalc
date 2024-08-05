// Copyright [2023] <Burr Etienne>

#include "../tests/tests_header.h"

START_TEST(input_validation_normal) {
  char const *string =
      "+1-2.22*(-3/"
      "1%x)^0*ln(1)+log(10)-sin(2)+cos(3)-asin(1)+acos(1)*tan(1)-atan(1)/"
      "sqrt(4)-10";
  char const *x = "";
  int x_in_exp = 0;
  int x_is_ok = 0;
  int exp_is_ok = 0;
  input_validation(string, x, &x_in_exp, &exp_is_ok, &x_is_ok);

  ck_assert_int_eq(x_in_exp, 1);
  ck_assert_int_eq(x_is_ok, 0);
  ck_assert_int_eq(exp_is_ok, 1);
}
END_TEST

START_TEST(input_validation_exp_error) {
  char const *string =
      "nn+1-2.22*(-3/"
      "1%x)^0*ln(1)+log(10)-sin(2)+cos(3)-asin(1)+acos(1)*tan(1)-atan(1)/"
      "sqrt(4)-10";
  char const *x = "1.1";
  int x_in_exp = 0;
  int x_is_ok = 0;
  int exp_is_ok = 0;
  input_validation(string, x, &x_in_exp, &exp_is_ok, &x_is_ok);

  ck_assert_int_eq(x_in_exp, 0);
  ck_assert_int_eq(x_is_ok, 1);
  ck_assert_int_eq(exp_is_ok, 0);
}
END_TEST

START_TEST(input_validation_exp_error_and_x_error) {
  char const *string =
      "nn+1-2.22*(-3/"
      "1%x)^0*ln(1)+log(10)-sin(2)+cos(3)-asin(1)+acos(1)*tan(1)-atan(1)/"
      "sqrt(4)-10";
  char const *x = "1.n1";
  int x_in_exp = 0;
  int x_is_ok = 0;
  int exp_is_ok = 0;
  input_validation(string, x, &x_in_exp, &exp_is_ok, &x_is_ok);

  ck_assert_int_eq(x_in_exp, 0);
  ck_assert_int_eq(x_is_ok, 0);
  ck_assert_int_eq(exp_is_ok, 0);
}
END_TEST

Suite *suite_input_validation(void) {
  Suite *suite = suite_create("Input validation");

  TCase *tests_one = tcase_create("Test input validation normal");
  tcase_add_test(tests_one, input_validation_normal);
  suite_add_tcase(suite, tests_one);

  TCase *tests_two = tcase_create("Test input validation exp error");
  tcase_add_test(tests_two, input_validation_exp_error);
  suite_add_tcase(suite, tests_two);

  TCase *tests_three =
      tcase_create("Test input validation exp error and x error");
  tcase_add_test(tests_three, input_validation_exp_error_and_x_error);
  suite_add_tcase(suite, tests_three);

  return suite;
}
