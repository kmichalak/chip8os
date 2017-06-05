#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>


#include <stdio.h>
#include "../inc/gdt.h"

static void test_mode_16bit_flag_value(void **state) {
	assert_true(MODE_16BIT == 0x40);
}

static void test_mode_32bit_flag_value(void **state) {
	assert_true(MODE_32BIT == 0xC0);
}

static void test_create_descriptor(void **state) {

	uint32_t base = 0;
	uint32_t limit = 65536;
	uint8_t flags = 0x9A;

	seg_desc_t descriptor = create_descriptor(base, limit, flags);

	assert_true(descriptor == 0x419a0000000000);
}

int main(int argc, char **varg) {

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_mode_16bit_flag_value),
		cmocka_unit_test(test_mode_32bit_flag_value),
		cmocka_unit_test(test_create_descriptor)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}