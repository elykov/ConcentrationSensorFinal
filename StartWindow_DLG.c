#include <stdio.h>
#include "GUILogic.h"
#include "DIALOG.h"
#include "stdbool.h"
#include "math.h"
#include "global_var.h"

#define ID_WINDOW_0   (GUI_ID_USER + 0x02)
#define ID_BUTTON_0   (GUI_ID_USER + 0x03)
#define ID_TEXT_0   	(GUI_ID_USER + 0x04)
#define ID_TEXT_1   	(GUI_ID_USER + 0x05)
#define ID_EDIT_1   	(GUI_ID_USER + 0x06)
#define ID_TEXT_2   	(GUI_ID_USER + 0x07)
#define ID_EDIT_2   	(GUI_ID_USER + 0x08)
#define ID_BUTTON_1   (GUI_ID_USER + 0x09)
#define ID_TEXT_3   	(GUI_ID_USER + 0x0A)

extern GUI_CONST_STORAGE GUI_FONT GUI_FontVerdana220;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontVerdana40;
static WM_HWIN window;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "StartWindow_", ID_WINDOW_0, 0, 0, 480, 272, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button_menu", ID_BUTTON_0, 5, 232, 100, 35, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button_Mode", ID_BUTTON_1, 375, 232, 100, 35, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "TextBigValue", ID_TEXT_0, 0, 40, 480, 180, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "TextReferens", ID_TEXT_1, 10, 10, 125, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditReferens", ID_EDIT_1, 135, 5, 90, 30, 0, 0x10, 0 },
  { TEXT_CreateIndirect, "TextDamper", ID_TEXT_2, 250, 10, 135, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditDamper", ID_EDIT_2, 385, 5, 90, 30, 0, 0x10, 0 },
	{ TEXT_CreateIndirect, "TextWorkMode", ID_TEXT_3, 115, 228, 250, 42, 0, 0x64, 0 },
};

uint8_t currWorkMode;

static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'StartWindow_'
    //
    window = hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x00FF2100));
    //
    // Initialization of 'Button_menu'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetFont(hItem, &GUI_FontVerdana20);
    BUTTON_SetText(hItem, "Меню");
    //
    // Initialization of 'Button_Mode'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetFont(hItem, &GUI_FontVerdana20);
    currWorkMode = 127;
		//
    // Initialization of 'TextBigValue'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetFont(hItem, &GUI_FontVerdana220);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
    //
    // Initialization of 'TextReferens'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
		TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Задание (%):");
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
		//
    // Initialization of 'TextWorkMode'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
		TEXT_SetFont(hItem, &GUI_FontVerdana40);    
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));              
		TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'EditReferens'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_1);
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    EDIT_SetFont(hItem, GUI_FONT_32_1);
    //
    // Initialization of 'TextDamper'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
		TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Заслонка (%):");
		TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
		//
    // Initialization of 'EditDamper'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_2);
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    EDIT_SetFont(hItem, GUI_FONT_32_1);

    RefreshWindow();
    break;
  case WM_NOTIFY_PARENT:
		if (keyBoard._keyboard != 0)
			break;
		else if (!keyBoard.isRefreshableFields) // Просмотр изменились ли данные на главном экране
		{
			char tempTxt[20], err[20];
      size_t len;

			if (keyBoard.currentEdit == WM_GetDialogItem(window, ID_EDIT_1)) // задание
			{
        float n_ref;
				EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_1), tempTxt, 19);
				len = strlen(tempTxt);
				if ((len < 1) || (sscanf(tempTxt, "%f%s", &n_ref, err) != 1) || isnan(n_ref))
				{
					// может какую-то надпись
					keyBoard.isRefreshableFields = true;
					break;
				}
				out_referens = n_ref;
				sendParam = 0x02;
				Flags.ch_ref = 1;
				Flags.answer_work = 1;
				keyBoard.isRefreshableFields = true;
			}
      else if (keyBoard.currentEdit == WM_GetDialogItem(window, ID_EDIT_2)) // заслонка
			{
				float n_damper; 
				EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_2), tempTxt, 19);
				len = strlen(tempTxt);
				if ((len < 1) || (sscanf(tempTxt, "%f%s", &n_damper, err) != 1) || isnan(n_damper))
				{
					// может какую-то надпись
					keyBoard.isRefreshableFields = true;
					break;
				}
				damper_manual = out_damper = n_damper;
				sendParam = 0x01;
				Flags.ch_damper = 1;
				Flags.answer_work = 1;
				keyBoard.isRefreshableFields = true;
			}
		}           

    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'Button_menu'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        TimerStopForButton();
				WindowChange(PasswordWindow);
        break;
      }
      break;
		case ID_BUTTON_1: // Notifications sent by 'Button_Mode'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
				// workMode: 0 - ручной, 1 - автомат 
				if (workMode == 0) // если 
				{
					out_workMode = 1;
					damper_manual = damper;
				}
				else	
				{
					out_workMode = 0;
          if (damper_manual < 0.f)
						out_damper = damper;
					else
						out_damper = damper_manual;
					Flags.ch_damper = 1;
				}
				sendParam = 0x01;
				Flags.answer_work = 1;
				
				TimerStartForButton();
        break;
      }
      break;
		case ID_EDIT_1: // Notifications sent by 'EditReferens'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
				ShowKeyBoard(WM_GetDialogItem(window, ID_EDIT_1), "Изменение задания");
        break;
      }
      break;
    case ID_EDIT_2: // Notifications sent by 'EditDamper'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
				if (workMode == 0)
					ShowKeyBoard(WM_GetDialogItem(window, ID_EDIT_2), "Изменение заслонки");
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

