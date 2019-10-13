//-----------------------------------------------------------------------------
// SamplAverageMotion.cpp
// �. ��������� 14.06.2019
//
// ������� �������� �� �� ������ � �������� ����������� ������ ��� ������� �����
// ������������ ��������� "�������������" ���������� ������ �������� ��
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
// ���������� Fly
#include <FlyCore.h>		// ����������� ���� ������ ��������
#include <FlyMMath.h>		// �������������� �������
#include <FlyCoreLink.h>	// ����������� ���� ���������

int TaskTurnsPredictionHAvr();

void main()
{
	// ����� �������� � ���������� ���������� 
	//setlocale(LC_CTYPE, "rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	// ������������� ���� Fly, ���������� ��������� ����� � ����������� 
	// �������������� ��������, ������������� ���������� ����������,
	// � ����������� �������� ������������� ���������� � ��...
	int rc = FlyGlobalInit(); if (rc) return;

	// ������ ��������������� �������� �� � ����������� ���������� ��������
	// � � ������������ ����������� ������ �������� ��.
	TaskTurnsPredictionHAvr();
}

//---------------------------------------------------------------------------
// ��������� �� � ����� ������������ ������� �������� 
// ������-������������ ������ 
// ��������� ������������� �� NU0 � ����� ��������� �� ��� � ���� ��� LSF
// �� ������ � �������� ���������� (e1z, e2z, az (��� havrz)) � ����� 
// � ���������� ������ uz.
// ���� ������� �������� havrz, ������� ������������ �� ����� ������ ������
// ����� ����� havrz.
// ��������� - ��������� ������� NUK.
//---------------------------------------------------------------------------
int RunUpSysEstimateNU(
	ZLSF& LSF,		   // ���� ���
	ZNU&  NU0,		   // �������� ��
	ZNU&  NUK,		   // ���������� �� (���������)
	double uz,		   // �������� ������ �� ������� ������� ��������� ������
	double e1z,		   // ��������� ������
	double e2z,
	double az,		   // ������� ������� (����� ����� ���� �� ������ havrz)
	double havrz = 0); // ��������� ������� ������ ������

//-------------------------------------------------------------------------------
// ������� �������� �� �� ������ � �������� ����������� ������ ��� ������� �����
// ������������ ��������� "�������������" ���������� ������ �������� ��
//		�������� ��������������� ������ � ������: 
//		ZNU    NU			- ��������� ������� - ���������
//		ZSC_SI SI			- �������� �������� �� - ���������
//		ZMSC   KA			- ������ ��������, ����� �������� � �� ���� ����� 
//							  ������ ������������ �������� (��) - �����
//		KA.Init(...)		- ������������� ������ �������� - ����� 
//		KA.GoToVit(...)		- ������������� �������� �� �� ������ ��������� 
//							  ����� - �����
//		KA.GoAlongVit(...)	- ������� ����� ����� � �������� ������������ 
//							- ���������� ����� - �����
//		ZCPrintManager PrintMng - ��������� ���������������� - �����
//-------------------------------------------------------------------------------

// ��������� �� ��� ���������� ������ ������� ������� ���������������� ���������
ZMSC_EXV_BEG(ZModExternHAvr)
	// ���������� ������ ������ ������� ���������������� ���������
	// ���������� ��������� � ���
	double GSK[6];
	int rc = ZASC1975toGSC(t, Xmain, GSK, 0);	RRC;
	// ���������� ������, ������ � �������
	double HBL[3];
	ZGRtoHBL(GSK, HBL, 0, 0);
	// ������������ ������ ����� ��������������� ��������� �������� 
	// ������ ����������� ������ �� ��� �������� � ��.
	Yexe[0] = 1e4*HBL[0];
	// � ��������� ��������� ������ ������ �������� ����� � ��������������
	// ����������� ��� ������� ��������� ����� ����! 
	// "KA.SetExternalEquationsCount(1) ;"
	// ������ ������� ������ ������� ������ ������ �������������� ���������
	// Yexe ����� �������... Yexe[0] = ... 
	// 
ZMSC_END

