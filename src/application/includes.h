#ifndef _INCLUDES_H
#define _INCLUDES_H

/* GUI Code Includes */

    #define ID_LISTVIEW (WM_USER + 1000)
    #define ID_SCAN (WM_USER + 1001)
    #define ID_NEW_SCAN (WM_USER + 1002)
    #define ID_VALUE (WM_USER + 1003)
    #define ID_CHANGE_VALUE (WM_USER + 1004)
    #define ID_SIZE (WM_USER + 1005)
    #define ID_PID (WM_USER + 1006)
    #define ID_SEARCH_CONDITION (WM_USER + 1007)
    #define ID_SELECT_PROCESS (WM_USER + 1008)
    #define ID_PROCESS_ID (WM_USER + 1009)
    #define ID_PROCESSES (WM_USER + 1010)
    #define ID_CHOOSE_PROCESS (WM_USER + 1011)
    #define ID_CHANGE_VALUE_WINDOW (WM_USER + 1012)
    #define ID_CHANGE_VALUE_WINDOW_BUTTON (WM_USER + 1013)
    #define ID_FILE_EXIT (WM_USER + 1014)
    #define ID_HELP_ABOUT (WM_USER + 1015)
    #define ID_FREEZE_VALUE (WM_USER + 1016)
    #define ID_THAW_VALUE (WM_USER + 1017)

    #define PROCESS_LIMIT 128
    #define FREEZE_LIMIT 5

    #ifndef WM_DPICHANGED
        #define WM_DPICHANGED 0x02E0
    #endif

    #ifndef LVS_EX_DOUBLEBUFFER
        #define LVS_EX_DOUBLEBUFFER 0x00010000
    #endif

    static LRESULT CALLBACK MainWindowProc(HWND, UINT, WPARAM, LPARAM);
    static LRESULT CALLBACK SelectProcessWindowProc(HWND, UINT, WPARAM, LPARAM);
    static LRESULT CALLBACK ChangeValueWindowProc(HWND, UINT, WPARAM, LPARAM);

    static string Title = (string)"Simple Memory Editor";
    static string DataSizes[] = { (string)"1", (string)"2", (string)"4", 
                                  (string)"8", (string)"4", (string)"8" };

    static string Datatypes[] = { (string)"Byte", (string)"Short Integer", (string)"Integer", 
                                  (string)"Long Integer", (string)"Float", (string)"Double" };

    static string SearchConditions[] = { (string)"Equals", (string)"Increased", (string)"Decreased" };

    static HWND ListView, Scan, Value, ChangeValue, Pid, ChoosePid, DataSize, DataSizeLabel,
                ValueLabel, SearchConditionLabel, SearchCondition, NewScan, SelectPidWindow,
                ProcessSelection, ChooseProcess, MemoryScannerWindow, ChangeValueWindow, 
                ChangeValueWindowNewValue, ChangeValueWindowButton;

    static INITCOMMONCONTROLSEX CommonControls;
    static HINSTANCE Instance;
    static HFONT Font;

/* End GUI Code Includes */

/* Memory Scanner Code Includes */

    #ifndef WRITABLE_MEMORY
        #define WRITABLE_MEMORY (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)
    #endif

    static int8 Processes[PROCESS_LIMIT][256];
    static int8 Pids[PROCESS_LIMIT][256];
    static int8 FrozenAddresses[FREEZE_LIMIT][256];
    static int8 FrozenValues[FREEZE_LIMIT][256];
    static int8 SelectedItemAddress[256];
    static int8 SelectedItemValue[256]; 
    static int8 SelectedPid[256];

    static HANDLE ScanThread;
    static HANDLE FreezeThread;
    static HANDLE MonitorSelectedProcessThread;

    static real8 CurrentValue;

    static uint32 CurrentProcess;
    static uint32 NumberOfProcesses;
    static uint32 ProcessCounter;
    static uint32 NumberOfAddressesFrozen;
    static uint32 ScreenDPI;

    static int32 IndexOfSelectedProcess;
    static LRESULT SelectedItem;

    static boolean SelectedProcessOpen;
    static boolean ScanRunning;
    static boolean FirstScanNotRun;
    static boolean AddressFrozen;

    /* A linked list of memory region information obtained by VirtualQueryEx(). */

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

    /* Search conditions. */

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

    static TYPE Type;
    static MEMORY_BLOCK *Scanner;

/* End Memory Scanner Code Includes */

#endif
