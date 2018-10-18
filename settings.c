/***********************************************************************

***********************************************************************/

#include "qspi.h"
#include "global_var.h"
#include "rl_net_lib.h"
#include "Net_User.h"

extern  LOCALM localm[];
#define LocM   localm[NETIF_ETH]

void Read_settings (void);
void Write_settings (void);

void float_to_buf_tx (float data)
{
	tempBuf[3]= ((unsigned char*)&(data))[3]; 
	tempBuf[2]= ((unsigned char*)&(data))[2]; 
	tempBuf[1]= ((unsigned char*)&(data))[1]; 
	tempBuf[0]= ((unsigned char*)&(data))[0];
}

float buf_tx_to_float (void)
{
		unsigned long temp;
		float tempf;
		temp = (tempBuf[3]<<24)|(tempBuf[2]<<16)|(tempBuf[1]<<8)|(tempBuf[0]);
		tempf = *((float *)&temp);
		return tempf;
}

void Read_settings (void) // читает из qspi и записывает в сетевые переменные
{

	if(nor_status == 0)//читаем настройки
	{
		QSPI_read_qspi_aRxBuffer ();		
	}

	if(qspi_aRxBuffer[511] != 0xAB)
	{
		Write_settings ();
	}
	else
	{
		LocM.IpAddr[0] = qspi_aRxBuffer[0];
		LocM.IpAddr[1] = qspi_aRxBuffer[1];
		LocM.IpAddr[2] = qspi_aRxBuffer[2];
		LocM.IpAddr[3] = qspi_aRxBuffer[3];	
		LocM.NetMask[0] = qspi_aRxBuffer[4];	
		LocM.NetMask[1] = qspi_aRxBuffer[5];	
		LocM.NetMask[2] = qspi_aRxBuffer[6];
		LocM.NetMask[3] = qspi_aRxBuffer[7];	
		LocM.DefGW[0] = qspi_aRxBuffer[8];
		LocM.DefGW[1] = qspi_aRxBuffer[9];
		LocM.DefGW[2] = qspi_aRxBuffer[10];	
		LocM.DefGW[3] = qspi_aRxBuffer[11];	
		LocM.PriDNS[0] = qspi_aRxBuffer[12];		
		LocM.PriDNS[1] = qspi_aRxBuffer[13];		
		LocM.PriDNS[2] = qspi_aRxBuffer[14];		
		LocM.PriDNS[3] = qspi_aRxBuffer[15];			
		LocM.SecDNS[0] = qspi_aRxBuffer[16];
		LocM.SecDNS[1] = qspi_aRxBuffer[17];	
		LocM.SecDNS[2] = qspi_aRxBuffer[18];	
		LocM.SecDNS[3] = qspi_aRxBuffer[19];	
		rem_ip[0] = qspi_aRxBuffer[20];
		rem_ip[1] = qspi_aRxBuffer[21];
		rem_ip[2] = qspi_aRxBuffer[22];
		rem_ip[3] = qspi_aRxBuffer[23];	

	}
}

void Write_settings (void) // записывает в qspi сетевые переменные
{
	qspi_aTxBuffer[0] = LocM.IpAddr[0];
	qspi_aTxBuffer[1] = LocM.IpAddr[1];
	qspi_aTxBuffer[2] = LocM.IpAddr[2];
	qspi_aTxBuffer[3] = LocM.IpAddr[3];	
	qspi_aTxBuffer[4] = LocM.NetMask[0];
	qspi_aTxBuffer[5] = LocM.NetMask[1];	
	qspi_aTxBuffer[6] = LocM.NetMask[2];
	qspi_aTxBuffer[7] = LocM.NetMask[3];	
	qspi_aTxBuffer[8] =	LocM.DefGW[0];
	qspi_aTxBuffer[9] =	LocM.DefGW[1];	
	qspi_aTxBuffer[10] =LocM.DefGW[2];	
	qspi_aTxBuffer[11] =LocM.DefGW[3];	
	qspi_aTxBuffer[12] = LocM.PriDNS[0];
	qspi_aTxBuffer[13] = LocM.PriDNS[1];	
	qspi_aTxBuffer[14] = LocM.PriDNS[2];
	qspi_aTxBuffer[15] = LocM.PriDNS[3];	
	qspi_aTxBuffer[16] = LocM.SecDNS[0];	
	qspi_aTxBuffer[17] = LocM.SecDNS[1];		
	qspi_aTxBuffer[18] = LocM.SecDNS[2];		
	qspi_aTxBuffer[19] = LocM.SecDNS[3];
	qspi_aTxBuffer[20] = rem_ip[0];
	qspi_aTxBuffer[21] = rem_ip[1];
	qspi_aTxBuffer[22] = rem_ip[2];
	qspi_aTxBuffer[23] = rem_ip[3];
	
	qspi_aTxBuffer[511] = 0xAB;
	QSPI_write_qspi_aTxBuffer ();
}


