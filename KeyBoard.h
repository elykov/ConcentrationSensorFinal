#ifndef __KEYBOARD__
	#define __KEYBOARD__
	#include "dialog.h"
	#include "stdbool.h"
	
	struct KeyBoard
	{
		WM_HWIN currentEdit; // Ячейка, которая сейчас изменяется
		WM_HWIN _keyboard; // Окно клавиатуры
		WM_HWIN _this_edit; // Ячейка на клавиатуре
		WM_HWIN _title_text; // Заголовок на клавиатуре
		bool isRefreshableFields: true; 
	};
	
	extern void HideKeyBoard(void);
	extern void Button_HandlePush(int id);
	extern void ShowKeyBoard(WM_HWIN edit, char* title_text);
	extern WM_HWIN CreateKeyBoardWindow(void);
	
#endif
