#ifndef _SCANNER_VARIABLES_H
#define _SCANNER_VARIABLES_H

#define MEM_WRITABLE (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)

static char processes[PROCESS_LIMIT][256];
static char pids[PROCESS_LIMIT][256];

static LRESULT SelectedItem;

static HANDLE ScanThread;
static HANDLE FreezeThread;
static HANDLE MonitorSelectedProcessThread;

static double CurrentValue;
static uint32 CurrentProcess;
static uint32 NumberOfProcesses;
static uint32 ProcessCounter;
static uint32 NumberOfAddressesFrozen;

static int32 IndexOfSelectedProcess;

static bool SelectedProcessOpen;
static bool ScanRunning;
static bool FirstScanNotRun;

// A linked list of memory region information obtained by VirtualQueryEx().

typedef struct _MEMORY_BLOCK
{
    HANDLE process;
    uint64 size;
    uint8 *address;
    uint8 *buffer;
    uint8 *match_flag;
    uint16 data_size;
    uint64 matches;
    struct _MEMORY_BLOCK *next;

} MEMORY_BLOCK;

// Search conditions.

typedef enum
{
    SEARCH_EQUALS,
    SEARCH_INCREASED,
    SEARCH_DECREASED

} SEARCH_CONDITION;

typedef enum
{
    TYPE_INTEGER,
    TYPE_FLOAT,
    TYPE_DOUBLE

} TYPE;


static TYPE type;
static MEMORY_BLOCK *scanner;

#endif
