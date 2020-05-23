#ifndef _bool_TYPES_H
#define _bool_TYPES_H

#ifndef bool
#define bool unsigned char
#endif

#ifndef true
#define true 0x01
#endif

#ifndef false
#define false 0x00
#endif

typedef enum 
{
    FMT_INT_BINARY = 2,
    FMT_INT_DECIMAL = 10,
    FMT_INT_HEXADECIMAL = 16

} _MCQLIBC_INTFMT;

#pragma pack(push, 1)

typedef union
{
    struct 
    {
        bool value : 1;
    };

} _MCQLIBC_BIT;

typedef union
{
    struct
    {
        bool bit_1 : 1;
        bool bit_2 : 1;
        bool bit_3 : 1;
        bool bit_4 : 1;
        bool bit_5 : 1;
        bool bit_6 : 1;
        bool bit_7 : 1;
        bool bit_8 : 1;

    } bits;

    char value;

} _MCQLIBC_BYTE;

#pragma pack(pop)

#ifdef INTFMT
#undef INTFMT
#endif

#ifdef bit
#undef bit
#endif

#ifdef byte
#undef byte
#endif

#define INTFMT _MCQLIBC_INTFMT
#define bit _MCQLIBC_BIT
#define byte _MCQLIBC_BYTE

#endif
