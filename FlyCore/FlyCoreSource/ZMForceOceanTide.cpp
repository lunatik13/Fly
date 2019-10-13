//---------------------------------------------------------------------------
// ZMForceOceanTide.cpp
// �. ��������� 03.08.2018
// ������� ������� �������� ������������� GPZ �� ���� ������������ ��������
//---------------------------------------------------------------------------
#include <stdafx.h>
#pragma   hdrstop

//---------------------------------------------------------------------------
#include <FlyMMath.h>	// �������������� �������
#include <FlyTime.h>	// �� ��� �����

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ZMForceOceanTide.h>

//---------------------------------------------------------------------------
void Doodson_arg(double Time_tide, double *DARG);
void Step2_GP (double *DARG, double *CD, int n, int m);
void TETf(double *tetf,char *ni,double *DARG, char n);

//---------------------------------------------------------------------------
//TOPEX/POSEIDON altimetr data
void O_Tide_GPZ( double *DARG, double *dCD, char n, char m )
{                                     // Cnm,Dnm-�� ����.
	double k_n[5] = {-0.3075, -0.195, -0.132, -0.1032, -0.0892};
	double Arg,CArg,SArg,F_n;
	//4*3.14159265*6.67259e-11*1025/9.780327;4*pi*G*pw/g
	static double F=8.78769133e-10;
	int    i,j,k;
	// name Doodson
	static char  cf[12][8] =  //[117][8]-��� Shwiderski
    {{ 0, 0, 2, 0, 0, 0, 0,2},//  57.555SSA   2  2   1  2
     { 0, 0, 1, 0, 0,-1, 0,2},//  56.554SA    2  2   2  4
     { 0, 1, 0,-1, 0, 0, 0,2},//  65.455MM    2  2   3  6
     { 0, 2, 0, 0, 0, 0, 0,2},//  75.555MF    2  2   4  8
     { 1,-2, 0, 1, 0, 0, 3,4},// 135.655Q1    4  4   5 12
     { 1,-1, 0, 0, 0, 0, 3,4},// 145.555O1    4  4   6 16
     { 1, 1,-2, 0, 0, 0, 3,4},// 163.555P1    4  4   7 20
     { 1, 1, 0, 0, 0, 0, 1,5},// 165.555K1    5  5   8 25
     { 2,-1, 0, 1, 0, 0, 0,4},// 245.655N2    4  4   9 29
     { 2, 0, 0, 0, 0, 0, 0,4},// 255.555M2    4  4  10 33
     { 2, 2,-2, 0, 0, 0, 0,5},// 273.555S2    5  5  11 38
     { 2, 2,-3, 0, 0, 1, 0,1},// 272.556T2    1  1  12 39
     };

	static struct SCS {
		char n; 
		char m; 
		double CS[4];
	} TEG_2B[]= {
	{2,0,{-0.7828350E+00,-0.277695E+00,-0.7828350000E+00,-0.277695000E+00}},//057.555SSA
	{3,0,{ 0.5310600E+00,-0.313475E+00, 0.5310600000E+00,-0.313475000E+00}},//057.555SSA
	{2,0,{ 0.5091200E+00, 0.103785E+01, 0.5091200000E+00, 0.103785000E+01}},//056.554SA
	{3,0,{-0.6464450E+00,-0.478655E+00,-0.6464450000E+00,-0.478655000E+00}},//056.554SA
	{2,0,{-0.5350200E+00,-0.981550E-01,-0.5350200000E+00,-0.981550000E-01}},//065.455MM
	{3,0,{ 0.2300250E+00,-0.117040E+00, 0.2300250000E+00,-0.117040000E+00}},//065.455MM
	{2,0,{-0.1057595E+01,-0.493480E+00,-0.1057595000E+01,-0.493480000E+00}},//075.555MF
	{3,0,{ 0.1509000E+00,-0.152505E+00, 0.1509000000E+00,-0.152505000E+00}},//075.555MF
	{2,1,{-0.4263600E+00,-0.470840E+00, 0.2383473500E+00, 0.533558500E-01}},//135.655Q1
	{3,1,{ 0.8717000E-01, 0.212540E+00,-0.2066501500E+00,-0.265719000E-01}},//135.655Q1
	{4,1,{-0.4446000E-01,-0.428480E+00,-0.9773183250E-01,-0.375624000E-01}},//135.655Q1
	{5,1,{ 0.1967100E+00, 0.179550E+00, 0.7787335050E-01, 0.347965500E-01}},//135.655Q1
	{2,1,{-0.2321360E+01,-0.208424E+01, 0.9214375000E+00, 0.531454000E+00}},//145.555O1
	{3,1,{-0.3624000E+00, 0.144849E+01,-0.9751025000E+00,-0.456573300E+00}},//145.555O1
	{4,1,{-0.3998800E+00,-0.203722E+01,-0.4842639000E+00,-0.319496000E+00}},//145.555O1
	{5,1,{ 0.6139800E+00, 0.942820E+00, 0.3194677365E+00, 0.205046000E+00}},//145.555O1
	{2,1,{-0.6698200E+00,-0.587260E+00, 0.3053339000E+00, 0.302544350E+00}},//163.555P1
	{3,1,{-0.4028400E+00, 0.374200E-01,-0.3312909500E+00,-0.358495600E+00}},//163.555P1
	{4,1,{ 0.1465200E+00,-0.622750E+00,-0.1944608100E+00,-0.174925600E+00}},//163.555P1
	{5,1,{-0.2244000E-01, 0.425160E+00, 0.1952050400E+00, 0.924759000E-01}},//163.555P1
	{2,1,{ 0.2182280E+01, 0.150283E+01,-0.9118725000E+00,-0.994540450E+00}},//165.555K1
	{2,1,{ 0.2182280E+01, 0.184183E+01,-0.9118725000E+00,-0.994540450E+00}},//165.555K1
	{3,1,{ 0.1162050E+01,-0.237560E+00, 0.9998636500E+00, 0.115408095E+01}},//165.555K1
	{4,1,{-0.7504700E+00, 0.215019E+01, 0.6245161000E+00, 0.576472000E+00}},//165.555K1
	{5,1,{-0.6624800E+00,-0.164992E+01,-0.5974459500E+00,-0.262219700E+00}},//165.555K1
	{2,2,{-0.3319800E+00, 0.612090E+00, 0.1651780500E+00,-0.189386500E-01}},//245.655N2
	{3,2,{ 0.8540000E-02,-0.797900E-01, 0.9720320000E-02, 0.389012800E-01}},//245.655N2
	{4,2,{ 0.1635500E+00,-0.180550E+00, 0.7083460500E-01, 0.128100390E+00}},//245.655N2
	{5,2,{ 0.4360000E-01, 0.735500E-01,-0.1648012000E+00, 0.123663950E-01}},//245.655N2
	{2,2,{-0.2005820E+01, 0.253477E+01, 0.4472785000E+00, 0.265232000E+00}},//255.555M2
	{3,2,{ 0.3403000E-01,-0.170650E+00,-0.1273156700E+00, 0.275968510E+00}},//255.555M2
	{4,2,{ 0.8089900E+00,-0.553560E+00,-0.5147295000E-01, 0.453599850E+00}},//255.555M2
	{5,2,{ 0.2319000E-01, 0.293090E+00,-0.6567444500E+00,-0.248952580E+00}},//255.555M2
	{2,2,{-0.9459500E+00, 0.719990E+00, 0.5169850000E-02, 0.154940200E+00}},//273.555S2
	{2,2,{-0.8149500E+00, 0.394990E+00, 0.5169850000E-02, 0.154940200E+00}},//273.555S2
	{3,2,{-0.1639700E+00,-0.195740E+00,-0.6691617500E-01,-0.700798000E-01}},//273.555S2
	{4,2,{ 0.2939700E+00,-0.783900E-01, 0.5219045000E-01, 0.353969700E-01}},//273.555S2
	{5,2,{ 0.2877000E-01, 0.175870E+00,-0.1853334000E+00,-0.106727770E+00}},//273.555S2
	{2,2,{-0.4823000E-01, 0.113080E+00, 0.7745905000E-02, 0.126103750E-01}} //272.556T2
	};

	if(n>5 || m>2) return;
	k=0; 
	for( i=0; i<12; i++ ){
		Arg=0;
		for(j=k; j<k+cf[i][7]; j++){
			if(n==TEG_2B[j].n && m==TEG_2B[j].m){
				if(!Arg){ 
					TETf(&Arg,cf[i],DARG,6); Arg+=cf[i][6]*k_polpi;
					SArg = sin( Arg ); CArg= cos( Arg );
				}
				F_n=(1+k_n[TEG_2B[j].n-2])/(2*TEG_2B[j].n+1)*F;
				dCD[0]+=F_n*((TEG_2B[j].CS[0]+TEG_2B[j].CS[2])*CArg+(TEG_2B[j].CS[1]+TEG_2B[j].CS[3])*SArg);
				if( m ) { 
					dCD[1]+=F_n*((TEG_2B[j].CS[1]-TEG_2B[j].CS[3])*CArg-(TEG_2B[j].CS[0]-TEG_2B[j].CS[2])*SArg);
				}
			}
		}
        k+=cf[i][7]; 
	}
}

