#include "qspi.h"
#include "global_var.h"
#include "rl_net_lib.h"
#include "Net_User.h"

extern  LOCALM localm[];
#define LocM   localm[NETIF_ETH]

void Read_settings (void);
void Write_settings (void);

extern union Converter converter;

#define CopyFloatInArray(i, variable) \
	converter.fdata = variable; \
	Send_WORK[i] = converter.cdata[0]; \
	Send_WORK[i + 1] = converter.cdata[1]; \
	Send_WORK[i + 2] = converter.cdata[2]; \
	Send_WORK[i + 3] = converter.cdata[3];

#define Copy2BytesArray(i) \
	converter.cdata[0] = Recive_WORK[i]; \
	converter.cdata[1] = Recive_WORK[i + 1]; \

#define Copy4BytesArray(i) \
	converter.cdata[0] = Recive_WORK[i]; \
	converter.cdata[1] = Recive_WORK[i + 1]; \
	converter.cdata[2] = Recive_WORK[i + 2]; \
	converter.cdata[3] = Recive_WORK[i + 3];

#define CopyNetDataFromBuf(x, i) \
	x[0] = Recive_WORK[i]; \
	x[1] = Recive_WORK[i + 1]; \
	x[2] = Recive_WORK[i + 2]; \
	x[3] = Recive_WORK[i + 3];

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

const unsigned char Crc8Table[256] = {
	0x00, 0x31, 0x62, 0x53, 0xC4, 0xF5, 0xA6, 0x97,
	0xB9, 0x88, 0xDB, 0xEA, 0x7D, 0x4C, 0x1F, 0x2E,
	0x43, 0x72, 0x21, 0x10, 0x87, 0xB6, 0xE5, 0xD4,
	0xFA, 0xCB, 0x98, 0xA9, 0x3E, 0x0F, 0x5C, 0x6D,
	0x86, 0xB7, 0xE4, 0xD5, 0x42, 0x73, 0x20, 0x11,
	0x3F, 0x0E, 0x5D, 0x6C, 0xFB, 0xCA, 0x99, 0xA8,
	0xC5, 0xF4, 0xA7, 0x96, 0x01, 0x30, 0x63, 0x52,
	0x7C, 0x4D, 0x1E, 0x2F, 0xB8, 0x89, 0xDA, 0xEB,
	0x3D, 0x0C, 0x5F, 0x6E, 0xF9, 0xC8, 0x9B, 0xAA,
	0x84, 0xB5, 0xE6, 0xD7, 0x40, 0x71, 0x22, 0x13,
	0x7E, 0x4F, 0x1C, 0x2D, 0xBA, 0x8B, 0xD8, 0xE9,
	0xC7, 0xF6, 0xA5, 0x94, 0x03, 0x32, 0x61, 0x50,
	0xBB, 0x8A, 0xD9, 0xE8, 0x7F, 0x4E, 0x1D, 0x2C,
	0x02, 0x33, 0x60, 0x51, 0xC6, 0xF7, 0xA4, 0x95,
	0xF8, 0xC9, 0x9A, 0xAB, 0x3C, 0x0D, 0x5E, 0x6F,
	0x41, 0x70, 0x23, 0x12, 0x85, 0xB4, 0xE7, 0xD6,
	0x7A, 0x4B, 0x18, 0x29, 0xBE, 0x8F, 0xDC, 0xED,
	0xC3, 0xF2, 0xA1, 0x90, 0x07, 0x36, 0x65, 0x54,
	0x39, 0x08, 0x5B, 0x6A, 0xFD, 0xCC, 0x9F, 0xAE,
	0x80, 0xB1, 0xE2, 0xD3, 0x44, 0x75, 0x26, 0x17,
	0xFC, 0xCD, 0x9E, 0xAF, 0x38, 0x09, 0x5A, 0x6B,
	0x45, 0x74, 0x27, 0x16, 0x81, 0xB0, 0xE3, 0xD2,
	0xBF, 0x8E, 0xDD, 0xEC, 0x7B, 0x4A, 0x19, 0x28,
	0x06, 0x37, 0x64, 0x55, 0xC2, 0xF3, 0xA0, 0x91,
	0x47, 0x76, 0x25, 0x14, 0x83, 0xB2, 0xE1, 0xD0,
	0xFE, 0xCF, 0x9C, 0xAD, 0x3A, 0x0B, 0x58, 0x69,
	0x04, 0x35, 0x66, 0x57, 0xC0, 0xF1, 0xA2, 0x93,
	0xBD, 0x8C, 0xDF, 0xEE, 0x79, 0x48, 0x1B, 0x2A,
	0xC1, 0xF0, 0xA3, 0x92, 0x05, 0x34, 0x67, 0x56,
	0x78, 0x49, 0x1A, 0x2B, 0xBC, 0x8D, 0xDE, 0xEF,
	0x82, 0xB3, 0xE0, 0xD1, 0x46, 0x77, 0x24, 0x15,
	0x3B, 0x0A, 0x59, 0x68, 0xFF, 0xCE, 0x9D, 0xAC
};