void Parsing_package_WORK (void)//разбор посылки от датчика
{
	tempBuf[0] = Recive_WORK[0];
	tempBuf[1] = Recive_WORK[1];
	tempBuf[2] = Recive_WORK[2];
	tempBuf[3] = Recive_WORK[3];
	
  C = buf_tx_to_float();
	
	up = ((Recive_WORK[7] << 24) + (Recive_WORK[6] << 16) + (Recive_WORK[5] << 8) + Recive_WORK[4]);
	
	down = ((Recive_WORK[11] << 24) + (Recive_WORK[10] << 16) + (Recive_WORK[9] << 8) + Recive_WORK[8]);
	
	sum = ((Recive_WORK[15] << 24) + (Recive_WORK[14] << 16) + (Recive_WORK[13] << 8) + Recive_WORK[12]);
	
	DumpSum = ((Recive_WORK[19] << 24) + (Recive_WORK[18] << 16) + (Recive_WORK[17] << 8) + Recive_WORK[16]);
	
	i_trowel = (Recive_WORK[21] << 8) + Recive_WORK[20];
	i_revers = (Recive_WORK[23] << 8) + Recive_WORK[22];	
	
	dump = (Recive_WORK[25] << 8) + Recive_WORK[24];	
	//dump = (Recive_WORK[27] << 8) + Recive_WORK[26];
	period_answer = (Recive_WORK[27] << 8) + Recive_WORK[26];
	
	tempBuf[0] = Recive_WORK[28];
	tempBuf[1] = Recive_WORK[29];
	tempBuf[2] = Recive_WORK[30];
	tempBuf[3] = Recive_WORK[31];
  a = buf_tx_to_float();	
	
	tempBuf[0] = Recive_WORK[32];
	tempBuf[1] = Recive_WORK[33];
	tempBuf[2] = Recive_WORK[34];
	tempBuf[3] = Recive_WORK[35];
  b = buf_tx_to_float();		
	
	tempBuf[0] = Recive_WORK[36];
	tempBuf[1] = Recive_WORK[37];
	tempBuf[2] = Recive_WORK[38];
	tempBuf[3] = Recive_WORK[39];
  c = buf_tx_to_float();		
	
	tempBuf[0] = Recive_WORK[40];
	tempBuf[1] = Recive_WORK[41];
	tempBuf[2] = Recive_WORK[42];
	tempBuf[3] = Recive_WORK[43];
  d = buf_tx_to_float();		
	tempBuf[0] = Recive_WORK[44];
	tempBuf[1] = Recive_WORK[45];
	tempBuf[2] = Recive_WORK[46];
	tempBuf[3] = Recive_WORK[47];
  e = buf_tx_to_float();		
	tempBuf[0] = Recive_WORK[48];
	tempBuf[1] = Recive_WORK[49];
	tempBuf[2] = Recive_WORK[50];
	tempBuf[3] = Recive_WORK[51];
  n = buf_tx_to_float();
	tempBuf[0] = Recive_WORK[52];
	tempBuf[1] = Recive_WORK[53];
	tempBuf[2] = Recive_WORK[54];
	tempBuf[3] = Recive_WORK[55];
  f = buf_tx_to_float();
	tempBuf[0] = Recive_WORK[56];
	tempBuf[1] = Recive_WORK[57];
	tempBuf[2] = Recive_WORK[58];
	tempBuf[3] = Recive_WORK[59];
  g = buf_tx_to_float();		
	tempBuf[0] = Recive_WORK[60];
	tempBuf[1] = Recive_WORK[61];
	tempBuf[2] = Recive_WORK[62];
	tempBuf[3] = Recive_WORK[63];
  h = buf_tx_to_float();

	DatIP[0] = Recive_WORK[64];
	DatIP[1] = Recive_WORK[65];	
	DatIP[2] = Recive_WORK[66];
	DatIP[3] = Recive_WORK[67];	
	DatMask[0] = Recive_WORK[68];
	DatMask[1] = Recive_WORK[69];
	DatMask[2] = Recive_WORK[70];
	DatMask[3] = Recive_WORK[71];
	DatDefGW[0] = Recive_WORK[72];
	DatDefGW[1] = Recive_WORK[73];
	DatDefGW[2] = Recive_WORK[74];
	DatDefGW[3] = Recive_WORK[75];
	DatPriDNS[0] = Recive_WORK[76];
	DatPriDNS[1] = Recive_WORK[77];
	DatPriDNS[2] = Recive_WORK[78];
	DatPriDNS[3] = Recive_WORK[79];	
	DatSecDNS[0] = Recive_WORK[80];
	DatSecDNS[1] = Recive_WORK[81];
	DatSecDNS[2] = Recive_WORK[82];
	DatSecDNS[3] = Recive_WORK[83];
	PortTCP = (Recive_WORK[85] << 8) + Recive_WORK[84];
	PortUDP = (Recive_WORK[87] << 8) + Recive_WORK[86];
	RemPortUDP = (Recive_WORK[89] << 8) + Recive_WORK[88];
	remUDPip[0] = Recive_WORK[90];
	remUDPip[1] = Recive_WORK[91];
	remUDPip[2] = Recive_WORK[92];
	remUDPip[3] = Recive_WORK[93];

	tempBuf[0] = Recive_WORK[94];
	tempBuf[1] = Recive_WORK[95];
	tempBuf[2] = Recive_WORK[96];
	tempBuf[3] = Recive_WORK[97];
  i4mA = buf_tx_to_float();
	tempBuf[0] = Recive_WORK[98];
	tempBuf[1] = Recive_WORK[99];
	tempBuf[2] = Recive_WORK[100];
	tempBuf[3] = Recive_WORK[101];
  i20mA = buf_tx_to_float();
	tempBuf[0] = Recive_WORK[102];
	tempBuf[1] = Recive_WORK[103];
	tempBuf[2] = Recive_WORK[104];
	tempBuf[3] = Recive_WORK[105];
  i_out = buf_tx_to_float();
		
	if(Recive_WORK[106] == 255) Flags.udp_enable = 1;
	else Flags.udp_enable = 0;

	tempBuf[0] = Recive_WORK[107];
	tempBuf[1] = Recive_WORK[108];
	tempBuf[2] = Recive_WORK[109];
	tempBuf[3] = Recive_WORK[110];
  Cb = buf_tx_to_float();
	tempBuf[0] = Recive_WORK[111];
	tempBuf[1] = Recive_WORK[112];
	tempBuf[2] = Recive_WORK[113];
	tempBuf[3] = Recive_WORK[114];
  Ci = buf_tx_to_float();	
	tempBuf[0] = Recive_WORK[115];
	tempBuf[1] = Recive_WORK[116];
	tempBuf[2] = Recive_WORK[117];
	tempBuf[3] = Recive_WORK[118];
  p_term = buf_tx_to_float();	
	tempBuf[0] = Recive_WORK[119];
	tempBuf[1] = Recive_WORK[120];
	tempBuf[2] = Recive_WORK[121];
	tempBuf[3] = Recive_WORK[122];
  i_term = buf_tx_to_float();	
	tempBuf[0] = Recive_WORK[123];
	tempBuf[1] = Recive_WORK[124];
	tempBuf[2] = Recive_WORK[125];
	tempBuf[3] = Recive_WORK[126];
  d_term = buf_tx_to_float();	
	P_factor = Recive_WORK[127];
	I_factor = Recive_WORK[128];
	D_factor = Recive_WORK[129];

	dump_i = (Recive_WORK[130] << 8) + Recive_WORK[131];
	out = (Recive_WORK[132] << 8) + Recive_WORK[133];
	i_out = (Recive_WORK[134] << 8) + Recive_WORK[135];	

	tempBuf[0] = Recive_WORK[136];
	tempBuf[1] = Recive_WORK[137];
	tempBuf[2] = Recive_WORK[138];
	tempBuf[3] = Recive_WORK[139];
  referens = buf_tx_to_float();	
	tempBuf[0] = Recive_WORK[140];
	tempBuf[1] = Recive_WORK[141];
	tempBuf[2] = Recive_WORK[142];
	tempBuf[3] = Recive_WORK[143];
  Output_I = buf_tx_to_float();	
}


