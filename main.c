#include "stm32f7xx_hal.h"
#include "stm32746g_discovery_sdram.h"
#include "stm32746g_discovery_qspi.h"
#include "RTE_Components.h"
#include "GUI.h"
#include "dialog.h"
#include "qspi.h"
#include "rl_net.h"
#include "cmsis_os.h"
#include "rl_net_lib.h"
#include "global_var.h"
#include "settings.h"
#include "GUILogic.h"
#include "serverPart.h"
#include "Net_User.h"
#include "modbus.h"

static void SystemClock_Config (void);
static void MPU_Config (void);
static void CPU_CACHE_Enable (void);

modbus_t mb;

extern mb_in_packet incomingPack;
extern mb_out_packet outcomingPack;
extern bool isSend;
extern uint8_t outBuffer[];

#ifdef RTE_CMSIS_RTOS_RTX
extern uint32_t os_time;

uint32_t HAL_GetTick(void) { 
  return os_time; 
}
#endif

extern  LOCALM localm[];
#define LocM   localm[NETIF_ETH]

unsigned char flag = 0, read = 0;
unsigned char Offset = 0;
//unsigned char s = 0, wr=0;
unsigned int q, w, t = 0;

int main (void) 
{
	Flags.answer_work = 0;
	
  MPU_Config();                             /* Configure the MPU              */
  CPU_CACHE_Enable();                       /* Enable the CPU Cache           */
  HAL_Init();                               /* Initialize the HAL Library     */
  BSP_SDRAM_Init();                         /* Initialize BSP SDRAM           */
  SystemClock_Config();                     /* Configure the System Clock     */
	
	#if GUI_WINSUPPORT
    WM_SetCreateFlags(WM_CF_MEMDEV);
  #endif 
	GUI_Init();   
	#if GUI_WINSUPPORT
    WM_MULTIBUF_Enable(1);
  #endif

	GUI_SetBkColor(0xFF2100);
  
	StartLogic();
	
  // QSPI device configuration 	
	status = BSP_QSPI_Init();
	QSPI_check ();
	
	NET_init ();
	Read_settings ();

	mb.port = 5000;
	initModbus(&mb);

	server_init();

  LOOPSTART:
	{

		GUI_Exec();                   /* Execute all GUI jobs ... Return 0 if nothing was done. */

		net_st = net_main();
		osThreadYield ();
		tcp_st_TECH  = tcp_get_state (tcp_soc_TECH);
		tcp_st_PLC   = tcp_get_state (tcp_soc_PLC); 
		tcp_st_WORK   = tcp_get_state (tcp_soc_WORK);
		
		if (tcp_st_WORK != tcpStateCLOSED) 
		{ }
		else
			soc_state = 0;

		send_data ();	

    if (isSend)
		{
			modbus_send(outBuffer, &mb);
			isSend = false;
		}

		server_working();

		if (Flags.incoming_work == 1) 
		{ 
			Parsing_package_WORK (); 
			Flags.incoming_work = 0;	
		} //если пришло от датчика
		
		if (Flags.incoming_server == 1) 
		{ 
			Parsing_package_SERVER (); 
			Flags.incoming_server = 0;	
      Flags.answer_work = 1;
		} //если пришло от ј—”“ѕ

		++q;
		
		if (q >= 3000)
		{
			RefreshWindow();			
			q = 0;
		}
		goto LOOPSTART;
	}
}

void HardFault_Handler(void)
{
	tcp_close(tcp_soc_WORK);
	tcp_close(tcp_soc_SERVER);
	tcp_close(tcp_soc_PLC);
	tcp_close(tcp_soc_TECH);
	SCB->AIRCR  = (uint32_t)((0x5FAUL << SCB_AIRCR_VECTKEY_Pos)    |
                           (SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) |
                            SCB_AIRCR_SYSRESETREQ_Msk);
}

/**
  * System Clock Configuration
  *   System Clock source            = PLL (HSE)
  *   SYSCLK(Hz)                     = 200000000
  *   HCLK(Hz)                       = 200000000
  *   AHB Prescaler                  = 1
  *   APB1 Prescaler                 = 4
  *   APB2 Prescaler                 = 2
  *   HSE Frequency(Hz)              = 25000000
  *   PLL_M                          = 25
  *   PLL_N                          = 400
  *   PLL_P                          = 2
  *   PLL_Q                          = 8
  *   VDD(V)                         = 3.3
  *   Main regulator output voltage  = Scale1 mode
  *   Flash Latency(WS)              = 6
  */
static void SystemClock_Config (void) {
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;  
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  /* Activate the OverDrive to reach the 200 MHz Frequency */
  HAL_PWREx_EnableOverDrive();
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
}

// Configure the MPU attributes
static void MPU_Config (void) {
  MPU_Region_InitTypeDef MPU_InitStruct;
  /* Disable the MPU */
  HAL_MPU_Disable();
  /* Configure the MPU attributes for SDRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0xC0200000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_2MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

// CPU L1-Cache enable
static void CPU_CACHE_Enable (void) {
  /* Enable I-Cache */
  SCB_EnableICache();
  /* Enable D-Cache */
  SCB_EnableDCache();
}
