#include "DIALOG.h"
#include "math.h"
#include "GUILogic.h"
#include "Net_User.h"
#include "settings.h"

#define ID_WINDOW_0        (GUI_ID_USER + 0x00)
#define ID_TEXT_TITLE        (GUI_ID_USER + 0x01)
#define ID_TEXT_1        (GUI_ID_USER + 0x02)
#define ID_TEXT_2        (GUI_ID_USER + 0x03)
#define ID_TEXT_3        (GUI_ID_USER + 0x04)
#define ID_TEXT_ERR        (GUI_ID_USER + 0x05)
#define ID_EDIT_0        (GUI_ID_USER + 0x06)
#define ID_EDIT_1        (GUI_ID_USER + 0x07)
#define ID_EDIT_2        (GUI_ID_USER + 0x08)
#define ID_BUTTON_RESET        (GUI_ID_USER + 0x09)
#define ID_BUTTON_EXIT        (GUI_ID_USER + 0x0A)
#define ID_BUTTON_SAVE        (GUI_ID_USER + 0x0B)
#define ID_BUTTON_PREV        (GUI_ID_USER + 0x0C)
#define ID_BUTTON_NEXT        (GUI_ID_USER + 0x0D)

#define FirstPage 0
#define SecondPage 1
#define ThirdPage 2

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "PrettyFormulaWindow", ID_WINDOW_0, 0, 0, 480, 272, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "TextTitle", ID_TEXT_TITLE, 90, 10, 300, 30, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "TextT1", ID_TEXT_1, 60, 50, 200, 30, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "TextT2", ID_TEXT_2, 60, 95, 200, 30, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "TextT3", ID_TEXT_3, 60, 140, 200, 30, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "TextErr", ID_TEXT_ERR, 50, 190, 380, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit1", ID_EDIT_0, 240, 50, 200, 30, 0, 0x14, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_1, 240, 95, 200, 30, 0, 0x14, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_2, 240, 140, 200, 30, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "ButtonReset", ID_BUTTON_RESET, 375, 235, 100, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonExit", ID_BUTTON_EXIT, 270, 235, 100, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonSave", ID_BUTTON_SAVE, 165, 235, 100, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonPrev", ID_BUTTON_PREV, 10, 235, 45, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonNext", ID_BUTTON_NEXT, 65, 235, 45, 30, 0, 0x0, 0 }
};

float* formulaValues[9] = { &oa, &ob, &oc, &od, &oe, &of, &og, &oh, &on };
char* titles[9] = { "Параметр a:", "Параметр b:", "Параметр c:", "Параметр d:", "Параметр e:", "Параметр f:", "Параметр g:", "Параметр h:", "Параметр n:" };
static uint8_t currPage = FirstPage;
static char tempStrValue[15];

void RefreshFormulaWindow(void)
{
	if (isTextErrChangable)
	{  
		if (tcp_get_state(tcp_soc_WORK) != tcpStateESTABLISHED)
			TEXT_SetText(WM_GetDialogItem(logic.window, ID_TEXT_ERR), "Соединение не установлено.");   
		else
			TEXT_SetText(WM_GetDialogItem(logic.window, ID_TEXT_ERR), "");
	}
}

void FormulaTextInit(WM_HWIN hItem)
{
	TEXT_SetFont(hItem, &GUI_FontVerdana20);
	TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
	TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
}

void EditInit(WM_HWIN hItem, float value)
{
	EDIT_SetFont(hItem, &GUI_FontVerdana20);
	EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
}

void RefreshFormulaEdits(void)
{
	int ids[3] = { ID_EDIT_0, ID_EDIT_1, ID_EDIT_2 };
	int ind = currPage * 3;
	for(int i = 0; i < 3; ++i, ++ind)
	{
		sprintf(tempStrValue, "%.12f", *formulaValues[ind]);
		EDIT_SetText(WM_GetDialogItem(logic.window, ids[i]), tempStrValue);
	}
}

void RefreshTextTitles(void)
{
	int ids[3] = { ID_TEXT_1, ID_TEXT_2, ID_TEXT_3 };
	int ind = currPage * 3;
	for(int i = 0; i < 3; ++i, ++ind)
	{
		TEXT_SetText(WM_GetDialogItem(logic.window, ids[i]), titles[ind]);
	}
}

void ResetValues()
{
	float ff[9] = { a, b, c, d, e, f, g, h, n};
	
	for(int i = 0; i < 9; i++)
		*formulaValues[i] = ff[i];
}


bool SaveFormula()
{
	char tempStrBuf[20];
	int edits[3] = { ID_EDIT_0, ID_EDIT_1, ID_EDIT_2};
	int len;
	float tempValues[3];
	char err[20];

	for(int i = 0; i < 3; i++)
	{
		EDIT_GetText(WM_GetDialogItem(logic.window, edits[i]), tempStrBuf, 19);
		if ((len < 1) || (sscanf(tempStrBuf, "%f%s", &tempValues[i], err) != 1))
		{
      return false;
		}
	}	

	int ind = currPage * 3;
	for(int i = 0; i < 3; ++i, ++ind)
	{
		*formulaValues[ind] = tempValues[i];
	}
	
	return true;
}

