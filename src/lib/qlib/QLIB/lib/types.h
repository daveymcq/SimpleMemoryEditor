#ifndef _QLIB_TYPES_H
#define _QLIB_TYPES_H

#ifndef uint8
    #define uint8 unsigned char
#endif

#ifndef uint16
    #define uint16 unsigned short
#endif

#ifndef uint32
   #define uint32 unsigned int
#endif

#ifndef uint64
    #define uint64 unsigned long long
#endif

#ifndef int8
    #define int8 char
#endif

#ifndef int16
    #define int16 short
#endif

#ifndef int32
   #define int32 int
#endif

#ifndef int64
    #define int64 long long
#endif

#ifndef wchar
    #define wchar uint16
#endif

#ifndef null
    #define null 0
#endif

#ifndef EXIT_SUCCESS
    #define EXIT_SUCCESS 0
#endif

#ifndef EXIT_FAILURE
    #define EXIT_FAILURE -1
#endif

#if defined(_MSC_VER)
    #if (_MSC_VER >= 1400)
        #ifndef uint64
             #define uint64 unsigned __int64
        #endif
        
        #ifndef int64
             #define int64 __int64
        #endif
    #endif
#endif

#ifndef string
    #define string int8 *
#endif

#ifndef wstring
    #define wstring wchar *
#endif

#ifndef bool
    #define bool unsigned char
#endif

#ifndef true
    #define true 0x01
#endif

#ifndef false
    #define false 0x00
#endif

#ifdef BIT
    #undef BIT
#endif

#ifdef bit
    #undef bit
#endif

#ifdef BYTE
    #undef BYTE
#endif

#ifdef byte
    #undef byte
#endif

typedef enum _INTFMT
{
    FMT_INT_BINARY = 2,
    FMT_INT_DECIMAL = 10,
    FMT_INT_HEXADECIMAL = 16

} INTFMT;

#pragma pack(push, 1)

typedef struct _BIT
{
    bool value : 1; 

} QBIT, qbit;

typedef union _BYTE
{
    struct _BITS
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

    int8 value;

} QBYTE, qbyte;

#define BIT QBIT
#define bit qbit

#define BYTE QBYTE
#define byte qbyte

#pragma pack(pop)

#endif
