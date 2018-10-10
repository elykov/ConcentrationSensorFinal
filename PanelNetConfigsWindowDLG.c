#include "GUILogic.h"
#include "stm32f7xx_hal.h"
#include "DIALOG.h"
#include "qspi.h"
#include "settings.h"

#define ID_WINDOW_0         (GUI_ID_USER + 0x06)
#define ID_TEXT_0         (GUI_ID_USER + 0x07)
#define ID_TEXT_1         (GUI_ID_USER + 0x08)
#define ID_EDIT_IP         (GUI_ID_USER + 0x09)
#define ID_EDIT_MASK         (GUI_ID_USER + 0x0A)
#define ID_TEXT_2         (GUI_ID_USER + 0x0B)
#define ID_TEXT_3         (GUI_ID_USER + 0x0C)
#define ID_EDIT_GW         (GUI_ID_USER + 0x0D)
#define ID_EDIT_DNS1         (GUI_ID_USER + 0x0E)
#define ID_TEXT_4         (GUI_ID_USER + 0x0F)
#define ID_BUTTON_0         (GUI_ID_USER + 0x10)
#define ID_BUTTON_1         (GUI_ID_USER + 0x11)
#define ID_TEXT_5         (GUI_ID_USER + 0x13)
#define ID_EDIT_DNS2         (GUI_ID_USER + 0x15)

void RefreshPanelNetConfigsWindow(void)
{
	WM_HWIN window = logic.window;
	char tempStr[25];
	
	// ip
	{
		sprintf(tempStr, "%d.%d.%d.%d", 
			LocM.IpAddr[0], LocM.IpAddr[1],
			LocM.IpAddr[2], LocM.IpAddr[3]);
		
		EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_IP), tempStr);
	}
	// mask                                        
	{
		sprintf(tempStr, "%d.%d.%d.%d", 
			LocM.NetMask[0], LocM.NetMask[1],
			LocM.NetMask[2], LocM.NetMask[3]);
	
		EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_MASK), tempStr);
	}
	// gateway
  {
	  sprintf(tempStr, "%d.%d.%d.%d", 
			LocM.DefGW[0], LocM.DefGW[1],
			LocM.DefGW[2], LocM.DefGW[3]);
	
		EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_GW), tempStr);
	}
	// dns1
  {
		sprintf(tempStr, "%d.%d.%d.%d", 
			LocM.PriDNS[0], LocM.PriDNS[1],
			LocM.PriDNS[2], LocM.PriDNS[3]);
	
		EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_DNS1), tempStr);
	}
	// dns2
  {
		sprintf(tempStr, "%d.%d.%d.%d", 
			LocM.SecDNS[0], LocM.SecDNS[1],
			LocM.SecDNS[2], LocM.SecDNS[3]);
	
		EDIT_SetText(WM_GetDialogItem(window, ID_EDIT_DNS2), tempStr);
	}
}

int SetNetConfigs()
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

	for(int i = 0; i < 4; i++)
	{
		LocM.IpAddr[i] = currentIP[i];
		LocM.NetMask[i] = currentMask[i];
		LocM.DefGW[i] = currentGW[i];
		LocM.PriDNS[i] = currentDNS1[i];
		LocM.SecDNS[i] = currentDNS2[i];
	}

	// save to qspi
	Write_settings();
	return 0;
}

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "PanelNetConfigsWindow", ID_WINDOW_0, 0, 0, 480, 272, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Header", ID_TEXT_0, 90, 5, 300, 30, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "TextTIP", ID_TEXT_1, 35, 60, 40, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EditIP", ID_EDIT_IP, 80, 60, 150, 30, 0, 0x14, 0 },
  { EDIT_CreateIndirect, "EditMask", ID_EDIT_MASK, 320, 60, 150, 30, 0, 0x14, 0 },
  { TEXT_CreateIndirect, "TextTMask", ID_TEXT_2, 235, 60, 80, 30, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "TextTDNS1", ID_TEXT_3, 10, 170, 65, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_DNS1, 80, 170, 150, 30, 0, 0x14, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_DNS2, 320, 170, 150, 30, 0, 0x14, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_4, 245, 170, 70, 30, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "ButtonOK", ID_BUTTON_0, 310, 235, 80, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ButtonCancel", ID_BUTTON_1, 395, 235, 80, 30, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_5, 130, 115, 100, 30, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_GW, 250, 115, 150, 30, 0, 0x14, 0 },
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
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Настроить панель (сеть)");
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'TextTIP'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "IP: ");
    TEXT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'EditIP'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_IP);
    EDIT_SetFont(hItem, GUI_FONT_20_1);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    //
    // Initialization of 'EditMask'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_MASK);
    EDIT_SetFont(hItem, GUI_FONT_20_1);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    //
    // Initialization of 'TextTMask'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Маска: ");
    TEXT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'TextTDNS1'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "DNS1: ");
    TEXT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'Edit'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_GW);
    EDIT_SetFont(hItem, GUI_FONT_20_1);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    //
    // Initialization of 'Edit'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_DNS1);
    EDIT_SetFont(hItem, GUI_FONT_20_1);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "DNS2: ");
    TEXT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'ButtonOK'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetFont(hItem, &GUI_FontVerdana20);
    BUTTON_SetText(hItem, "ОК");
    //
    // Initialization of 'ButtonCancel'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetFont(hItem, &GUI_FontVerdana20);
    BUTTON_SetText(hItem, "Отмена");
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_5);
    TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "Шлюз: ");
    TEXT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FFFF));
    //
    // Initialization of 'Edit'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_DNS2);
    EDIT_SetFont(hItem, GUI_FONT_20_1);
    EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    
		RefreshPanelNetConfigsWindow();
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'ButtonOK'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
      	if (SetNetConfigs() == 0)
				{
					SCB->AIRCR  = (uint32_t)((0x5FAUL << SCB_AIRCR_VECTKEY_Pos) |
						(SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) | SCB_AIRCR_SYSRESETREQ_Msk);
				}
        break;
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'ButtonCancel'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        HideKeyBoard();
				WindowChange(MenuWindow);
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
    case ID_EDIT_GW: // Notifications sent by 'Edit'
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
    case ID_EDIT_DNS1: // Notifications sent by 'Edit'
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
		case ID_EDIT_DNS2: // Notifications sent by 'Edit'
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

WM_HWIN CreatePanelNetConfigsWindow(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}