//---------------------------------------------------------------------------
double kij[6][5]= {
	{62.14397277,	-445267.1118864,	 0.0018543189,		-1.87719e-6,	 0},
	{218.31664563l,	-481269.278911278,	-0.0011574999,		 1.87239e-6,	 7e-8},
	{280.46645l,	 36000.7697489l,	 0.00030322222l,	 0.2E-7l,		-0.654E-8l},
	{ 83.35324312l,	 4069.01363525l,	-0.01032172222l,	-0.124991E-4l,	 0.5263E-7l},
	{234.95544499l,	 1934.13626197l,	-0.00207561111l,	-0.213944E-5l,	 0.165E-7l},
	{282.93734098l,	 1.71945766667l,	 0.00045688889l,	-0.1778E-7l,	-0.334E-8l}
} ;

//---------------------------------------------------------------------------
void Doodson_arg(double Time_tide, double *DARG)
{
	char   i,j;
	double T,dt,r;
	T=Time_tide+del_TDT/k_cbc;
	dt=fmod(T,1);
	T=(T-9131.5)/36525.;
	for(r=0,i=0; i<6; i++) {
		for(j=0; j<5; j++) { r*=T; r+=kij[i][4-j]; }
		DARG[i] = fmod(r / M_GRARAD, M_DPI);
	}
	DARG[0] += dt*M_DPI;
}

