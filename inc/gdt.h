#ifndef __GDT_H
#define __GDT_H

#include "types.h"

/* 
A segment descriptor is a 64-bits long data structure that contains data
in the following order:
 
 ____________________________________________________________________________
|31__________________________________16|15__________________________________0|
|                                      |                                     |
|              BASE 0:15               |              Limit 0:15             |                           
|                                      |                                     |
|______________________________________|_____________________________________|
|63______________56|55______52|51____48|47______________40|39______________32|
|                  |          |        |                  |                  |
|   Base 24:31     |   Flags  | Limit  |   Access Byte    |   Base 16:23     |
|                  |          |        |                  |                  |
|__________________|__________|________|__________________|__________________|


1010 0101 1100 0011 0000 0000 0000 0000 
0101 0110 


For the 32bit architecture four bits of flags are:
7: Gr - Granularity bit. If 0 the limit is in 1B blocks (byte granularity),
	if 1 the limit is in 4KiB blocks (page granularity).
6: Sz - Size bit. If 0 the selector defines 16 bit protected mode. 
	If 1 it defines 32 bit protected model. You can have both 16 and 32 bit 
	selectors at once.
5: 0 - this value should always be set to 0 for 32bit architecture. 
	In case of 64bit architecture this is an 'L' bit that is used to indicate
	x86-64 descriptor. 
4: 0 - this value should always be set to 0 for 32bit architecture. 
	For 64bit architecture this is a 'Sz' bit which has to be 0 when the 'L' bit
	is set, as the combination Sz = 1, L = 1 is reversed for future use 
	(and will throw an exception if you try to use it).

*/

typedef struct seg_desc {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t access;
	uint8_t granularity;
	uint8_t base_high;
} __attribute__((packed)) seg_desc_t;


typedef struct gdt {
	seg_desc_t null_segment;
	seg_desc_t unused_segment;
	seg_desc_t code_segment;
	seg_desc_t data_segment;
} __attribute__((packed)) gdt_t ;


typedef struct gdt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) gdt_ptr_t;


#define SEG_SET 1
#define SET_CLEAR 0

/* Granularity (0 for 1B - 1MB or 1 for 4KB - 4GB). */
#define SEG_GRAN(x) ((x) << 0x07)	
/* Size (0 for 16-bit, 1 for 32-bit). */
#define SEG_SIZE(x) ((x) << 0x06)
/* Long mode (64-bit). */
#define SEG_LONG(x) ((x) << 0x05)
/* Available for system use. */
#define SEG_SAVL(x) ((x) << 0x04)


#define MODE_16BIT (uint8_t) (SEG_GRAN(0) | SEG_SIZE(1) | \
							  SEG_LONG(0) | SEG_SAVL(0))
#define MODE_32BIT (uint8_t) (SEG_GRAN(1) | SEG_SIZE(1) | \
							  SEG_LONG(0) | SEG_SAVL(0))

// Only 32-bit segments are supported at now

uint32_t limit(seg_desc_t* descriptor);
uint32_t base(seg_desc_t* descriptor);
seg_desc_t create_descriptor(uint32_t base, uint32_t limit, uint8_t flags);

#endif