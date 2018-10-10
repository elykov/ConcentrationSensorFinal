#include "GUILogic.h"
#include "DIALOG.h"

#define ID_WINDOW_0  (GUI_ID_USER + 0x06)
#define ID_TEXT_0  (GUI_ID_USER + 0x07)
#define ID_TEXT_1  (GUI_ID_USER + 0x08)
#define ID_EDIT_0  (GUI_ID_USER + 0x09)
#define ID_EDIT_1  (GUI_ID_USER + 0x0A)
#define ID_TEXT_2  (GUI_ID_USER + 0x0B)
#define ID_TEXT_3  (GUI_ID_USER + 0x0C)
#define ID_EDIT_2  (GUI_ID_USER + 0x0D)
#define ID_EDIT_3  (GUI_ID_USER + 0x0E)
#define ID_TEXT_4  (GUI_ID_USER + 0x0F)
#define ID_BUTTON_0  (GUI_ID_USER + 0x10)
#define ID_BUTTON_1  (GUI_ID_USER + 0x11)
#define ID_TEXT_5  (GUI_ID_USER + 0x13)
#define ID_EDIT_4  (GUI_ID_USER + 0x15)

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "SensorNetConfigsWindow", ID_WINDOW_0, 0, 0, 480, 272, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Header", ID_TEXT_0, 90, 5, 300, 30, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "TextTIP", ID_TEXT_1, 35, 60, 40, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditIP", ID_EDIT_0, 80, 60, 150, 30, 0, 0x14, 0 },
  { EDIT_CreateIndirect, "EditMask", ID_EDIT_1, 320, 60, 150, 30, 0, 0x14, 0 },
  { TEXT_CreateIndirect, "TextTMask", ID_TEXT_2, 235, 60, 80, 30, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "TextTDNS1", ID_TEXT_3, 10, 170, 65, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_2, 80, 170, 150, 30, 0, 0x14, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_3, 320, 170, 150, 30, 0, 0x14, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_4, 245, 170, 70, 30, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "ButtonOK", ID_BUTTON_0, 310, 235, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonCancel", ID_BUTTON_1, 395, 235, 80, 30, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_5, 130, 115, 100, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_4, 250, 115, 150, 30, 0, 0x14, 0 },
};

void RefreshSensorNetConfigsWindow(void)
{
	WM_HWIN window = logic.window;
	char tempStr[25];
	
	// ip
	{
		sprintf(tempStr, "%d.%d.%d.%d", 
			DatIP[0], DatIP[1],
			DatIP[2], DatIP[3]);
		
		EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_0), tempStr);
	}
	// mask                                        
	{
		sprintf(tempStr, "%d.%d.%d.%d", 
			DatMask[0], DatMask[1],
			DatMask[2], DatMask[3]);

		EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_1), tempStr);
	}
	// gateway
  {
	  sprintf(tempStr, "%d.%d.%d.%d", 
			DatDefGW[0], DatDefGW[1],
			DatDefGW[2], DatDefGW[3]);
	
		EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_2), tempStr);
	}
	// dns1
  {
		sprintf(tempStr, "%d.%d.%d.%d", 
			DatPriDNS[0], DatPriDNS[1],
			DatPriDNS[2], DatPriDNS[3]);

		EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_3), tempStr);
	}
	// dns2
  {
		sprintf(tempStr, "%d.%d.%d.%d", 
			DatSecDNS[0], DatSecDNS[1],
			DatSecDNS[2], DatSecDNS[3]);

		EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_4), tempStr);
	}
}

void SaveNetData(void)
{
	
}

static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    // Initialization of 'SensorNetConfigsWindow'
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x00FF2100));

    // Initialization of 'Header'
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Настроить датчик (сеть)");
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));

    // Initialization of 'TextTIP'
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "IP: ");
    TEXT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));

    // Initialization of 'EditIP'
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
    EDIT_SetFont(hItem, GUI_FONT_20_1);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);

    // Initialization of 'EditMask'
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_1);
    EDIT_SetFont(hItem, GUI_FONT_20_1);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    //
    // Initialization of 'TextTMask'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Маска: ");
    TEXT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'TextTDNS1'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "DNS1: ");
    TEXT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'Edit'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_2);
    EDIT_SetFont(hItem, GUI_FONT_20_1);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    //
    // Initialization of 'Edit'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_3);
    EDIT_SetFont(hItem, GUI_FONT_20_1);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "DNS2: ");
    TEXT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'ButtonOK'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetFont(hItem, &GUI_FontVerdana20);
    BUTTON_SetText(hItem, "Ок");
    //
    // Initialization of 'ButtonCancel'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetFont(hItem, &GUI_FontVerdana20);
    BUTTON_SetText(hItem, "Отмена");
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_5);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Шлюз: ");
    TEXT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'Edit'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_4);
    EDIT_SetFont(hItem, GUI_FONT_20_1);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    
		RefreshSensorNetConfigsWindow();
		break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_EDIT_0: // Notifications sent by 'EditIP'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_0));
				break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_EDIT_1: // Notifications sent by 'EditMask'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_1));
				break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_EDIT_2: // Notifications sent by 'Edit'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_2));
				break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_EDIT_3: // Notifications sent by 'Edit'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_3));
				break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_BUTTON_0: // Notifications sent by 'ButtonOK'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        HideKeyBoard();
				WindowChange(MenuWindow);
				break;
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'ButtonCancel'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        HideKeyBoard();
				WindowChange(MenuWindow);
				break;
      }
      break;
    case ID_EDIT_4: // Notifications sent by 'Edit'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_4));
				break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
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

WM_HWIN CreateSensorNetConfigsWindow(void) {
  return GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
}
