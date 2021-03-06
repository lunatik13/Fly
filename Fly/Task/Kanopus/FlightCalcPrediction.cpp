//-------------------------------------------------------------------------------
// FlightCalcPrediction.cpp
// ������� �� �� �� �������� ��������� ������� � �������� ������������ 
// ������������� ����� ��
// 
// �. ��������� 14.02.2019
//-------------------------------------------------------------------------------
#include "stdafx.h"

//---------------------------------------------------------------------------
// ���������� Fly
#include <FlyCore.h>			// ����������� ���� ������ ��������

//-------------------------------------------------------------------------------
// ��������� �� ��� ���������� ������ ������� ������� ���������������� ���������
ZMSC_EXV_BEG(ZModKHAvr)
	// ������ ����� ������� ���������������� ���������
	double GSK[6] ;
	int rc = ZASC1975toGSC(t, Xmain, GSK, 0) ;	RRC ;
	double HBL[3] ;
	ZGRtoHBL(GSK, HBL, 0, 0) ;
	// �������������� ������ �����
	Yexe[0] = 1e4*HBL[0] ;
ZMSC_END

//-------------------------------------------------------------------------------
// ������� �������
//		NU	-	�� �� ������� ����� ��������������
//		LSF	-`	����� ��� � ������� ����������� �������
//		dt	-	�������� ��������������� (7 ����� ��������)
//		dL, dLs, Hmin, ... - ����������� ����������, �������� ����� �����,
//				���. ������, ... �� ����� ��
//	
int FlightCalcPrediction(ZNU& NU, ZLSF& LSF, double dt, char* FilePrintSticker, 
						 double* dL, double* dLs, double* Hmin, 
						 double* Hmax, double* Hs)
{
	printf("Motion prediction...\n") ;
	int rc = 0 ; 

	// �������������� ������ �������������� �������   
	// ����� ��������������� �������� ��� � �������� ������ ��������.
	// ������ �������� ��
	ZMSC KA ;
	// ������������� ������ ��������
	rc = KA.Init(NU, &LSF) ; RRC ;

	//---------------------------------------------------------------------------
	// ���� ��������� ������ ��� ��� � ������� ������, 
	// �� ���������� ����� � ������ ������ �������� ���������
	// ��� ������� ����������� �� ����� ������ (�������������������
	// ��������).
	if (Hmin || Hmax || Hs || dLs || dL) {
		ZModKHAvr KA ;
		// ����� ����� ������� ���������
		KA.SetExternalEquationsCount(1) ;
		// �������������� ������
		rc = KA.Init(NU, &LSF) ; RRC ;
		// �������� �������������� �������� ���������,
		// ��� ���� �������� ������� �� ��� ������� ���������
 		KA.ExternalEquationsOn(NULL) ;
		// ������������� ���������� ����� ����� ����� NU.vit
		KA.GoAlongVit(NU.vit) ;
		// �������� ������������������� �������� ���������
		double HAvr ;
		KA.ExternalEquationsOff(&HAvr) ;
		// ����� �� ������������� ������ ����� �������� 
		// ������������������ �������� ���������
		HAvr/= (KA.FV.Td/k_cbc) ;
		// �������� �������� � ������� ������������ ���������� �����
		KA.FV.HAvr = HAvr ;

		// ����������� ����� � ��������
		if (dL ) *dL = KA.FV.dL ;	
		// �������� �������� ����� � ��������
		if (dLs) *dLs= KA.FV.dLs ;	

		// ����� �� ��������� ��������� ��������� ����������� ������ �� �����
		ZSC_VP* HM1 = HMIN_VI(KA.FV) ;
		// ����� �� ��������� ���������� ��������� ������������ ������ �� �����
		ZSC_VP* HM2 = HMAX_VI(KA.FV) ;
		if (Hmin) *Hmin = HM1->H ; 
		if (Hmax) *Hmax = HM2->H ; 
		if (Hs  ) *Hs   = KA.FV.HAvr ; 
	}

	//---------------------------------------------------------------------------
	// ��������� ����������������
	ZCPrintManager PrintMng ;
	// ��������� ���������������� �������� �� ������
	PrintMng.MPK.FromVitOff() ;
	// ���������������� ����������� �������� �� ������� (�������������)
	PrintMng.MPK.AflatOn() ;

	//---------------------------------------------------------------------------
	FAST_SEMAPFORE_OPEN("FlightKanopusMPKBuild") ;

	//---------------------------------------------------------------------------
	// �������������� ����������������� ����������������� ����������� ��������.
	// ����������: ��� ���������, ��� �������� (����� ������ ����� �������),
	// ������������ ����������� �������� ��� ������������� � ��������.
	// �������� ��������������� ���������� �������� ����� ��� ������
	// ������� PMPK.UPP(0).value = ...
	PrintMng.MPK << ZSUserPrintParam(UPPdouble, 16, 6, "�����", "����") ;
	PrintMng.MPK.UPP(0).value = 0 ;
	// ���������� ��������������� ���������� ��������
	PrintMng.MPK << MODPRDATETIME ;
	PrintMng.MPK << MODPR_MSSV ;
	PrintMng.MPK << MODPR_T_iTE ;
	PrintMng.MPK << MODPR_T_WTE ;
	PrintMng.MPK << MODPR_T_wTE ;
	PrintMng.MPK << MODPR_T_uTE ;
	PrintMng.MPK << MODPR_H ;
	PrintMng.MPK << MODPR_Aosk ;
	PrintMng.MPK << MODPR_eosk ;
	PrintMng.MPK << MODPR_exosk ;
	PrintMng.MPK << MODPR_eyosk ;

	//---------------------------------------------------------------------------
	FAST_SEMAPFORE_CLOSE() ;

	//---------------------------------------------------------------------------
	// ������������� ����������������
	//---------------------------------------------------------------------------
	// ��������� ������ � ����� ����� ���������
	PrintMng.SetPrintFileSticker(FilePrintSticker) ;
	// �������� ���������� ���������
	PrintMng.PrePrint(&KA, "�������", "_FlightKanopus") ;
	// ��������� ���������
	PrintMng.PrintHeaderDoc("\n ������� ���������� �������� �� (FlightKanopus)\n") ;

	// ���������������� ��
	PrintMng.PrintNU(NU, NULL, 3) ;
	// ���������������� ���������� ����� ���
	PrintMng.PrintLSF(LSF, 1) ;

	//---------------------------------------------------------------------------
	// ���������� ���� ��������������� ���������� �������� ��
	//---------------------------------------------------------------------------

	// ���������������� ���������� ��������
	PrintMng.PrintMSC(&KA, NULL, NULL, NULL) ; 
	// ������� �����
	double t  = KA.GetTime() ;
	// ��������� �����
	double tn = t ;
	// �������� ����� �����
	double tk = t+dt ;
	int   dir = tk>=t ? 1:-1 ;
	// ���� ������������� 
	ZSC_SI SI ;
	while(1) {
		printf("\rCompleted %.2lf%s    ", (t-tn)/dt*100.0, "%") ;
		// �������� ������� ���������� ���������� �������������� (��������)
		if (t>=tk) break ;
		// ���� ��� ����������� � �������������� ������� ��� ��������, 
		// �� �� ����� ��� ������ ������� tk. �� ���� h <= tk-t, ���
		// t ���� ������� �����
		rc = KA.GoStepA(&SI, dir, &tk) ; if (rc) break ;
		// ����� ������� ����� 
		t = SI.ta ;
		// ���������������� ����������������� ����������� ��������.
		// ������� �������� ��������������� ���������,
		// �������������� ����� ��� ������ ������ ��������� 
		// PrintMng.MPK << ZSUserPrintParam(UPPdouble...
		PrintMng.MPK.UPP(0).value = (t-tn)*24.0;
		// ���������������� ���������� ���������� ��������
		PrintMng.PrintMSC(&KA, &SI, NULL, NULL) ;
	}
	printf("\n") ;

	//---------------------------------------------------------------------------
	// ���������� ����������������
	PrintMng.PostPrint() ; 
	PrintMng.ClosePrint();

	return 0 ;
}

//-------------------------------------------------------------------------------
