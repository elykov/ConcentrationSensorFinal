  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __QSPI_H
#define __QSPI_H

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "stm32746g_discovery_qspi.h"

#define BUFFER_SIZE         ((unsigned int)512)
#define WRITE_READ_ADDR     ((unsigned int)0x0000)

extern unsigned char qspi_aTxBuffer[BUFFER_SIZE];
extern unsigned char qspi_aRxBuffer[BUFFER_SIZE];
extern unsigned char status;
extern unsigned char nor_status;
extern void QSPI_read_qspi_aRxBuffer (void);
extern void QSPI_write_qspi_aTxBuffer (void);
extern void QSPI_check (void);


#endif /* __QSPI_H */


