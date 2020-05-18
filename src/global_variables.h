#ifndef _GLOBAL_VARIABLES_H
#define _GLOBAL_VARIABLES_H

#define ID_LISTVIEW (WM_USER + 1000)
#define ID_SCAN (WM_USER + 1001)
#define ID_NEW_SCAN (WM_USER + 1002)
#define ID_VALUE (WM_USER + 1003)
#define ID_CHANGE_VALUE (WM_USER + 1004)
#define ID_PID (WM_USER + 1005)
#define ID_SIZE (WM_USER + 1006)
#define ID_SEARCH_CONDITION (WM_USER + 1007)
#define ID_SELECT_PROCESS (WM_USER + 1008)
#define ID_PROCESS_ID (WM_USER + 1009)
#define ID_PROCESSES (WM_USER + 1010)
#define ID_CHOOSE_PROCESS (WM_USER + 1011)
#define ID_CHANGE_DLG_VALUE (WM_USER + 1012)
#define ID_CHANGE_DLG_BUTTON (WM_USER + 1013)
#define ID_FILE_EXIT (WM_USER + 1014)
#define ID_HELP_ABOUT (WM_USER + 1015)
#define ID_FREEZE_VALUE (WM_USER + 1016)
#define ID_UNFREEZE_VALUE (WM_USER + 1017)

#define MEM_WRITABLE (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)
#define FREEZE_LIMIT (1024 * 128)
#define PROCESS_LIMIT (1024)

const char *title = "Simple Memory Editor";
const char *data_sizes[] = { "1", "2", "4", "8", "4", "8" };
const char *data_types[] = { "8-bit Integer", "16-bit Integer", "32-bit Integer", "64-bit Integer", "Float", "Double" };
const char *search_conditions[] = { "Value Equal", "Value Increased", "Value Decreased" };

static char processes[PROCESS_LIMIT][256];
static char pids[PROCESS_LIMIT][256];

static char frozen_addresses[FREEZE_LIMIT][256];
static char frozen_values[FREEZE_LIMIT][256];

static DWORD Width;
static DWORD Height;

static HWND ListView, Scan, Value, ChangeValue, Pid, ChoosePid, DataSize, DataSizeLabel,
            PidLabel, ValueLabel, SearchConditionLabel, SearchCondition, NewScan, PidDlg,
            ProcessSelection, ChooseProcess, MainWindow, ChangeValueDlg, ChangeValueDlgValue,
            ChangeValueDlgButton;

static LVCOLUMN Column;

static LRESULT SelectedItem;

static HANDLE ScanThread;
static HANDLE FreezeThread;
static HANDLE MonitorSelectedProcessThread;
static HANDLE Process;

static HFONT font;
static NONCLIENTMETRICS metrics;

static int selected_process_index;
static int prev_selected_process_index;

static double current_value;

static unsigned int current_pid;
static unsigned int number_of_processes;
static unsigned int process_count;
static unsigned int addresses_frozen;

static bool SelectedProcessOpen;
static bool ScanRunning;
static bool FirstScanNotRun;

static HMENU MenuBar;
static HMENU FileMenu;
static HMENU HelpMenu;

// A linked list of memory region information obtained by VirtualQueryEx().

typedef struct _MEMORY_BLOCK
{
    HANDLE process;
    unsigned char *address;
    unsigned char *buffer;
    unsigned char *match_flag;
    unsigned long long size;
    unsigned short data_size;
    unsigned int matches;
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