//---------------------------------------------------------------------------
struct step2_20{ char nd[5]; double Aip; double Aop;} s20[]=     //   dkR;  dkI;
 { {{ 0, 0, 0, 1, 0},  16.6,    6.7},  //     55.565   1     1347, -541,
   {{ 0, 0, 0, 2, 0},  -0.1,    0.1},  //     55.575   2     1124, -488,
   {{ 0, 1, 0, 0,-1},  -1.2,    0.8},  // Sa  56.554   3      547, -349,
   {{ 0, 2, 0, 0, 0},  -5.5,    4.3},  //Ssa  57.555   4      403, -315,
   {{ 0, 2, 0, 1, 0},   0.1,   -0.1},  //     57.565   5      398, -313,
   {{ 0, 3, 0, 0,-1},  -0.3,    0.2},  //     58.554   6      326, -296,
   {{ 1,-2, 1, 0, 0},  -0.3,    0.7},  //Msm  63.655   7      101, -242,
   {{ 1, 0,-1,-1, 0},   0.1,   -0.2},  //     65.445   8       80, -237,
   {{ 1, 0,-1, 0, 0},  -1.2,    3.7},  // Mm  65.455   9       80, -237,
   {{ 1, 0,-1, 1, 0},   0.1,   -0.2},  //     65.465  10       79, -237,
   {{ 1, 0, 1, 0, 0},   0.1,   -0.2},  //     65.655  11       77, -236,
   {{ 2,-2, 0, 0, 0},   0.0,    0.6},  //Msf  73.555  12       -9, -216,
   {{ 2, 0,-2, 0, 0},   0.0,    0.3},  //     73.355  13      -18, -213,
   {{ 2, 0, 0, 0, 0},   0.6,    6.3},  //Mf   75.555  14      -19, -213,
   {{ 2, 0, 0, 1, 0},   0.2,    2.6},  //     75.565  15      -19, -213,
   {{ 2, 0, 0, 2, 0},   0.0,    0.2},  //     75.575  16      -19, -213,
   {{ 3,-2, 1, 0, 0},   0.1,    0.2}, //Mstm  83.655  17      -65, -202,
   {{ 3, 0,-1, 0, 0},   0.4,    1.1}, //Mtm   85.455  18      -71, -201,
   {{ 3, 0,-1, 1, 0},   0.2,    0.5},  //     85.465  19      -71, -201,
   {{ 4,-2, 0, 0, 0},   0.1,    0.2}, //Msqm  93.555  20     -102, -193,
   {{ 4, 0,-2, 0, 0},   0.1,    0.1}};//Mqm   95.355  21     -106, -192,

