#ifndef _MAIN_WINDOW_UPDATE_H
#define _MAIN_WINDOW_UPDATE_H

void AddItemToListView(void *address, const char *value)
{
    static LVITEM Item;

    Item.mask        = LVIF_TEXT;
    Item.iSubItem    = 0;
    Item.pszText     = (char *)address;

    SendMessage(ListView, LVM_INSERTITEM, 0, (LPARAM)&Item);

    Item.iSubItem    = 1;
    Item.pszText     = (char *)value;

    SendMessage(ListView, LVM_SETITEM, 0, (LPARAM)&Item);
}

#endif
