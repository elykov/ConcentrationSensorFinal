#include "GUILogic.h"
#include "DIALOG.h"
#include "Net_User.h"
#include "global_var.h"
#include "math.h"

#define ID_WINDOW_0   		(GUI_ID_USER + 0x00)
#define ID_TEXT_0    			(GUI_ID_USER + 0x01)
#define ID_BUTTON_0   		(GUI_ID_USER + 0x02)
#define ID_BUTTON_1   		(GUI_ID_USER + 0x03)
#define ID_BUTTON_2   		(GUI_ID_USER + 0x04)
//#define ID_BUTTON_3				(GUI_ID_USER + 0x05)

#define ID_TEXT_Trow  		(GUI_ID_USER + 0x11)
#define ID_TEXT_Rev    		(GUI_ID_USER + 0x12)
#define ID_TEXT_Dump    	(GUI_ID_USER + 0x13)
#define ID_TEXT_Period  	(GUI_ID_USER + 0x14)
#define ID_TEXT_Offset  	(GUI_ID_USER + 0x15)
#define ID_TEXT_Gain  		(GUI_ID_USER + 0x16)
#define ID_TEXT_DumpSum  	(GUI_ID_USER + 0x17)
#define ID_TEXT_Sum  			(GUI_ID_USER + 0x18)

#define ID_TEXT_Water  		(GUI_ID_USER + 0x21)
#define ID_TEXT_Air  			(GUI_ID_USER + 0x22)
#define ID_TEXT_TKM		    (GUI_ID_USER + 0x23)
#define ID_TEXT_KM  			(GUI_ID_USER + 0x24)

#define ID_EDIT_Trow    	(GUI_ID_USER + 0x31)
#define ID_EDIT_Rev    		(GUI_ID_USER + 0x32)
#define ID_EDIT_Dump    	(GUI_ID_USER + 0x33)
#define ID_EDIT_Period    (GUI_ID_USER + 0x34)
#define ID_EDIT_Offset  	(GUI_ID_USER + 0x35)
#define ID_EDIT_Gain  		(GUI_ID_USER + 0x36)
#define ID_EDIT_Water  		(GUI_ID_USER + 0x37)
#define ID_EDIT_Air  			(GUI_ID_USER + 0x38)

#define ID_TEXT_ERR   		(GUI_ID_USER + 0x20)


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "ParamsConfigsWindow", ID_WINDOW_0, 0, 0, 480, 272, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Header", ID_TEXT_0, 90, 5, 300, 30, 0, 0x64, 0 },
	{ BUTTON_CreateIndirect, "ButtonSave", ID_BUTTON_0, 165, 235, 100, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonExit", ID_BUTTON_1, 270, 235, 100, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonReset", ID_BUTTON_2, 375, 235, 100, 30, 0, 0x0, 0 },

	{ TEXT_CreateIndirect, "TextTrowel", ID_TEXT_Trow, 10, 45, 140, 30, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "EditTrowel", ID_EDIT_Trow, 150, 45, 100, 30, 0, 0xa, 0 },

	{ TEXT_CreateIndirect, "TextRevers", ID_TEXT_Rev, 10, 78, 140, 30, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "EditRevers", ID_EDIT_Rev, 150, 78, 100, 30, 0, 0xa, 0 },
	
	{ TEXT_CreateIndirect, "TextPeriod", ID_TEXT_Period, 10, 111, 140, 30, 0, 0x0, 0 },
	{ EDIT_CreateIndirect, "EditPeriod", ID_EDIT_Period, 150, 111, 100, 30, 0, 0xa, 0 },	


	{ TEXT_CreateIndirect, "TextDump", ID_TEXT_Dump, 265, 45, 100, 30, 0, 0x0, 0 },
	{ EDIT_CreateIndirect, "EditDump", ID_EDIT_Dump, 370, 45, 100, 30, 0, 0xa, 0 },

	{ TEXT_CreateIndirect, "TextOffset", ID_TEXT_Offset, 265, 78, 100, 30, 0, 0x0, 0 },
	{ EDIT_CreateIndirect, "EditOffset", ID_EDIT_Offset, 370, 78, 100, 30, 0, 0xa, 0 },

  { TEXT_CreateIndirect, "TextGain", ID_TEXT_Gain, 265, 111, 100, 30, 0, 0x0, 0 },
	{ EDIT_CreateIndirect, "EditGain", ID_EDIT_Gain, 370, 111, 100, 30, 0, 0xa, 0 },

	{ TEXT_CreateIndirect, "TextWater", ID_TEXT_Water, 10, 144, 80, 30, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "EditWater", ID_EDIT_Water, 90, 144, 110, 30, 0, 0xa, 0 },

  { TEXT_CreateIndirect, "TextAir", ID_TEXT_Air, 10, 177, 80, 30, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "EditAir", ID_EDIT_Air, 90, 177, 110, 30, 0, 0xa, 0 },

	{ TEXT_CreateIndirect, "TextTKM", ID_TEXT_TKM, 10, 235, 80, 30, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "TextKM", ID_TEXT_KM, 60, 235, 110, 30, 0, 0xa, 0 },
                                                 
	{ TEXT_CreateIndirect, "TextDumpSum", ID_TEXT_DumpSum, 10, 207, 160, 30, 0, 0x0, 0 },

	{ TEXT_CreateIndirect, "TextErr", ID_TEXT_ERR, 220, 155, 250, 60, 0, 0x0, 0 },
};

