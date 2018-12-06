#include "GUILogic.h"
#include "DIALOG.h"
#include "Net_User.h"
#include "math.h"

#define ID_WINDOW_0       (GUI_ID_USER + 0x00)
#define ID_TEXT_0         (GUI_ID_USER + 0x01)
#define ID_BUTTON_0       (GUI_ID_USER + 0x02)
#define ID_BUTTON_1       (GUI_ID_USER + 0x03)
#define ID_BUTTON_2       (GUI_ID_USER + 0x04)
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
#define ID_TEXT_10        (GUI_ID_USER + 0x13)
#define ID_TEXT_11        (GUI_ID_USER + 0x14)
#define ID_TEXT_12        (GUI_ID_USER + 0x15)
#define ID_EDIT_5        	(GUI_ID_USER + 0x16)
#define ID_TEXT_ERR       (GUI_ID_USER + 0x17)   

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "PIDConfigsWindow", ID_WINDOW_0, 0, 0, 480, 272, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Header", ID_TEXT_0, 90, 5, 300, 30, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "ButtonSave", ID_BUTTON_0, 165, 235, 100, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonExit", ID_BUTTON_1, 270, 235, 100, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonReset", ID_BUTTON_2, 375, 235, 100, 30, 0, 0x0, 0 },
	
	{ TEXT_CreateIndirect, "TextTReferens", ID_TEXT_4, 10, 45, 100, 27, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditReferens", ID_EDIT_3, 120, 45, 140, 27, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "TextTDumpI", ID_TEXT_5, 10, 78, 100, 27, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditDumpI", ID_EDIT_4, 120, 78, 140, 27, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "TextTPidPeriod", ID_TEXT_12, 10, 111, 100, 27, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditPidPeriod", ID_EDIT_5, 120, 111, 140, 27, 0, 0x64, 0 },

  { TEXT_CreateIndirect, "TextTConc", ID_TEXT_6, 		10, 145, 140, 27, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "TextConc", ID_TEXT_7, 	 160, 145, 110, 27, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "TextTAmp", ID_TEXT_8,  		10, 172, 140, 27, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "TextAmp", ID_TEXT_9,  	 160, 172, 110, 27, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "TextTDamper", ID_TEXT_10, 10, 199, 140, 27, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "TextDamper", ID_TEXT_11, 160, 199, 110, 27, 0, 0x64, 0 },

	{ TEXT_CreateIndirect, "TextTP", ID_TEXT_1, 320, 45, 30, 27, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditP", ID_EDIT_0, 350, 45, 120, 27, 0, 0xa, 0 },
  { TEXT_CreateIndirect, "TextTI", ID_TEXT_2, 320, 78, 30, 27, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditI", ID_EDIT_1, 350, 78, 120, 27, 0, 0xa, 0 },
  { TEXT_CreateIndirect, "TextTD", ID_TEXT_3, 320, 111, 30, 27, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditD", ID_EDIT_2, 350, 111, 120, 27, 0, 0xa, 0 },
	
  { TEXT_CreateIndirect, "TextErr", ID_TEXT_ERR, 258, 165, 222, 60, 0, 0x64, 0 },
};

static void FillPID(void)
{
	WM_HWIN window = logic.window; 
	char tempStr[9];

	// p
	sprintf(tempStr, "%.3f", P_factor);
	EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_0), tempStr);

	// i
	sprintf(tempStr, "%.3f", I_factor);
	EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_1), tempStr);
	
	// d
	sprintf(tempStr, "%.3f", D_factor);
	EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_2), tempStr);

	// Задание
	if (isnan(referens))
		sprintf(tempStr, "-");
	else 
		sprintf(tempStr, "%.3f", referens);
	EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_3), tempStr);

	// PID(dump_i)
	sprintf(tempStr, "%u", dump_i);
	EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_4), tempStr);

	sprintf(tempStr, "%u", pid_period);
	EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_5), tempStr);
}

