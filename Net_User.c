/********************************************************************************

********************************************************************************/

#include "stm32f746xx.h"
#include "rl_net.h"
#include <string.h>
#include "global_var.h"
#include "settings.h"


int tcp_soc_TECH, tcp_soc_PLC, tcp_soc_WORK;

unsigned char rem_ip[4] = {192,168,99,140};
int eth_st, net_st, tcp_st_TECH, tcp_st_PLC, tcp_st_WORK;
unsigned char state_socket_TECH = 0, state_socket_PLC = 0, state_socket_WORK = 0;
unsigned char soc_state = 0;
bool wait_ack;


unsigned int tcp_callback_TECH (int32_t soc, tcpEvent event, const uint8_t *buf, uint32_t len) 
{
  // This function is called on TCP event 
	unsigned short i, n;
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
			if(len > 0xff) n = 0xff;
			else n = len;
			for(i = 0; i < n; i++)//for(i = 0; i <= len; i++)
			{
				Recive_TECH[i] = *buf;
				buf++;
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
	unsigned short i, n;
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
			n = (len > 0xff) ? 0xff : len;
			
			for(i = 0; i < n; i++)//for(i = 0; i <= len; i++)
			{
				Recive_PLC[i] = *buf;
				buf++;
			}
			Flags.incoming_plc = 1;
			tcp_reset_window (soc);		
			__nop();
      break;
  }
  return (0);
}

unsigned int tcp_callback_WORK (int32_t soc, tcpEvent event, const uint8_t *buf, uint32_t len) 
{
  // This function is called on TCP event
	unsigned short i, n;
  
  switch (event) 
	{
    case tcpEventConnect:
      // Remote host is trying to connect to our TCP socket. 
      // 'buf' points to Remote IP, 'len' holds the remote port. 
//			soc_state = 2;
			__nop();
      // Return 1 to accept connection, or 0 to reject connection 
      return (1);
    case tcpEventAbort:
      // Connection was aborted 
			__nop();
      break;
    case tcpEventEstablished:
      // Socket is connected to remote peer. 
			soc_state = 2;
			__nop();
      break;
    case tcpEventClosed:
      // Connection has been closed 
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
			if(len > 0xff) n = 0xff;
			else n = len;
			for(i = 0; i < n; i++)//for(i = 0; i <= len; i++)
			{
				Recive_WORK[i] = *buf;
				buf++;
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
  
	tcp_soc_PLC = tcp_get_socket (TCP_TYPE_SERVER, 0, 10, tcp_callback_PLC);
	if (tcp_soc_PLC >= 0) 
	{
		tcp_listen (tcp_soc_PLC, 4000);
	}		
  tcp_soc_WORK = tcp_get_socket (TCP_TYPE_CLIENT, 0, 10, tcp_callback_WORK);
	soc_state = 0;
}


void send_data (void) //������
{
	uint32_t max;
	uint8_t *sendbuf;
//	unsigned char temp;
	
	if(soc_state == 0)
	{
			tcp_connect (tcp_soc_WORK, rem_ip, 4000, 0);
			wait_ack  = false;
			soc_state = 1;		
	}
	
	if(soc_state == 2)
	{
			if (wait_ack != true)
			{				
				if(Flags.answer_work)
				{
					max = tcp_max_data_size (tcp_soc_WORK);
					maxlen_work = (max > 0xff) ? 0xff : max;
					Change_Parameters();
					Form_package_WORK();
					sendbuf = tcp_get_buf(maxlen_work);
					memcpy(sendbuf, Send_WORK, maxlen_work);
					tcp_send(tcp_soc_WORK, sendbuf, maxlen_work);
					wait_ack = true;
					Flags.answer_work	= 0;				
				}
			}
	}
}



