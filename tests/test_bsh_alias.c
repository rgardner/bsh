#include "alias.h"

#include <check.h>
#include <stdlib.h>

#include "test_utils.h"

void
alias_setup()
{
  aliases_init();
}

void
alias_teardown()
{
  aliases_deinit();
}

START_TEST(test_alias_add_expand)
{
  char* argv[] = { "alias", "bob=echo" };
  ck_assert_int_eq(alias(2, argv), 0);

  char* expansion;
  ck_assert_int_eq(alias_exp("bob", &expansion), 1);
  ck_assert_str_eq(expansion, "echo");

  unalias_all();
  free(expansion);
}
END_TEST

START_TEST(test_alias_print_several)
{
  char* argv[] = { "alias", "bob=echo" };
  ck_assert_int_eq(alias(2, argv), 0);

  char* argv2[] = { "alias", "harry=echo" };
  ck_assert_int_eq(alias(2, argv2), 0);

  char* argv_print[] = { "alias", "bob", "harry" };
  ck_assert_int_eq(alias(3, argv_print), 0);

  unalias_all();
}
END_TEST

START_TEST(test_alias_print_some_do_not_exist)
{
  char* argv[] = { "alias", "bob=echo" };
  ck_assert_int_eq(alias(2, argv), 0);

  char* argv_print[] = { "alias", "bob", "harry" };
  ck_assert_int_ne(alias(3, argv_print), 0);

  unalias_all();
}
END_TEST

START_TEST(test_alias_print_all)
{
  char* argv[] = { "alias", "bob=echo" };
  ck_assert_int_eq(alias(2, argv), 0);

  char* argv2[] = { "alias", "harry=echo" };
  ck_assert_int_eq(alias(2, argv2), 0);

  char* argv_print[] = { "alias" };
  ck_assert_int_eq(alias(1, argv_print), 0);

  unalias_all();
}
END_TEST

START_TEST(test_unalias_no_args)
{
  char* argv[] = { "unalias" };
  ck_assert_int_ne(unalias(1, argv), 0);
}
END_TEST

START_TEST(test_unalias_exists)
{
  char* argv_alias[] = { "alias", "bob=echo" };
  ck_assert_int_eq(alias(2, argv_alias), 0);

  char* argv_unalias[] = { "unalias", "bob" };
  ck_assert_int_eq(unalias(2, argv_unalias), 0);

  char* expansion;
  alias_exp("bob", &expansion);
  ck_assert(!expansion);

  unalias_all();
}
END_TEST

START_TEST(test_unalias_does_not_exist)
{
  char* argv[] = { "unalias", "bob" };
  ck_assert_int_ne(unalias(2, argv), 0);
}
END_TEST

START_TEST(test_unalias_remove_all)
{
  // Arrange
  char* argv_alias[] = { "alias", "bob=echo" };
  ck_assert_int_eq(alias(2, argv_alias), 0);

  char* argv_alias2[] = { "alias", "harry=echo" };
  ck_assert_int_eq(alias(2, argv_alias2), 0);

  // Act
  char* argv[] = { "unalias", "-a" };
  const int unalias_result = unalias(2, argv);

  // Assert
  ck_assert_int_eq(unalias_result, c_bsh_alias_error_success);
  char* expanded = NULL;
  ck_assert_int_eq(alias_exp("bob", &expanded), 0);
  ck_assert_int_eq(alias_exp("harry", &expanded), 0);

  unalias_all();
}
END_TEST

START_TEST(test_alias_expand_does_not_exist)
{
  char* expansion = NULL;
  ck_assert_int_eq(alias_exp("alias", &expansion), 0);
  bsh_assert_ptr_null(expansion);
}
END_TEST

Suite*
make_alias_suite()
{
  Suite* s = suite_create("Alias");
  TCase* tc = tcase_create("Core");
  suite_add_tcase(s, tc);
  tcase_add_checked_fixture(tc, alias_setup, alias_teardown);

  tcase_add_test(tc, test_alias_add_expand);
  tcase_add_test(tc, test_alias_print_several);
  tcase_add_test(tc, test_alias_print_some_do_not_exist);
  tcase_add_test(tc, test_alias_print_all);
  tcase_add_test(tc, test_unalias_no_args);
  tcase_add_test(tc, test_unalias_exists);
  tcase_add_test(tc, test_unalias_does_not_exist);
  tcase_add_test(tc, test_unalias_remove_all);
  tcase_add_test(tc, test_alias_expand_does_not_exist);

  return s;
}
