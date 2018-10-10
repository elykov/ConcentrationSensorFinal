#ifndef __KEYBOARD__
	#define __KEYBOARD__
	#include "dialog.h"
	
	struct KeyBoard
	{
		WM_HWIN currentEdit; // Edit current. which we change by keyboard
		WM_HWIN _keyboard;
	};
	
	extern void HideKeyBoard(void);
	extern void Button_HandlePush(int id);
	extern void ShowKeyBoard(WM_HWIN edit);
	extern WM_HWIN CreateKeyBoardWindow(void);
	
#endif
