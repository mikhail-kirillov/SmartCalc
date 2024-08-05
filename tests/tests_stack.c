// Copyright [2023] <Burr Etienne>

#include "../tests/tests_header.h"

START_TEST(stack_tests_overload) {
  calc_token_stack stack = {};
  calc_stack_init(&stack, 1);

  calc_token op = {};
  calc_token_set_operator(&op, '+');

  calc_stack_push(&stack, op);
  calc_stack_push(&stack, op);

  ck_assert_int_eq(
      calc_stack_get_token_from_top(&stack).calc_data.calc_other.data, '+');

  calc_stack_free(&stack);
}
END_TEST

// START_TEST(stack_tests_over_malloc) {
//   calc_token_stack stack = {};
//   calc_stack_init(&stack, 1);

//   calc_token op = {};
//   calc_token_set_operator(&op, '+');

//   // Malloc check. So long
//   while (stack.size != 0) {
//     calc_stack_push(&stack, op);
//   }

//   calc_stack_free(&stack);
// }
// END_TEST

Suite *suite_stack(void) {
  Suite *suite = suite_create("Stack");

  TCase *tests_one = tcase_create("Test stack overload");
  tcase_add_test(tests_one, stack_tests_overload);
  suite_add_tcase(suite, tests_one);

  // So long
  // TCase *tests_two = tcase_create("Test stack over malloc");
  // tcase_set_timeout(tests_two, 2000);
  // tcase_add_test(tests_two, stack_tests_over_malloc);
  // suite_add_tcase(suite, tests_two);

  return suite;
}
