#ifndef __SERVER_PART__
	#define __SERVER_PART__
	#include "Net_User.h"

	extern int tcp_soc_SERVER;

	extern unsigned int tcp_callback_SERVER (int32_t soc, tcpEvent event, const uint8_t *buf, uint32_t len);
	extern int server_init (void);
	extern int server_working(void);
	//extern void Parsing_package_SERVER(void);
	
#endif	
