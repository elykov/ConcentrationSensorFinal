#include "GUILogic.h"
#include "DIALOG.h"
#include "Net_User.h"

#define ID_WINDOW_0   (GUI_ID_USER + 0x00)
#define ID_TEXT_0    	(GUI_ID_USER + 0x01)
#define ID_BUTTON_0   (GUI_ID_USER + 0x02)
#define ID_BUTTON_1   (GUI_ID_USER + 0x03)
#define ID_BUTTON_2   (GUI_ID_USER + 0x04)
#define ID_TEXT_1    	(GUI_ID_USER + 0x05)
#define ID_TEXT_2    	(GUI_ID_USER + 0x06)
#define ID_TEXT_3    	(GUI_ID_USER + 0x07)
#define ID_TEXT_4    	(GUI_ID_USER + 0x08)
#define ID_EDIT_0    	(GUI_ID_USER + 0x0A)
#define ID_EDIT_1    	(GUI_ID_USER + 0x0B)
#define ID_EDIT_2    	(GUI_ID_USER + 0x0C)
#define ID_EDIT_3    	(GUI_ID_USER + 0x09)
#define ID_TEXT_ERR   (GUI_ID_USER + 0x0D)


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "ParamsConfigsWindow", ID_WINDOW_0, 0, 0, 480, 272, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Header", ID_TEXT_0, 90, 5, 300, 30, 0, 0x64, 0 },
	{ BUTTON_CreateIndirect, "ButtonSave", ID_BUTTON_0, 165, 235, 100, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonExit", ID_BUTTON_1, 270, 235, 100, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonReset", ID_BUTTON_2, 375, 235, 100, 30, 0, 0x0, 0 },
  
	{ TEXT_CreateIndirect, "TextTTrowel", ID_TEXT_1, 10, 65, 90, 30, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "EditTrowel", ID_EDIT_0, 110, 65, 100, 30, 0, 0xa, 0 },

	{ TEXT_CreateIndirect, "TextTRevers", ID_TEXT_2, 225, 65, 140, 30, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "EditRevers", ID_EDIT_1, 370, 65, 100, 30, 0, 0xa, 0 },
	
	{ TEXT_CreateIndirect, "TextTDump", ID_TEXT_3, 10, 120, 90, 30, 0, 0x0, 0 },
	{ EDIT_CreateIndirect, "EditDump", ID_EDIT_2, 110, 120, 100, 30, 0, 0xa, 0 },

  { TEXT_CreateIndirect, "TextTPeriod", ID_TEXT_4, 225, 120, 140, 30, 0, 0x0, 0 },
	{ EDIT_CreateIndirect, "EditPeriod", ID_EDIT_3, 370, 120, 100, 30, 0, 0xa, 0 },	
	
	{ TEXT_CreateIndirect, "TextErr", ID_TEXT_ERR, 20, 170, 440, 30, 0, 0x0, 0 },
};

void RefreshParamsWindow(void)
{
	if (isTextErrChangable)
	{
		if (tcp_get_state(tcp_soc_WORK) != tcpStateESTABLISHED)
			TEXT_SetText(WM_GetDialogItem(logic.window, ID_TEXT_ERR), "Соединение не установлено.");
		else
			TEXT_SetText(WM_GetDialogItem(logic.window, ID_TEXT_ERR), "");
	}
}

void FillParamsEditsWindow(void)
{
	WM_HWIN window = logic.window; 
	char tempStr[15];

	// i_trowel
	sprintf(tempStr, "%u", i_trowel);
	EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_0), tempStr);

	// i_revers
	sprintf(tempStr, "%u", i_revers);
	EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_1), tempStr);
	
	// dump
	sprintf(tempStr, "%u", dump);
	EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_2), tempStr);

	// period_answer
	sprintf(tempStr, "%u", period_answer);
	EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_3), tempStr);

	isTextErrChangable = true;
	RefreshParamsWindow();
}


