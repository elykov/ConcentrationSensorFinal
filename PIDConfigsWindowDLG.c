#include "GUILogic.h"
#include "DIALOG.h"
#include "Net_User.h"

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
#define ID_TEXT_7         (GUI_ID_USER + 0x10)
#define ID_TEXT_8         (GUI_ID_USER + 0x11)
#define ID_TEXT_9         (GUI_ID_USER + 0x12)
#define ID_TEXT_ERR       (GUI_ID_USER + 0x13)   

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "PIDConfigsWindow", ID_WINDOW_0, 0, 0, 480, 272, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Header", ID_TEXT_0, 90, 5, 300, 30, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "ButtonSave", ID_BUTTON_0, 165, 235, 100, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonExit", ID_BUTTON_1, 270, 235, 100, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonReset", ID_BUTTON_2, 375, 235, 100, 30, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "TextTP", ID_TEXT_1, 340, 50, 30, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditP", ID_EDIT_0, 370, 50, 100, 30, 0, 0xa, 0 },
  { TEXT_CreateIndirect, "TextTI", ID_TEXT_2, 340, 90, 30, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditI", ID_EDIT_1, 370, 90, 100, 30, 0, 0xa, 0 },
  { TEXT_CreateIndirect, "TextTD", ID_TEXT_3, 340, 130, 30, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditD", ID_EDIT_2, 370, 130, 100, 30, 0, 0xa, 0 },
	{ TEXT_CreateIndirect, "TextTReferens", ID_TEXT_4, 10, 50, 100, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditReferens", ID_EDIT_3, 120, 50, 140, 30, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "TextTDumpI", ID_TEXT_5, 10, 90, 100, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditDumpI", ID_EDIT_4, 120, 90, 140, 30, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "TextTConc", ID_TEXT_6, 10, 150, 140, 30, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "TextConc", ID_TEXT_7, 160, 150, 110, 30, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "TextTAmp", ID_TEXT_8, 10, 190, 140, 30, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "TextAmp", ID_TEXT_9, 160, 190, 110, 30, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "TextErr", ID_TEXT_ERR, 270, 165, 210, 60, 0, 0x64, 0 },
};

void RefreshPIDWindow(void)
{
	WM_HWIN window = logic.window; 
	// функция работает только когда это окно текущее, а оно хранится в logic.window
	char tempStr[15];

	// conc
	sprintf(tempStr, "%.3f %%", Cb);
	TEXT_SetText(WM_GetDialogItem(window, ID_TEXT_7), tempStr);
	
	// amp
	sprintf(tempStr, "%.2f мА", Output_I);
	TEXT_SetText(WM_GetDialogItem(window, ID_TEXT_9), tempStr);
                                                                                 
	if (isTextErrChangable)
	{  
		if (tcp_get_state(tcp_soc_WORK) != tcpStateESTABLISHED)
			TEXT_SetText(WM_GetDialogItem(window, ID_TEXT_ERR), "Соединение\nне установлено.");   
		else
			TEXT_SetText(WM_GetDialogItem(window, ID_TEXT_ERR), "");
	}
}

static void FillPID(void)
{
	WM_HWIN window = logic.window; 
	char tempStr[9];

	// p
	sprintf(tempStr, "%u", P_factor);
	EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_0), tempStr);

	// i
	sprintf(tempStr, "%u", I_factor);
	EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_1), tempStr);
	
	// d
	sprintf(tempStr, "%u", D_factor);
	EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_2), tempStr);

	// Задание
	sprintf(tempStr, "%.3f", referens);
	EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_3), tempStr);

	// PID(dump_i)
	sprintf(tempStr, "%u", dump_i);
	EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_4), tempStr);
}

