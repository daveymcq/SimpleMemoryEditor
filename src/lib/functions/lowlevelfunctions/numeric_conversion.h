#ifndef _CONVERSION_NUMERIC_H
#define _CONVERSION_NUMERIC_H

#include "types.h"
#include "strings.h"

#include "memory_manipulation.h"

// Convert string to an unsigned 64 bit integer.
// Supports hexadecimal, decimal, and binary conversions.

unsigned long long StringToUnsignedInteger(const char *str, INTFMT base)
{
    unsigned long long result = 0ULL;
    char *pstr = (char *)str;

    switch(base)
    {
        case FMT_INT_BINARY:
        {
            if((*pstr == '0') && (*(pstr + 1) == 'x'))
            {
                result = 0xff00ff00ff00ff00ULL;
                break;
            }

            else if((*pstr == '0') && (*(pstr + 1) == 'b'))
            {
                pstr += 2;
            }

            unsigned short place_values = (unsigned short)StringLength(pstr);

            while(place_values)
            {
                unsigned char data;

                if((*pstr <= '1') && (*pstr >= '0'))
                {
                    data = (unsigned char)(*pstr & 0xF);
                }

                else
                {
                    result = 0xff00ff00ff00ff00ULL;
                    break;
                }

                unsigned long long power = 1ULL;
                unsigned short i;

                for(i = (unsigned short)power; i < place_values; i++)
                {
                    power *= base;
                }

                result += (unsigned long long)(data * power);

                place_values--;
                pstr++;
            }
        }

        break;

        case FMT_INT_DECIMAL:
        {
            if((*pstr == '0') && (*(pstr + 1) == 'x'))
            {
                result = 0xff00ff00ff00ff00ULL;
                break;
            }

            else if((*pstr == '0') && (*(pstr + 1) == 'b'))
            {
                result = 0xff00ff00ff00ff00ULL;
                break;
            }

            unsigned short place_values = (unsigned short)StringLength(pstr);

            while(place_values)
            {
                unsigned char data;

                if((*pstr <= '9') && (*pstr >= '0'))
                {
                    data = (unsigned char)(*pstr & 0xF);
                }

                else
                {
                    result = 0xff00ff00ff00ff00ULL;
                    break;
                }

                unsigned long long power = 1ULL;
                unsigned short i;

                for(i = (unsigned short)power; i < place_values; i++)
                {
                    power *= base;
                }

                result += (unsigned long long)(data * power);

                place_values--;
                pstr++;
            }
        }

        break;

        case FMT_INT_HEXADECIMAL:
        {
            if((*pstr == '0') && (*(pstr + 1) == 'x'))
            {
                pstr += 2;
            }

            else if((*pstr == '0') && (*(pstr + 1) == 'b'))
            {
                result = 0xff00ff00ff00ff00ULL;
                break;
            }

            unsigned short place_values = (unsigned short)StringLength(pstr);

            while(place_values)
            {
                unsigned char data;

                if((*pstr <= 'f') && (*pstr >= 'A'))
                {
                    data = (unsigned char)((*pstr | 0x20) - 0x57);
                }

                else if((*pstr <= '9') && (*pstr >= '0'))
                {
                    data = (unsigned char)(*pstr & 0xF);
                }

                else
                {
                    result = 0xff00ff00ff00ff00ULL;
                    break;
                }

                unsigned long long power = 1ULL;
                unsigned short i;

                for(i = (unsigned short)power; i < place_values; i++)
                {
                    power *= base;
                }

                result += (unsigned long long)(data * power);

                place_values--;
                pstr++;
            }
        }

        break;

        default:

            result = 0xff00ff00ff00ff00ULL;

        break;
    }

    return result;
}

// Convert string to a signed 64 bit integer.
// Supports hexadecimal, decimal, and binary conversions.

long long StringToInteger(const char *str, INTFMT base)
{
    long long result = 0LL;
    char *pstr = (char *)str;
    bool negative = (*pstr == '-');

    if(negative)
    {
       result = (long long)StringToUnsignedInteger(pstr + 1, base);
       result = -result;
    }
    
    else
    {
       result = StringToUnsignedInteger(pstr, base);
    }

    return result;
}

// Convert string to a double precision floating point.
// Supports only decimal conversions.

double StringToDouble(const char *str) 
{
    double result = 0.0;

    char *pstr = (char *)str;

    short exponent = 0;
    unsigned short number_of_digits = 0;
    unsigned short number_of_decimals = 0;
    double power = 10.0;

    bool negative = false;
    int number;

    if(*pstr == '-')
    {
        negative = true;
        pstr++;
    }

    else if(*pstr == '+')
    {
        pstr++;
    }

    while((*pstr <= '9') && (*pstr >= '0')) 
    {
        result = result * power + (*pstr - '0');
        pstr++;
        number_of_digits++;
    }

    if(*pstr == '.') 
    {
        pstr++;

        while((*pstr <= '9') && (*pstr >= '0')) 
        {
            result = result * power + (*pstr - '0');
            pstr++;
            number_of_digits++;
            number_of_decimals++;
        }

        exponent -= number_of_decimals;
    }

    if(number_of_digits == 0) 
    {
        result = 0.0;
        return result;
    }

    if(negative) 
    {
        result = -result;
    }

    if(exponent < -307  || exponent > 1024) 
    {
        result = -1;
        return result;
    }

    number = exponent;

    if(number < 0) 
    {
        number = -number;
    }

    while(number) 
    {
        if(number & 1) 
        {
            if(exponent < 0) 
            {
               result /= power;
            } 

            else 
            {
               result *= power;
            }
        } 

        number >>= 1;
        power *= power;
    }

    return result;
}

#endif
