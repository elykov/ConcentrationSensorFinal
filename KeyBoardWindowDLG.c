#include "GuiLogic.h"
#include "KeyBoardWindowDLG.h"

extern struct GUILogic logic;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "KeyBoardWindow", ID_WINDOW_0, 0, 82, 480, 190, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "TextTitle", ID_TEXT_TITLE, 90, 5, 300, 30, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "EditString", ID_EDIT_0, 90, 40, 300, 30, 0, 0x64, 0 },
	
  { BUTTON_CreateIndirect, "Button1", ID_BUTTON_1, 	 5,  80, 50, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button2", ID_BUTTON_2, 	60,  80, 50, 50, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Button3", ID_BUTTON_3, 115,  80, 50, 50, 0, 0x0, 0 },	
  { BUTTON_CreateIndirect, "Button4", ID_BUTTON_4, 170,  80, 50, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button5", ID_BUTTON_5, 225,  80, 50, 50, 0, 0x0, 0 },

  { BUTTON_CreateIndirect, "Button6", ID_BUTTON_6, 	 5, 135, 50, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button7", ID_BUTTON_7,  60, 135, 50, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button8", ID_BUTTON_8, 115, 135, 50, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button9", ID_BUTTON_9, 170, 135, 50, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button0", ID_BUTTON_0, 225, 135, 50, 50, 0, 0x0, 0 },

  { BUTTON_CreateIndirect, "ButtonDot", ID_BUTTON_10, 	280,	80, 50,  50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonMinus", ID_BUTTON_13, 280, 135, 50,  50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonBackS", ID_BUTTON_11, 335, 	80, 70,  50, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "ButtonESC", ID_BUTTON_14, 	335, 135, 70,  50, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "ButtonOK", ID_BUTTON_12, 		410,  80, 65, 105, 0, 0x0, 0 },
};

static void InitNumButton(WM_HWIN hItem, char* str) 
{
	BUTTON_SetText(hItem, str);
	BUTTON_SetFont(hItem, GUI_FONT_32B_ASCII);
}

static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;

  switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
		{
			// Initialization of 'KeyBoardWindow'
			hItem = pMsg->hWin;
			WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x008C0000));
			//
			keyBoard._title_text = hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TITLE);
			TEXT_SetFont(hItem, &GUI_FontVerdana20);
			TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
			//  
			keyBoard._this_edit = hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
			EDIT_SetText(hItem, "");
			EDIT_SetFont(hItem, &GUI_FontVerdana20);
			EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
			//
			// Initialization of buttons from 'Button1' to 'ButtonOK'
			InitNumButton(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1), "1");
			InitNumButton(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2), "2");
			InitNumButton(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3), "3");
			InitNumButton(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4), "4");
			InitNumButton(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5), "5");
			InitNumButton(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_6), "6");
			InitNumButton(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_7), "7");
			InitNumButton(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_8), "8");
			InitNumButton(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_9), "9");
			InitNumButton(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0), "0");
			InitNumButton(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_10), ".");
			InitNumButton(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_11), "<=");
			InitNumButton(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_12), "OK");
			InitNumButton(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_13), "-");
		  InitNumButton(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_14), "ESC");
			break;
		}
		case WM_NOTIFY_PARENT:
		{
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			
			if (Id == ID_EDIT_0)
				break;
			else if (NCode == WM_NOTIFICATION_CLICKED)
				Button_HandlePush(Id);
			break;
		}
		default:
		{
			WM_DefaultProc(pMsg);
			break;
		}
  }
}

WM_HWIN CreateKeyBoardWindow(void)
{
  return GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
}
