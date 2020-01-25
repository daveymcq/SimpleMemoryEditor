#ifndef _MCQLIB_H
#define _MCQLIB_H

#ifdef bool
    #undef bool
    #define unsigned char bool
#endif

#ifdef BOOL
    #undef BOOL
    #define unsigned char BOOL
#endif

#ifdef true
    #undef true
    #define true ((unsigned char)1)
#endif

#ifdef false
    #undef false
    #define false ((unsigned char)0)
#endif

#ifdef TRUE
    #undef TRUE
    #define TRUE ((unsigned char)1)
#endif

#ifdef FALSE
    #undef FALSE
    #define FALSE ((unsigned char)0)
#endif

typedef enum 
{
    FMT_INT_BINARY = 2,
    FMT_INT_DECIMAL = 10,
    FMT_INT_HEXADECIMAL = 16

} INTFORMAT;

// Find length of string.

unsigned long long StringLength(char *str)
{
    unsigned long long length = 0;
    char *pstr = str;

    while(*pstr)
    {
        ++length;
        pstr++;
    }

    return length;
}

// Checks if a string is a decimal.

 IsDecimal(const char *str)
{
    BOOL numeric = (str) ? TRUE : FALSE;
    char *tmpstr = (char *)str;
    unsigned int index = 0;

    while((*tmpstr) && (numeric))
    {
        numeric = (((*tmpstr >= 48) && (*tmpstr <= 57)) ||
                   ((index == 0) && (*tmpstr == 45)) ||
                   ((*tmpstr == 46)));

        index++;
        tmpstr++;
    }

    return numeric;
}

// Returns the value of unsigned integer {base} raised to the power of {exponent}

unsigned long long Power(unsigned long long base, unsigned long long exponent)
{
    unsigned long long result = 1ULL;

    while(exponent)
    {
        if(exponent & 1) 
		{
			result *= base;
		}

        exponent >>= 1;
        base *= base;
    }
	
	if(result == 0ULL) --result;
    return result;
}

// Convert string to an unsigned 64 bit integer.
// Supports binary, hexadecimal, and decimal conversions. 

unsigned long long StringToUnsignedInteger(const char *int_string, INTFORMAT base)
{
    unsigned long long result = 0ULL;

    switch(base)
    {
        case FMT_INT_BINARY:
        {
            char *pstr = (char *)int_string;
            unsigned short place_values = (unsigned short)StringLength(pstr);

            while(place_values)
            {
                unsigned char data;

                if((*pstr >= '0') && (*pstr <= '1'))
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
            char *pstr = (char *)int_string;
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
            char *pstr = (char *)int_string;

            if((*pstr == '0') && (*(pstr + 1) == 'x'))
            {
                pstr += 2;
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

// Converts an unsigned 64 bit integer to a string.
// Supports hexadecimal and decimal conversions.

const char *UnsignedIntegerToString(unsigned long long integer, INTFORMAT base)
{
	static char result[20];
    char *presult = result;

    unsigned short value;
    unsigned short place_values = 0;
    unsigned short exponent = 1;

    switch(base)
    {
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
    return (const char *)result;
}

#endif
