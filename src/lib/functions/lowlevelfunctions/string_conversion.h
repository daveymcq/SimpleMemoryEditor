#ifndef _CONVERSION_STRING_H
#define _CONVERSION_STRING_H

#include "types.h"
#include "strings.h"
#include "math.h"

#include "memory_manipulation.h"

// Converts an unsigned 64 bit integer to a string.
// Supports hexadecimal, decimal, and binary conversions.

const char *UnsignedIntegerToString(unsigned long long integer, char *out_result, unsigned int out_result_length, INTFMT base)
{
    static char result[65];

    char *presult = result;
    unsigned short value;
    unsigned short place_values = 0;
    unsigned short exponent = 1;

    switch(base)
    {
        case FMT_INT_BINARY:

            do
            {
                value = (unsigned short)(integer / Power(base, exponent));
                exponent++;
                place_values++;

            } while((value > 0) && (place_values < 64));

            while(place_values)
            {
                value = (unsigned short)(((integer % Power(base, place_values)) - (integer % Power(base, place_values - 1))) / Power(base, place_values - 1));

                char data = (value | 0x30);

                *presult = data;
                presult++;
                place_values--;
            }

        break;

        case FMT_INT_DECIMAL:

            do
            {
                value = (unsigned short)(integer / Power(base, exponent));
                exponent++;
                place_values++;

            } while((value > 0) && (place_values < 20));

            while(place_values)
            {
                value = (unsigned short)(((integer % Power(base, place_values)) - (integer % Power(base, place_values - 1))) / Power(base, place_values - 1));

                char data = (value | 0x30);

                *presult = data;
                presult++;
                place_values--;
            }

        break;

        case FMT_INT_HEXADECIMAL:

            do
            {
                value = (unsigned short)(integer / Power(base, exponent));
                exponent++;
                place_values++;

            } while((value > 0) && (place_values < 16));

            while(place_values)
            {
                value = (unsigned short)(((integer % Power(base, place_values)) - (integer % Power(base, place_values - 1))) / Power(base, place_values - 1));

                char data = (value | 0x30);

                if(value > 9)
                {
                    data += 7;
                }

                *presult = data;
                presult++;
                place_values--;
            }

        break;
    }

    *presult = 0;

    if(out_result)
    {
        MemoryCopy(out_result, result, out_result_length);
        return (const char *)out_result;
    }

    return (const char *)result;
}


// Converts a signed 64 bit integer to a string.
// Supports hexadecimal, decimal, and binary conversions.

const char *SignedIntegerToString(long long integer, char *out_result, unsigned int out_result_length, INTFMT base)
{
    static char result[66];
    char *presult = (char *)result;
    bool negative = (integer < 0);

    if(negative)
    {
        *presult++ = '-';
        integer = -integer;
    }

    UnsignedIntegerToString(integer, presult, sizeof(presult), base);

    if(out_result)
    {
        MemoryCopy(out_result, result, out_result_length);
        return (const char *)out_result;
    }

    return (char *)result;
}

const char *IntegerToString(long long integer, char *out_result, unsigned int out_result_length, INTFMT base)
{
    if(integer >= 0)
    {
        return UnsignedIntegerToString((unsigned long long)integer, out_result, out_result_length, base);
    }

    return SignedIntegerToString(integer, out_result, out_result_length, base);
}

// Convert double precision floating point number to a string.
// Supports only decimal conversions.

const char *DoubleToString(double number, char *out_result, unsigned int out_result_length)
{
    static char result[83];
    char *presult = result;

    bool negative = (number < 0);

    long long whole = (long long)number;
    double fraction = (number - whole);

    INTFMT format = FMT_INT_DECIMAL;

    IntegerToString(whole, presult, 20, format);

    presult += StringLength(presult);
    *presult++ = '.';
    fraction *= Power(10, 15);

    if(negative)
    {
        fraction = -fraction;
    }

    IntegerToString((long long)fraction, presult, 60, format);

    if(out_result)
    {
        MemoryCopy(out_result, result, out_result_length);
        return (const char *)out_result;
    }

    return (const char *)result;
}

#endif