//-------------------------------------------------------------------------------
int TaskTurnsPredictionHAvr()
{
	cout << "\n   Prediction by turns whith hight averaging...\n";

	int  rc = 0;
	long Vit;
	long Vitn = 100;	// ����� ������ �������������
	long Vitk = 212;	// ����� ���������� �������������

	cout << "   Fron turn " << Vitn << "  to " << Vitk << "\n";

	PZCPrintManager PrintMng;	// ��������� ����������������
	PZMSC		    KA;			// ������ �������� ��
	ZNU             NU;			// ��������� ������� ��������

	//---------------------------------------------------------------------------
	// ���������� ����� ���
	ZLSF LSF; memset(&LSF, 0, sizeof(LSF));
	LSF.num   = 1;			// �������� �����
	LSF.vgpz  = GPZ90;		// ���� ���� ����� ��-90 (!!!! �� 90.11 !!!!)
	LSF.ngpz  = 16;			// ����������� ������������� 
	LSF.mgpz  = 16;			// � ��������� ��������
	LSF.vatm  = ATMSMA81;	// ����������� ������ ��������� ���� 1981 ����
	LSF.vsvd  = 0;			// ������� ������ ��������� ��������
	LSF.varsp = 0;			//
	LSF.vTide = TIDENULL;	// ������� ����� ������� ������������ ��������
	LSF.isDU  = 0;			// �������� �� ���� ������ ������������ ���������
	LSF.isSun = 1;			// ���� ��������������� ���������� ������
	LSF.isMoon= 1;			// ���� ��������������� ���������� ����

	//---------------------------------------------------------------------------
	// ������������ ��

	// ����� ��
	NU.Key.ka = 1; NU.Key.nom = 15; NU.Key.mod = 0;
	NU.Key.type = 8; NU.Key.bc = 1;

	// �������� ����� ���������� ����� ���
	NU.numlsf = 1;
	// ����� �� � ��� (NU.t)
	DATE dt = DATE(18, 7, 2018); 	// ����������� ����
	TIME tm = TIME(9, 30, 0, 0);	// ����������������� �����
	TA_DMB(1, &dt, &tm, &NU.t);		// �������������� ����������������� ���� 
	// � ������� � ��� (������)
	// �����
	NU.vit = 99;

	// ������� ��������� - ������������ ��������, �������� ������������
	// �������� ����� 1975 ����
	NU.sk = _SK_OSK1975;
	NU.X[0] = TO_DTKM(6650.0);
	NU.X[1] = 0.0001;
	NU.X[2] = TO_RAD(97.0);
	NU.X[3] = TO_RAD(120.0);
	NU.X[4] = TO_RAD(45.0);
	NU.X[5] = TO_RAD(15);
	NU.Sb = 0.001;
	NU.Kp = 0;
	NU.M0 = 900;
	NU.MF = 50;

	//---------------------------------------------------------------------------
	// ����� ���������� �������������� ������ - �������� ���������� ������
	{
		// ��������� ������������� ������
		double T, i, a, p, e, e1, e2;
		// ������ �� ������� ������
		double Hs = 270;
		// ���������� ���������� �������� ���������� ������ ������������ ���������
		// ������ � �������� ������� ������� Hs.
		rc = MBuildSSOMIV_hs(Hs, T, i, p, e1, e2, nullptr); 
		if (rc) return rc;

		// ����������� �� - ��������� ��������� �� ����������� 
		// ����������� �������� ��� ���
		e = sqrt(e1*e1 + e2*e2);
		a = p / (1 - e*e);
		NU.sk = _SK_OSKTE;
		NU.X[0] = TO_DTKM(a);
		NU.X[1] = e;
		NU.X[2] = TO_RAD(i);
		NU.X[3] = TO_RAD(120.0);
		NU.X[4] = TO_RAD(0);
		NU.X[5] = TO_RAD(1e-15);

		// ��������� ���������� ��
		ZNU NUK;
		rc = RunUpSysEstimateNU(LSF, NU, NUK, 0, e1, e2, 0, Hs); 
		if (rc) return rc;
		NU = NUK;
	}

	//---------------------------------------------------------------------------
	// ����������� ����������� ������� ��������� (������������ ���������� ������)
	KA->SetExternalEquationsCount(1);
	// ������������� ������ �������� ��� ������� �� �������� �� (NU)
	// � �������� ���� ��� (LSF)
	rc = KA->Init(NU, &LSF); 
	if (rc) return rc;

	//---------------------------------------------------------------------------
	// ����������� ������� ��������� �������� ���������� ����������������

	// ������� ����������������, �� ���� �� �������
	PrintMng->MPK.AflatOn();
	// ���������������� ���������� �������� �� ������
	PrintMng->MPK.FromVitOn();
	// ���������� ������ ����������
	PrintMng->MPK << MODPRDATE;		// ���� �����������������
	PrintMng->MPK << MODPRTIME;		// ����� �����������������
	PrintMng->MPK << MODPR_VIT;			// �����
	PrintMng->MPK << MODPR_MSSV;		// ������� ������� ��������� �����
	PrintMng->MPK << MODPR_L;			// ������� ������������
	PrintMng->MPK << MODPR_Aosk;		// ������� �������
	PrintMng->MPK << MODPR_eosk;		// ��������������
	PrintMng->MPK << MODPR_V_Hsr;		// ������� �� ����� ������
	PrintMng->MPK << MODPR_V_Hmaxt;		// ����� ���������� ����. ������ �� �����
	PrintMng->MPK << MODPR_V_HmaxU;		// �������� ������ ����. ������
	PrintMng->MPK << MODPR_V_Hmax;		// ���� ������������ ������
	PrintMng->MPK << MODPR_V_Hmint;		// ���������� ��� ����������� ������: 
	PrintMng->MPK << MODPR_V_HminU;		// �����, �������� ������, ���� ������
	PrintMng->MPK << MODPR_V_Hmin;		//
	PrintMng->MPK << MODPR_V_Rmaxt;		// ���������� ��� ������������� �� ����� 
	PrintMng->MPK << MODPR_V_RmaxU;		// �������.
	PrintMng->MPK << MODPR_V_Rmax;		//
	PrintMng->MPK << MODPR_V_Rmint;		// ���������� ��� ������������ �� ����� 
	PrintMng->MPK << MODPR_V_RminU;		// �������
	PrintMng->MPK << MODPR_V_Rmin;		//

	// �������� ���������� ���������
	PrintMng->PrePrint(&KA, "�������", "_TaskTurnsPredictionHAvr");
	PrintMng->PrintHeaderDoc("������� �������� ��");
	PrintMng->PrintText("������� �������� �� �� ������ � �������� ����������� \
������ ��� ������� �����. �� ���������� �� �������� ��������� �������� � ����� \
������������ �������� ������� ������ ������.");

	// ���������������� �� � ���
	PrintMng->PrintNU (NU, nullptr, 3);
	PrintMng->PrintLSF(LSF, 1);

	// ��������� ��� ��������� ��������� �������� ��
	ZSC_SI SI;
	// ��������������� ��������� �� �� ����� ������ �������������
	cout << "\r       turn  " << Vitn << "...    ";
	rc = KA->GoToVit(Vitn, &SI); 
	if (rc) return rc;
	Vit = Vitn;

	while (1) {
		// ����������� �������� ����� ������� ��������� 
		// ��������� ������� ��� �������� ��������� � ������ ������ 
		// ����� ���������� ��������.
		KA->ExternalEquationsOn();
		// ������������� �������� �� ����� ����� Vit � �������� ���� ���������
		// �������� ��, � ��� ����� � ������������ ������������� �����.
		cout << "\r       turn  " << Vit << "...    ";
		rc = KA->GoAlongVit(Vit, _SK_ASKTE); if (rc) break;
		// ���������� ������ ����������� �� ����� ����� �������� ������ ���
		// �������������� ��������.
		// ������� �������� ������� ����������, ������� ��������������� ���������
		// � ������ �������� ������� ����������.
		double HAvr;
		KA->ExternalEquationsOff(&HAvr);
		// ���������� �������� ������� �� ������������� ������ ���������� � �������
		// ��������� ������.
		HAvr /= (KA->FV.Td / k_cbc);
		// ����� ������� � ���������� ������-������������ �� ����� �������� ������
		// ����� ��
		KA->FV.HAvr = HAvr;
		// ���������������� ������� ��������� �������� ��.
		PrintMng->PrintMSC(&KA);
		// �������� � ���������� �����
		Vit++;
		// �������� ������� ���������� �������������
		if (Vit>Vitk) break;
	}
	cout << "\n";

	// ���������� ����������������.
	PrintMng->PostPrint();
	PrintMng->ClosePrint();

	// �������� ����������� ���������� ������������ �������
	PrintMng->ShowTextFile();
	PrintMng->ShowHtmlFile();

	return rc;
}

//-------------------------------------------------------------------------------


