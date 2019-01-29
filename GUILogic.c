#include "GuiLogic.h"
#include "qspi.h"
#include "global_var.h"
#include "timer6.h"

// Функции создания окон
extern WM_HWIN CreateStartWindow(void);
extern WM_HWIN CreatePasswordWindow(void);
extern WM_HWIN CreateMenuWindow(void);
extern WM_HWIN CreateSensorNetConfigsWindow(void);
extern WM_HWIN CreatePanelNetConfigsWindow(void);
extern WM_HWIN CreatePIDConfigsWindow(void);
extern WM_HWIN CreateParamsConfigsWindow(void);
extern WM_HWIN CreateModBusWindow(void);

// =======================================
// Функции обновления информации на окнах
extern void RefreshStartWindow(void);
extern void RefreshPIDWindow(void);
extern void RefreshParamsWindow(void);
extern void RefreshSensorNetConfigsWindow(void);
extern void RefreshPanelNetConfigsWindow(void);

extern struct GUILogic logic;
WM_HWIN (*fWindowCreators[WindowsCount])(void);

void RefreshWindow(void)
{
	switch(logic.currentWindow)
	{
		case StartWindow:
			RefreshStartWindow();
			break;
		case PIDConfigsWindow:
			RefreshPIDWindow();
			break;
		case ParamsConfigsWindow:
			RefreshParamsWindow();
      break;
		case SensorNetConfigsWindow:
			RefreshSensorNetConfigsWindow();
			break;
    case PanelNetConfigsWindow:
		  RefreshPanelNetConfigsWindow();
			break;
		default:
			break;
	}   
}

// Начало работы с окнами
void StartLogic(void)
{
	fWindowCreators[ModBusWindow] = CreateModBusWindow;
	fWindowCreators[ParamsConfigsWindow] = CreateParamsConfigsWindow;
  fWindowCreators[PIDConfigsWindow] = CreatePIDConfigsWindow;
	fWindowCreators[PanelNetConfigsWindow] = CreatePanelNetConfigsWindow;
	fWindowCreators[SensorNetConfigsWindow] = CreateSensorNetConfigsWindow;
	fWindowCreators[MenuWindow] = CreateMenuWindow;
	fWindowCreators[PasswordWindow] = CreatePasswordWindow;
	fWindowCreators[StartWindow] = CreateStartWindow;
	WindowChange(StartWindow);

	InitTimer6();
}

void WindowChange(int windowNum) // Функция для смены окна
{
	WM_HideWindow(logic.window);
	WM_DeleteWindow(logic.window);
	logic.currentWindow = windowNum;
	logic.window = fWindowCreators[logic.currentWindow]();
	keyBoard.isRefreshableFields = true;
	WM_ShowWindow(logic.window);
}

int get_IP(const char* ip_str, uint8_t* resIP) // Функция для расшифровки IP из текста в массив чисел
{
	uint32_t inputIP[4];
	memset(inputIP, 0, sizeof(uint32_t) * 4); // обнуление массива
	
	sscanf(ip_str, "%d.%d.%d.%d", &inputIP[0], &inputIP[1], &inputIP[2], &inputIP[3]);
	
	for	(int i = 0; i < 4; ++i) // если какой-то элемент больше 255 - неверно
		if (inputIP[i] > 255)
			return 1;
	                           
	for	(int i = 0; i < 4; ++i)
		resIP[i] = (uint8_t)inputIP[i];

	return 0;
}
