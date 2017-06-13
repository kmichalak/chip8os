#include "inc/types.h"
#include "inc/display.h"
#include "inc/gdt.h"

uint16_t detect_bios_area_hardware(void) {
	const uint16_t* bda_detected_hardware_ptr = (const uint16_t*) 0x410;
	return *bda_detected_hardware_ptr;
}

enum video_type get_bios_area_video_type() {
	return (enum video_type) (detect_bios_area_hardware() & 0x30);
}

void clear_screen(void) {
	int8_t *vidptr = (int8_t *) 0xb8000;
	unsigned int j = 0;

	// Clear screen 80x20
	while (j< 80 * 25 * 2) {
		vidptr[j] = ' ';
		vidptr[j + 1] = 0x07;
		j = j + 2;
	}
}

void print(int8_t* str) {
	clear_screen();
	int8_t *vidptr = (int8_t *) 0xb8000;

	unsigned int i = 0;
	unsigned int j = 0;
	
	while(str[i] != '\0') {
		vidptr[j] = str[i];
		vidptr[j + 1] = VGA_COLOR_LIGHT_GREY;
		++i;
		j = j + 2;
	}
}

void kmain(void* info, uint32_t magic) {
	
	
	enum video_type vid = get_bios_area_video_type();

	switch (vid) {
		case VIDEO_TYPE_NONE:
			print("VIDEO_TYPE_NONE");
			break;
		case VIDEO_TYPE_COLOR:
			print("VIDEO_TYPE_COLOR");
			break;
		case VIDEO_TYPE_MONOCHROME:
			print("VIDEO_TYPE_MONOCHROME");
			break;
	}

	print("Everything works fine.\n");

	gdt_t gdt;
	gdt.null_segment = create_descriptor(0, 0, 0);
	gdt.unused_segment = create_descriptor(0, 0, 0);
	gdt.code_segment = create_descriptor(0, 64 * 1024 * 1024, 0x9A);
	gdt.data_segment = create_descriptor(0, 64 * 1024 * 1024, 0x92);
	
	gdt_ptr_t gdt_ptr;
	gdt_ptr.base = (uint32_t*) &gdt;
	gdt_ptr.limit = sizeof(gdt) - 1;

	// call some sexy function to set the GDT

	asm volatile("lgdt (%0)": :"p" (((uint8_t*) &gdt_ptr) + 1));

	print("GDT set.");

	while (1);
}

