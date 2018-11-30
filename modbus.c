#include "modbus.h"
#include "rl_net.h"
#include "global_var.h"

union Converter converter;

mb_in_packet incomingPack;
mb_out_packet outcomingPack;
bool isSend;
uint8_t outBuffer[260];
bool isReversInput = false, isReversOutput = false;

unsigned int modbus_callback(int32_t soc, tcpEvent event, const uint8_t *buf, uint32_t len);

// Создает сервер modbus - создает сокет и включает сервер в прослушивание
int initModbus(modbus_t* mb)
{
	mb->sock = tcp_get_socket (TCP_TYPE_SERVER, 0, 10, modbus_callback);
	if (mb->sock < 0) 
		return -1;
	tcp_listen(mb->sock, mb->port);
	return 0;
}

// Записывает в структуру пакета приходящих данных mbp входящее сообщение message 
void mb_inc_packet_parse(const uint8_t* message, mb_in_packet* mbp)
{
	uint8_t junByte, sinByte;
	if (!isReversInput)
	{
		junByte = 1;
		sinByte = 0;
	}
	else
	{
		junByte = 0;
		sinByte = 1;
	}
	
	converter.cdata[junByte] = message[0];
	converter.cdata[sinByte] = message[1];
	mbp->header.transactionID = converter.sdata;

	converter.cdata[junByte] = message[2];
	converter.cdata[sinByte] = message[3]; 
	mbp->header.protocolID = converter.sdata; 

	converter.cdata[junByte] = message[4];
	converter.cdata[sinByte] = message[5];
	mbp->header.length = converter.sdata; 
	
	mbp->header.unitID = message[6];
  mbp->commandCode = message[7];
	
	converter.cdata[junByte] = message[8];
	converter.cdata[sinByte] = message[9]; 
	mbp->addressFirstRegister = converter.sdata;

	converter.cdata[junByte] = message[10];
  converter.cdata[sinByte] = message[11]; 
	mbp->registersCount = converter.sdata;
}

// Записывает в сообщение message данные из структуры пакета исходящих данных mbp
void mb_out_packet_form(uint8_t* data, mb_out_packet* mbp)
{
	uint8_t junByte, sinByte;
	if (!isReversOutput) // реверсивная ли передача данных назад
	{
		junByte = 1;
		sinByte = 0;
	}
	else
	{
		junByte = 0;
		sinByte = 1;
	}

	converter.sdata = mbp->header.transactionID;
	data[junByte] = converter.cdata[0];
	data[sinByte] = converter.cdata[1]; 

	converter.sdata = mbp->header.protocolID;
	data[junByte + 2] = converter.cdata[0];
	data[sinByte + 2] = converter.cdata[1]; 

	converter.sdata = mbp->header.length;
	data[junByte + 4] = converter.cdata[0];
	data[sinByte + 4] = converter.cdata[1]; 
	
	data[6] = mbp->header.unitID;
  data[7] = mbp->commandCode;
	
	data[8] = mbp->answerLength;
	
	if (isReversOutput)
	{
		for(int i = 0; i < mbp->answerLength; i++)	
			data[i + 9] = mbp->data[i];
	}
	else
	{
		for(int i = 0; i < mbp->answerLength; i++)	
			data[i + 9] = mbp->data[i];	
	}


	isSend = true;
}

// отправка данных по modbus
void modbus_send(const uint8_t* message, modbus_t* mb)
{
	uint8_t* sendbuf;
	int32_t len = 9 + message[8];
	if (tcp_check_send(mb->sock))
	{
		sendbuf = tcp_get_buf(len);
		memcpy(sendbuf, message, len);
		tcp_send(mb->sock, sendbuf, len);
	}
}

// Разбор и ответ пришедших данных
void mb_answer_handle(mb_in_packet* inPack, mb_out_packet* outPack)
{
	// Копирование данных для формирования исходящего пакета
	outPack->header.transactionID = inPack->header.transactionID; 
	outPack->header.protocolID = inPack->header.protocolID; 
	outPack->header.unitID = inPack->header.unitID;
	outPack->commandCode = inPack->commandCode;
	
	// Формирование массива данных в панели
	uint8_t arr[12];
	converter.fdata = Cb;
	for(int i = 0; i < 4; i++)
		arr[i] = converter.cdata[i];

	converter.fdata = referens;
	for(int i = 0; i < 4; i++)
		arr[i + 4] = converter.cdata[i];

	converter.fdata = damper;
	for(int i = 0; i < 4; i++)
		arr[i + 8] = converter.cdata[i];

	outPack->answerLength = inPack->registersCount * 2;
	outPack->header.length = outPack->answerLength + 3;

	// Если пришел запрос на считывание данных и регистры данных в указанных пределах 
	if (inPack->commandCode == 3 && 
			inPack->addressFirstRegister > 30000 && 
			inPack->addressFirstRegister < 39999)
	{
    int diff = inPack->addressFirstRegister + outPack->answerLength - 30013; // проверяем зашли ли за границу существующих данных
		
		int i = 0;
		int startIndex = inPack->addressFirstRegister - 30001,
				lastIndex = outPack->answerLength - diff;

		for(int j = startIndex; i < lastIndex; i++, j++)
			outPack->data[i] = arr[j];

		if (diff > 0) // если зашли за границу данных - заполнение данных нулями
		{
			for(diff += i; i < diff; i++)
				outPack->data[i] = 0;
		}
	}
	else
	{
		for(int i = 0; i < outPack->answerLength; i++)
			outPack->data[i] = 0;
	}
}

unsigned int modbus_callback (int32_t soc, tcpEvent event, const uint8_t *buf, uint32_t len) 
{
  // This function is called on TCP event
  switch (event) 
	{
    case tcpEventConnect:
		{
      // Remote host is trying to connect to our TCP socket. 
      // 'buf' points to Remote IP, 'len' holds the remote port.
			__nop();
      return 1; // Return 1 to accept connection, or 0 to reject connection 
		}
    case tcpEventAbort: // Connection was aborted 
    {  
			__nop();
      break;
		}
    case tcpEventEstablished: // Socket is connected to remote peer. 
		{
			__nop();
      break;
		}
    case tcpEventClosed: // Connection has been closed 
		{
			__nop();
      break;
		}
    case tcpEventACK: // Our sent data has been acknowledged by remote peer
    { 
			__nop();
      break;
		}
    case tcpEventData:
		{
      // TCP data frame has been received, 'buf' points to data 
      // Data length is 'len' bytes
			/*
			char txt[300];
			sprintf(txt, "TID: %d, PID: %d, Len: %d, UID: %d, Func: %d, First: %d, RegCnt: %d", 
				incomingPack.header.transactionID, 
				incomingPack.header.protocolID, 
				incomingPack.header.length,
				incomingPack.header.unitID,
				incomingPack.commandCode,
				incomingPack.addressFirstRegister,
				incomingPack.registersCount);
      */
			
			mb_inc_packet_parse(buf, &incomingPack);
			mb_answer_handle(&incomingPack, &outcomingPack);
			mb_out_packet_form(outBuffer, &outcomingPack);
			tcp_reset_window (soc);
			__nop();
      break;
		}
  }
  return 0;
}
