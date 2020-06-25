#ifndef _GUI_H
#define _GUI_H

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChooseProcessWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChangeValueWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

#include "window-creation/custom-components/window-frame.h"
#include "window-creation/custom-components/buttons.h"

#include "window-creation/main-window.h"
#include "window-creation/select-process-window.h"
#include "window-creation/change-value-window.h"

#include "window-events/main-window-event-handlers.h"
#include "window-events/select-process-window-event-handlers.h"
#include "window-events/change-value-window-event-handlers.h"

#include "window-procedures/main-window-procedures.h"
#include "window-procedures/select-process-window-procedures.h"
#include "window-procedures/change-value-window-procedures.h"

#endif
