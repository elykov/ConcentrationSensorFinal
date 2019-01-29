/********************************************************************************

********************************************************************************/
#ifndef NET_USER_H_
#define NET_USER_H_

#include "rl_net.h"

//extern int tcp_soc_TECH, tcp_soc_PLC;
//extern int eth_st, net_st, tcp_st_TECH, tcp_st_PLC,

extern int tcp_soc_WORK;
extern unsigned char rem_ip[4];
extern int tcp_st_WORK;
extern unsigned char soc_state;

extern void NET_init (void); 
extern void send_data (void);

#endif /* NET_USER_H_ */