static int SavePIDParams(void)
{
	WM_HWIN window = logic.window;
	char tempTxt[20], err[15];
	size_t len;
	
	if (tcp_get_state(tcp_soc_WORK) == tcpStateESTABLISHED) // нет смысла что-то делать, если нет соединения
	{
		float n_ref; // задание
		EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_3), tempTxt, 19);
		len = strlen(tempTxt);
		if ((len < 1) || (sscanf(tempTxt, "%f%s", &n_ref, err) != 1))
			return 1;
		
		uint32_t n_dump_i, n_P, n_I, n_D;
    EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_4), tempTxt, 14);
		len = strlen(tempTxt);
		if ((len < 1) || (sscanf(tempTxt, "%u%s", &n_dump_i, err) != 1) || n_dump_i > 65535)
			return 2;
		
		EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_0), tempTxt, 14);
		len = strlen(tempTxt);
		if ((len < 1) || (sscanf(tempTxt, "%u%s", &n_P, err) != 1) || n_P > 255)
			return 3;
		
		EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_1), tempTxt, 14);
		len = strlen(tempTxt);
		if ((len < 1) || (sscanf(tempTxt, "%u%s", &n_I, err) != 1) || n_I > 255)
			return 4;

		EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_2), tempTxt, 14);
		len = strlen(tempTxt);
		if ((len < 1) || (sscanf(tempTxt, "%u%s", &n_D, err) != 1) || n_D > 255)
			return 5;
		
		out_referens = n_ref;
		out_dump_i = (unsigned short)n_dump_i;
    out_P_factor = (uint8_t)n_P;
		out_I_factor = (uint8_t)n_I;
    out_D_factor = (uint8_t)n_D;

    Flags.ch_P = Flags.ch_I = Flags.ch_D = // change PID 
			Flags.ch_dump_i = Flags.ch_ref = 1; // change other PID params
		
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
    // Initialization of 'PIDConfigsWindow'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x00FF2100));
    //
    // Initialization of 'Header'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Настроить ПИД");
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
    EDIT_SetFont(hItem, &GUI_FontVerdana20);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		//
    // Initialization of 'TextTReferens'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Задание:");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
		//
    // Initialization of 'EditReferens'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_3);
    EDIT_SetFont(hItem, &GUI_FontVerdana20);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		//
    // Initialization of 'TextTDumpI'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_5);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Буфер:");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
		//
    // Initialization of 'EditDumpI'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_4);
    EDIT_SetFont(hItem, &GUI_FontVerdana20);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		//
    // Initialization of 'TextTConc'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_6);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Концентрация:");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
		//
    // Initialization of 'TextConc'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_7);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
		//
    // Initialization of 'TextTAmp'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_8);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Выходной ток:");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
		//
    // Initialization of 'TextAmp'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_9);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
    //
    // Initialization of 'TextErr'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
		TEXT_SetText(hItem, "");
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));

		RefreshPIDWindow();
    FillPID();
    break;
  case WM_NOTIFY_PARENT:
		if (keyBoard._keyboard != 0)
			break;
	
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'ButtonSave'
      switch(NCode) {
				case WM_NOTIFICATION_CLICKED:
					break;
				case WM_NOTIFICATION_RELEASED:
				{
					// USER START (Optionally insert code for reacting on notification message)
					int res = SavePIDParams();
					switch(res)
					{
						case 0:
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "Настройки применены.");
							break;
						case 10:
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "  Ошибка:\nНет подключения.");
							break;
						case 1:
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "  Ошибка:\nНеверно задано задание.");
							break;
						case 2:
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "  Ошибка:\nНеверно задан буфер.");
							break;
						case 3:
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "  Ошибка:\nНеверно задан P.");
							break;
						case 4:
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "  Ошибка:\nНеверно задан I.");
							break;
						case 5:                                                                             
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "  Ошибка:\nНеверно задан D.");
							break;
					}
					TimerStart();
					// USER END
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
        RefreshPIDWindow();
				FillPID();
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_EDIT_0: // Notifications sent by 'EditP'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_0), "Изменение параметра P в ПИД");
				// USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_EDIT_1: // Notifications sent by 'EditI'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_1), "Изменение параметра I в ПИД");
				// USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_EDIT_2: // Notifications sent by 'EditD'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_2), "Изменение параметра D в ПИД");
				// USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
		case ID_EDIT_3: // Notifications sent by 'EditReferens'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_3), "Изменение задания концентрации");
				// USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
		case ID_EDIT_4: // Notifications sent by 'EditDumpI'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_4), "Изменение Dump_i");
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

WM_HWIN CreatePIDConfigsWindow(void) {
  return GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
}
