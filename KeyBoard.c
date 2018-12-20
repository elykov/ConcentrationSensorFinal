#include "KeyBoard.h"
#include "KeyBoardWindowDLG.h"
#include <string.h>
#include <stdbool.h>

extern struct KeyBoard keyBoard;
char oldEditText[25];
char txt[25];

void ShowKeyBoard(WM_HWIN edit, char* title_text) // done
{
	if (keyBoard._keyboard == 0)
		keyBoard._keyboard = CreateKeyBoardWindow();
		
	keyBoard.currentEdit = edit;
	EDIT_GetText(keyBoard.currentEdit, oldEditText, 24);
	EDIT_SetText(keyBoard._this_edit, oldEditText);
	TEXT_SetText(keyBoard._title_text, title_text);
	WM_ShowWindow(keyBoard._keyboard);
}

void HideKeyBoard(void) // done
{
	WM_HideWindow(keyBoard._keyboard);
	WM_DeleteWindow(keyBoard._keyboard);
	keyBoard._keyboard = 0;
}

void Button_HandlePush(int id) // done
{
	char symb;
	
	if (id == ID_BUTTON_12) // OK
	{
		EDIT_GetText(keyBoard._this_edit, oldEditText, 24);
		EDIT_SetText(keyBoard.currentEdit, oldEditText);
		keyBoard.isRefreshableFields = false;
		HideKeyBoard();
		return;
	}
	else if (id == ID_BUTTON_14) // esc
	{
		EDIT_SetText(keyBoard.currentEdit, oldEditText);
		HideKeyBoard();
		return;
	}
	
	EDIT_GetText(keyBoard._this_edit, txt, 24);
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
				EDIT_SetText(keyBoard._this_edit, txt);
			}
			return;
		}
	}
		
	txt[txtsize] = symb;
	txt[txtsize + 1] = '\0';
	EDIT_SetText(keyBoard._this_edit, txt);
}
