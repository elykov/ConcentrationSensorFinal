/********************************************************************************

********************************************************************************/
#ifndef GLOBAL_VAR_H_
#define GLOBAL_VAR_H_

//#include "type.h"
#include "rl_net_lib.h"

struct TFlags 
{
	//uint8_t 
	unsigned int answer_server;
	unsigned int incoming_server;
	unsigned int answer_work:					1;
	unsigned int incoming_work:				1;		
	unsigned int answer_tech:					1;
	unsigned int incoming_tech:				1;	
	unsigned int answer_plc:					1;
	unsigned int incoming_plc:				1;	
	unsigned int udp_enable:					1;
	unsigned int ch_IP:								1;
	unsigned int ch_Mask:							1;
	unsigned int ch_DefGW:						1;
	unsigned int ch_PriDNS:						1;
	unsigned int ch_SecDNS:						1;
	unsigned int ch_RemUDP:						1;
	unsigned int ch_a:								1;
	unsigned int ch_b:								1;
	unsigned int ch_c:								1;		
	unsigned int ch_d:								1;
	unsigned int ch_e:								1;
	unsigned int ch_n:								1;
	unsigned int ch_f:								1;
	unsigned int ch_g:								1;
	unsigned int ch_h:								1;
	unsigned int ch_i_tr:							1;
	unsigned int ch_i_rev:						1;
	unsigned int ch_dump:							1;
	unsigned int ch_period:						1;
	unsigned int ch_4mA:							1;
	unsigned int ch_20mA:							1;
	unsigned int ch_P:								1;
	unsigned int ch_I:								1;
	unsigned int ch_D:								1;
	unsigned int ch_dump_i:						1;
	unsigned int ch_ref:							1;
	unsigned int ch_Cb:								1;
	unsigned int ch_Output_I:					1;
	unsigned int ch_damper:						1;
	unsigned int ch_offset:						1;
	unsigned int ch_gain:							1;
};

extern struct TFlags Flags;

extern unsigned short PortTCP;
extern unsigned short TechPortTCP;
extern unsigned short PortUDP;
extern unsigned short RemPortUDP;
extern unsigned char  remUDPip[4];
extern unsigned char DatIP[4];
extern unsigned char DatMask[4];
extern unsigned char DatDefGW[4];
extern unsigned char DatPriDNS[4];
extern unsigned char DatSecDNS[4];
extern unsigned char outDatIP[4];
extern unsigned char outDatMask[4];
extern unsigned char outDatDefGW[4];
extern unsigned char outDatPriDNS[4];
extern unsigned char outDatSecDNS[4];
extern unsigned char OUTremUDPip[4];


extern unsigned char Send_WORK[256];
extern unsigned char Recive_WORK[256];
extern unsigned char Send_TECH[256];
extern unsigned char Recive_TECH[256];
extern unsigned char Send_PLC[256];
extern unsigned char Recive_PLC[256];
extern unsigned char Send_SERVER[256];
extern unsigned char Recive_SERVER[256];

extern unsigned char tempBuf[4];
extern unsigned char  P_factor, I_factor, D_factor;
extern unsigned char  out_P_factor, out_I_factor, out_D_factor;
extern unsigned char maxlen_work;

extern unsigned short dump;
extern unsigned short out_dump;
extern unsigned short period_answer;
extern unsigned short out_period_answer;
extern unsigned short i_trowel;
extern unsigned short i_revers;
extern unsigned short out_i_trowel;
extern unsigned short out_i_revers;
extern unsigned short dump_i;
extern unsigned short out_dump_i;
extern unsigned short out;

extern unsigned int up;
extern unsigned int down;
extern unsigned int sum;
extern unsigned int DumpSum;
extern unsigned short i_out;

extern float a;
extern float b;
extern float c;
extern float d;
extern float e;
extern float n;
extern float f;
extern float g;
extern float h;
extern float C;
extern float i4mA;
extern float i20mA;
extern float out_i4mA;
extern float out_i20mA;

extern float Cb;
extern float Ci;
extern float p_term, i_term, d_term;
extern float Output_I, referens;
extern float out_referens;
extern float oa;
extern float ob;
extern float oc;
extern float od;
extern float oe;
extern float on;
extern float of;
extern float og;
extern float oh;

// ==============================================

extern  LOCALM localm[];
#define LocM   localm[NETIF_ETH]

extern uint8_t sendParam;
extern uint8_t workMode;
extern float damper, offset, gain;
extern float out_damper,out_offset, out_gain;
extern uint8_t out_workMode;
extern float damper_manual;

#endif /* GLOBAL_VAR_H_ */











