#ifndef _MAIN_WINDOW_UPDATE_H
#define _MAIN_WINDOW_UPDATE_H

void AddItemToListView(void *address, const string value)
{
    static LVITEM Item = { 0 };

    Item.mask        = LVIF_TEXT;
    Item.iSubItem    = 0;
    Item.pszText     = (string)address;

    SendMessage(ListView, LVM_INSERTITEM, 0, (LPARAM)&Item);

    Item.iSubItem    = 1;
    Item.pszText     = (string)value;

    SendMessage(ListView, LVM_SETITEM, 0, (LPARAM)&Item);
}

#endif
