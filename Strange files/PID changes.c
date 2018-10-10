/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.36                          *
*        Compiled Aug 31 2016, 10:53:09                              *
*        (c) 2016 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
#include "GUILogic.h"
// USER END

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0         (GUI_ID_USER + 0x00)
#define ID_TEXT_0         (GUI_ID_USER + 0x01)
#define ID_BUTTON_0         (GUI_ID_USER + 0x02)
#define ID_BUTTON_1         (GUI_ID_USER + 0x03)
#define ID_BUTTON_2         (GUI_ID_USER + 0x04)
#define ID_TEXT_1         (GUI_ID_USER + 0x05)
#define ID_EDIT_0         (GUI_ID_USER + 0x06)
#define ID_TEXT_2         (GUI_ID_USER + 0x07)
#define ID_EDIT_1         (GUI_ID_USER + 0x08)
#define ID_TEXT_3         (GUI_ID_USER + 0x09)
#define ID_EDIT_2         (GUI_ID_USER + 0x0A)
#define ID_TEXT_4         (GUI_ID_USER + 0x0B)
#define ID_TEXT_5         (GUI_ID_USER + 0x0C)
#define ID_TEXT_6         (GUI_ID_USER + 0x0D)
#define ID_EDIT_3         (GUI_ID_USER + 0x0E)
#define ID_EDIT_4         (GUI_ID_USER + 0x0F)
#define ID_EDIT_5         (GUI_ID_USER + 0x10)
#define ID_TEXT_7         (GUI_ID_USER + 0x11)
#define ID_TEXT_8         (GUI_ID_USER + 0x12)
#define ID_TEXT_9         (GUI_ID_USER + 0x13)

// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "PIDConfigsWindow", ID_WINDOW_0, 0, 0, 480, 272, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Header", ID_TEXT_0, 90, 5, 300, 30, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "ButtonOK", ID_BUTTON_0, 225, 235, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonCancel", ID_BUTTON_1, 310, 235, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonApply", ID_BUTTON_2, 395, 235, 80, 30, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "TextTP", ID_TEXT_1, 20, 60, 20, 30, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "TextP", ID_TEXT_7, 45, 60, 50, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditP", ID_EDIT_0, 100, 60, 60, 30, 0, 0xa, 0 },

  { TEXT_CreateIndirect, "TextTI", ID_TEXT_2, 170, 60, 20, 30, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "TextI", ID_TEXT_8, 195, 60, 50, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditI", ID_EDIT_1, 250, 60, 60, 30, 0, 0xa, 0 },

  { TEXT_CreateIndirect, "TextTD", ID_TEXT_3, 320, 60, 20, 30, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "TextD", ID_TEXT_9, 345, 60, 50, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditD", ID_EDIT_2, 400, 60, 60, 30, 0, 0xa, 0 },
  
  { TEXT_CreateIndirect, "TextTConc", ID_TEXT_4, 10, 120, 140, 30, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "TextTAmp", ID_TEXT_5, 170, 120, 140, 30, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "TextTPID", ID_TEXT_6, 330, 120, 140, 30, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "EditConc", ID_EDIT_3, 10, 160, 140, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditAmp", ID_EDIT_4, 170, 160, 140, 30, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "EditPID", ID_EDIT_5, 330, 160, 140, 30, 0, 0x64, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
void RefreshPIDWindow(void)
{
	
}
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'PIDConfigsWindow'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x00FF2100));
    //
    // Initialization of 'Header'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Настройка ПИД");
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'ButtonOK'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetFont(hItem, &GUI_FontVerdana20);
    BUTTON_SetText(hItem, "ОК");
    //
    // Initialization of 'ButtonCancel'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetFont(hItem, &GUI_FontVerdana20);
    BUTTON_SetText(hItem, "Отмена");
    //
    // Initialization of 'ButtonApply'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
    BUTTON_SetText(hItem, "Прим.");
    BUTTON_SetFont(hItem, &GUI_FontVerdana20);
    //
    // Initialization of 'TextTP'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "P:");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'EditP'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
    EDIT_SetText(hItem, "0");
    EDIT_SetFont(hItem, &GUI_FontVerdana20);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    //
    // Initialization of 'TextTI'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "I:");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'EditI'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_1);
    EDIT_SetText(hItem, "0");
    EDIT_SetFont(hItem, &GUI_FontVerdana20);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    //
    // Initialization of 'TextTD'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "D:");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'EditD'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_2);
    EDIT_SetText(hItem, "0");
		EDIT_SetFont(hItem, &GUI_FontVerdana20);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    //
    // Initialization of 'TextTConc'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Концентрация:");
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'TextTAmp'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_5);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Выходной ток:");
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
		//
    // Initialization of 'TextTPID'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_6);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "ПИД:");
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
		
		//
    // Initialization of 'EditConc'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_3);
    EDIT_SetText(hItem, "0");
		EDIT_SetFont(hItem, &GUI_FontVerdana20);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		//
    // Initialization of 'EditAmp'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_4);
    EDIT_SetText(hItem, "0");
		EDIT_SetFont(hItem, &GUI_FontVerdana20);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		//
    // Initialization of 'EditPID'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_5);
    EDIT_SetText(hItem, "0");
		EDIT_SetFont(hItem, &GUI_FontVerdana20);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'ButtonOK'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				// USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        HideKeyBoard();
				WindowChange(MenuWindow);
				// USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'ButtonCancel'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        HideKeyBoard();
				WindowChange(MenuWindow);
				// USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'ButtonApply'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_EDIT_0: // Notifications sent by 'EditP'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_0));
				// USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_EDIT_1: // Notifications sent by 'EditI'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_1));
				// USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_EDIT_2: // Notifications sent by 'EditD'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_2));
				// USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
		case ID_EDIT_3: // Notifications sent by 'EditD'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_3));
				// USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
		case ID_EDIT_4: // Notifications sent by 'EditD'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_4));
				// USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
		case ID_EDIT_5: // Notifications sent by 'EditD'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_5));
				// USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreatePIDConfigsWindow
*/
WM_HWIN CreatePIDConfigsWindow(void);
WM_HWIN CreatePIDConfigsWindow(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
