#include "GUILogic.h"
#include "DIALOG.h"
#include "Net_User.h"
#include "settings.h"

#define ID_WINDOW_0         (GUI_ID_USER + 0x00)
#define ID_BUTTON_0         (GUI_ID_USER + 0x01)
#define ID_BUTTON_1         (GUI_ID_USER + 0x02)
#define ID_BUTTON_2         (GUI_ID_USER + 0x03)

#define ID_TEXT_HEADER         	(GUI_ID_USER + 0x04)
#define ID_TEXT_IP         	(GUI_ID_USER + 0x05)
#define ID_TEXT_MASK         	(GUI_ID_USER + 0x06)
#define ID_TEXT_DNS1         	(GUI_ID_USER + 0x07)
#define ID_TEXT_DNS2         	(GUI_ID_USER + 0x08)
#define ID_TEXT_GW         	(GUI_ID_USER + 0x09)

#define ID_EDIT_IP        	(GUI_ID_USER + 0x0A)
#define ID_EDIT_MASK      	(GUI_ID_USER + 0x0B)
#define ID_EDIT_GW        	(GUI_ID_USER + 0x0C)
#define ID_EDIT_DNS1      	(GUI_ID_USER + 0x0D)
#define ID_EDIT_DNS2      	(GUI_ID_USER + 0x0E)
#define ID_TEXT_PORTTCP         	(GUI_ID_USER + 0x0F)
#define ID_TEXT_ERR         	(GUI_ID_USER + 0x10)


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "SensorNetConfigsWindow", ID_WINDOW_0, 0, 0, 480, 272, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Header", ID_TEXT_HEADER, 90, 5, 300, 30, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "ButtonSave", ID_BUTTON_0, 165, 235, 100, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonExit", ID_BUTTON_1, 270, 235, 100, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonReset", ID_BUTTON_2, 375, 235, 100, 30, 0, 0x0, 0 },
	
	{ TEXT_CreateIndirect, "TextTIP", ID_TEXT_IP, 		10, 60, 40, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditIP", ID_EDIT_IP, 			80, 60, 150, 30, 0, 0x14, 0 },

  { TEXT_CreateIndirect, "TextTMask", ID_TEXT_MASK, 10, 110, 60, 30, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "EditMask", 	ID_EDIT_MASK, 80, 110, 150, 30, 0, 0x14, 0 },
  
  { TEXT_CreateIndirect, "TextTGW", ID_TEXT_GW, 		10, 160, 60, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditGW", 	ID_EDIT_GW, 		80, 160, 150, 30, 0, 0x14, 0 },

	{ TEXT_CreateIndirect, "TextTDNS1", ID_TEXT_DNS1, 245, 60, 65, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditDNS1", 	ID_EDIT_DNS1, 320, 60, 150, 30, 0, 0x14, 0 },
  
	{ TEXT_CreateIndirect, "TextTDNS2", ID_TEXT_DNS2, 245, 110, 65, 30, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "EditDNS2", 	ID_EDIT_DNS2, 320, 110, 150, 30, 0, 0x14, 0 },

	{ TEXT_CreateIndirect, "TextTPortTCP", ID_TEXT_PORTTCP, 10, 200, 140, 30, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "TextErr", ID_TEXT_ERR, 245, 160, 220, 60, 0, 0x64, 0 },
};

// показывает только IP для подключения если датчик не подключен
void RefreshSensorNetConfigsWindow(void) // сделано
{
	WM_HWIN window = logic.window;
	char tempStr[25];
	
	if (tcp_get_state(tcp_soc_WORK) == tcpStateESTABLISHED)
	{
		// ip
		{
			sprintf(tempStr, "%d.%d.%d.%d", 
				DatIP[0], DatIP[1],
				DatIP[2], DatIP[3]);
			
			EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_IP), tempStr);
		}
		// mask                                        
		{
			sprintf(tempStr, "%d.%d.%d.%d", 
				DatMask[0], DatMask[1],
				DatMask[2], DatMask[3]);

			EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_MASK), tempStr);
		}
		// gateway
		{
			sprintf(tempStr, "%d.%d.%d.%d", 
				DatDefGW[0], DatDefGW[1],
				DatDefGW[2], DatDefGW[3]);
		
			EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_GW), tempStr);
		}
		// dns1
		{
			sprintf(tempStr, "%d.%d.%d.%d", 
				DatPriDNS[0], DatPriDNS[1],
				DatPriDNS[2], DatPriDNS[3]);

			EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_DNS1), tempStr);
		}
		// dns2
		{
			sprintf(tempStr, "%d.%d.%d.%d", 
				DatSecDNS[0], DatSecDNS[1],
				DatSecDNS[2], DatSecDNS[3]);

			EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_DNS2), tempStr);
		}
		TEXT_SetText(WM_GetDialogItem(window, ID_TEXT_ERR), "");
	}
	else
	{
		// ip
		sprintf(tempStr, "%d.%d.%d.%d", 
			rem_ip[0], rem_ip[1],
			rem_ip[2], rem_ip[3]);
		
		EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_IP), tempStr);
		EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_MASK), "");
		EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_GW), "");
		EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_DNS1), "");
		EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_DNS2), "");

		TEXT_SetText(WM_GetDialogItem(logic.window, ID_TEXT_ERR), "Соединение\nне установлено.");
	}
}