void Form_package_WORK (void)//сборка посылки в датчик (копирует в буфер из outПеременных)
{
	Send_WORK[0] =  outDatIP[0];
	Send_WORK[1] =  outDatIP[1];	
	Send_WORK[2] =  outDatIP[2];	
	Send_WORK[3] =  outDatIP[3];	
	Send_WORK[4] =  outDatMask[0];	
	Send_WORK[5] =  outDatMask[1];	
	Send_WORK[6] =  outDatMask[2];	
	Send_WORK[7] =  outDatMask[3];		
	Send_WORK[8] =  outDatDefGW[0];
	Send_WORK[9] =  outDatDefGW[1];
	Send_WORK[10] =  outDatDefGW[2];
	Send_WORK[11] =  outDatDefGW[3];
	Send_WORK[12] =  outDatPriDNS[0];	
	Send_WORK[13] =  outDatPriDNS[1];		
	Send_WORK[14] =  outDatPriDNS[2];		
	Send_WORK[15] =  outDatPriDNS[3];	
	Send_WORK[16] =  outDatSecDNS[0];	
	Send_WORK[17] =  outDatSecDNS[1];	
	Send_WORK[18] =  outDatSecDNS[2];	
	Send_WORK[19] =  outDatSecDNS[3];		
	Send_WORK[20] = (unsigned char)PortTCP;
	Send_WORK[21] = (unsigned char)(PortTCP >> 8);
	Send_WORK[22] = (unsigned char)PortUDP;
	Send_WORK[23] = (unsigned char)(PortUDP >> 8);
	Send_WORK[24] = (unsigned char)RemPortUDP;
	Send_WORK[25] = (unsigned char)(RemPortUDP >> 8);
	Send_WORK[26] = OUTremUDPip[0];
	Send_WORK[27] = OUTremUDPip[1];
	Send_WORK[28] = OUTremUDPip[2];
	Send_WORK[29] = OUTremUDPip[3];
	float_to_buf_tx (oa);
	Send_WORK[30] = tempBuf[0];
	Send_WORK[31] = tempBuf[1];
	Send_WORK[32] = tempBuf[2];
	Send_WORK[33] = tempBuf[3];
	float_to_buf_tx (ob);
	Send_WORK[34] = tempBuf[0];
	Send_WORK[35] = tempBuf[1];
	Send_WORK[36] = tempBuf[2];
	Send_WORK[37] = tempBuf[3];
	float_to_buf_tx (oc);
	Send_WORK[38] = tempBuf[0];
	Send_WORK[39] = tempBuf[1];
	Send_WORK[40] = tempBuf[2];
	Send_WORK[41] = tempBuf[3];
	float_to_buf_tx (od);
	Send_WORK[42] = tempBuf[0];
	Send_WORK[43] = tempBuf[1];
	Send_WORK[44] = tempBuf[2];
	Send_WORK[45] = tempBuf[3];
	float_to_buf_tx (oe);
	Send_WORK[46] = tempBuf[0];
	Send_WORK[47] = tempBuf[1];
	Send_WORK[48] = tempBuf[2];
	Send_WORK[49] = tempBuf[3];
	float_to_buf_tx (on);
	Send_WORK[50] = tempBuf[0];
	Send_WORK[51] = tempBuf[1];
	Send_WORK[52] = tempBuf[2];
	Send_WORK[53] = tempBuf[3];
	float_to_buf_tx (of);
	Send_WORK[54] = tempBuf[0];
	Send_WORK[55] = tempBuf[1];
	Send_WORK[56] = tempBuf[2];
	Send_WORK[57] = tempBuf[3];
	float_to_buf_tx (og);
	Send_WORK[58] = tempBuf[0];
	Send_WORK[59] = tempBuf[1];
	Send_WORK[60] = tempBuf[2];
	Send_WORK[61] = tempBuf[3];
	float_to_buf_tx (oh);
	Send_WORK[62] = tempBuf[0];
	Send_WORK[63] = tempBuf[1];
	Send_WORK[64] = tempBuf[2];
	Send_WORK[65] = tempBuf[3];
	Send_WORK[67] = (unsigned char)out_i_trowel;
	Send_WORK[66] = (unsigned char)(out_i_trowel >> 8);
	Send_WORK[69] = (unsigned char)out_i_revers;
	Send_WORK[68] = (unsigned char)(out_i_revers >> 8);
	Send_WORK[71] = (unsigned char)out_dump;
	Send_WORK[70] = (unsigned char)(out_dump >> 8);
	Send_WORK[73] = (unsigned char)out_period_answer;
	Send_WORK[72] = (unsigned char)(out_period_answer >> 8);
	float_to_buf_tx (out_i4mA);
	Send_WORK[74] = tempBuf[0];
	Send_WORK[75] = tempBuf[1];
	Send_WORK[76] = tempBuf[2];
	Send_WORK[77] = tempBuf[3];
	float_to_buf_tx (out_i20mA);
	Send_WORK[78] = tempBuf[0];
	Send_WORK[79] = tempBuf[1];
	Send_WORK[80] = tempBuf[2];
	Send_WORK[81] = tempBuf[3];
	Send_WORK[82] = out_P_factor;
	Send_WORK[83] = out_I_factor;
	Send_WORK[84] = out_D_factor;
	Send_WORK[86] = (unsigned char)out_dump_i;
	Send_WORK[85] = (unsigned char)(out_dump_i >> 8);
	float_to_buf_tx (out_referens);
	Send_WORK[87] = tempBuf[0];
	Send_WORK[88] = tempBuf[1];
	Send_WORK[89] = tempBuf[2];
	Send_WORK[90] = tempBuf[3];
	Send_WORK[91] = (Flags.udp_enable) ? 255 : 0;
}

