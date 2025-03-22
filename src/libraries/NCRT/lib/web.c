/* Encode url */

string urlencode(string url)
{
    static int8 encoded_url[2048];

    int8 hex_lookup_table[] = "0123456789ABCDEF";
    uint32 index, i;

    index = 0;

    if(StringLength(url) >= sizeof(encoded_url))
    {
        encoded_url[index] = null;
        return (string)encoded_url;
    }

    MemoryZero(encoded_url, sizeof(encoded_url));
    
    for(i = 0; i < StringLength(url); i++) 
    {
        if(('a' <= url[i] && url[i] <= 'z')
            || ('A' <= url[i] && url[i] <= 'Z')
            || ('0' <= url[i] && url[i] <= '9')) 
        {
            encoded_url[index++] = url[i];
        } 

        else 
        {
            encoded_url[index++] = '%';
            encoded_url[index++] = hex_lookup_table[url[i] >> 0x4];
            encoded_url[index++] = hex_lookup_table[url[i] & 0xF];
        }
    }

    encoded_url[index] = null;
    return (string)encoded_url;
}

/* Decode url */

string urldecode(string url)
{
    static int8 encoded_url[2048];

    string purl = url;
    uint32 index = 0;

    if(StringLength(url) >= sizeof(encoded_url))
    {
        encoded_url[index] = null;
        return (string)encoded_url;
    }

    MemoryZero(encoded_url, sizeof(encoded_url));

    while(*purl)
    {
       if(*purl == '%')
       {
           uint8 code[3];

           code[0] = *(purl + 1);
           code[1] = *(purl + 2);
           code[2] = null;

           encoded_url[index++] = (uint8)StringToInteger((string)code, FMT_INT_HEXADECIMAL);

           purl += 3;

           continue;
       }
       
       encoded_url[index++] = *purl++;
    }

    encoded_url[index] = null;
    return (string)encoded_url;
}