void RefreshPIDWindow(void)
{
	WM_HWIN window = logic.window; 
	// функция работает только когда это окно текущее, а оно хранится в logic.window
	char tempStr[30];

	// conc
	if (isnan(Cb))
		sprintf(tempStr, "A/0       ");
	else if (Cb < 0)
		sprintf(tempStr, "0.000 %%  ");
	else
		sprintf(tempStr, "%.3f %%  ", Cb);
	TEXT_SetText(WM_GetDialogItem(window, ID_TEXT_7), tempStr);
	
	// amp
	if (isnan(Output_I))
		sprintf(tempStr, "A/0       ");
	else
		sprintf(tempStr, "%.2f мА", Output_I);
	TEXT_SetText(WM_GetDialogItem(window, ID_TEXT_9), tempStr);
                         
	if (isnan(damper))
		sprintf(tempStr, "A/0      ");
	else
		sprintf(tempStr, "%.2f %%", damper);
	TEXT_SetText(WM_GetDialogItem(window, ID_TEXT_11), tempStr);
                                                      
	if (isTextErrChangable)
	{  
		if (tcp_get_state(tcp_soc_WORK) != tcpStateESTABLISHED)
			TEXT_SetText(WM_GetDialogItem(window, ID_TEXT_ERR), "Соединение\nне установлено.");   
		else
			TEXT_SetText(WM_GetDialogItem(window, ID_TEXT_ERR), "");
	}

	if (keyBoard.isRefreshableFields)
		FillPID();
}

static int SavePIDParams(void)
{
	WM_HWIN window = logic.window;
	char tempTxt[20], err[15];
	size_t len;
	
	if (tcp_get_state(tcp_soc_WORK) == tcpStateESTABLISHED) // нет смысла что-то делать, если нет соединения
	{
		int32_t n_dump_i, n_pid_period;
		float n_ref, n_P, n_I, n_D; // задание

		EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_3), tempTxt, 19);
		len = strlen(tempTxt);
		if ((len < 1) || (sscanf(tempTxt, "%f%s", &n_ref, err) != 1) || isnan(n_ref))
			return 1;
		
    EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_4), tempTxt, 14);
		len = strlen(tempTxt);
		if ((len < 1) || (sscanf(tempTxt, "%u%s", &n_dump_i, err) != 1) || n_dump_i > 65535 || n_dump_i < 0)
			return 2;
		
		EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_0), tempTxt, 14);
		len = strlen(tempTxt);
		if ((len < 1) || (sscanf(tempTxt, "%f%s", &n_P, err) != 1) || isnan(n_P))
			return 3;
		
		EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_1), tempTxt, 14);
		len = strlen(tempTxt);
		if ((len < 1) || (sscanf(tempTxt, "%f%s", &n_I, err) != 1) || isnan(n_I))
			return 4;

		EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_2), tempTxt, 14);
		len = strlen(tempTxt);
		if ((len < 1) || (sscanf(tempTxt, "%f%s", &n_D, err) != 1) || isnan(n_D))
			return 5;
		
		EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_5), tempTxt, 14);
		len = strlen(tempTxt);
		if ((len < 1) || (sscanf(tempTxt, "%u%s", &n_pid_period, err) != 1) || n_pid_period > 65535 || n_pid_period < 0)
			return 6;

		out_referens = n_ref;
		out_dump_i = (unsigned short)n_dump_i;
    out_P_factor = n_P;
		out_I_factor = n_I;
    out_D_factor = n_D;
		out_pid_period = n_pid_period;

    Flags.ch_P = Flags.ch_I = Flags.ch_D = // change PID 
			Flags.ch_dump_i = Flags.ch_ref = Flags.ch_pid_period = 1; // change other PID params
		
		sendParam = 0x02;
		Flags.answer_work = 1; 
		return 0;
	}
	
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
    // Initialization of 'TextTPidPeriod'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_12);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Период:");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
		//
    // Initialization of 'EditPidPeriod'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_5);
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
    // Initialization of 'TextTDamper'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_10);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Заслонка:");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
		//
    // Initialization of 'TextDamper'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_11);
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
							keyBoard.isRefreshableFields = true;
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
						case 6:                                                                             
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "  Ошибка:\nНеверно задан период.");
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
				keyBoard.isRefreshableFields = true;
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
				keyBoard.isRefreshableFields = true;
        RefreshPIDWindow();
				FillPID();
        break;
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
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_4), "Изменение буфера");
				break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
		case ID_EDIT_5: // Notifications sent by 'EditPidPeriod'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_5), "Изменение периода ПИД");
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
