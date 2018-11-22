#ifndef __LOGIC__
	#define __LOGIC__
	#include "KeyBoard.h"
	#include "global_var.h"
	#include <string.h>
  #include "stdio.h"
	#include "timer6.h"
	
	#define StartWindow		 							0
  #define PasswordWindow		 					1
	#define MenuWindow									2
	#define SensorNetConfigsWindow			3
	#define PanelNetConfigsWindow				4
	#define PIDConfigsWindow						5
	#define ParamsConfigsWindow					6
	#define ModBusWindow								7

	#define WindowsCount 8
	
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