WM_HWIN CreateStartWindow(void) {
  return GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
}

void ModeChanging(void)
{
	TEXT_SetTextColor(WM_GetDialogItem(window, ID_TEXT_3), GUI_MAKE_COLOR(0x000090FF));
}

void ModeChanged(void)
{
	TEXT_SetTextColor(WM_GetDialogItem(window, ID_TEXT_3), GUI_MAKE_COLOR(0x00FFFFFF));
}

void RefreshStartWindow(void)
{
	char tempStr[10];
	//char tempStr2[60];
	{ // set big concentracion
		if (isnan(Cb))
			sprintf(tempStr, "A/0");
		else if (Cb < 0.001f || Cb > 8.5f)
			sprintf(tempStr, "---");
		else
			sprintf(tempStr, "%1.2f", Cb);  // (round(Cb * 100) / 100) -  округление до тысячных
		TEXT_SetText(WM_GetDialogItem(window, ID_TEXT_0), tempStr);	
	}
	
	if (!keyBoard.isRefreshableFields)
		return;
		
	{ // задание
		sprintf(tempStr, "%1.2f", referens);
		EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_1), tempStr);	
	}
	
	{ //заслонка
		sprintf(tempStr, "%.2f", damper);
		EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_2), tempStr);	
	}

	if (currWorkMode != workMode)
	{
		if (workMode == 0)
		{
			TEXT_SetText(WM_GetDialogItem(window, ID_TEXT_3), "Режим: РУЧН");
			BUTTON_SetText(WM_GetDialogItem(window, ID_BUTTON_1), "Автомат");
			EDIT_SetTextColor(WM_GetDialogItem(window, ID_EDIT_2), EDIT_CI_ENABLED, GUI_MAKE_COLOR(0x00000000));
		}
		else	
		{
			TEXT_SetText(WM_GetDialogItem(window, ID_TEXT_3), "Режим: АВТО");
			BUTTON_SetText(WM_GetDialogItem(window, ID_BUTTON_1), "Ручной");
			EDIT_SetTextColor(WM_GetDialogItem(window, ID_EDIT_2), EDIT_CI_ENABLED, GUI_MAKE_COLOR(0x001000FF));
		}	
		TimerStopForButton();
		currWorkMode = workMode;
	}
}