static int SaveParams(void)
{
	WM_HWIN window = logic.window;
	char tempTxt[20], err[15];
	size_t len;
	
	if (tcp_get_state(tcp_soc_WORK) == tcpStateESTABLISHED) // нет смысла что-то делать, если нет соединения
	{
		uint32_t n_tr, n_rev, n_dump, n_period;
		
    EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_0), tempTxt, 14);
		len = strlen(tempTxt);
		if ((len < 1) || (sscanf(tempTxt, "%u%s", &n_tr, err) != 1) || n_tr > 65535)
			return 1;
		
		EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_1), tempTxt, 14);
		len = strlen(tempTxt);
		if ((len < 1) || (sscanf(tempTxt, "%u%s", &n_rev, err) != 1) || n_rev > 65535)
			return 2;
	
		EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_2), tempTxt, 14);
		len = strlen(tempTxt);
		if ((len < 1) || (sscanf(tempTxt, "%u%s", &n_dump, err) != 1) || n_dump > 65535)
			return 3;

		EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_3), tempTxt, 14);
		len = strlen(tempTxt);
		if ((len < 1) || (sscanf(tempTxt, "%u%s", &n_period, err) != 1) || n_period > 65535)
			return 4;
		
		out_i_trowel = (unsigned short)n_tr;	
    out_i_revers = (unsigned short)n_rev;
		out_dump = (unsigned short)n_dump;
		out_period_answer = (unsigned short)n_period;
	
		Flags.ch_i_tr = Flags.ch_i_rev = Flags.ch_dump = Flags.ch_period = 1; // change amperage params
		Flags.answer_work = 1; 
		return 0;
	}

	//Flags.ch_i_tr = Flags.ch_i_rev = Flags.ch_dump = Flags.ch_period = 1; // change amperage params
	//Flags.ch_20mA = Flags.ch_4mA = 1; // change amperage	
	//Flags.ch_a = Flags.ch_b = Flags.ch_c = Flags.ch_d = Flags.ch_e = 
	//Flags.ch_f = Flags.ch_g = Flags.ch_h = Flags.ch_n = 1; // change params	
	
	return 10;
}

static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'ParamsConfigsWindow'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x00FF2100));
    //
    // Initialization of 'Header'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Настроить параметры");
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'ButtonSave'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetFont(hItem, &GUI_FontVerdana20);
    BUTTON_SetText(hItem, "Сохранить");
    //
    // Initialization of 'ButtonExit'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetFont(hItem, &GUI_FontVerdana20);
    BUTTON_SetText(hItem, "Выйти");
    //
    // Initialization of 'ButtonReset'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
    BUTTON_SetText(hItem, "Сброс");
    BUTTON_SetFont(hItem, &GUI_FontVerdana20);
    //
    // Initialization of 'TextTTrowel'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Ток хода:");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'TextTRevers'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Ток реверса:");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'TextTDump'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
		TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Буфер:");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'TextTPeriod'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
		TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Период ответа:");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
		//
    // Initialization of 'EditTrowel'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
    EDIT_SetFont(hItem, &GUI_FontVerdana20);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    //
    // Initialization of 'EditRevers'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_1);
    EDIT_SetFont(hItem, &GUI_FontVerdana20);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    //
    // Initialization of 'EditDump'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_2);
    EDIT_SetFont(hItem, &GUI_FontVerdana20);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    //
    // Initialization of 'EditPeriod'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_3);
    EDIT_SetFont(hItem, &GUI_FontVerdana20);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		//
    // Initialization of 'TextErr'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR);
		TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
	  TEXT_SetText(hItem, "");
		FillParamsEditsWindow();
		RefreshParamsWindow();
    break;
  case WM_NOTIFY_PARENT:
		if (keyBoard._keyboard != 0)
			break;
	
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'ButtonSave'
      switch(NCode) 
			{
				case WM_NOTIFICATION_CLICKED:
					break;
				case WM_NOTIFICATION_RELEASED:
				{
					int res = SaveParams();
					switch(res)
					{
						case 0:
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "Настройки применены.");
							break;
						case 10:
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "Ошибка: Нет подключения.");
							break;
						case 1:
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "Ошибка: Неверно задан ток хода.");
							break;
						case 2:
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "Ошибка: Неверно задан ток реверса.");
							break;
						case 3:
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "Ошибка: Неверно задан буфер.");
							break;
						case 4:
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "Ошибка: Неверно задан период ответа.");
							break;
					}
					TimerStart();
					break;
				}
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'ButtonExit'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        TimerStop();
				WindowChange(MenuWindow);
				// USER END
        break;
      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'ButtonReset'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
				TimerStop();
        FillParamsEditsWindow();
				break;
      }
      break;
    case ID_EDIT_0: // Notifications sent by 'EditTrowel'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_0), "Изменение тока хода");
				// USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_EDIT_1: // Notifications sent by 'EditRevers'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_1), "Изменение тока реверса");
				// USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_EDIT_2: // Notifications sent by 'EditDump'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_2), "Изменение буфера");
				// USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
		case ID_EDIT_3: // Notifications sent by 'EditDump'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_3), "Изменение периода ответа");
				// USER END
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

WM_HWIN CreateParamsConfigsWindow(void) {
  return GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
}
