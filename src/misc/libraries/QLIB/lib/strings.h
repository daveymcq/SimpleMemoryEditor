#ifndef _QLIB_STRINGS_H
#define _QLIB_STRINGS_H

/* Copies a string from one memory location to an other memory location. */

uint32 CopyString(cstring to, cstring from, uint32 length)
{
    uint32 bytes_copied = 0;

    while(bytes_copied < length)
    {
        uint8 byte = (to[bytes_copied] = from[bytes_copied]);
        ++bytes_copied;

        if(!byte) break;
    }

    return bytes_copied;
}

/* Find length of string. Rewrite of libc strlen(). */

uint32 StringLength(const cstring str)
{
    uint32 length = 0;
    cstring pstr = (cstring)str;

    while(*pstr)
    {
        ++length;
        pstr++;
    }

    return length;
}

// Join two strings.

cstring StringConcat(cstring str1, cstring str2)
{
    CopyString(str1 + StringLength(str1), str2, StringLength(str2));
    return str1;
}

/* Checks equality of two c-strings. */

bool StringCompare(const cstring string_a, const cstring string_b, bool case_sensitive)
{
    bool result;

    if(result = (StringLength(string_a) == StringLength(string_b)))
    {
        uint32 offset;
        
        for(offset = 0; result && (offset < StringLength(string_a)); offset++)
        {
            result = (string_a[offset] == string_b[offset]);
            if(!result && !case_sensitive) result = ((string_a[offset] | 0x20) == (string_b[offset] | 0x20));
        }
    }

    return result;
}

/* Finds first occurrence of a string within another string. Returns the index into the string or zero if non existent. */

int32 FindFirstOccurrenceOfString(const cstring haystack, const cstring needle, bool case_sensitive)
{
    int32 index = 0;
    uint32 needle_length = StringLength(needle);
    uint32 haystack_length = StringLength(haystack);

    if(haystack_length >= 2048)
    {
        return -1; 
    }

    if(needle_length > haystack_length)
    {
        return -1; 
    }

    while(index < haystack_length)
    {
        char haystack_buffer[2048];
 
        CopyString((cstring)haystack_buffer, (cstring)haystack, haystack_length);
        haystack_buffer[needle_length] = 0;

        if(StringCompare(haystack_buffer, needle, case_sensitive))
        {
            return index; 
        }

        index++;
        haystack++;
    }

    return 0; 
}

/* Checks if a string is numeric. */

bool IsNumeric(const cstring str)
{
    bool numeric = (str) ? true : false;
    cstring pstr = (cstring)str;
    uint32 index = 0;
    bool hex = ((*pstr == '0') && (*(pstr + 1) == 'x'));

    if(hex)
    {
        pstr += 2;
    }

    while((*pstr) && (numeric))
    {
        numeric = (((*pstr >= '0') && (*pstr <= '9')) ||
                   ((index == 0) && (*pstr == '-')) ||
                   ((*pstr == '.')));

        if(hex)
        {
            numeric = numeric || (((*pstr >= 'a') && (*pstr <= 'f')) ||
                                  ((*pstr >= 'A') && (*pstr <= 'F')));
        }

        index++;
        pstr++;
    }

    return numeric;
}

#endif
