//-------------------------------------------------------------------------------
// ZPR.h
// �. ��������� 03.08.2018
//-------------------------------------------------------------------------------
#ifndef _ZPR_H
#define _ZPR_H

extern double Global_m2000_75[9] ;

enum MODEL_PRCJ2000 {
	PRC1976=0,			// ��������� ���������� ��������� ���������
						// �� ������������� IAU 1976 ����, ��� �� �������� 
						// ������� � ��-50...
	PRC2000=1,			// ��������� ���������� ��������� ��������� � ������ 
						// ������������ IAU 2000 ���� 
	PRC2003=2,			// ��������� ���������� ��������� ��������� � ������ 
						// ������������ IAU 2003 ���� 
	PRCIAU1976=3,		// ���������� SOFA ������ IAU 1976 ����
	PRCIAU2000B=4,		// ���������� SOFA ���������� ������ IAU2000B
	PRCIAU2000A=5,		// ���������� SOFA ������ ������ IAU2000A
	PRCIAU2006=6 } ;	// ���������� SOFA ������ Convention 2006

// �������� �� ��������� ������ �������� ���������� ���������
#define DEF_MODPRC PRCIAU1976

//----------------------------------------------------------------------------
// ������ ���������� ��������� �� ����� t �������� � ����� Scale.
// Scale - ������� �������
//		TIME2000ASD	= 0, � ��� �� 2000 (0h 01.01.2000),
//		IME1975ASD	= 1, � ��� �� 1975 (0h 01.01.1975),
//		TIMEJ2000JC	= 2. � ��������� ��������� �� J2000 (12h 01.01.2000).
// ��������� ��������, � ������� ��������
//		Pr[0] - ���,
//		Pr[1] - Z,
//		Pr[2] - ����.
//
// ��������� ��������� ������ ���� ��������� ������!!!! 
// ��� �������������� �� ��������������� ����� J2000,0 � ����� t �� ����
// �������� ������� ����� � ��������
FLYCORE_API int	ZPrecessJ2000(double t, double *Pr,
							  TIMESCALE Scale=DEFTIME_SCALE, 
							  MODEL_PRCJ2000 ModPrc=DEF_MODPRC) ;

//----------------------------------------------------------------------------
// ���������� ������� ��������� M �� ����� t, �������� � �������� Scale.
// Scale - ������� �������
//		TIME2000ASD	= 0, � ��� �� 2000 (0h 01.01.2000),
//		IME1975ASD	= 1, � ��� �� 1975 (0h 01.01.1975),
//		TIMEJ2000JC	= 2. � ��������� ��������� �� J2000 (12h 01.01.2000).
FLYCORE_API int ZBuildMatrPrecessJ2000(double t, double* M,
									   TIMESCALE Scale=DEFTIME_SCALE, 
									   MODEL_PRCJ2000 ModPrc=DEF_MODPRC) ;

//----------------------------------------------------------------------------
// ������ ���������� ��������� �� ����� t � ������������ �
// �������������� IAU 1976 ����
//		t - ����� � ����� Scale
//		Pr- �������������� ��������� ��������� � ������� ��������
FLYCORE_API int ZPrecessJ2000LikeIAU1976(double t, double *Pr,
										 TIMESCALE Scale=DEFTIME_SCALE) ;

//----------------------------------------------------------------------------
// ������ ���������� ��������� �� t1 �� t2 (� ������� ��������)
// IAU 1976 precession model.
FLYCORE_API void Preces1976t1t2(double t1, double t2, double *Pr,
								TIMESCALE Scale=DEFTIME_SCALE) ;

//----------------------------------------------------------------------------
// ������ ���������� ��������� �� ����� t � ������������ �
// �������������� IAU 1976 ����
//		t - ����� � ����� Scale
//		Pr- �������������� ��������� ��������� � ������� ��������
FLYCORE_API int ZPrecessJ2000LikeIAU2000(double t, double *Pr,
										 TIMESCALE Scale=DEFTIME_SCALE) ;

//----------------------------------------------------------------------------
// ������ ���������� ��������� �� ����� t � ������������ �
// Convention 2003 ���� IERS
//		t - ����� � ����� Scale
//		Pr- �������������� ��������� ��������� � ������� ��������
FLYCORE_API int ZPrecessJ2000LikeIAU2003(double t, double *Pr,
										 TIMESCALE Scale=DEFTIME_SCALE) ;

//----------------------------------------------------------------------------
// ���������� ������� ��������� M �� ���������� ��������� Precess.
// Pr - ������������� ��������� ����������� ����� 
// � �������� � ������� ��������	
FLYCORE_API void ZMatrPrecess(double* Pr, double* M);

#endif _ZPR_H
