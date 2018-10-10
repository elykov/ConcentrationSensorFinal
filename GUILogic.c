#include "GuiLogic.h"
#include "qspi.h"
#include "global_var.h"

extern WM_HWIN CreateStartWindow(void);
extern WM_HWIN CreateMenuWindow(void);
extern WM_HWIN CreateSensorNetConfigsWindow(void);
extern WM_HWIN CreatePanelNetConfigsWindow(void);
extern WM_HWIN CreatePIDConfigsWindow(void);
extern WM_HWIN CreateParamsConfigsWindow(void);

// =======================================
// RefreshWindow
extern void RefreshStartWindow(void);
extern void RefreshPIDWindow(void);

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
		default:
			break;
	}   
}

void StartLogic(void)
{
	fWindowCreators[ParamsConfigsWindow] = CreateParamsConfigsWindow;
  fWindowCreators[PIDConfigsWindow] = CreatePIDConfigsWindow;
	fWindowCreators[PanelNetConfigsWindow] = CreatePanelNetConfigsWindow;
	fWindowCreators[SensorNetConfigsWindow] = CreateSensorNetConfigsWindow;
	fWindowCreators[MenuWindow] = CreateMenuWindow;
	fWindowCreators[StartWindow] = CreateStartWindow;
	WindowChange(StartWindow);
	//InitData();
}

void WindowChange(int windowNum)
{
	WM_HideWindow(logic.window);
	GUI_Clear();
	WM_DeleteWindow(logic.window);
	logic.currentWindow = windowNum;
	logic.window = fWindowCreators[logic.currentWindow]();
	WM_ShowWindow(logic.window);
}

int get_IP(const char* ip_str, uint8_t* resIP)
{
	uint32_t inputIP[4];
	memset(inputIP, 0, sizeof(uint32_t) * 4); // ��������� �������
	
	sscanf(ip_str, "%d.%d.%d.%d", &inputIP[0], &inputIP[1], &inputIP[2], &inputIP[3]);
	
	for	(int i = 0; i < 4; i++) // ���� �����-�� ������� ������ 255 - �������
		if (inputIP[i] > 255)
			return 1;
	
	for	(int i = 0; i < 4; i++)
		resIP[i] = (uint8_t)inputIP[i];

	return 0;
}
