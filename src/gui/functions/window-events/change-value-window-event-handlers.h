#ifndef _CHANGE_VALUE_WINDOW_EVENT_HANDLERS_H
#define _CHANGE_VALUE_WINDOW_EVENT_HANDLERS_H

bool ProcessUpdateValueEvent(void)
{
    int8 address[256], val[256], buffer[256];

    ListView_GetItemText(ListView, SelectedItem, 0, address, sizeof(address));
    SendMessage(ChangeValueDlgValue, WM_GETTEXT, sizeof(val), (LPARAM)val);

    uint8 *addr = (uint8 *)(uintptr_t)StringToInteger(address, FMT_INT_HEXADECIMAL);
    double v = StringToDouble(val);

    LVITEM Item;

    MemoryZero(&Item, sizeof(Item));

    Item.mask = LVIF_TEXT;
    Item.iItem = SelectedItem;
    Item.iSubItem = 1;

    SendMessage(Value, WM_GETTEXT, sizeof(buffer), (LPARAM)buffer);
    Item.pszText = buffer;
    ListView_SetItem(ListView, &Item);

    if(type == TYPE_FLOAT)
    {
        if(PokeFloat(scanner->process, addr, v, scanner->data_size))
        {
            DestroyWindow(ChangeValueDlg);
            SetForegroundWindow(MainWindow);

            SendMessage(ChangeValueDlgValue, WM_GETTEXT, sizeof(val), (LPARAM)val);

            float tmp = PeekFloat(scanner->process, addr, scanner->data_size);

            DoubleToString(tmp, val, sizeof(val));
            ListView_SetItemText(ListView, SelectedItem, 1, val);

            MessageBeep(MB_OK);

            return true;
        }

        else
        {
            int8 error[] = "Memory operation failed!";

            EnableWindow(MainWindow, false);
            MessageBox(MainWindow, error, "Error!", MB_OK | MB_ICONERROR);
            EnableWindow(MainWindow, true);
            SetForegroundWindow(MainWindow);

            return false;
        }
    }

    else if(type == TYPE_DOUBLE)
    {
        if(PokeDouble(scanner->process, addr, v, scanner->data_size))
        {
            DestroyWindow(ChangeValueDlg);
            SetForegroundWindow(MainWindow);

            SendMessage(ChangeValueDlgValue, WM_GETTEXT, sizeof(val), (LPARAM)val);

            double tmp = PeekDouble(scanner->process, addr, scanner->data_size);

            DoubleToString(tmp, val, sizeof(val));
            ListView_SetItemText(ListView, SelectedItem, 1, val);

            MessageBeep(MB_OK);

            return true;
        }

        else
        {
            int8 error[] = "Memory operation failed!";

            EnableWindow(MainWindow, false);
            MessageBox(MainWindow, error, "Error!", MB_OK | MB_ICONERROR);
            EnableWindow(MainWindow, true);
            SetForegroundWindow(MainWindow);

            return false;
        }
    }

    else if(type == TYPE_INTEGER)
    {
        if(PokeDecimal(scanner->process, addr, (int64)v, scanner->data_size))
        {
            DestroyWindow(ChangeValueDlg);
            SetForegroundWindow(MainWindow);

            SendMessage(ChangeValueDlgValue, WM_GETTEXT, sizeof(val), (LPARAM)val);

            int64 tmp = PeekDecimal(scanner->process, addr, scanner->data_size);

            IntegerToString(tmp, val, sizeof(val), FMT_INT_DECIMAL);
            ListView_SetItemText(ListView, SelectedItem, 1, val);

            MessageBeep(MB_OK);

            return true;
        }

        else
        {
            int8 error[] = "Memory operation failed!";

            EnableWindow(MainWindow, false);
            MessageBox(MainWindow, error, "Error!", MB_OK | MB_ICONERROR);
            EnableWindow(MainWindow, true);
            SetForegroundWindow(MainWindow);

            return false;
        }
    }

    return false;
}

#endif
