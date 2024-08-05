// Copyright [2023] <Burr Etienne>

#include "../tests/tests_header.h"

int main(void) {
  SRunner *suite_runner = srunner_create(suite_annuitet_credit_calculation());
  Suite *list_cases[] = {suite_diff_credit_calculation(), suite_calculation(),
                         suite_input_validation(), suite_stack(), NULL};

  for (Suite **testcase = list_cases; *testcase != NULL; testcase++) {
    srunner_add_suite(suite_runner, *testcase);
  }

  srunner_run_all(suite_runner, CK_NORMAL);
  int failed_tests = 0;
  failed_tests = srunner_ntests_failed(suite_runner);
  srunner_free(suite_runner);

  return (failed_tests == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