void FillParamsEditsWindow(void)
{
	WM_HWIN window = logic.window; 
	char tempStr[15];

	// i_trowel
	sprintf(tempStr, "%u", i_trowel);
	EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_Trow), tempStr);

	// i_revers
	sprintf(tempStr, "%u", i_revers);
	EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_Rev), tempStr);
	
	// dump
	sprintf(tempStr, "%u", dump);
	EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_Dump), tempStr);

	// period_answer
	sprintf(tempStr, "%u", period_answer);
	EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_Period), tempStr);

	// offset
	sprintf(tempStr, "%.2f", offset);
	EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_Offset), tempStr);

	// gain
	sprintf(tempStr, "%.2f", gain);
	EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_Gain), tempStr);

  // water
	sprintf(tempStr, "%u", water);
	EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_Water), tempStr);
	
	// air
	sprintf(tempStr, "%u", air);
	EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_Air), tempStr);
}

void RefreshParamsWindow(void)
{
	if (isTextErrChangable)
	{
		if (tcp_get_state(tcp_soc_WORK) != tcpStateESTABLISHED)
		{
			TEXT_SetText(WM_GetDialogItem(logic.window, ID_TEXT_ERR), "Соединение\nне установлено.");
		}
		else
			TEXT_SetText(WM_GetDialogItem(logic.window, ID_TEXT_ERR), "");
	}

	char tempStr[15];
	// km
	sprintf(tempStr, "%.3f", km);
	TEXT_SetText(WM_GetDialogItem(logic.window, ID_TEXT_KM), tempStr);

	// DumpSum
	sprintf(tempStr, "Б: %u; М: %u", DumpSum, sum);
	TEXT_SetText(WM_GetDialogItem(logic.window, ID_TEXT_DumpSum), tempStr);

	if (keyBoard.isRefreshableFields)
		FillParamsEditsWindow();
}

static int SaveParams(void)
{
	WM_HWIN window = logic.window;
	char tempTxt[20], err[15];
	size_t len;
	
	if (tcp_get_state(tcp_soc_WORK) == tcpStateESTABLISHED) // нет смысла что-то делать, если нет соединения
	{
		uint32_t n_tr, n_rev, n_dump, n_period;
		
    EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_Trow), tempTxt, 14);
		len = strlen(tempTxt);
		if ((len < 1) || (sscanf(tempTxt, "%u%s", &n_tr, err) != 1) || n_tr > 65535)
			return 1;
		
		EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_Rev), tempTxt, 14);
		len = strlen(tempTxt);
		if ((len < 1) || (sscanf(tempTxt, "%u%s", &n_rev, err) != 1) || n_rev > 65535)
			return 2;
	
		EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_Dump), tempTxt, 14);
		len = strlen(tempTxt);
		if ((len < 1) || (sscanf(tempTxt, "%u%s", &n_dump, err) != 1) || n_dump > 65535)
			return 3;

		EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_Period), tempTxt, 14);
		len = strlen(tempTxt);
		if ((len < 1) || (sscanf(tempTxt, "%u%s", &n_period, err) != 1) || n_period > 65535)
			return 4;
		
		EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_Offset), tempTxt, 14);
		len = strlen(tempTxt);
		if ((len < 1) || (sscanf(tempTxt, "%f%s", &out_offset, err) != 1) || isnan(out_offset))
			return 5;

		EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_Gain), tempTxt, 14);
		len = strlen(tempTxt);
		if ((len < 1) || (sscanf(tempTxt, "%f%s", &out_gain, err) != 1) || isnan(out_gain))
			return 6;

		EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_Air), tempTxt, 14);
		len = strlen(tempTxt);
		if ((len < 1) || (sscanf(tempTxt, "%u%s", &out_air, err) != 1))
			return 7;

		EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_Water), tempTxt, 14);
		len = strlen(tempTxt);
		if ((len < 1) || (sscanf(tempTxt, "%u%s", &out_water, err) != 1))
			return 8;

		out_i_trowel = (unsigned short)n_tr;	
    out_i_revers = (unsigned short)n_rev;
		out_dump = (unsigned short)n_dump;
		out_period_answer = (unsigned short)n_period;

		Flags.ch_i_tr = Flags.ch_i_rev = Flags.ch_dump = 
		Flags.ch_period = Flags.ch_offset = Flags.ch_gain =
		Flags.ch_air = Flags.ch_water = 1; // change amperage params
		sendParam = 0x01;
		Flags.answer_work = 1; 
		return 0;
	}

	return 10;
}