unsigned char Crc8(unsigned char *buf, unsigned char len)
{
	unsigned char crc = 0xFF;
	while (len--) 
		crc = Crc8Table[crc ^ *buf++];
	return crc;
}

void Parsing_package_WORK (void)//разбор посылки от датчика
{ 
	Copy4BytesArray(0);
	sum = converter.idata;
	Copy4BytesArray(4);
  DumpSum = converter.idata;
	Copy2BytesArray(8);
	i_trowel = converter.sdata;
	Copy2BytesArray(10);
	i_revers = converter.sdata;
	Copy2BytesArray(12);
	dump = converter.sdata;
	Copy2BytesArray(14);
	period_answer = converter.sdata;

	Copy4BytesArray(16);
  a = converter.fdata;
	Copy4BytesArray(20);
  b = converter.fdata;
	Copy4BytesArray(24);
  c = converter.fdata;
	Copy4BytesArray(28);
  d = converter.fdata;
	Copy4BytesArray(32);
  e = converter.fdata;
	Copy4BytesArray(36);
  n = converter.fdata;
	Copy4BytesArray(40);
  f = converter.fdata;
	Copy4BytesArray(44);
  g = converter.fdata;
	Copy4BytesArray(48);
  h = converter.fdata;

	CopyNetDataFromBuf(DatIP, 52);
  CopyNetDataFromBuf(DatMask, 56);
	CopyNetDataFromBuf(DatDefGW, 60);
	CopyNetDataFromBuf(DatPriDNS, 64);
	CopyNetDataFromBuf(DatSecDNS, 68);

	Copy4BytesArray(72);
  i4mA = converter.fdata;
	Copy4BytesArray(76);
  i20mA = converter.fdata;
	Copy4BytesArray(80);
  Cb = converter.fdata;
	Copy4BytesArray(84);
  Ci = converter.fdata;
	
  dump_i = (Recive_WORK[88] << 8) + Recive_WORK[89];
	Copy4BytesArray(90);
  referens = converter.fdata;
	Copy4BytesArray(94);
  Output_I = converter.fdata;

	workMode = Recive_WORK[98];
  Copy4BytesArray(99);
  damper = converter.fdata;
	Copy4BytesArray(103);
  offset = converter.fdata;
	Copy4BytesArray(107);
  gain = converter.fdata;
	
	pid_period = (Recive_WORK[111] << 8) + Recive_WORK[112];
	Copy4BytesArray(113);
  P_factor = converter.fdata;
	Copy4BytesArray(117);
  I_factor = converter.fdata;
	Copy4BytesArray(121);
  D_factor = converter.fdata;
	
	Copy4BytesArray(125);
  km = converter.fdata;

	air = ((Recive_WORK[129] << 24) + (Recive_WORK[130] << 16) + (Recive_WORK[131] << 8) + Recive_WORK[132]);
	water = ((Recive_WORK[133] << 24) + (Recive_WORK[134] << 16) + (Recive_WORK[135] << 8) + Recive_WORK[136]); 
}

