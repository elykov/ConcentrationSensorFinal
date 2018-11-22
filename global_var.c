#include "global_var.h"
#include "GUILogic.h"

struct TFlags Flags;

unsigned short PortTCP = 4000;
unsigned short TechPortTCP = 4001;
unsigned short PortUDP = 3000;
unsigned short RemPortUDP = 4000;
unsigned char  remUDPip[4] = {192,168,99,101};
unsigned char  OUTremUDPip[4] = {192,168,99,101};
unsigned char DatIP[4];
unsigned char DatMask[4];
unsigned char DatDefGW[4];
unsigned char DatPriDNS[4];
unsigned char DatSecDNS[4];
unsigned char outDatIP[4];
unsigned char outDatMask[4];
unsigned char outDatDefGW[4];
unsigned char outDatPriDNS[4];
unsigned char outDatSecDNS[4];

unsigned char Send_WORK[256];
unsigned char Recive_WORK[256];
unsigned char Send_TECH[256];
unsigned char Recive_TECH[256];
unsigned char Send_PLC[256];
unsigned char Recive_PLC[256];
unsigned char Send_SERVER[256];
unsigned char Recive_SERVER[256];
unsigned char maxlen_work;

unsigned char tempBuf[4];
float P_factor, I_factor, D_factor;
float out_P_factor, out_I_factor, out_D_factor;

unsigned short dump = 0;
unsigned short out_dump = 0;
unsigned short period_answer = 0;
unsigned short out_period_answer = 0;
unsigned short i_trowel = 0;   
unsigned short i_revers = 500;
unsigned short out_i_trowel = 0;
unsigned short out_i_revers = 0;
unsigned short i_out = 0;
unsigned short dump_i = 0;
unsigned short out_dump_i = 0;
unsigned short out = 0;

unsigned int up;
unsigned int down;
unsigned int sum;
unsigned int DumpSum;

float a = -1.972;
float b = 0;
float c = 0.00000000529;
float d = 0;
float e = 0;
float n = 0;
float f = -0.001;
float g = 0;
float h = 0;
float C;
float i4mA = 0;
float i20mA = 0;
float out_i4mA = 0;
float out_i20mA = 0;
float Cb;
float Ci;
float p_term = 0, i_term = 0, d_term = 0;
float Output_I = 0, referens  = 0;
float out_referens  = 0;
float oa = 0;
float ob = 0;
float oc = 0;
float od = 0;
float oe = 0;
float on = 0;
float of = 0;
float og = 0;
float oh = 0;

// ==============================================

struct GUILogic logic;             
struct KeyBoard keyBoard = {0};
float damper  = 0, out_damper = 0;
uint8_t workMode = 1;
float offset = 0, gain = 0;
float out_offset = 0, out_gain = 0;
uint8_t out_workMode;
float damper_manual = 50.f;
unsigned short pid_period = 0, out_pid_period = 0;


uint8_t sendParam = 0;

