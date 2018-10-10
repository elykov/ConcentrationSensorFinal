/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2016  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.36 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional. 
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information

Licensor:                 SEGGER Software GmbH
Licensed to:              ARM Ltd, 110 Fulbourn Road, CB1 9NJ Cambridge, UK
Licensed SEGGER software: emWin
License number:           GUI-00181
License model:            LES-SLA-20007, Agreement, effective since October 1st 2011 
Licensed product:         MDK-ARM Professional
Licensed platform:        ARM7/9, Cortex-M/R4
Licensed number of seats: -
----------------------------------------------------------------------
File        : main.c
Purpose     : Main program
---------------------------END-OF-HEADER------------------------------
*/


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
#include "Net_User.h"
#include "global_var.h"
#include "settings.h"
#include "GUILogic.h"

static void SystemClock_Config (void);
static void MPU_Config (void);
static void CPU_CACHE_Enable (void);

#ifdef RTE_CMSIS_RTOS_RTX
extern uint32_t os_time;

uint32_t HAL_GetTick(void) { 
  return os_time; 
}
#endif


unsigned char flag = 0, read = 0;
unsigned char Offset = 0;

extern  LOCALM localm[];
#define LocM   localm[NETIF_ETH]
//unsigned int eth_tickstart;
//unsigned char ip1, ip2, ip3, ip4;
//extern void timer_tick (void);
//extern _Bool tick;

unsigned char s = 0, wr=0;
unsigned int q, w, t = 0;

/*********************************************************************
*
*       Main
*/
int main (void) 
{
	Flags.answer_work = 0;
	
  MPU_Config();                             /* Configure the MPU              */
  CPU_CACHE_Enable();                       /* Enable the CPU Cache           */
  HAL_Init();                               /* Initialize the HAL Library     */
  BSP_SDRAM_Init();                         /* Initialize BSP SDRAM           */
  SystemClock_Config();                     /* Configure the System Clock     */

	{  // old GUI init (doesn't work)
		//GUI_Init();
		//GUI_Clear();
		//WM_MULTIBUF_Enable(1);
		//Init_GUIThread();

		//myWin = CreateStartWindow();
		//WM_ShowWindow(myWin);
	}
	
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

/*	
	ip1 = LocM.IpAddr[0];
	ip2 = LocM.IpAddr[1];
	ip3 = LocM.IpAddr[2];
	ip4 = LocM.IpAddr[3];		
*/
		
  for (;;)
	{

		GUI_Exec();                   /* Execute all GUI jobs ... Return 0 if nothing was done. */

		net_st = net_main();
		osThreadYield ();
		tcp_st_TECH  = tcp_get_state (tcp_soc_TECH);
		tcp_st_PLC   = tcp_get_state (tcp_soc_PLC); 
		tcp_st_WORK   = tcp_get_state (tcp_soc_WORK);
		
		switch (tcp_st_WORK) 
		{
			case tcpStateUNUSED: //Socket is free and not allocated yet. The function cannot return this value.
				s = 1;
				break;
			case tcpStateCLOSED: //Socket is allocated to an application but the connection is closed.
				soc_state = 0;
				s = 2;
				break;
			case tcpStateLISTEN: //Socket is listening for incoming connections.
				s = 3;
				break;
			case tcpStateSYN_RECEIVED: //Socket has received a TCP packet with the flag SYN set.
				s = 4;
				break;
			case tcpStateSYN_SENT: //Socket has received a TCP packet with the flag SYN set.
				s = 5;
				break;
			case tcpStateFIN_WAIT_1: //Socket has sent a FIN packet, to start the closing of the connection.
				s = 6;
				break;
			case tcpStateFIN_WAIT_2: //Socket has received acknowledgement from the remote machine for the FIN packet it sent out from the local machine. Socket is now waiting for a FIN packet from the remote machine.
				s = 7;
				break;
			case tcpStateCLOSING: //Socket has received a FIN packet from the remote machine independently
				s = 8;
				break;
			case tcpStateLAST_ACK: //Socket is waiting for the last ACK packet to the FIN packet it sent out.
				s = 9;
				break;
			case tcpStateTIME_WAIT: //Socket is waiting on a 2 ms timeout before closing the connection.
				s = 10;
				break;
			case tcpStateESTABLISHED: //Socket has established a TCP connection. You can transfer data only in this state. 
				s = 11;
				break;
			default:
				break;
		}

		send_data ();	

		if (Flags.incoming_work == 1) 
		{ 
			Parsing_package_WORK (); 
			Flags.incoming_work = 0;	
		} //���� ������ �� �������
		
		q++;
		
		if (q >= 10000) // ???????????????????????????????????????
		{
			RefreshWindow();			
			q = 0;
		}
		
		//******************************** ��� ������ ���� ��������. ��� ����� ������
		/*
		if(s == 11)
			w++;
		
		if (w >= 500000)
		{
			if(wr != 0)
				Flags.answer_work = 1;
			t++;	
			w = 0;
		}
		*/
		//*********************************
		
		if (flag) 
		{
			Write_settings(); 
			flag = 0;
		} // �������������� ������ � NOR
	}
}

/*************************** End of file ****************************/


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