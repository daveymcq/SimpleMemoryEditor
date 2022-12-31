#ifndef _CHANGE_VALUE_WINDOW_FUNCTIONS_H
#define _CHANGE_VALUE_WINDOW_FUNCTIONS_H

/* Once an address is found, this function updates the value at that address. */

boolean UpdateValue(VOID)
{
    int8 address[256];
    int8 value[256];
    int8 buffer[256];

    LVITEM item;
    INTFMT search_number_format;

    PVOID addr;
    real8 val;

    item.mask = LVIF_TEXT;
    item.iItem = SelectedItem;
    item.iSubItem = 1;
    item.pszText = buffer;

    ListView_GetItemText(ListView, SelectedItem, 0, address, sizeof(address) - 1);
    SendMessageA(ChangeValueWindowNewValue, WM_GETTEXT, sizeof(value) - 1, (LPARAM)value);
    SendMessageA(Value, WM_GETTEXT, sizeof(buffer) - 1, (LPARAM)buffer);
    
    ListView_SetItem(ListView, &item);

    if((IsNumeric(value)) && ((value[0] == '0') && (value[1] == 'x')))
    {
        search_number_format = FMT_INT_HEXADECIMAL;
    }

    else if(IsNumeric(value) && ((value[0] != '0') && (value[1] != 'x')))
    {
        search_number_format = FMT_INT_DECIMAL;
    }

    if(Type == TYPE_FLOAT)
    {
        addr = (PVOID )(uintptr_t)StringToInteger(address, FMT_INT_HEXADECIMAL);
        val = StringToDouble(value);

        if(PokeFloat(Scanner->process, addr, (real4)val, Scanner->data_size))
        {
            real4 current_value;

            SendMessageA(ChangeValueWindowNewValue, WM_GETTEXT, sizeof(value) - 1, (LPARAM)value);

            current_value = PeekFloat(Scanner->process, addr, Scanner->data_size);

            DoubleToString((real8)current_value, value, sizeof(value) - 1);

            ListView_SetItemText(ListView, SelectedItem, 1, value);

            DestroyWindow(ChangeValueWindow);
            SetForegroundWindow(MemoryScannerWindow);

            return MessageBeep(MB_OK);
        }
    }

    else if(Type == TYPE_DOUBLE)
    {
        addr = (PVOID)(uintptr_t)StringToInteger(address, FMT_INT_HEXADECIMAL);
        val = StringToDouble(value);

        if(PokeDouble(Scanner->process, addr, val, Scanner->data_size))
        {
            real8 current_value;

            SendMessageA(ChangeValueWindowNewValue, WM_GETTEXT, sizeof(value) - 1, (LPARAM)value);

            current_value = PeekDouble(Scanner->process, addr, Scanner->data_size);

            DoubleToString(current_value, value, sizeof(value) - 1);

            ListView_SetItemText(ListView, SelectedItem, 1, value);

            DestroyWindow(ChangeValueWindow);
            SetForegroundWindow(MemoryScannerWindow);

            return MessageBeep(MB_OK);
        }
    }

    else if(Type == TYPE_INTEGER)
    {
        addr = (PVOID)(uintptr_t)StringToInteger(address, FMT_INT_HEXADECIMAL);
        val = StringToInteger(value, search_number_format);

        if(PokeInteger(Scanner->process, addr, (int64)val, Scanner->data_size))
        {
            int64 current_value;

            SendMessageA(ChangeValueWindowNewValue, WM_GETTEXT, sizeof(value) - 1, (LPARAM)value);

            current_value = PeekInteger(Scanner->process, addr, Scanner->data_size);

            if(search_number_format == FMT_INT_HEXADECIMAL)
            {
                value[0] = '0';
                value[1] = 'x';

                IntegerToString(current_value, value + 2, sizeof(value) - 3, search_number_format);
            }

            else if(search_number_format == FMT_INT_DECIMAL)
            {
                IntegerToString(current_value, value, sizeof(value) - 1, search_number_format);
            }

            ListView_SetItemText(ListView, SelectedItem, 1, value);

            DestroyWindow(ChangeValueWindow);
            SetForegroundWindow(MemoryScannerWindow);

            return MessageBeep(MB_OK);
        }
    }

    return false;
}

#endif

