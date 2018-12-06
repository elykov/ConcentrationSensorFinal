#ifndef __KEYBOARD__
	#define __KEYBOARD__
	#include "dialog.h"
	#include "stdbool.h"
	
	struct KeyBoard
	{
		WM_HWIN currentEdit; // Edit current. which we change by keyboard
		WM_HWIN _keyboard;
		WM_HWIN _this_edit;
		WM_HWIN _title_text;
		bool isRefreshableFields: true;
	};
	
	extern void HideKeyBoard(void);
	extern void Button_HandlePush(int id);
	extern void ShowKeyBoard(WM_HWIN edit, char* title_text);
	extern WM_HWIN CreateKeyBoardWindow(void);
	
#endif
