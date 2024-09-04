#ifndef _MEMORY_SCANNER_WINDOW_FUNCTIONS_H
#define _MEMORY_SCANNER_WINDOW_FUNCTIONS_H

/* Add scan results to ListView control. */

VOID AddItemToListView(string address, string value)
{
    static LVITEMA item;

    item.mask        = LVIF_TEXT;
    item.iSubItem    = 0;
    item.pszText     = (string)address;

    SendMessageA(ListView, LVM_INSERTITEM, 0, (LPARAM)&item);

    item.iSubItem    = 1;
    item.pszText     = (string)value;

    SendMessageA(ListView, LVM_SETITEM, 0, (LPARAM)&item);
}

/* Add scan results to memory scanner window. */

VOID DisplayScanResults(MEMORY_BLOCK *mblock, INTFMT display_format, uint32 display_limit)
{
    SIZE_T i = 0;
    MEMORY_BLOCK *mb = mblock;
    uint32 limit = (GetMatchCount(mb) > display_limit) ? display_limit : (uint32)GetMatchCount(mb);

    while(mb)
    {
        uint64 offset;

        for(offset = 0; offset < mb->size; offset += mb->data_size)
        {
            if(AddressNotDiscarded(mb, offset))
            {
                int8 address[256];
                int8 val[256];

                IntegerToString((uint64)(uintptr_t)mb->address + offset, address, sizeof(address) - 1, FMT_INT_HEXADECIMAL);

                if(Type == TYPE_INTEGER)
                {
                    int64 value = PeekInteger(mb->process, mb->address + offset, mb->data_size);

                    if(display_format == FMT_INT_DECIMAL)
                    {
                        IntegerToString(value, val, sizeof(val) - 1, display_format);
                    }

                    else if(display_format == FMT_INT_HEXADECIMAL)
                    {
                        val[0] = '0';
                        val[1] = 'x';

                        IntegerToString(value, val + 2, sizeof(val) - 3, display_format);
                    }
                }

                else if(Type == TYPE_FLOAT)
                {
                    real4 value = PeekFloat(mb->process, mb->address + offset, mb->data_size);
                    DoubleToString((real8)value, val, sizeof(val) - 1);
                }

                else if(Type == TYPE_DOUBLE)
                {
                    real8 value = PeekDouble(mb->process, mb->address + offset, mb->data_size);
                    DoubleToString(value, val, sizeof(val) - 1);
                }

                if(limit)
                {
                    AddItemToListView(address, val);
                    --limit;
                }

                else
                {
                    EnableWindow(ListView, false);
                }
            }
        }

        mb = ArrayListGet(ArrayList, ++i); 
    }

    EnableWindow(ListView, true); 
}

#endif
