#ifndef _GUI_VARIABLES_H
#define _GUI_VARIABLES_H

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
#define ID_CLOSE (WM_USER + 1018)
#define ID_CLOSE_CHOOSE_PROCESS (WM_USER + 1019)
#define ID_CLOSE_CHANGE_VALUE (WM_USER + 1020)

#define FREEZE_LIMIT (1024 * 128)
#define PROCESS_LIMIT (1024)

static const char *title = "Simple Memory Editor";
static const char *data_sizes[] = { "1", "2", "4", "8", "4", "8" };
static const char *data_types[] = { "Byte", "Short Integer", "Integer", "Long Integer", "Float", "Double" };
static const char *search_conditions[] = { "Equals", "Increased", "Decreased" };

static char frozen_addresses[FREEZE_LIMIT][256];
static char frozen_values[FREEZE_LIMIT][256]; 
static char PID[256];
 
static DWORD MainWindowWidth, MainWindowHeight; 
static DWORD ChooseProcessWindowWidth, ChooseProcessWindowHeight;
static DWORD ChangeValueWindowWidth, ChangeValueWindowHeight;

static HFONT Font;
static HICON Icon;

static HWND ListView, Scan, Value, ChangeValue, Pid, ChoosePid, DataSize, DataSizeLabel,
            PidLabel, ValueLabel, SearchConditionLabel, SearchCondition, NewScan, PidDlg,
            ProcessSelection, ChooseProcess, MainWindow, ChangeValueDlg, ChangeValueDlgValue,
            ChangeValueDlgButton, MainWindowCloseButton, ChooseProcessWindowCloseButton, 
            ChangeValueWindowCloseButton;

#endif
