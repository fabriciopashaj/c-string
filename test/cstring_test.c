#include "lib.h"
#include "testing.h"

void test_String_new(void) {
	String str = String_new();
	CU_ASSERT_PTR_NOT_NULL_FATAL(str);
	CU_ASSERT_EQUAL_FATAL(String_len(str), 0);
	CU_ASSERT_EQUAL_FATAL(StringBuffer_len(str), 1);
	String_cleanup(str);
}
void test_String_from_bytes(void) {
	String str = String_from_bytes("ab\0cd\x1b", 6);
	CU_ASSERT_PTR_NOT_NULL_FATAL(str);
	CU_ASSERT_EQUAL_FATAL(String_len(str), 6);
	CU_ASSERT_EQUAL_FATAL(StringBuffer_len(str), 7);
	String_cleanup(str);
}
void test_String_from(void) {
	String str = String_from("random string");
	CU_ASSERT_PTR_NOT_NULL_FATAL(str);
	CU_ASSERT_EQUAL_FATAL(String_len(str), 13);
	CU_ASSERT_EQUAL_FATAL(strlen(str), 13);
	CU_ASSERT_EQUAL_FATAL(StringBuffer_len(str), 14);
	String_cleanup(str);
}
void test_String_clone(void) {
	String str1 = String_from("another random string");
	String str2 = String_clone(str1);
	CU_ASSERT_STRING_EQUAL_FATAL(str1, str2);
	String_cleanup(str1);
	String_cleanup(str2);
}
void test_String_concat(void) {
	String str1 = String_from("foo ");
	String str2 = String_from("bar");
	String str = String_concat(str1, str2);
	CU_ASSERT_EQUAL_FATAL(String_len(str),
							String_len(str1) + String_len(str2));
	CU_ASSERT_STRING_EQUAL_FATAL(str, "foo bar");
}
void test_String_slice(void) {
	String str = String_from("out of ideas");
	CU_ASSERT_PTR_NOT_NULL_FATAL(str);
	String slice1 = String_slice(str, 0, 3);
	CU_ASSERT_STRING_EQUAL_FATAL(slice1, "out");
	String slice2 = String_slice(str, 4, -1);
	CU_ASSERT_STRING_EQUAL_FATAL(slice2, "of ideas");
	String slice3 = String_slice(str, -6, -1);
	CU_ASSERT_STRING_EQUAL_FATAL(slice3, "ideas");
	String slice4 = String_slice(str, -9, 6);
	CU_ASSERT_STRING_EQUAL_FATAL(slice4, "of");
	String_cleanup(str);
	String_cleanup(slice1);
	String_cleanup(slice2);
	String_cleanup(slice3);
	String_cleanup(slice4);
}
void test_String_equal(void) {
	String str1 = String_from("foo");
	String str2 = String_clone(str1);
	String str3 = String_from("bar");
	CU_ASSERT_FATAL(String_equal(str1, str2));
	CU_ASSERT_FATAL(!String_equal(str1, str3));
}

void test_StringBuffer_push_bytes(void) {
	StringBuffer sb = StringBuffer_new();
	CU_ASSERT_PTR_NOT_NULL_FATAL(sb);
	CU_ASSERT_EQUAL_FATAL(
			StringBuffer_push_bytes(&sb, "abcdef\0\x34", 8), 0);
	CU_ASSERT_EQUAL_FATAL(StringBuffer_len(sb), 8);
	CU_ASSERT_FATAL(!memcmp(sb, "abcdef\0\x34", 8));
	StringBuffer_cleanup(sb);
}
void test_StringBuffer_push_str(void) {
	StringBuffer sb = StringBuffer_new();
	CU_ASSERT_EQUAL_FATAL(StringBuffer_push_str(&sb, "lejs me limon"), 0);
	CU_ASSERT_EQUAL_FATAL(StringBuffer_len(sb), 13);
	CU_ASSERT_FATAL(!memcmp(sb, "lejs me limon", 13));
}

int main(int argc, char **argv) {
	int status = 0;
	CU_TestInfo String_tests[] = {
		{ "new",        test_String_new        },
		{ "from_bytes", test_String_from_bytes },
		{ "from",       test_String_from       },
		{ "clone",      test_String_clone	   },
		{ "concat",     test_String_concat     },
		{ "slice",      test_String_slice      },
		{ "equal",      test_String_equal      },
		CU_TEST_INFO_NULL
	};
	CU_TestInfo StringBuffer_tests[] = {
		{ "push_bytes", test_StringBuffer_push_bytes },
		{ "push_str",   test_StringBuffer_push_str   },
		CU_TEST_INFO_NULL
	};
	CU_SuiteInfo suites[] = {
		{ "String",       NULL, NULL, NULL, NULL, String_tests       },
		{ "StringBuffer", NULL, NULL, NULL, NULL, StringBuffer_tests },
		CU_SUITE_INFO_NULL
	};
	CU_initialize_registry();
	if ((status = CU_register_suites(suites)) != CUE_SUCCESS)
		goto cleanup;
	RUN_TESTS;
cleanup:
	CU_cleanup_registry();
	return status;
}