struct step2_21{char nd[5]; double Aip; double Aop;}s21[]=       //   dkfE       AE       dkfA
 { {{-3, 0, 2, 0, 0},  -0.1,   0.0},  //2Q1 125.755    1     -29,     -0.1,        3,          0.0
   {{-3, 2, 0, 0, 0},  -0.1,   0.0},  // d1 127.555    2     -30,     -0.1,        3,          0.0
   {{-2, 0, 1,-1, 0},  -0.1,   0.0},  //    135.645    3     -45,     -0.1,        5,          0.0
   {{-2, 0, 1, 0, 0},  -0.7,   0.1},  // Q1 135.655    4     -46,     -0.7,        5,          0.1
   {{-2, 2,-1, 0, 0},  -0.1,   0.0},  // p1 137.455    5     -49,     -0.1,        5,          0.0
   {{-1, 0, 0,-1, 0},  -1.3,   0.1},  //    145.545    6     -82,     -1.3,        7,          0.1
   {{-1, 0, 0, 0, 0},  -6.8,   0.6},  // O1 145.555    7     -83,     -6.8,        7,          0.6
   {{-1, 2, 0, 0, 0},   0.1,   0.0}, //TAU1 147.555    8     -91,      0.1,        9,          0.0
   {{ 0,-2, 1, 0, 0},   0.1,   0.0},//NTAU1 153.655    9    -168,      0.1,       14,          0.0
   {{ 0, 0,-1,-1, 0},   0.1,   0.0},  //    155.445   10    -193,      0.1,       16,          0.0
   {{ 0, 0,-1, 0, 0},   0.4,   0.0},  //LK1 155.455   11    -194,      0.4,       16,          0.0
   {{ 0, 0, 1, 0, 0},   1.3,  -0.1},  //NO1 155.655   12    -197,      1.3,       16,         -0.1
   {{ 0, 0, 1, 1, 0},   0.3,   0.0},  //    155.665   13    -198,      0.3,       16,          0.0
   {{ 0, 2,-1, 0, 0},   0.3,   0.0},  //XI1 157.455   14    -231,      0.3,       18,          0.0
   {{ 0, 2,-1, 1, 0},   0.1,   0.0},  //    157.465   15    -233,      0.1,       18,          0.0
   {{ 1,-3, 0, 0, 1},  -1.9,   0.1},  //pi1 162.556   16    -834,     -1.9,       58,          0.1
   {{ 1,-2, 0,-1, 0},   0.5,   0.0},  //    163.545   17   -1117,      0.5,       76,          0.0
   {{ 1,-2, 0, 0, 0}, -43.4,   2.9},  // P1 163.555   18   -1138,    -43.4,       77,          2.9
   {{ 1,-1, 0, 0,-1},   0.6,   0.0},  //    164.554   19   -1764,      0.6,      104,          0.0
   {{ 1,-1, 0, 0, 1},   1.6,  -0.1},  // S1 164.556   20   -1764,      1.6,      104,         -0.1
   {{ 1, 0,-2,-1, 0},   0.1,   0.0},  //    164.345   21   -3048,      0.1,       92,          0.0
   {{ 1, 0, 0,-2, 0},   0.1,   0.0},  //    165.535   22   -3630,      0.1,      195,          0.0
   {{ 1, 0, 0,-1, 0},   8.8,   0.5},  //    165.545   23   -3845,      8.8,      229,          0.5
   {{ 1, 0, 0, 0, 0}, 470.9, -30.2},  // K1 165.555   24   -4084,    470.9,      262,        -30.2
   {{ 1, 0, 0, 1, 0},  68.1,  -4.6},  //    165.565   25   -4355,     68.1,      297,         -4.6
   {{ 1, 0, 0, 2, 0},  -1.6,   0.1},  //    165.575   26   -4665,     -1.6,      334,          0.1
   {{ 1, 1,-1, 0, 0},   0.1,   0.0},  //    166.455   27  85693.,      0.1,    21013,          0.0
   {{ 1, 1, 0,-1,-1},  -0.1,   0.0},  //    166.544   28  35203.,     -0.1,     2084,          0.0
   {{ 1, 1, 0, 0,-1}, -20.6,  -0.3}, //PSI1 166.554   29   22794,    -20.6,      358,         -0.3
   {{ 1, 1, 0, 0, 1},   0.3,   0.0},  //    166.556   30   22780,      0.3,      358,          0.0
   {{ 1, 1, 0, 1,-1},  -0.3,   0.0},  //    166.564   31   16842,     -0.3,      -85,          0.0
   {{ 1, 2,-2, 0, 0},  -0.2,   0.0},  //    167.355   32    3755,     -0.2,     -189,          0.0
   {{ 1, 2,-2, 1, 0},  -0.1,   0.0},  //    167.365   33    3552,     -0.1,     -182,          0.0
   {{ 1, 2, 0, 0, 0},  -5.0,   0.3},  //FI1 167.555   34    3025,     -5.0,     -160,          0.3
   {{ 1, 2, 0, 1, 0},   0.2,   0.0},  //    167.565   35    2892,      0.2,     -154,          0.0
   {{ 1, 3, 0, 0,-1},  -0.2,   0.0},  //    168.554   36    1638,     -0.2,      -93,          0.0
   {{ 2,-2, 1, 0, 0},  -0.5,   0.0}, //TET1 173.655   37     370,     -0.5,      -20,          0.0
   {{ 2,-2, 1, 1, 0},  -0.1,   0.0},  //    173.665   38     369,     -0.1,      -20,          0.0
   {{ 2, 0,-1,-1, 0},   0.1,   0.0},  //    175.445   39     325,      0.1,      -17,          0.0
   {{ 2, 0,-1, 0, 0},  -2.1,   0.1},  // J1 175.455   40     324,     -2.1,      -17,          0.1
   {{ 2, 0,-1, 1, 0},  -0.4,   0.0},  //    175.465   41     323,     -0.4,      -16,          0.0
   {{ 3,-2, 0, 0, 0},  -0.2,   0.0},  //SO1 183.555   42     194,     -0.2,       -8,          0.0
   {{ 3, 0,-2, 0, 0},  -0.1,   0.0},  //    185.355   43     185,     -0.1,       -7,          0.0
   {{ 3, 0, 0, 0, 0},  -0.6,   0.0},  //OO1 185.555   44     184,     -0.6,       -7,          0.0
   {{ 3, 0, 0, 1, 0},  -0.4,   0.0},  //    185.565   45     184,     -0.4,       -7,          0.0
   {{ 3, 0, 0, 2, 0},  -0.1,   0.0},  //    185.575   46     184,     -0.1,       -7,          0.0
   {{ 4, 0,-1, 0, 0},  -0.1,   0.0},  // v1 195.455   47     141,     -0.1,       -4,          0.0
   {{ 4, 0,-1, 1, 0},  -0.1,   0.0}}; //    195.465   49     141,     -0.1,       -4,          0.0