void Form_package_WORK(void)//сборка посылки в датчик (копирует в буфер из outПеременных)
{
	int i;
	//if (sendParam & 0x08)
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
	}
  /*
	else
	{
		for(i = 0; i < 20; i++)
			Send_WORK[i] = 0;
	}

	if (sendParam & 0x04)
	{
	*/
		CopyFloatInArray(20, oa);
		CopyFloatInArray(24, ob);
		CopyFloatInArray(28, oc);
		CopyFloatInArray(32, od);
		CopyFloatInArray(36, oe);
		CopyFloatInArray(40, on);
		CopyFloatInArray(44, of);
		CopyFloatInArray(48, og);
		CopyFloatInArray(52, oh);	
	/*
	}
	else
	{
		for(i = 20; i < 56; i++)
			Send_WORK[i] = 0;
	}

	if (sendParam & 0x02)
	{
	*/
		Send_WORK[56] = (unsigned char)(out_dump_i >> 8);
		Send_WORK[57] = (unsigned char)out_dump_i;
		CopyFloatInArray(58, out_referens);
		CopyFloatInArray(62, out_i4mA);
		CopyFloatInArray(66, out_i20mA);
		Send_WORK[70] = (unsigned char)(out_pid_period >> 8);
		Send_WORK[71] = (unsigned char)out_pid_period;
		CopyFloatInArray(72, out_P_factor);
		CopyFloatInArray(76, out_I_factor);
		CopyFloatInArray(80, out_D_factor);
	/*
  }
	else
	{
		for(i = 56; i < 84; i++)
			Send_WORK[i] = 0;
	}

	if (sendParam & 0x01)
	{
	*/
		Send_WORK[84] = (unsigned char)(out_i_trowel >> 8);
		Send_WORK[85] = (unsigned char)out_i_trowel;
		Send_WORK[86] = (unsigned char)(out_i_revers >> 8);
		Send_WORK[87] = (unsigned char)out_i_revers;
		Send_WORK[88] = (unsigned char)(out_dump >> 8);
		Send_WORK[89] = (unsigned char)out_dump;
		Send_WORK[90] = (unsigned char)(out_period_answer >> 8);
		Send_WORK[91] = (unsigned char)out_period_answer;

		Send_WORK[92] = out_workMode;

		CopyFloatInArray(93, out_damper);
		CopyFloatInArray(97, out_offset);
		CopyFloatInArray(101, out_gain);

		Send_WORK[105] = (unsigned char)(out_air >> 24);
		Send_WORK[106] = (unsigned char)(out_air >> 16);
		Send_WORK[107] = (unsigned char)(out_air >> 8);
		Send_WORK[108] = (unsigned char)out_air;
		
		Send_WORK[109] = (unsigned char)(out_water >> 24);
		Send_WORK[110] = (unsigned char)(out_water >> 16);
		Send_WORK[111] = (unsigned char)(out_water >> 8);
	  Send_WORK[112] = (unsigned char)out_water;
	
	for(i = 113; i < 253; i++)
		Send_WORK[i] = 0;

	Send_WORK[253] = 1;
	Send_WORK[254] = sendParam;
	Send_WORK[255] = Crc8((unsigned char*)Send_WORK, 254);
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
	
	if (!Flags.ch_DefGW)
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
		outDatPriDNS[1] = DatPriDNS[1];
		outDatPriDNS[2] = DatPriDNS[2];
		outDatPriDNS[3] = DatPriDNS[3];
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

	if(!Flags.ch_damper) {	out_damper = damper;	}
	else Flags.ch_damper = 0;

	if(!Flags.ch_offset) { out_offset = offset; }
	else Flags.ch_offset = 0;

	if(!Flags.ch_gain) { out_gain = gain; }
	else Flags.ch_gain = 0;

	if(!Flags.ch_pid_period) { out_pid_period = pid_period; }
	else Flags.ch_pid_period = 0;

	if(!Flags.ch_air) { out_air = air; }
	else Flags.ch_air = 0;

	if(!Flags.ch_water) { out_water = water; }
	else Flags.ch_water = 0;

	OUTremUDPip[0] = remUDPip[0];
	OUTremUDPip[1] = remUDPip[1];
	OUTremUDPip[2] = remUDPip[2];
	OUTremUDPip[3] = remUDPip[3];
}

