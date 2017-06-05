#include "inc/gdt.h"

uint32_t limit(seg_desc_t* descriptor) {
	uint8_t* descriptor_bytes = (uint8_t*) descriptor;
	uint32_t limit = descriptor_bytes[6] & 0xf;
	limit = (limit << 8) + descriptor_bytes[1];
	limit = (limit << 8) + descriptor_bytes[0];
	return limit;
}

uint32_t base(seg_desc_t* descriptor) {
	uint8_t* descriptor_bytes = (uint8_t*) descriptor;
	uint32_t base = descriptor_bytes[7];
	base = (base << 8) + descriptor_bytes[4];
	base = (base << 8) + descriptor_bytes[3];
	base = (base << 8) + descriptor_bytes[2];
	return base;
}

seg_desc_t create_descriptor(uint32_t base, uint32_t limit, uint8_t flags) {
	
	seg_desc_t descriptor = 0;
	uint8_t* descriptor_bytes = (uint8_t*) &descriptor;

	if (limit > 65536) {
		if ((limit & 0xfff) != 0xfff) {
			// this is a wrong case, you are trying to address something bad
			// we can fix it by setting all 12 bits of the limit to 1
			// or we can raise some error  
		} else {
			descriptor_bytes[6] = MODE_32BIT;
			// Adjust granularity, as all values should change by 4096 bytes.
			limit = (limit >> 12);
		}
	} else {
		descriptor_bytes[6] = MODE_16BIT;
	}

	uint8_t* base_bytes = (uint8_t*) &base;	
	descriptor_bytes[2] = base_bytes[0] & 0xFF;
	descriptor_bytes[3] = base_bytes[1] & 0xFF;
	descriptor_bytes[4] = base_bytes[2] & 0xFF;
	descriptor_bytes[7] = base_bytes[3] & 0xFF;

	uint8_t* limit_bytes = (uint8_t*) &limit;
	descriptor_bytes[0] = limit_bytes[0];
	descriptor_bytes[1] = limit_bytes[1];
	descriptor_bytes[6] |= (limit_bytes[2] & 0xF); // do not overwrite flags

	descriptor_bytes[5] = flags;

	return descriptor;
}
