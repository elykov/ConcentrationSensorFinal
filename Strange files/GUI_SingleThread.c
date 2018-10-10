
#include "cmsis_os.h"                   // CMSIS RTOS header file
#include "GUI.h"
#include "dialog.h"
#include "GUILogic.h"

#ifdef _RTE_
#include "RTE_Components.h"             // Component selection
#endif

/*----------------------------------------------------------------------------
 *      GUIThread: GUI Thread for Single-Task Execution Model
 *---------------------------------------------------------------------------*/
 
void GUIThread (void const *argument);              // thread function
osThreadId tid_GUIThread;                           // thread id
osThreadDef (GUIThread, osPriorityIdle, 1, 2048);   // thread object

int Init_GUIThread (void) {

  tid_GUIThread = osThreadCreate (osThread(GUIThread), NULL);
  if (!tid_GUIThread) return(-1);
  
  return(0);
}

void GUIThread (void const *argument) {

	#if GUI_WINSUPPORT
    WM_SetCreateFlags(WM_CF_MEMDEV);
  #endif
   
	GUI_Init();
   
	#if GUI_WINSUPPORT
    WM_MULTIBUF_Enable(1);
  #endif

	//GUI_Clear();
	GUI_SetBkColor(0xB70000);
  
	StartLogic();
	
	//GUI_Init();                     /* Initialize the Graphics Component */

  /* Add GUI setup code here */

  while (1) {
    
    /* All GUI related activities might only be called from here */

    GUI_Exec();                   /* Execute all GUI jobs ... Return 0 if nothing was done. */
    GUI_X_ExecIdle();             /* Nothing left to do for the moment ... Idle processing */
  }
}

