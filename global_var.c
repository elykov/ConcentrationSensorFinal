#include "global_var.h"
#include "GUILogic.h"

// Сетевые переменные

	unsigned char Recive_WORK[256];
	unsigned char Send_WORK[256];
	unsigned char maxlen_work;
	unsigned short PortTCP = 4000;

// Входящие сетевые параметры: 

	unsigned char DatIP[4];
	unsigned char DatMask[4];
	unsigned char DatDefGW[4];
	unsigned char DatPriDNS[4];
	unsigned char DatSecDNS[4];

// Исходящие сетевые параметры

	unsigned char outDatIP[4];
	unsigned char outDatMask[4];
	unsigned char outDatDefGW[4];
	unsigned char outDatPriDNS[4];
	unsigned char outDatSecDNS[4];

// Входящие переменные
	
	// ПИД

	float Cb; // Текущая концентрация
	float Output_I = 0; // Выходной ток
	float damper  = 0; // Заслонка
	
	float P_factor, I_factor, D_factor;

	float referens  = 0;
	unsigned short dump_i = 0;
	unsigned short pid_period = 0;

	// Параметры процесса
	
	unsigned short i_trowel = 0; // Ток хода  
	unsigned short i_revers = 0; // Ток реверса
	unsigned short period_answer = 0; // Период ответа
	unsigned short dump = 0; // Буфер
	float offset = 0, gain = 0;
	uint32_t water = 0, air = 0;
	float km = 0;

	// Параметры формулы

	float a = 0, b = 0, c = 0;
	float d = 0, e = 0, n = 0;
	float f = 0, g = 0, h = 0;

	// Остальные

	float damper_manual = -3.f;
	unsigned int up = 0, down = 0;
	unsigned int sum = 0, DumpSum = 0;

// Исходящие переменные

	// ПИД
		
	float out_referens  = 0;
	unsigned short out_pid_period = 0;
	unsigned short out_dump_i = 0;

	float out_P_factor, out_I_factor, out_D_factor;
	
	// Параметры процесса

	unsigned short out_i_trowel = 0;
	unsigned short out_i_revers = 0;
	unsigned short out_period_answer = 0;
	unsigned short out_dump = 0;
	float out_offset = 0, out_gain = 0;
	uint32_t out_water = 0, out_air = 0;

	// Параметры формулы

	float oa = 0, ob = 0, oc = 0, od = 0, oe = 0, on = 0, of = 0, og = 0, oh = 0;

	// Остальные переменные

	float out_damper = 0;
	uint8_t workMode = 1;
	uint8_t out_workMode;
	uint8_t sendParam = 0;


	struct TFlags Flags;
	struct GUILogic logic;             
	struct KeyBoard keyBoard = {0};


// Нужны ли?
	
	unsigned short out;
	unsigned short i_out = 0;
	float C, Ci;
	float i4mA = 0;
	float i20mA = 0;
	float out_i4mA = 0;
	float out_i20mA = 0;

// Старые переменные

//float p_term = 0, i_term = 0, d_term = 0;