struct step2_22
{ char nd[5]; double A; } s22[] =  //dkR;
       { {{-1, 0, 1, 0, 0},-0.3 }, //N2  245.655    1     6,
         {{ 2, 0, 0, 0, 0},-1.2 }  //M2  255.555    2     4,
} ;
double N20 = 2.236068000e+12,
       N21 = 1.290994450e+12,
       N22 = 0.645497224e+12 ;

//----------------------------------------------------------------------------
// ������������� �������� 2 �������
//----------------------------------------------------------------------------
void Step2_GP(double *DARG, double *CD, int n, int m) // Cnm,Dnm-�� ����.
{
	if (n!=2) return ;
	char   i ;
	double ttf ;
	if (m==0) {
		for (i=0; i<21; i++) {
			ttf = 0 ;  
			TETf(&ttf,s20[i].nd,DARG+1,5) ;
			CD[0]+= (s20[i].Aip*cos(ttf)+s20[i].Aop*sin(ttf))/N20 ;
		}
	} else { 
		if (m==1) {
			//for(i=0; i<49; i++)
			for(i=0; i<48; i++) {
				ttf=DARG[0] ; 
				TETf(&ttf,s21[i].nd,DARG+1,5) ;
				CD[0]+= (s21[i].Aip*sin(ttf)+s21[i].Aop*cos(ttf))/N21 ;
				CD[1]+= (s21[i].Aip*cos(ttf)-s21[i].Aop*sin(ttf))/N21 ;
			}	
		} else {
			for (i=0; i<2; i++) {
				ttf = 2*DARG[0] ;   
				TETf(&ttf,s22[i].nd,DARG+1,5) ;
				CD[0]+= s22[i].A*cos(ttf)/N22 ;
				CD[1]+= s22[i].A*sin(ttf)/N22 ;
	}	}	}
}