/*
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

// old
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
	
	i_trowel = Recive_WORK[20] + (Recive_WORK[21] << 8);
	i_revers = Recive_WORK[22] + (Recive_WORK[23] << 8);	
	dump = Recive_WORK[24] + (Recive_WORK[25] << 8);	
	period_answer = Recive_WORK[26] + (Recive_WORK[27] << 8);
	
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
	
	// 3 пустых бита (тут были ПИД)

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
	
	workMode = Recive_WORK[144];

	tempBuf[0] = Recive_WORK[145];
	tempBuf[1] = Recive_WORK[146];
	tempBuf[2] = Recive_WORK[147];
	tempBuf[3] = Recive_WORK[148];
  damper = buf_tx_to_float();
	tempBuf[0] = Recive_WORK[149];
	tempBuf[1] = Recive_WORK[150];
	tempBuf[2] = Recive_WORK[151];
	tempBuf[3] = Recive_WORK[152];
  offset = buf_tx_to_float();
	tempBuf[0] = Recive_WORK[153];
	tempBuf[1] = Recive_WORK[154];
	tempBuf[2] = Recive_WORK[155];
	tempBuf[3] = Recive_WORK[156];
  gain = buf_tx_to_float();

	// новое положение вещественного ПИДа
  pid_period = (Recive_WORK[157] << 8) + Recive_WORK[158];

	tempBuf[0] = Recive_WORK[159];
	tempBuf[1] = Recive_WORK[160];
	tempBuf[2] = Recive_WORK[161];
	tempBuf[3] = Recive_WORK[162];
  P_factor = buf_tx_to_float();
	
	tempBuf[0] = Recive_WORK[163];
	tempBuf[1] = Recive_WORK[164];
	tempBuf[2] = Recive_WORK[165];
	tempBuf[3] = Recive_WORK[166];
	I_factor = buf_tx_to_float();

  tempBuf[0] = Recive_WORK[167];
	tempBuf[1] = Recive_WORK[168];
	tempBuf[2] = Recive_WORK[169];
	tempBuf[3] = Recive_WORK[170];
	D_factor = buf_tx_to_float();


	tempBuf[0] = Recive_WORK[171];
	tempBuf[1] = Recive_WORK[172];
	tempBuf[2] = Recive_WORK[173];
	tempBuf[3] = Recive_WORK[174];
	km = buf_tx_to_float();

	air = (Recive_WORK[178] + 
		(Recive_WORK[177] << 8) +
		(Recive_WORK[176] << 16) + 
		(Recive_WORK[175] << 24));

	water = (Recive_WORK[182] + 
		(Recive_WORK[181] << 8) +
		(Recive_WORK[180] << 16) + 
		(Recive_WORK[179] << 24));
}

//old
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

	// Поддержка старых версий
	//Send_WORK[82] = (uint8_t)out_P_factor;
	//Send_WORK[83] = (uint8_t)out_I_factor;
	//Send_WORK[84] = (uint8_t)out_D_factor;
	
	Send_WORK[86] = (unsigned char)out_dump_i;
	Send_WORK[85] = (unsigned char)(out_dump_i >> 8);
	float_to_buf_tx (out_referens);
	Send_WORK[87] = tempBuf[0];
	Send_WORK[88] = tempBuf[1];
	Send_WORK[89] = tempBuf[2];
	Send_WORK[90] = tempBuf[3];
	Send_WORK[91] = (Flags.udp_enable) ? 255 : 0;
	float_to_buf_tx(out_damper);
	Send_WORK[92] = tempBuf[0];
	Send_WORK[93] = tempBuf[1];
	Send_WORK[94] = tempBuf[2];
	Send_WORK[95] = tempBuf[3];

	Send_WORK[96] = out_workMode;

	float_to_buf_tx (out_offset);
	Send_WORK[97] = tempBuf[0];
	Send_WORK[98] = tempBuf[1];
	Send_WORK[99] = tempBuf[2];
	Send_WORK[100] = tempBuf[3];
	float_to_buf_tx (out_gain);
	Send_WORK[101] = tempBuf[0];
	Send_WORK[102] = tempBuf[1];
	Send_WORK[103] = tempBuf[2];
	Send_WORK[104] = tempBuf[3];

  Send_WORK[105] = (unsigned char)(out_pid_period >> 8);
	Send_WORK[106] = out_pid_period;

	float_to_buf_tx (out_P_factor);
	Send_WORK[107] = tempBuf[0];
	Send_WORK[108] = tempBuf[1];
	Send_WORK[109] = tempBuf[2];
	Send_WORK[110] = tempBuf[3];

	float_to_buf_tx (out_I_factor);
	Send_WORK[111] = tempBuf[0];
	Send_WORK[112] = tempBuf[1];
	Send_WORK[113] = tempBuf[2];
	Send_WORK[114] = tempBuf[3];

	float_to_buf_tx (out_D_factor);
	Send_WORK[115] = tempBuf[0];
	Send_WORK[116] = tempBuf[1];
	Send_WORK[117] = tempBuf[2];
	Send_WORK[118] = tempBuf[3];

	
  Send_WORK[122] = (unsigned char)out_air;
	Send_WORK[121] = (unsigned char)(out_air >> 8);
	Send_WORK[120] = (unsigned char)(out_air >> 16);
	Send_WORK[119] = (unsigned char)(out_air >> 24);

	Send_WORK[126] = (unsigned char)out_water;
	Send_WORK[125] = (unsigned char)(out_water >> 8);
	Send_WORK[124] = (unsigned char)(out_water >> 16);
	Send_WORK[123] = (unsigned char)(out_water >> 24);
	

	for(int i = 127; i < 253; i++)
		Send_WORK[i] = 0;

	Send_WORK[253] = 1;
	Send_WORK[254] = sendParam;
}
*/
