#include "stm32f7xx_hal.h"
#include "serverPart.h"
#include "global_var.h"
#include <string.h>
#include "Net_User.h"
#include "settings.h"
#include "timer7.h"

int32_t tcp_soc_SERVER, clientsCount = 0;  

extern union Converter converter;

int server_init(void)
{
	InitTimer7();

	tcp_soc_SERVER = tcp_get_socket(TCP_TYPE_SERVER, 0, 10, tcp_callback_SERVER);	
	if (tcp_soc_SERVER < 0)
		return -1;	
	tcp_listen (tcp_soc_SERVER, 4000);
	return 0;
}

void Form_package_SERVER(void) // копирует принятый пакет данных, добавляет данные с панели и собирает это в буфер
{
	memcpy(Send_SERVER, Recive_WORK, 256);

	#define CopyNetDataToBuf(x, i) \
		Send_SERVER[i] = x[0]; \
		Send_SERVER[i + 1] = x[1]; \
		Send_SERVER[i + 2] = x[2]; \
		Send_SERVER[i + 3] = x[3];
	
	//converter.fdata = damper;
	//CopyNetDataToBuf(converter.cdata, 146);

	CopyNetDataToBuf(LocM.IpAddr, 170);
  CopyNetDataToBuf(LocM.NetMask, 174);
	CopyNetDataToBuf(LocM.DefGW, 178);
	CopyNetDataToBuf(LocM.PriDNS, 182);
	CopyNetDataToBuf(LocM.SecDNS, 186);
	CopyNetDataToBuf(rem_ip, 190);
	// есть ли коннект
	Send_SERVER[194] = (soc_state != 2) ? 0 : 255;
}

