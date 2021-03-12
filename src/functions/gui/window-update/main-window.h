#ifndef _MAIN_WINDOW_UPDATE_H
#define _MAIN_WINDOW_UPDATE_H

void AddItemToListView(string address, string value)
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

#endif
