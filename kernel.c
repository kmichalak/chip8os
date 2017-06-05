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

void kmain(void) {
	
	clear_screen();
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
	return;
}

