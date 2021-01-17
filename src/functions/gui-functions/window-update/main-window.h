#ifndef _MAIN_WINDOW_UPDATE_H
#define _MAIN_WINDOW_UPDATE_H

void AddItemToListView(const string address, const string value)
{
    LVITEM Item;

    Item.mask        = LVIF_TEXT;
    Item.iSubItem    = 0;
    Item.pszText     = (string)address;

    SendMessageA(ListView, LVM_INSERTITEM, 0, (LPARAM)&Item);

    Item.iSubItem    = 1;
    Item.pszText     = (string)value;

    SendMessageA(ListView, LVM_SETITEM, 0, (LPARAM)&Item);
}

#endif