//---------------------------------------------------------------------------
void TETf(double *tetf, char *ni, double *DARG, char n)
{
  char j;
  for(j=0; j<n; j++) *tetf+=ni[j]*DARG[j];
  *tetf=fmod(*tetf,k_dwapi);
}

//---------------------------------------------------------------------------
// ������������� ������� ��������
void LegendreInit (struct LEGENDRE* Leg, double value)
{
  memset(Leg,0,sizeof(*Leg));
  Leg->cst  = value;
  Leg->snt  = sqrt (1-value*value);
  Leg->fval = Leg->val = 1;
}

//---------------------------------------------------------------------------
// ��������� ������� �������� (m+1,m+1)
void LegendreNextRow (struct LEGENDRE* Leg)
{
  Leg->val  = Leg->fval *= (2*Leg->m+1)*Leg->snt;
  Leg->bval = 0;
  Leg->n    = ++Leg->m;
}

//---------------------------------------------------------------------------
// ��������� ������� �������� (n+1,m)
void LegendreNext (struct LEGENDRE* Leg)
{
  double
  tmp = ((2*Leg->n+1)*Leg->cst*Leg->val-(Leg->n+Leg->m)*Leg->bval)/(Leg->n-Leg->m+1);
  Leg->bval = Leg->val;
  Leg->val  = tmp;
  Leg->n++;
}

//---------------------------------------------------------------------------