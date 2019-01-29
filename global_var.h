#ifndef GLOBAL_VAR_H_
	#define GLOBAL_VAR_H_
	#include "rl_net_lib.h"

	extern  LOCALM localm[];
	#define LocM   localm[NETIF_ETH]

	union Converter
	{
		int idata;
		short sdata;
		float fdata;
		char cdata[4];
	};

	struct TFlags 
	{
		uint8_t  answer_server;
		uint8_t  incoming_server;
		uint8_t  modbus;
		uint8_t  answer_work:					1;
		uint8_t  incoming_work:				1;		
		uint8_t  answer_tech:					1;
		uint8_t  incoming_tech:				1;	
		uint8_t  answer_plc:					1;
		uint8_t  incoming_plc:				1;
		uint8_t  udp_enable:					1;
		uint8_t  ch_IP:								1;
		uint8_t  ch_Mask:							1;
		uint8_t  ch_DefGW:						1;
		uint8_t  ch_PriDNS:						1;
		uint8_t  ch_SecDNS:						1;
		uint8_t  ch_RemUDP:						1;
		uint8_t  ch_a:								1;
		uint8_t  ch_b:								1;
		uint8_t  ch_c:								1;		
		uint8_t  ch_d:								1;
		uint8_t  ch_e:								1;
		uint8_t  ch_n:								1;
		uint8_t  ch_f:								1;
		uint8_t  ch_g:								1;
		uint8_t  ch_h:								1;
		uint8_t  ch_i_tr:							1;
		uint8_t  ch_i_rev:						1;
		uint8_t  ch_dump:							1;
		uint8_t  ch_period:						1;
		uint8_t  ch_4mA:							1;
		uint8_t  ch_20mA:							1;
		uint8_t  ch_P:								1;
		uint8_t  ch_I:								1;
		uint8_t  ch_D:								1;
		uint8_t  ch_dump_i:						1;
		uint8_t  ch_ref:							1;
		uint8_t  ch_Cb:								1;
		uint8_t  ch_Output_I:					1;
		uint8_t  ch_damper:						1;
		uint8_t  ch_offset:						1;
		uint8_t  ch_gain:							1;
		uint8_t  ch_pid_period:				1;
		uint8_t  ch_water:						1;
		uint8_t  ch_air:							1;
		uint8_t  ch_km:								1;
	};

	extern struct TFlags Flags;

// Сетевые переменные	
	extern unsigned char Send_WORK[256];
	extern unsigned char Recive_WORK[256];
	extern unsigned char maxlen_work;
	extern unsigned short PortTCP;
	
// Входящие сетевые параметры: 

	extern unsigned char DatIP[4];
	extern unsigned char DatMask[4];
	extern unsigned char DatDefGW[4];
	extern unsigned char DatPriDNS[4];
	extern unsigned char DatSecDNS[4];

// Исходящие сетевые параметры

	extern unsigned char outDatIP[4];
	extern unsigned char outDatMask[4];
	extern unsigned char outDatDefGW[4];
	extern unsigned char outDatPriDNS[4];
	extern unsigned char outDatSecDNS[4];

// Входящие переменные	
	// ПИД

	extern float P_factor, I_factor, D_factor;


	// Параметры процесса

	extern unsigned short dump;
	extern unsigned short period_answer;
	extern unsigned short i_trowel;
	extern unsigned short i_revers;
	extern unsigned short dump_i;
	

	// Параметры формулы

	extern float a;
	extern float b;
	extern float c;
	extern float d;
	extern float e;
	extern float n;
	extern float f;
	extern float g;
	extern float h;

	// Остальные

	extern unsigned int up;
	extern unsigned int down;
	extern unsigned int sum;
	extern unsigned int DumpSum;
	


extern float Cb;
	extern float Output_I, referens;
	extern uint8_t sendParam;
	extern uint8_t workMode;
	extern float damper, offset, gain;
	extern float damper_manual;
	extern unsigned short pid_period;
	extern uint32_t water, air;
	extern float km;
		


// Исходящие переменные 
	// ПИД

	extern float out_referens;
	extern unsigned short out_pid_period;
	extern unsigned short out_dump_i;

	extern float out_P_factor, out_I_factor, out_D_factor;

	// Параметры процесса

	extern unsigned short out_i_trowel;
	extern unsigned short out_i_revers;
	extern unsigned short out_period_answer;
	extern unsigned short out_dump;
	extern float out_offset, out_gain;
	extern uint32_t out_water, out_air;
	
	// Параметры формулы

	extern float oa;
	extern float ob;
	extern float oc;
	extern float od;
	extern float oe;
	extern float on;
	extern float of;
	extern float og;
	extern float oh;
	
	// Остальные переменные

	extern float out_damper;
	extern uint8_t out_workMode;

	// Нужны ли?

	extern unsigned short out;
	extern unsigned short i_out;
	extern float C, Ci;
	extern float i4mA;
	extern float i20mA;
	extern float out_i4mA;
	extern float out_i20mA;
	

// Старые параметры

	//extern float p_term, i_term, d_term;

#endif /* GLOBAL_VAR_H_ */
