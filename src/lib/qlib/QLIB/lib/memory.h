#ifndef _QLIB_MEMORY_H
#define _QLIB_MEMORY_H

/* Zero out a memory location. */

uint64 MemoryZero(void *address, uint64 length)
{
    uint64 bytes_zeroed = 0;
    int8 *paddress = (int8 *)address;

    while(length) 
    {
        paddress[bytes_zeroed++] = 0x00;
        length--;
    }

    return bytes_zeroed;
}

/* Rewrite of libc memset(). */

uint64 MemorySet(void *address, int8 value, uint64 length)
{
    uint64 bytes_set = 0;
    int8 *paddress = (int8 *)address;

    while(bytes_set < length)
    {
        paddress[bytes_set] = value;
        bytes_set += sizeof(value);
    }

    return bytes_set;
}

/* Copies memory from one memory location to an other memory location. */

uint64 MemoryCopy(void *to_address, void *from_address, uint64 length_in_bytes)
{
    uint64 bytes_copied = 0;
    int8 *pto = (int8 *)to_address;
    int8 *pfrom = (int8 *)from_address;

    while(length_in_bytes - bytes_copied)
    {
        pto[bytes_copied] = pfrom[bytes_copied];
        bytes_copied++;
    }

    return bytes_copied;
}

/* Bitwies functions. Bits to bytes and vice-versa. */

bit *ByteToBits(uint8 byte, bit *bit_array)
{
    static bit bits[8];
    uint8 bit;
    
    for(bit = 0; bit < 8; bit++)
    {
        bits[bit].value = ((byte >> bit) & 1);
    }

    if(bit_array)
    {
        MemoryCopy((string)bit_array, (string)bits, sizeof(bits));
        return bit_array;
    }

    return bits;
}

byte BitsToByte(bit *bits)
{
    byte byte;
    uint8 bit;

    byte.value = 0x00;

    for(bit = 0; bit < 8; bit++)
    {
        byte.value |= (bits[bit].value << bit);
    }

    return byte;
}

#endif
