//-----------------------------------------------------------------------------
// MatchingPrediction.cpp
// ������ ��������� ������� ������ ����� ��
//
// �. ��������� 23.04.2019
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
// ���������� Fly
#include <FlyCore.h>		// ������ ��������

//-----------------------------------------------------------------------------
int MatchingPrediction()
{
	int rc = 0;
	// �� 
	ZNU NU;

	cout << endl << "  ������ ��������������� ���������� �������� ��." 
		 << endl << endl;

	// �������� �� �� ��� ��01 
	cout << "  ��:" << endl;
	rc = LoadNU01("NU01_prognoz_003.txt", NULL, NULL, NU, nullptr, true);
	if (rc) return rc;
	NU.M0 = 420.0;
	NU.MF = 50.0;

	// ���������� ����� ��� ��� ������� ������
	ZLSF LSF;
	LSF.num  = 1;
	LSF.vgpz = GPZ90;
	LSF.ngpz = 16;
	LSF.mgpz = 16;
	LSF.vatm = ATMNULL;
	LSF.vsvd = 0;
	LSF.varsp= 0;
	LSF.vTide = TIDENULL;
	LSF.isDU  = 0;
	LSF.isSun = false;
	LSF.isMoon = false;

	//-------------------------------------------------------------------------
	// ��������� ����������������
	ZCPrintManager PrintMng;

	// �������������� ���������������� ����������� ��������

	// �������������� ����������������
	PrintMng.MPK.AflatOn();
	// ���������������� �� �������.
	PrintMng.MPK.FromVitOff();
	PrintMng.MPK << MODPRDATE;
	PrintMng.MPK << MODPRTIME;
	PrintMng.MPK << MODPR_VIT;
	PrintMng.MPK << MODPR_T_RGSC;
	PrintMng.MPK << MODPR_T_VGSC;
	PrintMng.MPK << MODPR_Aosk;
	PrintMng.MPK << MODPR_eosk;

	//---------------------------------------------------------------------------
	ZMSC   *KA;		// ������ �������� ��
	ZSC_SI SI;		// ��������� ��� ��������� �� ������ ��������� �������� ��
	double t0,		// ����� ������ �������������
		   tk,		// ����� ����� �������������
		   t,		// ������� �����
		   h;		// ��� �������������
	DATETIMEDMB DTNU;
	DTNU << NU.t;
	DTNU << TIME(10, 0, 0, 0);
	h  = 600.0 / k_cbc;
	t0<< DTNU;
	tk = t0 + 4.0;
	t  = t0;
	
	// �������� � ���� ������ �������� ��
	KA = new ZMSC; if (!KA) return-1;

	//---------------------------------------------------------------------------
	// ������������� ������ �������� ��� ������� �� �������� �� (NU)
	// � �������� ���� ��� (LSF)

	// �������� ������������� � ���������� ����� � 60 ������ 
	//KA->PreInitInfo.defautFixedStep = 60.0;

	// ��������������� ���� ������������� ������ ��������
	rc = KA->Init(NU, &LSF); if (rc) return rc;

	//---------------------------------------------------------------------------
	// �������� ���������� ��������� ��� ���������������� ����������� ��������
	PrintMng.PrePrint(KA, "������ ��������", "_TaskMatchingPrediction_prognoz_003");
	// ��������� ���������� ���������
	PrintMng.PrintHeaderDoc("\n   ������ ����������� ���������������\n\n");
	// ���������������� �� � ���������� ����� ���
	PrintMng.PrintNU(NU, NULL, 3);
	PrintMng.PrintLSF(LSF, 1);

	//---------------------------------------------------------------------------
	// �������� ���� ������������� 
	while (1) {
		// ������� ���������� ���������������
		if (t > tk) break;

		// ������� �� �������� �����
		rc = KA->GoToTime(t, &SI); if (rc) break;

		// ���������������� ����������� �������� 
		PrintMng.PrintMSC(KA, &SI, NULL, NULL);

		// ����������� ���� � ������� � �������
		//cout << SI.Data << " " << SI.Time << endl;

		// ����� ������� ����� 
		t+= h;
	}
	cout << "\n";
	
	// ���������� ����������������
	PrintMng.PostPrint();
	PrintMng.ClosePrint();

	return rc;
}

//-----------------------------------------------------------------------------