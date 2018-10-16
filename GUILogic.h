#ifndef __LOGIC__
	#define __LOGIC__
	#include "KeyBoard.h"
	#include "global_var.h"
	#include <string.h>
  #include "stdio.h"
	
	#define StartWindow		 							0
	#define MenuWindow									1
	#define SensorNetConfigsWindow			2
	#define PanelNetConfigsWindow				3
	#define PIDConfigsWindow						4
	#define ParamsConfigsWindow					5
	
	#define WindowsCount 6
	
	struct GUILogic
	{
		WM_HWIN window;
		int currentWindow;		
	};
	
	// ============= Variables ================	
	extern struct KeyBoard keyBoard;
	extern struct GUILogic logic;
	extern GUI_CONST_STORAGE GUI_FONT GUI_FontVerdana20;
	
	// ============= Functions ================
	extern void StartLogic(void); // инициализирует все графические обьекты и создает начальное окно
	
	extern void WindowChange(int windowNum); // меняет окна на указанные
	
	extern void RefreshWindow(void); // обновляет данные текущем окне
		
	// принимает строку с IP и записывает данные в resIP.
	// resIP должна быть проинициализированна.
	extern int get_IP(const char* ip_str, uint8_t* resIP);  

#endif

