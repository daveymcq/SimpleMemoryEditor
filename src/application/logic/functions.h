#ifndef _LOGIC_FUNCTIONS_H
#define _LOGIC_FUNCTIONS_H

/* Checks if the bit in MEMORY_BLOCK.match_flag corresponding to an offset in 
   MEMORY_BLOCK.address was cleared in the previous scan. */

bool AddressNotDiscarded(MEMORY_BLOCK *mblock, uint64 offset)
{
    MEMORY_BLOCK *mb = mblock;

    if(mb)
    {
        bool not_discarded = (mb->match_flag[(offset) / 8] & (1 << ((offset) % 8))); 
        return not_discarded;
    }

    return false;
}

/* Clears the bit in MEMORY_BLOCK.match_flag corresponding to an offset in MEMORY_BLOCK.address. */

bool DiscardAddress(MEMORY_BLOCK *mblock, uint64 offset)
{
    MEMORY_BLOCK *mb = mblock;

    if(mb)
    {
        mb->match_flag[(offset) / 8] &= ~(1 << ((offset) % 8));
        return true;
    }

    return false;
}

/* Finds the number of matches from the last scan. */

uint64 GetMatchCount(MEMORY_BLOCK *mblock)
{
    MEMORY_BLOCK *mb = mblock;
    uint64 matches = 0;

    while(mb)
    {
        matches += mb->matches;
        mb = mb->next;
    }

    return matches;
}

/* A set of helper functions that reads/writes the memory at the address specified. */

float PeekFloat(HANDLE process, void *address, uint16 data_size)
{
    float value;

    if(ReadProcessMemory(process, address, (float *)&value, data_size, 0))
    {
        return value;
    }

    return 0.0f;
}

double PeekDouble(HANDLE process, void *address, uint16 data_size)
{
    double value;

    if(ReadProcessMemory(process, address, (double *)&value, data_size, 0))
    {
        return value;
    }

    return 0.0;
}

int64 PeekInteger(HANDLE process, void *address, uint16 data_size)
{
    int64 value = 0;

    if(ReadProcessMemory(process, address, (int64 *)&value, data_size, 0))
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

bool PokeFloat(HANDLE process, void *address, float value, uint16 data_size)
{
    SIZE_T bytes_written;

    if(WriteProcessMemory(process, address, &value, data_size, &bytes_written))
    {
        return ((uint16)bytes_written == data_size); 
    }

    return false;
}

bool PokeDouble(HANDLE process, void *address, double value, uint16 data_size)
{
    SIZE_T bytes_written;

    if(WriteProcessMemory(process, address, &value, data_size, &bytes_written))
    {
        return ((uint16)bytes_written == data_size); 
    }

    return false;
}

bool PokeInteger(HANDLE process, void *address, int64 value, uint16 data_size)
{
    SIZE_T bytes_written;

    if(WriteProcessMemory(process, address, &value, data_size, &bytes_written))
    {
        return ((uint16)bytes_written == data_size);
    }

    return false;
}

/* Constructs and allocates the MEMORY_BLOCK linked list structure. */

MEMORY_BLOCK *CreateMemoryBlock(HANDLE process, MEMORY_BASIC_INFORMATION *mbi, uint16 data_size)
{
    MEMORY_BLOCK *mb = (MEMORY_BLOCK *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*mb));

    if(mb)
    {
        mb->process             = process;
        mb->size                = (uint64)mbi->RegionSize;
        mb->address             = (uint8 *)mbi->BaseAddress;
        mb->buffer              = (uint8 *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, mbi->RegionSize);
        mb->match_flag          = (uint8 *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (mbi->RegionSize / 8));
        mb->matches             = (uint64)mbi->RegionSize;
        mb->data_size           = data_size; 
        mb->next                = null;

        if(mb->match_flag) 
        {
            MemorySet(mb->match_flag, 0xff, (mbi->RegionSize / 8));
        }
    }

    return mb;
}

/* Cleans up the memory allocated by CreateMemoryScanner(). */

void FreeMemoryScanner(MEMORY_BLOCK *mblock)
{
    MEMORY_BLOCK *mb = mblock;

    if(mb->process)
    {
        CloseHandle(mb->process);
    }

    while(mb)
    {
        MEMORY_BLOCK *pmb = mb;

        if(pmb->buffer) 
        {
            HeapFree(GetProcessHeap(), 0, pmb->buffer);
        }

        if(pmb->match_flag) 
        {
            HeapFree(GetProcessHeap(), 0, pmb->match_flag);
        }

        HeapFree(GetProcessHeap(), 0, pmb);

        mb = mb->next;
    }
}

/* Finds the initial valid memory information and sets up for UpdateMemoryBlock(). */

MEMORY_BLOCK *CreateMemoryScanner(uint32 pid, uint16 data_size)
{
    MEMORY_BLOCK *mblock;
    MEMORY_BASIC_INFORMATION mbi;
    HANDLE process;
    uint8 *address;

    process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE, false, pid);

    if(process)
    {
        CurrentProcess = pid;
        SelectedProcessOpen = true;
        address = 0;
        mblock = null;

        while(VirtualQueryEx(process, address, &mbi, sizeof(mbi)))
        {
            if((mbi.State & MEM_COMMIT) && (mbi.Protect & WRITABLE_MEMORY))
            {
                MEMORY_BLOCK *mb = CreateMemoryBlock(process, &mbi, data_size); 

                if(mb)
                {
                    mb->next = mblock;
                    mblock = mb;
                }
            }

            address = (uint8 *)mbi.BaseAddress + mbi.RegionSize;
        }
    }

    return mblock;
}