int SaveNetData(void)
{
	WM_HWIN window = logic.window;
	uint8_t sensIP[4];
	char iptxt[25];
	
	EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_IP), iptxt, 24); // ip
	if (strlen(iptxt) < 1)
		return 11;
	if (get_IP(iptxt, sensIP) > 0)
		return 1;
	
	if (tcp_get_state(tcp_soc_WORK) == tcpStateESTABLISHED)
	{
		uint8_t sensMask[4], sensGW[4], sensDNS1[4], sensDNS2[4];
		
		EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_MASK), iptxt, 24); // mask
		if (strlen(iptxt) < 1)
			return 12;
		if (get_IP(iptxt, sensMask) > 0)
			return 2;
		
		EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_GW), iptxt, 24); // gateway
		if (strlen(iptxt) < 1)
			return 13;
		if (get_IP(iptxt, sensGW) > 0)
			return 3;

		EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_DNS1), iptxt, 24); // dns1
		if (strlen(iptxt) < 1)
			return 14;
		if (get_IP(iptxt, sensDNS1) > 0)
			return 4;

		EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_DNS2), iptxt, 24); // dns2
		if (strlen(iptxt) < 1)
			return 15;
		if (get_IP(iptxt, sensDNS2) > 0)
			return 5;

		for(int i = 0; i < 4; i++)
		{
			outDatIP[i] = sensIP[i];
			outDatMask[i] = sensMask[i];
			outDatDefGW[i] = sensGW[i];
			outDatPriDNS[i] = sensDNS1[i];
			outDatSecDNS[i] = sensDNS2[i];
		}
		
		Flags.ch_IP = Flags.ch_Mask = Flags.ch_DefGW = Flags.ch_PriDNS = Flags.ch_SecDNS = 1;
		Flags.answer_work = 1;
	}

	for(int i = 0; i < 4; i++)
		rem_ip[i] = sensIP[i];
	  
	// save to qspi
	Write_settings();
	return 0;
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
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_HEADER);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Настроить датчик (сеть)");
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
    BUTTON_SetFont(hItem, &GUI_FontVerdana20);
    BUTTON_SetText(hItem, "Сброс");
    //
    // Initialization of 'TextTIP'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_IP);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "IP: ");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'EditIP'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_IP);
    EDIT_SetFont(hItem, GUI_FONT_20_1);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    //
    // Initialization of 'TextTMask'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_MASK);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Маска: ");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
		//
    // Initialization of 'EditMask'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_MASK);
    EDIT_SetFont(hItem, GUI_FONT_20_1);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		//
    // Initialization of 'TextTGW'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_GW);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Шлюз: ");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'EditGW'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_GW);
    EDIT_SetFont(hItem, GUI_FONT_20_1);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    //
    // Initialization of 'TextTDNS1'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_DNS1);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "DNS1: ");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'EditDNS1'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_DNS1);
    EDIT_SetFont(hItem, GUI_FONT_20_1);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    //
    // Initialization of 'TextTDNS2'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_DNS2);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "DNS2: ");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'EditDNS2'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_DNS2);
    EDIT_SetFont(hItem, GUI_FONT_20_1);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		//
    // Initialization of 'TextTPortTCP'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_PORTTCP);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Port TCP: 4000");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
		//
    // Initialization of 'TextERR'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    
		RefreshSensorNetConfigsWindow();
		break;
  case WM_NOTIFY_PARENT:
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
					HideKeyBoard();
					int res = SaveNetData();
					switch(res)
					{
						case 0:
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "Настройки применены.");
							break;
						case 1:
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "  Ошибка:\nНеверно задан IP.");
							break;
						case 2:
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "  Ошибка:\nНеверно задана маска.");
							break;
						case 3:
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "  Ошибка:\nНеверно задан шлюз.");
							break;
						case 4:
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "  Ошибка:\nНеверно задан DNS1.");
							break;
						case 5:
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "  Ошибка:\nНеверно задан DNS2.");
							break;
					}
					break;
				}
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'ButtonExit'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        HideKeyBoard();
				WindowChange(MenuWindow);
				break;
      }
      break;
		case ID_BUTTON_2: // Notifications sent by 'ButtonReset'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        HideKeyBoard();
				RefreshSensorNetConfigsWindow();
				break;
      }
      break;
    case ID_EDIT_IP: // Notifications sent by 'EditIP'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_IP));
				break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_EDIT_MASK: // Notifications sent by 'EditMask'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_MASK));
				break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_EDIT_GW: // Notifications sent by 'EditGW'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_GW));
				break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_EDIT_DNS1: // Notifications sent by 'EditDNS1'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_DNS1));
				break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_EDIT_DNS2: // Notifications sent by 'EditDNS2'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_DNS2));
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
