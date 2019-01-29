#include "modbus.h"
#include "rl_net.h"
#include "global_var.h"

union Converter converter;

mb_in_packet incomingPack; // 
mb_out_packet outcomingPack;
bool isSend;
uint8_t outBuffer[260];
bool isReversInput = false, isReversOutput = false;
uint8_t modBusBuffer[12];

unsigned int modbus_callback(int32_t soc, tcpEvent event, const uint8_t *buf, uint32_t len);
uint8_t junByte, sinByte;

// ������� ������ modbus - ������� ����� � �������� ������ � �������������
int initModbus(modbus_t* mb)
{
	mb->sock = tcp_get_socket (TCP_TYPE_SERVER, 0, 10, modbus_callback);
	if (mb->sock < 0) 
		return -1;
	tcp_listen(mb->sock, mb->port);
	return 0;
}

// ���������� � ��������� ������ ���������� ������ mbp �������� ��������� message 
void mb_inc_packet_parse(const uint8_t* message, mb_in_packet* mbp)
{
	//uint8_t junByte, sinByte;
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

// ���������� � ��������� message ������ �� ��������� ������ ��������� ������ mbp
void mb_out_packet_form(uint8_t* data, mb_out_packet* mbp)
{
	if (!isReversOutput) // ����������� �� �������� ������ �����
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

// �������� ������ �� modbus
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

// ������ � ����� ��������� ������
void mb_answer_handle(mb_in_packet* inPack, mb_out_packet* outPack)
{
	// ����������� ������ ��� ������������ ���������� ������
	outPack->header.transactionID = inPack->header.transactionID; 
	outPack->header.protocolID = inPack->header.protocolID; 
	outPack->header.unitID = inPack->header.unitID;
	outPack->commandCode = inPack->commandCode;
	
	// ������������ ������� ������ � ������
	uint8_t arr[6];
	uint16_t iCb = (uint16_t)(Cb * 1000.f);
  uint16_t iRef = (uint16_t)(referens * 1000.f);
	uint16_t iDamper = (uint16_t)(damper * 100.f);

	if (!isReversOutput) // ����������� �� �������� ������ �����
	{
		junByte = 1;
		sinByte = 0;
	}
	else
	{
		junByte = 0;
		sinByte = 1;
	}

	arr[junByte] = iCb;
	arr[sinByte] = iCb >> 8;

  arr[junByte + 2] = iRef;
	arr[sinByte + 2] = iRef >> 8;

	arr[junByte + 4] = iDamper;
	arr[sinByte + 4] = iDamper >> 8;

	outPack->answerLength = inPack->registersCount * 2;
	outPack->header.length = outPack->answerLength + 3;

	// ���� ������ ������ �� ���������� ������ � �������� ������ � ��������� �������� 
	if ((inPack->commandCode == 3 || inPack->commandCode == 4) && 
			inPack->addressFirstRegister > 30000 && 
			inPack->addressFirstRegister < 39999)
	{
    int diff = inPack->addressFirstRegister + outPack->answerLength - 30007; // ��������� ����� �� �� ������� ������������ ������
		
		int i = 0;
		int startIndex = inPack->addressFirstRegister - 30001,
				lastIndex = outPack->answerLength - diff;

		for(int j = startIndex; i < lastIndex; i++, j++)
			outPack->data[i] = arr[j];

		if (diff > 0) // ���� ����� �� ������� ������ - ���������� ������ ������
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
	const uint8_t* pbuf;

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
			//if (len != 12) { tcp_reset_window (soc); __nop(); break; }
			pbuf = buf;	
			for(int i = 0; i < 12; i++)
			{
				modBusBuffer[i] = *pbuf;
				++pbuf;
			}
			Flags.modbus = 1;
			tcp_reset_window (soc);
			__nop();
      break;
		}
  }
  return 0;
}

void modbus_handler(void)
{
	mb_inc_packet_parse(modBusBuffer, &incomingPack);
	mb_answer_handle(&incomingPack, &outcomingPack);
	mb_out_packet_form(outBuffer, &outcomingPack);
}