/* A thread to monitor addresses for change. */

DWORD WINAPI FreezeAddresses(void)
{
    while(Scanner)
    {
        INTFMT search_number_format;
        uint32 offset;
        double value;
        void *address;

        address = (void *)(uintptr_t)StringToInteger(FrozenAddress, FMT_INT_HEXADECIMAL);
        value = StringToDouble(FrozenValue);

        if((IsNumeric(FrozenValue)) && ((FrozenValue[0] == '0') && (FrozenValue[1] == 'x')))
        {
            search_number_format = FMT_INT_HEXADECIMAL;
        }

        else if(IsNumeric(FrozenValue) && ((FrozenValue[0] != '0') && (FrozenValue[1] != 'x')))
        {
            search_number_format = FMT_INT_DECIMAL;
        }

        else
        {
            search_number_format = (INTFMT)0;
        }

        if(search_number_format)
        {
            value = StringToInteger(FrozenValue, search_number_format);
        }

        switch(Type)
        {
            case TYPE_FLOAT:

                CurrentValue = PeekFloat(Scanner->process, address, Scanner->data_size);

                if(value != CurrentValue)
                {
                    PokeFloat(Scanner->process, address, value, Scanner->data_size);
                }

            break;

            case TYPE_DOUBLE:

                CurrentValue = PeekDouble(Scanner->process, address, Scanner->data_size);

                if(value != CurrentValue)
                {
                    PokeDouble(Scanner->process, address, value, Scanner->data_size);
                }

            break;

            case TYPE_INTEGER:
                
                CurrentValue = PeekInteger(Scanner->process, address, Scanner->data_size);

                if(value != CurrentValue)
                {
                    PokeInteger(Scanner->process, address, value, Scanner->data_size);
                }

            break;
        }
    }

    return EXIT_SUCCESS;
}

/* Filters memory information aquired by CreateMemoryScanner() and subsequent calls to this function. */

void UpdateMemoryBlock(MEMORY_BLOCK *mblock, SEARCH_CONDITION condition, TYPE Type, double value)
{
    MEMORY_BLOCK *mb = mblock;

    while(mb)
    {
        if(mb->matches)
        {
            static int8 buffer[256 * 1024];

            static uint64 total_read;
            static uint64 bytes_left;
            static uint64 bytes_to_read;
            static uint64 bytes_read;

            int32 selection_id;
            uint64 bytes_copied;

            total_read = 0;
            mb->matches = 0;
            bytes_left = mb->size;

            selection_id = (int32)SendMessageA(DataSize, CB_GETCURSEL, 0, 0);
            mb->data_size = (uint16)StringToInteger(DataSizes[selection_id], FMT_INT_DECIMAL);

            while(bytes_left)
            {
                uint64 offset;
                double tmpval, prevval;
                bool match;

                bytes_to_read = (bytes_left > sizeof(buffer)) ? sizeof(buffer) : bytes_left;

                ReadProcessMemory(mb->process, mb->address + total_read, buffer, bytes_to_read, (SIZE_T *)&bytes_read);

                if(bytes_read != bytes_to_read) break;

                for(offset = 0; offset < bytes_read; offset += mb->data_size)
                {
                    if(AddressNotDiscarded(mb, total_read + offset)) 
                    {
                        if(Type == TYPE_INTEGER)
                        {
                            switch(mb->data_size)
                            {
                                case sizeof(int8):

                                    tmpval = *((int8 *)&buffer[offset]);
                                    prevval = *((int8 *)&mb->buffer[total_read + offset]);

                                break;

                                case sizeof(int16):

                                    tmpval = *((int16 *)&buffer[offset]);
                                    prevval = *((int16 *)&mb->buffer[total_read + offset]);

                                break;

                                case sizeof(int32):

                                    tmpval = *((int32 *)&buffer[offset]);
                                    prevval = *((int32 *)&mb->buffer[total_read + offset]);

                                break;

                                case sizeof(int64):

                                    tmpval = *((int64 *)&buffer[offset]);
                                    prevval = *((int64 *)&mb->buffer[total_read + offset]);

                                break;
                            }
                        }

                        else if(Type == TYPE_FLOAT)
                        {
                            if(mb->data_size == sizeof(float))
                            {
                                tmpval = *((float *)&buffer[offset]);
                                prevval = *((float *)&mb->buffer[total_read + offset]);
                            }
                        }

                        else if(Type == TYPE_DOUBLE)
                        {
                            if(mb->data_size == sizeof(double))
                            {
                                tmpval = *((double *)&buffer[offset]);
                                prevval = *((double *)&mb->buffer[total_read + offset]);
                            }
                        }

                        switch(condition)
                        {
                            case SEARCH_EQUALS:

                                match = (tmpval == value);

                            break;

                            case SEARCH_INCREASED:

                                match = (tmpval > prevval);

                            break;

                            case SEARCH_DECREASED:

                                match = (tmpval < prevval);
                                
                            break;
                        }

                        if(match)
                        {
                            ++mb->matches;
                        }

                        else 
                        {
                            DiscardAddress(mb, total_read + offset);
                        }
                    }
                }

                MemoryCopy(mb->buffer + total_read, buffer, bytes_read);

                bytes_left -= bytes_read;
                total_read += bytes_read;
            }

            mb->size = total_read;
        }

        mb = mb->next; 
    }
}

#endif