void Parsing_package_SERVER(void)
{
	#define WriteInConverter4Bytes(s/*source*/, ind) \
		converter.cdata[0] = s[ind]; \
    converter.cdata[1] = s[ind + 1]; \
		converter.cdata[2] = s[ind + 2]; \
		converter.cdata[3] = s[ind + 3];
	
	// если меняются сетевые настройки - перезапуск панели
	{
		// сетевые настройки панели
		int i = 0;
		LOCALM newLocM;
		uint8_t newRemIp[4];

		while (i < 4)
		{
			newLocM.IpAddr[i] = Recive_SERVER[150 + i];
			newLocM.NetMask[i] = Recive_SERVER[154 + i];
			newLocM.DefGW[i] = Recive_SERVER[158 + i];
			newLocM.PriDNS[i] = Recive_SERVER[162 + i];
			newLocM.SecDNS[i] = Recive_SERVER[166 + i];
			newRemIp[i] = Recive_SERVER[170 + i];
			++i;
		}

		i = 0;
		while (i < 4)
		{
			if (newLocM.IpAddr[i] != LocM.IpAddr[i]
					|| newLocM.NetMask[i] != LocM.NetMask[i]
					|| newLocM.DefGW[i] != LocM.DefGW[i]
					|| newLocM.PriDNS[i] != LocM.PriDNS[i]
					|| newLocM.SecDNS[i] != LocM.SecDNS[i]
					|| newRemIp[i] != rem_ip[i])
			{
				// запись настроек и затем перезапуск
				i = 0;
				while (i < 4)
				{
					LocM.IpAddr[i] = newLocM.IpAddr[i];
					LocM.NetMask[i] = newLocM.NetMask[i];
					LocM.DefGW[i] = newLocM.DefGW[i];
					LocM.PriDNS[i] = newLocM.PriDNS[i];
					LocM.SecDNS[i] = newLocM.SecDNS[i];
					rem_ip[i] = newRemIp[i];
					++i;
				}
				
				Write_settings();
				SCB->AIRCR  = (uint32_t)((0x5FAUL << SCB_AIRCR_VECTKEY_Pos)    |
				 (SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) | SCB_AIRCR_SYSRESETREQ_Msk);
			}
			++i;
		}
	}
	
	outDatIP[0] = Recive_SERVER[64];
	outDatIP[1] = Recive_SERVER[65];	
	outDatIP[2] = Recive_SERVER[66];
	outDatIP[3] = Recive_SERVER[67];	
	outDatMask[0] = Recive_SERVER[68];
	outDatMask[1] = Recive_SERVER[69];
	outDatMask[2] = Recive_SERVER[70];
	outDatMask[3] = Recive_SERVER[71];
	outDatDefGW[0] = Recive_SERVER[72];
	outDatDefGW[1] = Recive_SERVER[73];
	outDatDefGW[2] = Recive_SERVER[74];
	outDatDefGW[3] = Recive_SERVER[75];
	outDatPriDNS[0] = Recive_SERVER[76];
	outDatPriDNS[1] = Recive_SERVER[77];
	outDatPriDNS[2] = Recive_SERVER[78];
	outDatPriDNS[3] = Recive_SERVER[79];	
	outDatSecDNS[0] = Recive_SERVER[80];
	outDatSecDNS[1] = Recive_SERVER[81];
	outDatSecDNS[2] = Recive_SERVER[82];
	outDatSecDNS[3] = Recive_SERVER[83];
	
	PortTCP = (Recive_SERVER[85] << 8) + Recive_SERVER[84];
	PortUDP = (Recive_SERVER[87] << 8) + Recive_SERVER[86];
	RemPortUDP = (Recive_SERVER[89] << 8) + Recive_SERVER[88];
	OUTremUDPip[0] = Recive_SERVER[90];
	OUTremUDPip[1] = Recive_SERVER[91];
	OUTremUDPip[2] = Recive_SERVER[92];
	OUTremUDPip[3] = Recive_SERVER[93];

	WriteInConverter4Bytes(Recive_SERVER, 28);
	oa = converter.fdata;
  WriteInConverter4Bytes(Recive_SERVER, 32);
	ob = converter.fdata;
	WriteInConverter4Bytes(Recive_SERVER, 36);
	oc = converter.fdata;
	WriteInConverter4Bytes(Recive_SERVER, 40);
	od = converter.fdata;
	WriteInConverter4Bytes(Recive_SERVER, 44);
	oe = converter.fdata;
	WriteInConverter4Bytes(Recive_SERVER, 48);
	on = converter.fdata;
	WriteInConverter4Bytes(Recive_SERVER, 52);
	of = converter.fdata;
	WriteInConverter4Bytes(Recive_SERVER, 56);
	og = converter.fdata;
	WriteInConverter4Bytes(Recive_SERVER, 60);
	oh = converter.fdata;
	
	out_i_trowel = (Recive_SERVER[21] << 8) + Recive_SERVER[20];
	out_i_revers = (Recive_SERVER[23] << 8) + Recive_SERVER[22];
	
	out_dump = (Recive_SERVER[25] << 8) + Recive_SERVER[24];	
	out_period_answer = (Recive_SERVER[27] << 8) + Recive_SERVER[26];

  WriteInConverter4Bytes(Recive_SERVER, 94);
	out_i4mA = converter.fdata;
	WriteInConverter4Bytes(Recive_SERVER, 98);
	out_i20mA = converter.fdata;

	//out_P_factor = Recive_SERVER[127];
	//out_I_factor = Recive_SERVER[128];
	//out_D_factor = Recive_SERVER[129];

	out_dump_i = (Recive_SERVER[130] << 8) + Recive_SERVER[131];

	WriteInConverter4Bytes(Recive_SERVER, 136);
	out_referens = converter.fdata;

	Flags.udp_enable = (Recive_SERVER[106] == 255) ? 1 : 0;

	Flags.ch_IP = Flags.ch_Mask = Flags.ch_DefGW = Flags.ch_PriDNS = Flags.ch_SecDNS = 1; // change panel configs
	Flags.ch_P = Flags.ch_I = Flags.ch_D = 1; // change PID 
	Flags.ch_dump_i = Flags.ch_Cb = Flags.ch_Output_I = 1; // change other PID params
	Flags.ch_i_tr = Flags.ch_i_rev = Flags.ch_dump = 1; // change amperage params
	Flags.ch_20mA = Flags.ch_4mA = 1; // change amperage	
	Flags.ch_a = Flags.ch_b = Flags.ch_c = Flags.ch_d = Flags.ch_e = 
	Flags.ch_f = Flags.ch_g = Flags.ch_h = Flags.ch_n = 1; // change params
	Flags.ch_period = Flags.ch_ref = 1; // change other
}

unsigned int tcp_callback_SERVER (int32_t soc, tcpEvent event, const uint8_t *buf, uint32_t len) 
{
  switch (event) 
	{
		case tcpEventData:
		{
			int n = (len > 0xff) ? 0xff : len;
			for(int i = 0; i < n; ++i)
			{
				Recive_SERVER[i] = *buf;
				++buf;
			}
			Flags.incoming_server = 1;
			tcp_reset_window (soc);
			__nop();
			break;
		}

    case tcpEventConnect:
			if (clientsCount == 0)
				Timer7Start();
			++clientsCount;
			__nop();
      return 1;
		
		
		case tcpEventAbort: 			 
		case tcpEventClosed: 			 		
			--clientsCount;
			if (clientsCount == 0)
				Timer7Stop();
			__nop();
			break;

		case tcpEventEstablished:  // Socket is connected to remote peer. 	
		case tcpEventACK: // Our sent data has been acknowledged by remote peer 
			__nop();
			break;
  }
  return 0;
}

int server_working (void)
{
	uint8_t* sendbuf;
  int32_t max, maxlen;

	if (clientsCount > 0)
	{		
		if (tcp_check_send(tcp_soc_SERVER) && Flags.answer_server) 
		{
			max = tcp_max_data_size (tcp_soc_SERVER);
			maxlen = (max > 0xff) ? 0xff : max;
			Form_package_SERVER();
			sendbuf = tcp_get_buf(maxlen);
			memcpy(sendbuf, Send_SERVER, maxlen);
			tcp_send(tcp_soc_SERVER, sendbuf, maxlen);
			Flags.answer_server = 0;
		}
	}
	return 0;
}