static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  
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
    // Initialization of 'ButtonReset'
    //
    //hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
    //BUTTON_SetText(hItem, "Modbus");
    //BUTTON_SetFont(hItem, &GUI_FontVerdana20);
    //
    // Initialization of 'TextTTrowel'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Trow);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Ток хода:");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'TextTRevers'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Rev);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Ток реверса:");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'TextTDump'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Dump);
		TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Буфер:");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'TextTPeriod'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Period);
		TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Период ответа:");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
		//
    // Initialization of 'TextOffset'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Offset);
		TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Смещение:");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
		//
    // Initialization of 'TextGain'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Gain);
		TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Усиление:");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
		//
    // Initialization of 'EditTrowel'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_Trow);
    EDIT_SetFont(hItem, &GUI_FontVerdana20);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    //
    // Initialization of 'EditRevers'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_Rev);
    EDIT_SetFont(hItem, &GUI_FontVerdana20);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    //
    // Initialization of 'EditDump'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_Dump);
    EDIT_SetFont(hItem, &GUI_FontVerdana20);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    //
    // Initialization of 'EditPeriod'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_Period);
    EDIT_SetFont(hItem, &GUI_FontVerdana20);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		//
    // Initialization of 'EditOffset'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_Offset);
    EDIT_SetFont(hItem, &GUI_FontVerdana20);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		//
    // Initialization of 'EditGain'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_Gain);
    EDIT_SetFont(hItem, &GUI_FontVerdana20);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		//
    // Initialization of 'TextWater'
    //
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Water);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Вода: ");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
		//
    // Initialization of 'EditWater'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_Water);
    EDIT_SetFont(hItem, &GUI_FontVerdana20);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		//
    // Initialization of 'TextAir'
    //
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_Air);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Воздух: ");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
		//
    // Initialization of 'EditAir'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_Air);
    EDIT_SetFont(hItem, &GUI_FontVerdana20);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		//
    // Initialization of 'TextTKM'
    //
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TKM);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "КМ: ");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
		//
    // Initialization of 'TextKM'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_KM);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
		//
    // Initialization of 'TextTDumpSum'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_DumpSum);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
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
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "Ошибка: Неверно\nзадан ток хода.");
							break;
						case 2:
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "Ошибка: Неверно\nзадан ток реверса.");
							break;
						case 3:
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "Ошибка: Неверно\nзадан буфер.");
							break;
						case 4:
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "Ошибка: Неверно\nзадан период ответа.");
							break;
						case 5:
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "Ошибка: Неверно\nзадано смещение.");
							break;
						case 6:
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "Ошибка: Неверно\nзадано усиление.");
							break;	
						case 7:
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "Ошибка: Неверно\nзадан воздух.");
							break;
						case 8:
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "Ошибка: Неверно\nзадана вода.");
							break;
					}
					TimerStart();
					keyBoard.isRefreshableFields = true;
					break;
				}
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'ButtonExit'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        TimerStop();
				keyBoard.isRefreshableFields = true;
				isTextErrChangable = true;
				WindowChange(MenuWindow);
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
				FillParamsEditsWindow();
				isTextErrChangable = true;
				break;
      }
      break;
    case ID_EDIT_Trow: // Notifications sent by 'EditTrowel'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_Trow), "Изменение тока хода");
				// USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_EDIT_Rev: // Notifications sent by 'EditRevers'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_Rev), "Изменение тока реверса");
				// USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_EDIT_Dump: // Notifications sent by 'EditDump'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_Dump), "Изменение буфера");
				// USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
		case ID_EDIT_Period: // Notifications sent by 'EditDump'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_Period), "Изменение периода ответа");
				// USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
		case ID_EDIT_Offset: // Notifications sent by 'EditOffset'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_Offset), "Изменение смещения");
				// USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
		case ID_EDIT_Gain: // Notifications sent by 'EditGain'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_Gain), "Изменение усиления");
				// USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
		case ID_EDIT_Air: // Notifications sent by 'EditAir'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_Air), "Изменение воздуха");
				break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
		case ID_EDIT_Water: // Notifications sent by 'EditWater'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_Water), "Изменение воды");
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
