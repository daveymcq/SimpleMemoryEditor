#ifndef _IO_H
#define _IO_H

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   ArrayList implementation                                    //
///////////////////////////////////////////////////////////////////////////////////////////////////

/* Create an ArrayList */

ARRAY_LIST *CreateArrayList(SIZE_T initial_capacity) 
{
    ARRAY_LIST *list = (ARRAY_LIST *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*list));

    if(list)
    {
        list->size = 0;
        list->capacity = initial_capacity;
        list->array = (MEMORY_BLOCK *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, initial_capacity * sizeof(*list->array));

        if(!list->array)
        {
            HeapFree(GetProcessHeap(), 0, list);
            list = null;
        }
    }

    return list;
}

/* Increase the capacity of an ArrayList */

ARRAY_LIST *ArrayListIncreaseCapacity(ARRAY_LIST *list) 
{
    list->capacity *= 2;
    list->array = (MEMORY_BLOCK *)HeapReAlloc(GetProcessHeap(), 
                                              HEAP_ZERO_MEMORY, list->array, 
                                              list->capacity * sizeof(*list->array));

    if(!list->array)
    {
        HeapFree(GetProcessHeap(), 0, list->array);
        list->array = null;
    }

    return list;
}

/* Add Item to an ArrayList */

void ArrayListAdd(ARRAY_LIST *list, MEMORY_BLOCK *mb) 
{
    if(list->size >= list->capacity) 
    {
        ArrayListIncreaseCapacity(list);
    }

    MemoryCopy(&list->array[list->size++], mb, sizeof(*list->array));
}

/* Get Item from an ArrayList */

MEMORY_BLOCK *ArrayListGet(ARRAY_LIST *list, SIZE_T index) 
{
    if(index < list->size) 
    {
        return &list->array[index];
    }

    return null;
}

/* Free resourses used by an ArrayList */

void FreeArrayList(ARRAY_LIST *list) 
{
    list->size = 0;
    list->capacity = 0;
    HeapFree(GetProcessHeap(), 0, list->array);
    list->array = null;
    HeapFree(GetProcessHeap(), 0, list);
    list = null;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Memory IO WinAPI Wrappers                                   //
///////////////////////////////////////////////////////////////////////////////////////////////////

/* A set of helper functions that reads/writes the memory at the address specified. */

real4 ReadFloat(HANDLE process, void *address)
{
    real4 value;

    if(ReadProcessMemory(process, address, (real4 *)&value, sizeof(real4), null))
    {
        return value;
    }

    return 0.0f;
}

real8 ReadDouble(HANDLE process, void *address)
{
    real8 value;

    if(ReadProcessMemory(process, address, (real8 *)&value, sizeof(real8), null))
    {
        return value;
    }

    return 0.0;
}

int64 ReadInteger(HANDLE process, void *address, uint16 data_size)
{
    int64 value = 0;

    if(ReadProcessMemory(process, address, (int64 *)&value, data_size, null))
    {
        switch(data_size)
        {
            case sizeof(int8):

                value = (int8)value;
                return value;

            break;

            case sizeof(int16):

                value = (int16)value;
                return value; 

            break;
            
            case sizeof(int32):

                value = (int32)value; 
                return value; 

            break;

            case sizeof(int64):

                value = (int64)value;
                return value;

            break;
        }
    }

    return value;
}

boolean WriteFloat(HANDLE process, void *address, real4 value)
{
    SIZE_T bytes_written = 0;

    if(WriteProcessMemory(process, address, &value, sizeof(value), &bytes_written))
    {
        return (bytes_written == sizeof(value)); 
    }

    return false;
}

boolean WriteDouble(HANDLE process, void *address, real8 value)
{
    SIZE_T bytes_written = 0;

    if(WriteProcessMemory(process, address, &value, sizeof(value), &bytes_written))
    {
        return (bytes_written == sizeof(value)); 
    }

    return false;
}

boolean WriteInteger(HANDLE process, void *address, int64 value, uint16 data_size)
{
    SIZE_T bytes_written = 0;

    if(WriteProcessMemory(process, address, &value, data_size, &bytes_written))
    {
        return (bytes_written == data_size);
    }

    return false;
}

#endif