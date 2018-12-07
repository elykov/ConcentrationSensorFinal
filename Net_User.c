#include "stm32f746xx.h"
#include "rl_net.h"
#include <string.h>
#include "global_var.h"
#include "settings.h"

int sendCount = 0;
int tcp_soc_TECH, tcp_soc_PLC, tcp_soc_WORK;

unsigned char rem_ip[4] = {192,168,99,200};
int eth_st, net_st, tcp_st_TECH, tcp_st_PLC, tcp_st_WORK;
unsigned char state_socket_TECH = 0, state_socket_PLC = 0, state_socket_WORK = 0;
unsigned char soc_state = 0;
bool wait_ack;

unsigned int tcp_callback_TECH (int32_t soc, tcpEvent event, const uint8_t *buf, uint32_t len) 
{
  // This function is called on TCP event 
	unsigned short i, _n;
	const uint8_t* pbuf;
  // ..
  switch (event) 
	{
    case tcpEventConnect:
      // Remote host is trying to connect to our TCP socket. 
      // 'buf' points to Remote IP, 'len' holds the remote port. 
			__nop();
      // Return 1 to accept connection, or 0 to reject connection 
      return (1);
    case tcpEventAbort:
      // Connection was aborted 
			__nop();
      break;
    case tcpEventEstablished:
      // Socket is connected to remote peer. 
			__nop();
      break;
    case tcpEventClosed:
      // Connection has been closed 
			__nop();
      break;
    case tcpEventACK:
      // Our sent data has been acknowledged by remote peer 
			__nop();
      break;
    case tcpEventData:
      // TCP data frame has been received, 'buf' points to data 
      // Data length is 'len' bytes
			if(len > 0xff) _n = 0xff;
			else _n = len;
			pbuf = buf;
			for(i = 0; i < _n; ++i)
			{
				Recive_TECH[i] = *pbuf;
				++pbuf;
			}
			Flags.incoming_tech = 1;		
			tcp_reset_window (soc);
			__nop();
      break;
  }
  return (0);
}

unsigned int tcp_callback_PLC (int32_t soc, tcpEvent event, const uint8_t *buf, uint32_t len) 
{
  // This function is called on TCP event 
  // ..
  switch (event) 
	{
    case tcpEventConnect:
      // Remote host is trying to connect to our TCP socket. 
      // 'buf' points to Remote IP, 'len' holds the remote port.
			
			__nop();
      return (1);
    case tcpEventAbort: // Connection was aborted 
			__nop();
      break;
    case tcpEventEstablished: // Socket is connected to remote peer. 
			__nop();
      break;
    case tcpEventClosed: // Connection has been closed 
			__nop();
      break;
    case tcpEventACK: // Our sent data has been acknowledged by remote peer
			__nop();
      break;
    case tcpEventData:
		{
      // TCP data frame has been received, 'buf' points to data 
      // Data length is 'len' bytes
			register unsigned short i;
			unsigned short _n;
			
      _n = (len > 0xff) ? 0xff : len;
			
			for(i = 0; i < _n; ++i)
			{
				Recive_PLC[i] = *buf;
				++buf;
			}
			Flags.incoming_plc = 1;
			tcp_reset_window (soc);		
			__nop();
      break;
		}
  }
  return (0);
}

unsigned int tcp_callback_WORK (int32_t soc, tcpEvent event, const uint8_t *buf, uint32_t len) 
{
  // This function is called on TCP event
	unsigned short i, _n;
  
  switch (event) 
	{
    case tcpEventConnect:
      // Remote host is trying to connect to our TCP socket. 
      // 'buf' points to Remote IP, 'len' holds the remote port. 
			__nop();
      // Return 1 to accept connection, or 0 to reject connection 
			__nop();
      return (1);
    case tcpEventAbort:
      // Connection was aborted 
			i_trowel = i_revers = dump = period_answer = 
			referens = Cb = Output_I = dump_i = 
			P_factor = I_factor = D_factor = 0;
			offset = gain = damper = 0;
			__nop();
      break;
    case tcpEventEstablished:
      // Socket is connected to remote peer. 
			soc_state = 2;
			__nop();
      break;
    case tcpEventClosed:
      // Connection has been closed 
			i_trowel = i_revers = dump = period_answer = 
			referens = Cb = Output_I = dump_i = 
			P_factor = I_factor = D_factor = 
			offset = gain = damper = 0;
			__nop();
      break;
    case tcpEventACK:
      // Our sent data has been acknowledged by remote peer 
			wait_ack = false;
			__nop();
      break;
    case tcpEventData:
      // TCP data frame has been received, 'buf' points to data 
      // Data length is 'len' bytes
			if (len > 0xff)
				_n = 0xff;
			else
				_n = len;
			
			for(i = 0; i < _n; ++i)
			{
				Recive_WORK[i] = *buf;
				++buf;
			}
			Flags.incoming_work = 1;
			tcp_reset_window (soc);		
			__nop();
      break;
  }
  return (0);
}


void NET_init (void)
{
	eth_st = net_initialize ();

  tcp_soc_TECH = tcp_get_socket (TCP_TYPE_SERVER, 0, 10, tcp_callback_TECH);
	if (tcp_soc_TECH >= 0) 
	{
		tcp_listen (tcp_soc_TECH, 4001);
	}	
  
	//tcp_soc_PLC = tcp_get_socket (TCP_TYPE_SERVER, 0, 10, tcp_callback_PLC);
	//if (tcp_soc_PLC >= 0) 
	//{
		//tcp_listen (tcp_soc_PLC, 4003);
	//}		
  tcp_soc_WORK = tcp_get_socket (TCP_TYPE_CLIENT, 0, 10, tcp_callback_WORK);
	soc_state = 0;
}


void send_data (void) //датчик
{
	uint32_t max;
	uint8_t *sendbuf;
	
	if(soc_state == 0)
	{
			tcp_connect (tcp_soc_WORK, rem_ip, 4000, 4002);
			wait_ack  = false;
			soc_state = 1;		
	}
	
	if(soc_state == 2) // установлено соединение
	{
		if (wait_ack != true) // не ожидаем ответа
		{				
			if(Flags.answer_work) // желаем отправить пакет
			{
				if (tcp_check_send(tcp_soc_WORK))
				{
					max = tcp_max_data_size (tcp_soc_WORK);
					if (max > 0xff)
						maxlen_work = 0xff;
          else
						maxlen_work = max;
	
					Change_Parameters();
					Form_package_WORK();
					sendbuf = tcp_get_buf(maxlen_work);
					memcpy(sendbuf, Send_WORK, maxlen_work);
					tcp_send(tcp_soc_WORK, sendbuf, maxlen_work);
					wait_ack = true;
					Flags.answer_work	= 0;
					sendParam = 0;
				}
			}
		}
	}
}
