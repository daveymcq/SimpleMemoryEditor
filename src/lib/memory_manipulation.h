#ifndef _MANIPULATION_MEMORY_H
#define _MANIPULATION_MEMORY_H

// Zero out a memory location.

unsigned int MemoryZero(void *address, unsigned long long length)
{
    unsigned int bytes_zeroed = 0;
    char *paddress = (char *)address;

    while(length) 
    {
        paddress[bytes_zeroed++] = 0x00;
        length--;
    }

    return bytes_zeroed;
}

unsigned int MemorySet(void *address, char value, unsigned int length)
{
    unsigned int bytes_set = 0;
    char *paddress = (char *)address;

    while(bytes_set < length)
    {
        paddress[bytes_set] = value;
        bytes_set += sizeof(value);
    }

    return bytes_set;
}


// Copies memory from one memory location to an other memory location.

unsigned int MemoryCopy(void *to_address, void *from_address, unsigned int length_in_bytes)
{
    unsigned int bytes_copied = 0;
    char *pto = (char *)to_address;
    char *pfrom = (char *)from_address;

    while(length_in_bytes - bytes_copied)
    {
        pto[bytes_copied] = pfrom[bytes_copied];
        bytes_copied++;
    }

    return bytes_copied;
}

bit *ByteToBits(char byte, bit *bit_array)
{
    static bit bits[8];
    unsigned char bit;
    
    for(bit = 0; bit < 8; bit++)
    {
        bits[bit].value = ((byte >> bit) & 1);
    }

    if(bit_array)
    {
        MemoryCopy((char *)bit_array, (char *)bits, sizeof(bits));
        return bit_array;
    }

    return bits;
}

byte BitsToByte(bit *bits)
{
    byte byte;
    unsigned char bit;

    byte.value = 0x00;

    for(bit = 0; bit < 8; bit++)
    {
        byte.value |= (bits[bit].value << bit);
    }

    return byte;
}

#endif
