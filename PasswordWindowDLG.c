#include "DIALOG.h"
#include "GUILogic.h"

#define ID_WINDOW_0     		(GUI_ID_USER + 0x00)
#define ID_BUTTON_0         (GUI_ID_USER + 0x01)
#define ID_BUTTON_1         (GUI_ID_USER + 0x02)
#define ID_BUTTON_2         (GUI_ID_USER + 0x03)
#define ID_BUTTON_3         (GUI_ID_USER + 0x04)
#define ID_BUTTON_4         (GUI_ID_USER + 0x05)
#define ID_BUTTON_5         (GUI_ID_USER + 0x06)
#define ID_BUTTON_6         (GUI_ID_USER + 0x07)
#define ID_BUTTON_7         (GUI_ID_USER + 0x08)
#define ID_BUTTON_8         (GUI_ID_USER + 0x09)
#define ID_BUTTON_9         (GUI_ID_USER + 0x0A)
#define ID_BUTTON_10     		(GUI_ID_USER + 0x0B)
#define ID_BUTTON_11     		(GUI_ID_USER + 0x0C)
#define ID_BUTTON_12     		(GUI_ID_USER + 0x0D)
#define ID_TEXT_0     			(GUI_ID_USER + 0x0E)
#define ID_MULTIEDIT_0     	(GUI_ID_USER + 0x0F)

#define STAR_CHAR "*"
#define PSWD "13467982"
#define MAX_PSWD_CHAR_COUNT 10

extern GUI_CONST_STORAGE GUI_FONT GUI_FontVerdana20;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontVerdana40;

//uint8_t isLogged = 0;

static void AppendEditByKey(uint8_t key) {
	WM_HWIN hItem = WM_GetDialogItem(logic.window, ID_MULTIEDIT_0);
	uint8_t textLength = MULTIEDIT_GetTextSize(hItem);
  if (textLength < MAX_PSWD_CHAR_COUNT) {
		MULTIEDIT_AddKey(hItem, key);
	}
}

static void DeleteAllChars(void) {
	WM_HWIN hItem = WM_GetDialogItem(logic.window, ID_MULTIEDIT_0);
	MULTIEDIT_SetText(hItem, "");	
}

void InitNumButton(int ButtinId, char* num)
{
	WM_HWIN hItem = WM_GetDialogItem(logic.window, ButtinId);
	BUTTON_SetText(hItem, num);
	BUTTON_SetFont(hItem, GUI_FONT_D24X32);
}

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, 		"Window", 	ID_WINDOW_0, 		0, 		0, 480,  272, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, 			"lStatus", 	ID_TEXT_0, 			40, 	5, 400, 	30, 0, 0x64, 0 },
	{ MULTIEDIT_CreateIndirect, "mePasswd", ID_MULTIEDIT_0, 75, 40, 240, 	35, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, 		"btnOK", 		ID_BUTTON_10, 	330, 40, 	95, 	35, 0, 0x0, 0 },

  { BUTTON_CreateIndirect, "btnOne", ID_BUTTON_1, 	 75, 90, 55, 55, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "btnTwo", ID_BUTTON_2, 	140, 90, 55, 55, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "btnThree", ID_BUTTON_3, 205, 90, 55, 55, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "btnZero", ID_BUTTON_0, 	270, 90, 155, 55, 0, 0x0, 0 },

  { BUTTON_CreateIndirect, "btnFour", ID_BUTTON_4, 	 75, 150, 55, 55, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "btnFive", ID_BUTTON_5, 	140, 150, 55, 55, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "btnSix", ID_BUTTON_6, 	205, 150, 55, 55, 0, 0x0, 0 },

  { BUTTON_CreateIndirect, "btnEight", ID_BUTTON_7,  75, 210, 55, 55, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "btnSeven", ID_BUTTON_8, 140, 210, 55, 55, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "btnNine", ID_BUTTON_9, 	205, 210, 55, 55, 0, 0x0, 0 },

	{ BUTTON_CreateIndirect, "btnBack", ID_BUTTON_11, 		 270, 210, 155, 55, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "btnBackspace", ID_BUTTON_12, 270, 150, 155, 55, 0, 0x0, 0 },
};

static char* nums[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
		//
    // Initialization of 'PasswordWindow'
    //
    logic.window = hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x00FF2100));
    //
    // Initialization of NumButtons
    //
		for(int i = ID_BUTTON_0; i < ID_BUTTON_10; i++)
		{
			InitNumButton(i, nums[i - ID_BUTTON_0]);
    }
    //
    // Initialization of 'btnOk'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_10);
    BUTTON_SetText(hItem, "OK");
    BUTTON_SetFont(hItem, GUI_FONT_32B_1);
    //
    // Initialization of 'btnBack'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_11);
    BUTTON_SetFont(hItem, &GUI_FontVerdana40);
    BUTTON_SetText(hItem, "Назад");
    //
    // Initialization of 'btnBackspace'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_12);
    BUTTON_SetText(hItem, "Стереть");
    BUTTON_SetFont(hItem, &GUI_FontVerdana40);
    //
    // Initialization of 'lStatus'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetText(hItem, "Введите пароль");
		TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    //
    // Initialization of 'mePasswd'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_0);
    MULTIEDIT_SetText(hItem, "");
    MULTIEDIT_SetFont(hItem, GUI_FONT_32B_1);
    MULTIEDIT_SetTextAlign(hItem, GUI_TA_CENTER);
		MULTIEDIT_SetPasswordMode(hItem, 1);
		MULTIEDIT_SetMaxNumChars(hItem, MAX_PSWD_CHAR_COUNT);
	  break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
			case ID_BUTTON_0: 
			case ID_BUTTON_1: 
			case ID_BUTTON_2: 
      case ID_BUTTON_3: 
      case ID_BUTTON_4: 
      case ID_BUTTON_5: 
      case ID_BUTTON_6: 
      case ID_BUTTON_7: 
      case ID_BUTTON_8: 
      case ID_BUTTON_9: 
      {
				switch(NCode) 
				{
					case WM_NOTIFICATION_CLICKED:
						break;
					case WM_NOTIFICATION_RELEASED:
						AppendEditByKey(47 + Id);
						break;
				}
				break;
			}
		case ID_BUTTON_10: // Notifications sent by 'OK'
      switch(NCode) 
			{
				case WM_NOTIFICATION_CLICKED:
					break;
				case WM_NOTIFICATION_RELEASED:
				{
					char psword[20];
          MULTIEDIT_GetText(WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_0), psword, 15);

					if (strcmp(psword, PSWD) != 0)
					{
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_0), "Пароль неверный. Повторите попытку.");
					}
					else
					{
						
						WindowChange(MenuWindow);
					}
					break;
				}
      }
      break;
    
		case ID_BUTTON_11: // Notifications sent by 'Back'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				break;
      case WM_NOTIFICATION_RELEASED:
        WindowChange(StartWindow);
      	break;
      }
      break;

    case ID_BUTTON_12: // Notifications sent by 'Backspace'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
      	break;
      case WM_NOTIFICATION_RELEASED:
        DeleteAllChars();
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

WM_HWIN CreatePasswordWindow(void) {
  return GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
}