void Change_Parameters (void)//внесение изменений в отправляемую посылку
{
	if(!Flags.ch_IP)
	{
		outDatIP[0] = DatIP[0];
		outDatIP[1] = DatIP[1];
		outDatIP[2] = DatIP[2];
		outDatIP[3] = DatIP[3];
	}
	else Flags.ch_IP = 0;
	
	if(!Flags.ch_Mask)
	{
		outDatMask[0] = DatMask[0];
		outDatMask[1] = DatMask[1];
		outDatMask[2] = DatMask[2];
		outDatMask[3] = DatMask[3];
	}
	else Flags.ch_Mask = 0;		
	
	if(!Flags.ch_DefGW)
	{
		outDatDefGW[0] = DatDefGW[0];
		outDatDefGW[1] = DatDefGW[1];
		outDatDefGW[2] = DatDefGW[2];
		outDatDefGW[3] = DatDefGW[3];
	}
	else Flags.ch_DefGW = 0;				

	if(!Flags.ch_PriDNS)
	{
		outDatPriDNS[0] = DatPriDNS[0];
		outDatPriDNS[0] = DatPriDNS[0];
		outDatPriDNS[0] = DatPriDNS[0];
		outDatPriDNS[0] = DatPriDNS[0];
	}
	else Flags.ch_PriDNS = 0;	

	if(!Flags.ch_SecDNS)
	{
		outDatSecDNS[0] = DatSecDNS[0];
		outDatSecDNS[1] = DatSecDNS[1];
		outDatSecDNS[2] = DatSecDNS[2];
		outDatSecDNS[3] = DatSecDNS[3];
	}
	else Flags.ch_SecDNS = 0;	

	if(!Flags.ch_a) {	oa = a;	}
	else Flags.ch_a = 0;	

	if(!Flags.ch_b) {	ob = b;	}
	else Flags.ch_b = 0;	
	
	if (!Flags.ch_c) {	oc = c;	}
	else Flags.ch_c = 0;	

	if(!Flags.ch_d) {	od = d;	}
	else Flags.ch_d = 0;		

	if(!Flags.ch_e) {	oe = e;	}
	else Flags.ch_e = 0;		

	if(!Flags.ch_n) {	on = n;	}
	else Flags.ch_n = 0;		

	if(!Flags.ch_f) {	of = f;	}
	else Flags.ch_f = 0;		

	if(!Flags.ch_g) {	og = g;	}
	else Flags.ch_g = 0;		

	if(!Flags.ch_h) {	oh = h;	}
	else Flags.ch_h = 0;			

	if(!Flags.ch_i_tr) {	out_i_trowel = i_trowel;	}
	else Flags.ch_i_tr = 0;

	if(!Flags.ch_i_rev) {	out_i_revers = i_revers;	}
	else Flags.ch_i_rev = 0;

	if(!Flags.ch_dump) {	out_dump = dump;	}
	else Flags.ch_dump = 0;

	if(!Flags.ch_period) {	out_period_answer = period_answer;	}
	else Flags.ch_period = 0;

	if(!Flags.ch_4mA) {	out_i4mA = i4mA;	}
	else Flags.ch_4mA = 0;	

	if(!Flags.ch_20mA) {	out_i20mA = i20mA;	}
	else Flags.ch_20mA = 0;	

	if(!Flags.ch_P) {	out_P_factor = P_factor;	}
	else Flags.ch_P = 0;	

	if(!Flags.ch_I) {	out_I_factor = I_factor;	}
	else Flags.ch_I = 0;

	if(!Flags.ch_D) {	out_D_factor = D_factor;	}
	else Flags.ch_D = 0;	

	if(!Flags.ch_dump_i) {	out_dump_i = dump_i;	}
	else Flags.ch_dump_i = 0;

	if(!Flags.ch_ref) {	out_referens = referens;	}
	else Flags.ch_ref = 0;

	OUTremUDPip[0] = remUDPip[0];
	OUTremUDPip[1] = remUDPip[1];
	OUTremUDPip[2] = remUDPip[2];
	OUTremUDPip[3] = remUDPip[3];
}

//Flags.ch_IP = Flags.ch_Mask = Flags.ch_DefGW = Flags.ch_PriDNS = Flags.ch_SecDNS = 1; // change panel configs
	
	//Flags.ch_P = Flags.ch_I = Flags.ch_D = 1; // change PID 
	//Flags.ch_dump_i = Flags.ch_Cb = Flags.ch_Output_I = 1; // change other PID params

	//Flags.ch_i_tr = Flags.ch_i_rev = Flags.ch_dump = 1; // change amperage params

	//Flags.ch_20mA = Flags.ch_4mA = 1; // change amperage	
	//Flags.ch_a = Flags.ch_b = Flags.ch_c = Flags.ch_d = Flags.ch_e = 
	//Flags.ch_f = Flags.ch_g = Flags.ch_h = Flags.ch_n = 1; // change params
	//Flags.ch_period = Flags.ch_ref = 1; // change other


