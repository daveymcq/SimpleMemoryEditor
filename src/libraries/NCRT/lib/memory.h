#ifndef _NCRT_MEMORY_H
#define _NCRT_MEMORY_H

/* Zero out a memory location. */

uint64 MemoryZero(void *, uint64);

/* Rewrite of libc memset(). */

void *MemorySet(void *, int8, uint64);

/* Rewrite of libc memcpy(). */

void *MemoryCopy(void *, void *, uint64);

/* Bitwies functions: Set and clear bits, convert bits to bytes and vice-versa. */

BIT *ByteToBits(BYTE, BIT *);
BYTE BitsToByte(BIT *);

boolean CheckBit(void *, uint8);
boolean SetBit(void *, uint8);
boolean ClearBit(void *, uint8);
boolean ToggleBit(void *, uint8);

#endif
