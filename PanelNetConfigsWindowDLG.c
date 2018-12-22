#include "GUILogic.h"
#include "stm32f7xx_hal.h"
#include "DIALOG.h"
#include "qspi.h"
#include "settings.h"  
#include "serverPart.h"
#include "Net_User.h"
#include "modbus.h"

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
#define ID_TEXT_ERR         	(GUI_ID_USER + 0x0F)

#define ID_TEXT_MODBUS 			(GUI_ID_USER + 0x11)


	
void EditTextSetting(uint8_t* address, uint32_t id)
{
	char tempStr[25];
	sprintf(tempStr, "%d.%d.%d.%d", 
			address[0], address[1],
			address[2], address[3]);
		
	EDIT_SetText(WM_GetDialogItem(logic.window, id), tempStr);
}

void SetPanelNetEdits(void)
{
	EditTextSetting(LocM.IpAddr, ID_EDIT_IP);
	EditTextSetting(LocM.NetMask, ID_EDIT_MASK);
	EditTextSetting(LocM.DefGW, ID_EDIT_GW);
	EditTextSetting(LocM.PriDNS, ID_EDIT_DNS1);
	EditTextSetting(LocM.SecDNS, ID_EDIT_DNS2);
}

void RefreshPanelNetConfigsWindow(void)
{
	if (isTextErrChangable)
		TEXT_SetText(WM_GetDialogItem(logic.window, ID_TEXT_ERR), "");   

	if (keyBoard.isRefreshableFields)
		SetPanelNetEdits();
}

int SetPanelNetConfigs()
{
	WM_HWIN window = logic.window;
	uint8_t currentIP[4], currentMask[4], currentGW[4], currentDNS1[4], currentDNS2[4];
	char iptxt[25];

  EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_IP), iptxt, 24); // ip
	if (get_IP(iptxt, currentIP) > 0)
		return 1;
	
	EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_MASK), iptxt, 24); // mask
	if (get_IP(iptxt, currentMask) > 0)
		return 2;
	
	EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_GW), iptxt, 24); // gateway
	if (get_IP(iptxt, currentGW) > 0)
		return 3;
	
	EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_DNS1), iptxt, 24); // dms1
	if (get_IP(iptxt, currentDNS1) > 0)
		return 4;
	
	EDIT_GetText(WM_GetDialogItem(window, ID_EDIT_DNS2), iptxt, 24); // dns2
	if (get_IP(iptxt, currentDNS2) > 0)
		return 5;

	tcp_close(tcp_soc_WORK);
	tcp_close(tcp_soc_SERVER);
	tcp_close(tcp_soc_PLC);
	tcp_close(tcp_soc_TECH);

	for(int i = 0; i < 4; ++i)
	{
		LocM.IpAddr[i] = currentIP[i];
		LocM.NetMask[i] = currentMask[i];
		LocM.DefGW[i] = currentGW[i];
		LocM.PriDNS[i] = currentDNS1[i];
		LocM.SecDNS[i] = currentDNS2[i];
	}

	// save to qspi
	Write_settings(); 

	SCB->AIRCR  = (uint32_t)((0x5FAUL << SCB_AIRCR_VECTKEY_Pos) |
		(SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) | SCB_AIRCR_SYSRESETREQ_Msk);

	return 0;
}

extern modbus_t mb;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "PanelNetConfigsWindow", ID_WINDOW_0, 0, 0, 480, 272, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Header", ID_TEXT_HEADER, 90, 5, 300, 30, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "ButtonSave", ID_BUTTON_0, 165, 235, 100, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonExit", ID_BUTTON_1, 270, 235, 100, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonReset", ID_BUTTON_2, 375, 235, 100, 30, 0, 0x0, 0 },
	
	{ TEXT_CreateIndirect, "TextTIP", 		ID_TEXT_IP, 10, 60, 40, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditIP", 			ID_EDIT_IP, 80, 60, 150, 30, 0, 0x10, 0 },

  { TEXT_CreateIndirect, "TextTMask", ID_TEXT_MASK, 10, 110, 60, 30, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "EditMask", 	ID_EDIT_MASK, 80, 110, 150, 30, 0, 0x10, 0 },
  
  { TEXT_CreateIndirect, "TextTGW", ID_TEXT_GW, 		10, 160, 60, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditGW", 	ID_EDIT_GW, 		80, 160, 150, 30, 0, 0x10, 0 },

	{ TEXT_CreateIndirect, "TextTDNS1", ID_TEXT_DNS1, 245, 60, 65, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditDNS1", 	ID_EDIT_DNS1, 320, 60, 150, 30, 0, 0x10, 0 },
  
	{ TEXT_CreateIndirect, "TextTDNS2", ID_TEXT_DNS2, 245, 110, 65, 30, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "EditDNS2", 	ID_EDIT_DNS2, 320, 110, 150, 30, 0, 0x10, 0 },
	
	{ TEXT_CreateIndirect, "TextPortModbus", ID_TEXT_MODBUS, 10, 200, 240, 30, 0, 0x64, 0 },
	
	{ TEXT_CreateIndirect, "TextErr", ID_TEXT_ERR, 245, 160, 220, 60, 0, 0x64, 0 },
};

static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'PanelNetConfigsWindow'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x00FF2100));
    //
    // Initialization of 'Header'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_HEADER);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Настроить панель (сеть)");
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
    // Initialization of 'TextTDNS2'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_MODBUS);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
		char qweqwe[40];
		sprintf(qweqwe, "Порт Modbus: %u", mb.port);
    TEXT_SetText(hItem, qweqwe);
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'TextErr'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "");
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));

		SetPanelNetEdits();
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
					int res = SetPanelNetConfigs();
					switch(res)
					{
						case 0:
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_ERR), "Настройки применены.");
							keyBoard.isRefreshableFields = true;
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
        keyBoard.isRefreshableFields = true;
				TimerStop();
				WindowChange(MenuWindow);
			  break;
      }
      break;
		case ID_BUTTON_2: // Notifications sent by 'ButtonReset'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
				keyBoard.isRefreshableFields = true;
        TimerStop();
				SetPanelNetEdits();
				break;
      }
      break;
    case ID_EDIT_IP: // Notifications sent by 'EditIP'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_IP), "Изменение IP-адреса панели");
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
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_MASK), "Изменение маски панели");
				break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_EDIT_GW: // Notifications sent by 'Edit'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_GW), "Изменение шлюза панели");
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
        ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_DNS1), "Изменение DNS1 панели");
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
    		ShowKeyBoard(WM_GetDialogItem(pMsg->hWin, ID_EDIT_DNS2), "Изменение DNS2 панели");
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

WM_HWIN CreatePanelNetConfigsWindow(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}
