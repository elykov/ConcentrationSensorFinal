#ifndef __MODBUS_TCP__
	#define __MODBUS_TCP__

  #include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "rl_net.h"

	#ifdef  __cplusplus
		#define MODBUS_BEGIN_DECLS  extern "C" {
		#define MODBUS_END_DECLS    }
	#else
		#define MODBUS_BEGIN_DECLS
		#define MODBUS_END_DECLS
	#endif

	//#define MODBUS_TCP_DEFAULT_PORT   502
	//#define MODBUS_TCP_SLAVE         0xFF
  //#define MODBUS_BROADCAST_ADDRESS    0
  //#define MODBUS_TCP_MAX_ADU_LENGTH  260

	MODBUS_BEGIN_DECLS

	//=================================================
	//================= Data Types ====================
  
		typedef struct modbus_tcp {
			int32_t sock; 	// socket
			uint16_t port;	// TCP port
		} modbus_t;

		typedef struct _MBAP_HEADER
		{
			uint16_t 	transactionID;
			uint16_t 	protocolID;
			uint16_t 	length;
			uint8_t 	unitID;
		} mb_header;

		typedef struct _MODBUS_INCOMING_PACKET
		{
			mb_header header;
			uint8_t 	commandCode;
			uint16_t 	addressFirstRegister;
			uint16_t 	registersCount;
		} mb_in_packet;
		
		typedef struct _MODBUS_OUTCOMING_PACKET
		{
			mb_header header;
			uint8_t 	commandCode;
			uint8_t 	answerLength;
			uint8_t 	data[256];
		} mb_out_packet;

	//================ /Data Types ====================
  //=================================================
  
	//=================================================
	//================= Functions =====================
  
	extern void modbus_handler(void);
	extern int initModbus(modbus_t* mb);
	extern void mb_inc_packet_parse(const uint8_t* message, mb_in_packet* mbp);
	extern void mb_out_packet_form(uint8_t* data, mb_out_packet* mbp);
	extern void modbus_send(const uint8_t* message, modbus_t* mb);
	
	//================ /Functions =====================
  //=================================================

  MODBUS_END_DECLS
#endif
