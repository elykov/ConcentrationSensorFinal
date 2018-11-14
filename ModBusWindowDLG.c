#include "DIALOG.h"
#include "modbus.h"
#include "GUILogic.h"

#define ID_WINDOW_0         (GUI_ID_USER + 0x00)
#define ID_TEXT_0         (GUI_ID_USER + 0x01)
#define ID_TEXT_1         (GUI_ID_USER + 0x02)
#define ID_CHECKBOX_0         (GUI_ID_USER + 0x07)
#define ID_CHECKBOX_1         (GUI_ID_USER + 0x08)
#define ID_BUTTON_0         (GUI_ID_USER + 0x09)
#define ID_TEXT_2         (GUI_ID_USER + 0x0A)
#define ID_TEXT_3         (GUI_ID_USER + 0x0B)

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "ModBusWindow", ID_WINDOW_0, 0, 0, 480, 272, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "TextTitle", ID_TEXT_0, 90, 5, 300, 30, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "TextPort", ID_TEXT_1, 140, 45, 200, 30, 0, 0x64, 0 },
  { CHECKBOX_CreateIndirect, "CheckboxIn", ID_CHECKBOX_0, 340, 100, 40, 40, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "CheckboxOut", ID_CHECKBOX_1, 340, 165, 40, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonToParam", ID_BUTTON_0, 5, 237, 120, 30, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "TextIn", ID_TEXT_2, 70, 100, 280, 40, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "TextOut", ID_TEXT_3, 70, 165, 280, 40, 0, 0x64, 0 },
};

extern bool isReversInput, isReversOutput;
extern modbus_t mb;

static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'ModBusWindow'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x00FF2100));
    //
    // Initialization of 'TextTitle'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Настройка ModBus");
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'TextPort'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
		char txt[50];
    sprintf(txt, "Порт ModBus: %d", mb.port);
    TEXT_SetText(hItem, txt);
    //
    // Initialization of 'CheckboxIn'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
    CHECKBOX_SetText(hItem, "");
    CHECKBOX_SetState(hItem, isReversInput);
		//
    // Initialization of 'CheckboxOut'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1);
    CHECKBOX_SetText(hItem, "");
    CHECKBOX_SetState(hItem, isReversOutput);
    //
    // Initialization of 'ButtonToParam'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetFont(hItem, &GUI_FontVerdana20);
    BUTTON_SetText(hItem, "Назад");
    //
    // Initialization of 'TextIn'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Реверс входящих данных");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'TextOut'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Реверс исходящих данных");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_CHECKBOX_0: // Notifications sent by 'CheckboxIn'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
				isReversInput = !isReversInput;
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_CHECKBOX_1: // Notifications sent by 'CheckboxOut'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
				isReversOutput = !isReversOutput;
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_BUTTON_0: // Notifications sent by 'ButtonToParam'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
				WindowChange(ParamsConfigsWindow);
        break;
      }
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

WM_HWIN CreateModBusWindow(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}
