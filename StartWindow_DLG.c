/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.36                          *
*        Compiled Aug 31 2016, 10:53:09                              *
*        (c) 2016 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
#include <stdio.h>
#include "GUILogic.h"
// USER END

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0   (GUI_ID_USER + 0x02)
#define ID_BUTTON_0   (GUI_ID_USER + 0x03)
#define ID_TEXT_0   (GUI_ID_USER + 0x04)
#define ID_TEXT_1   (GUI_ID_USER + 0x05)
#define ID_TEXT_2   (GUI_ID_USER + 0x07)
#define ID_TEXT_3   (GUI_ID_USER + 0x08)
#define ID_TEXT_4   (GUI_ID_USER + 0x09)


// USER START (Optionally insert additional defines)
extern GUI_CONST_STORAGE GUI_FONT GUI_FontVerdana200asc;
//char menu[4] = {140,165,173,238};

// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
static WM_HWIN window;
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "StartWindow_", ID_WINDOW_0, 0, 0, 480, 272, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button_menu", ID_BUTTON_0, 0, 242, 70, 30, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 0, 0, 480, 200, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_1, 390, 234, 80, 31, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "��������:", ID_TEXT_2, 300, 241, 90, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_3, 90, 241, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_4, 180, 234, 90, 31, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  // USER END

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
    BUTTON_SetText(hItem, "����");
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetFont(hItem, &GUI_FontVerdana200asc);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //TEXT_SetText(hItem, "0.00");
		TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    //TEXT_SetText(hItem, "100%");
		TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, GUI_FONT_32_1);
		//
    // Initialization of '��������:'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
		TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "��������:");
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
		TEXT_SetFont(hItem, &GUI_FontVerdana20);
    TEXT_SetText(hItem, "�������:");
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
    //TEXT_SetText(hItem, "9,99%");
		TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, GUI_FONT_32_1);
    // USER START (Optionally insert additional code for further widget initialization)
		RefreshWindow();
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'Button_menu'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				// USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        WindowChange(MenuWindow);
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateStartWindow
*/
WM_HWIN CreateStartWindow(void);
WM_HWIN CreateStartWindow(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
void RefreshStartWindow(void)
{
	char tempStr[10];
	{ // set big concentracion
		if (Cb <= 0)
			sprintf(tempStr, "---");
		else
			sprintf(tempStr, "%1.2f", Cb);
		TEXT_SetText(WM_GetDialogItem(window, ID_TEXT_0), tempStr);	
	}
	
	{ // �������
		sprintf(tempStr, "%1.2f %%", referens);
		WM_HWIN hItem = WM_GetDialogItem(window, ID_TEXT_4);
		TEXT_SetText(hItem, tempStr);	
	}
		
	{ //��������
		float aaa = Output_I;
		float b = (aaa - 4.f) / ((20.f - 4.f) / 100.f);
		
		if (b > 100) 
			b = 100;
		else if (b < 0) 
			b = 0;
		
		sprintf(tempStr, "%3.0f %%", b);
		WM_HWIN hItem = WM_GetDialogItem(window, ID_TEXT_1);
		TEXT_SetText(hItem, tempStr);	
	}
}
// USER END

/*************************** End of file ****************************/
