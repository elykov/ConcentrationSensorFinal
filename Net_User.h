/********************************************************************************

********************************************************************************/
#ifndef NET_USER_H_
#define NET_USER_H_

#include "rl_net.h"

extern int tcp_soc_TECH, tcp_soc_PLC, tcp_soc_WORK;
extern unsigned char rem_ip[4];
extern int eth_st, net_st, tcp_st_TECH, tcp_st_PLC, tcp_st_WORK;
extern unsigned char soc_state;



extern unsigned int tcp_callback_TECH (int32_t soc, tcpEvent event, const uint8_t *buf, uint32_t len);
extern void NET_init (void); 
extern void send_data (void);

#endif /* NET_USER_H_ */