static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'PrettyFormulaWindow'
    //
		currPage = FirstPage;
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x00FF2100));
    //
    // Initialization of 'TextTitle'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TITLE);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Настройка формулы");
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    
		// Initialization of 'Texts'
    FormulaTextInit(WM_GetDialogItem(pMsg->hWin, ID_TEXT_1));
		FormulaTextInit(WM_GetDialogItem(pMsg->hWin, ID_TEXT_2));
		FormulaTextInit(WM_GetDialogItem(pMsg->hWin, ID_TEXT_3));
		//
    // Initialization of 'Edits'
		EditInit(WM_GetDialogItem(pMsg->hWin, ID_EDIT_0), *formulaValues[0]);
    EditInit(WM_GetDialogItem(pMsg->hWin, ID_EDIT_1), *formulaValues[1]);
		EditInit(WM_GetDialogItem(pMsg->hWin, ID_EDIT_2), *formulaValues[2]);
    //
    // Initialization of 'TextErr'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "");
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));

    // Initialization of 'ButtonSave'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_SAVE);
    BUTTON_SetFont(hItem, &GUI_FontVerdana20);
    BUTTON_SetText(hItem, "Сохранить");
    // Initialization of 'ButtonExit'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_EXIT);
    BUTTON_SetFont(hItem, &GUI_FontVerdana20);
    BUTTON_SetText(hItem, "Выйти");
		// Initialization of 'ButtonReset'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_RESET);
    BUTTON_SetFont(hItem, &GUI_FontVerdana20);
    BUTTON_SetText(hItem, "Сброс");
    //
    // Initialization of 'Button'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_PREV);
    BUTTON_SetFont(hItem, GUI_FONT_20B_1);
		//BUTTON_SetFont(hItem, &GUI_FontVerdana20);
    BUTTON_SetText(hItem, "<<");
    //
    // Initialization of 'Button'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_NEXT);
    BUTTON_SetFont(hItem, GUI_FONT_20B_1);
    BUTTON_SetText(hItem, ">>");
    // USER START (Optionally insert additional code for further widget initialization)
    if (tcp_get_state(tcp_soc_WORK) == tcpStateESTABLISHED && Flags.answer_work != 1)
			ResetValues();
		
		RefreshFormulaEdits();
		RefreshTextTitles();
		// USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_EDIT_0: // Notifications sent by 'Edit1'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				//if (tcp_get_state(tcp_soc_WORK) == tcpStateESTABLISHED)	
          ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, Id), titles[currPage * 3 + (Id - ID_EDIT_0)]);
         break;
      case WM_NOTIFICATION_RELEASED:  
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_EDIT_1: // Notifications sent by 'Edit2'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				//if (tcp_get_state(tcp_soc_WORK) == tcpStateESTABLISHED)
					ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, Id), titles[currPage * 3 + (Id - ID_EDIT_0)]);
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_EDIT_2: // Notifications sent by 'Edit3'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				//if (tcp_get_state(tcp_soc_WORK) == tcpStateESTABLISHED)
					ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, Id), titles[currPage * 3 + (Id - ID_EDIT_0)]);
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_BUTTON_RESET: // Notifications sent by 'Button'
      switch(NCode) 
			{
				case WM_NOTIFICATION_CLICKED:
					break;
				case WM_NOTIFICATION_RELEASED:
					ResetValues();
					RefreshFormulaEdits();
					break;
      }
      break;
    case ID_BUTTON_EXIT: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
				WindowChange(MenuWindow);
        break;
      }
      break;
    case ID_BUTTON_SAVE: // Notifications sent by 'Button'
      switch(NCode) 
			{
				case WM_NOTIFICATION_CLICKED:
					break;
				case WM_NOTIFICATION_RELEASED:
				{
					if (!SaveFormula())
					{
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "Ошибка: Неверно задан параметр.");
						break;
					}
					Flags.answer_work = 1;
					Flags.ch_a = Flags.ch_b = Flags.ch_c = Flags.ch_d = Flags.ch_e = 
						Flags.ch_f = Flags.ch_g = Flags.ch_h = Flags.ch_n = 1; // change params	
					
          if (tcp_get_state(tcp_soc_WORK) == tcpStateESTABLISHED)	
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "Настройки применены.");
					else	
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "Настройки применятся при подключении.");
				}
				TimerStart();
        break;
      }
      break;
    case ID_BUTTON_PREV: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
				if (tcp_get_state(tcp_soc_WORK) == tcpStateESTABLISHED)
				{
					if (!SaveFormula())
					{
            TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "Ошибка: Неверно задан параметр.");
						break;
					}
				}

				if (currPage == FirstPage)
					currPage = ThirdPage;
				else
					--currPage;
					
				RefreshFormulaEdits();
				RefreshTextTitles();
        break;
      }
      break;
    case ID_BUTTON_NEXT: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
				if (tcp_get_state(tcp_soc_WORK) == tcpStateESTABLISHED)
				{
					if (!SaveFormula())
					{
            TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "Ошибка: Неверно задан параметр.");
						break;
					}
				}
					
				if (currPage == ThirdPage)
					currPage = FirstPage;
				else
					++currPage;
					
				RefreshFormulaEdits();
				RefreshTextTitles();
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

WM_HWIN CreateFormulaWindow(void) {
  return GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
}
