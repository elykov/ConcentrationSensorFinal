#include "KeyBoard.h"
#include "KeyBoardWindowDLG.h"
#include <string.h>
#include <stdbool.h>

extern struct KeyBoard keyBoard;

void ShowKeyBoard(WM_HWIN edit)
{
	if (keyBoard._keyboard > 0)
	{
		if (edit != keyBoard.currentEdit)
		{
			keyBoard.currentEdit = edit;
		}                             
		return;
	}
	
	keyBoard._keyboard = CreateKeyBoardWindow();
	keyBoard.currentEdit = edit;
	WM_ShowWindow(keyBoard._keyboard);
}

void HideKeyBoard(void)
{
	WM_HideWindow(keyBoard._keyboard);
	keyBoard._keyboard = 0;
}

void Button_HandlePush(int id)
{
	char symb;
	
	if (id == ID_BUTTON_12)
	{
		HideKeyBoard();
		return;
	}
	
	char txt[20];
	EDIT_GetText(keyBoard.currentEdit, txt, 19);
	size_t txtsize = strlen(txt);

	switch(id)
	{
		case ID_BUTTON_0:
		case ID_BUTTON_1:
		case ID_BUTTON_2:
		case ID_BUTTON_3:
		case ID_BUTTON_4:
		case ID_BUTTON_5:
		case ID_BUTTON_6:
		case ID_BUTTON_7:
		case ID_BUTTON_8:
		case ID_BUTTON_9:
		{
			symb = (char)(47 + (id - GUI_ID_USER));
			break;
		}
		case ID_BUTTON_10: // Notifications sent by 'ButtonDot'
			symb = '.';
			break;
		case ID_BUTTON_13:
		{
			symb = '-';
			break;
		}
		case ID_BUTTON_11: // Notifications sent by 'ButtonBackS'
		{
			if (txtsize > 0)
			{
				txt[txtsize - 1] = '\0';
				EDIT_SetText(keyBoard.currentEdit, txt);
			}
			return;
		}
	}
		
	txt[txtsize] = symb;
	txt[txtsize + 1] = '\0';
	EDIT_SetText(keyBoard.currentEdit, txt);
}

