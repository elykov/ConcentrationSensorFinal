#include "GUILogic.h"
#include "DIALOG.h"

#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_BUTTON_0    (GUI_ID_USER + 0x01)
#define ID_BUTTON_1    (GUI_ID_USER + 0x02)
#define ID_BUTTON_2    (GUI_ID_USER + 0x03)
#define ID_BUTTON_3    (GUI_ID_USER + 0x04)
#define ID_BUTTON_4    (GUI_ID_USER + 0x05)

extern GUI_CONST_STORAGE GUI_FONT GUI_FontVerdana20;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "MenuWindow", ID_WINDOW_0, 0, 0, 480, 272, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonToStart", ID_BUTTON_0, 275, 230, 200, 35, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonPanelConf", ID_BUTTON_2, 90, 20, 300, 35, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonSensorConf", ID_BUTTON_1, 90, 70, 300, 35, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonPIDConf", ID_BUTTON_3, 90, 120, 300, 35, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonParams", ID_BUTTON_4, 90, 170, 300, 35, 0, 0x0, 0 },
};

static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'MenuWindow'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x00FF2100));
    //
    // Initialization of 'ButtonToStart'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetFont(hItem, &GUI_FontVerdana20);
    BUTTON_SetText(hItem, "К главному экрану");
    //
    // Initialization of 'ButtonSensorConf'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetFont(hItem, &GUI_FontVerdana20);
    BUTTON_SetText(hItem, "Настроить датчик (сеть)");
    //
    // Initialization of 'ButtonPanelConf'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
    BUTTON_SetFont(hItem, &GUI_FontVerdana20);
    BUTTON_SetText(hItem, "Настроить панель (сеть)");
    //
    // Initialization of 'ButtonPIDConf'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
    BUTTON_SetFont(hItem, &GUI_FontVerdana20);
    BUTTON_SetText(hItem, "Настроить ПИД");
    //
    // Initialization of 'ButtonParams'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);
    BUTTON_SetFont(hItem, &GUI_FontVerdana20);
    BUTTON_SetText(hItem, "Настроить параметры");
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'ButtonToStart'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
				WindowChange(StartWindow);
        break;
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'ButtonSensorConf'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
				WindowChange(SensorNetConfigsWindow);
        break;
      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'ButtonPanelConf'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
				WindowChange(PanelNetConfigsWindow);
        break;
      }
      break;
    case ID_BUTTON_3: // Notifications sent by 'ButtonPIDConf'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        WindowChange(PIDConfigsWindow);
        break;
      }
      break;
    case ID_BUTTON_4: // Notifications sent by 'ButtonParams'
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

WM_HWIN CreateMenuWindow(void) {
  return GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
}
