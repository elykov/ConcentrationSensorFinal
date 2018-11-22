#ifndef __TIMER6__
	#define __TIMER6__
	#include "stdbool.h"
	
	extern bool isTextErrChangable;
	extern void InitTimer6	(void);
	extern void TimerStart(void);
	extern void TimerStop(void);

	extern void TimerStartForButton(void);
	extern void TimerStopForButton(void);

#endif
