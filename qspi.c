/*------------------------------------------------------------------*/
#include "qspi.h"
#include "global_var.h"
#include "string.h"

static QSPI_Info pQSPI_Info;
unsigned char status;
unsigned char nor_status = 0;

extern unsigned char Offset;

unsigned char qspi_aTxBuffer[BUFFER_SIZE];
unsigned char qspi_aRxBuffer[BUFFER_SIZE];

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void QSPI_read_qspi_aRxBuffer (void)/* read buffer*/
{
	if(BSP_QSPI_Read(qspi_aRxBuffer, WRITE_READ_ADDR, BUFFER_SIZE) != QSPI_OK)
	{
		nor_status = 6;
	}
}

void QSPI_write_qspi_aTxBuffer (void)/* write buffer*/
{
  if(BSP_QSPI_Erase_Block(WRITE_READ_ADDR) != QSPI_OK)      /* Erase QSPI memory */ 
	{
		nor_status = 4;
	}
	else
	{
		// QSPI memory read/write access  #################################
		//заполнить буфер перед записью 
		if(BSP_QSPI_Write(qspi_aTxBuffer, WRITE_READ_ADDR, BUFFER_SIZE) != QSPI_OK)/* Write data to the QSPI memory */
		{
			nor_status = 5;
		}
	}
}

void QSPI_check (void) // Инициализация QSPI
{
  if (status == QSPI_NOT_SUPPORTED)
	{
		nor_status = 1;
	}
  else if (status == QSPI_ERROR)
	{
		nor_status = 2;
	}
	else
	{
		/* Check the QSPI info */
		/* Initialize the structure */
		pQSPI_Info.FlashSize          = (uint32_t)0x00;
		pQSPI_Info.EraseSectorSize    = (uint32_t)0x00;
		pQSPI_Info.EraseSectorsNumber = (uint32_t)0x00;
		pQSPI_Info.ProgPageSize       = (uint32_t)0x00;
		pQSPI_Info.ProgPagesNumber    = (uint32_t)0x00;
		BSP_QSPI_GetInfo(&pQSPI_Info);/* Read the QSPI memory info */
		/* Test the correctness */
		if((pQSPI_Info.FlashSize != 0x1000000) || (pQSPI_Info.EraseSectorSize != 0x1000)  ||
			 (pQSPI_Info.ProgPageSize != 0x100)  || (pQSPI_Info.EraseSectorsNumber != 4096) ||
			 (pQSPI_Info.ProgPagesNumber != 0x10000))
		{
			nor_status = 3;
		}	
	}
}


//static void Fill_Buffer(uint8_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
//{
//  uint32_t tmpIndex = 0;
//  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ )/* Put in global buffer different values */
//  {
//    pBuffer[tmpIndex] = tmpIndex + uwOffset;
//  }
//}

/*****************************END OF FILE****/
