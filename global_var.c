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
unsigned char maxlen_work;

unsigned char tempBuf[4];
unsigned char  P_factor, I_factor, D_factor;
unsigned char  out_P_factor, out_I_factor, out_D_factor;

unsigned short dump = 200;
unsigned short out_dump = 200;
unsigned short period_answer = 200;
unsigned short out_period_answer = 200;
unsigned short i_trowel = 0;
unsigned short i_revers = 500;
unsigned short out_i_trowel = 0;
unsigned short out_i_revers = 500;
unsigned short i_out = 0;
unsigned short dump_i = 100;
unsigned short out_dump_i = 100;
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
float i4mA = 1.0;
float i20mA = 8.0;
float out_i4mA = 1.0;
float out_i20mA = 8.0;
float Cb;
float Ci;
float p_term = 0, i_term = 0, d_term = 0;
float Output_I, referens;
float out_referens;
float oa = -1.972;
float ob = 0;
float oc = 0.00000000529;
float od = 0;
float oe = 0;
float on = 0;
float of = -0.001;
float og = 0;
float oh = 0;

// ==============================================

struct GUILogic logic;             
struct KeyBoard keyBoard = {0};

float out_Cb, out_Output_I;

