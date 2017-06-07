#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>


#include <stdio.h>
#include "../inc/gdt.h"

static void assert_seg_desc_equal(seg_desc_t* current, seg_desc_t* expected) {
	assert_true(current->limit_low == expected->limit_low);
	assert_true(current->base_low == expected->base_low);
	assert_true(current->base_mid == expected->base_mid);
	assert_true(current->access == expected->access);
	assert_true(current->granularity == expected->granularity);
	assert_true(current->base_high == expected->base_high);
}

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

	seg_desc_t expected = {
		.limit_low = 0x0000,
		.base_low = 0x0000,
		.base_mid = 0x00,
		.access = 0x9A,
		.granularity = 0x41,
		.base_high = 0x00
	};

	assert_seg_desc_equal(&descriptor, &expected);
}

static void test_create_descriptor_for_32bit_mode(void **state) {
	uint32_t base = 0;
	uint32_t limit = 327679;
	uint8_t flags = 0x9A;

	seg_desc_t descriptor = create_descriptor(base, limit, flags);

	seg_desc_t expected = {
		.limit_low = 0x004F,
		.base_low = 0x0000,
		.base_mid = 0x00,
		.access = 0x9A,
		.granularity = 0xC0,
		.base_high = 0x00
	};

	assert_seg_desc_equal(&descriptor, &expected);
}

static void test_limit_selector(void** state) {
	seg_desc_t descriptor = {
		.limit_low = 0x0000,
		.base_low = 0x0000,
		.base_mid = 0x00,
		.access = 0x9A,
		.granularity = 0x41,
		.base_high = 0x00
	};

	uint32_t limit_val = limit(&descriptor);

	assert_true(limit_val == 65536);
}

static void test_large_limit_selector(void** state) {
	seg_desc_t descriptor = {
		.limit_low = 0x004F,
		.base_low = 0x0000,
		.base_mid = 0x00,
		.access = 0x9A,
		.granularity = 0xC0,
		.base_high = 0x00
	};

	uint32_t limit_val = limit(&descriptor);

	assert_true(limit_val == 327679);
}

static void test_base_selector(void **state) {
	seg_desc_t descriptor = {
		.limit_low = 0xFFFF,
		.base_low = 0x5600,
		.base_mid = 0x07,
		.access = 0x41,
		.granularity = 0xC4,
		.base_high = 0x00
	};

	uint32_t base_val = base(&descriptor);

	assert_true(base_val == 0x75600);
}

static void test_large_base_selector(void** state) {
	seg_desc_t descriptor = {
		.limit_low = 0x0000,
		.base_low = 0x5600,
		.base_mid = 0x9A,
		.access = 0x41,
		.granularity = 0xC3,
		.base_high = 0xA5
	};

	uint32_t base_val = base(&descriptor);

	assert_true(base_val == 0xA59A5600);
}



int main(int argc, char **varg) {

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_mode_16bit_flag_value),
		cmocka_unit_test(test_mode_32bit_flag_value),
		cmocka_unit_test(test_create_descriptor),
		cmocka_unit_test(test_create_descriptor_for_32bit_mode),
		cmocka_unit_test(test_limit_selector),
		cmocka_unit_test(test_large_limit_selector),
		cmocka_unit_test(test_base_selector),
		cmocka_unit_test(test_large_base_selector)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

