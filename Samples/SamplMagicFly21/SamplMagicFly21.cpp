//-----------------------------------------------------------------------------
// SamplMagicFly21.cpp
//
// �. ��������� 20.06.2019
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
// ���������� Fly
#include <FlyCore.h>
#include <FlyMMath.h>		// �������������� �������
#include <FlyMagicLNS.h>	// ����� ������� �����
#include <FlyCoreLink.h>

int TestMagicFly21(int TypeOrb);

int TaskMagicFlyBegin(ZSMagicLnsSolution* MS, int task, ZMSC& KA, ZNU& NU,
	ZLSF& LSF, bool ResetNU, ZCPrintManager* PrintMng,
	FILE** pfp);

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

	//
	int TypeOrb = SPO_ORB_HBH;
	TestMagicFly21(TypeOrb);
}

//-----------------------------------------------------------------------------
int TestMagicFly21(int TypeOrb)
{
	// ������������� ������ (����� ������� ����� �������� ��������������)
	int indexTask = 21;
	int rc = 0;

	//---------------------------------------------------------------------------
	// ������ "������ �������"
	// �� ���� ����� ������ ������� ��� ���������, ������� ��������: 
	//		- �������������� ������������ ���������, 
	//		- ������ ���������� ������ (�����),
	//		- ������� �� ������� ������ (������ ����������, ������ ���������� 
	//		  ���������� � ������������� ������).
	// ... ����� ������� ��� ����� ������ ����� �������� ����� �������.
	//---------------------------------------------------------------------------
	ZSMagicLnsSolution* MS = CreateMagicLnsSolution(); if (!MS) return 1;

	//---------------------------------------------------------------------------
	// TypeOrb - ��� ��������� ������
	//		SPO_ORB_EL	- ��������� ��������� ������ ������� ��� e, w.
	//		SPO_ORB_HBH	- ��������� ��������� ������ ������� ��� Hmin, Bhmin.
	//		SPO_ORB_OMIV- ��������� ��������� ������ ������������ �����������.
	//					  ��������� ������ �� ����� (e, w ���������� ������ �� 
	//					  ������� ����).
	//		SPO_ORB_DU	- ��������� ��������� ������ ������������ ������������
	//					  ������������ (e, w ���������� ������ �� ������� ��).
	//---------------------------------------------------------------------------
	MS->MZ.ORB() = TypeOrb;

	//--------------------------------------------------------------------------
	// ������������� ��, ��� � ������ ��������, ��������� ����������������
	//--------------------------------------------------------------------------
	ZNU   NU;			// �� �������� ��.
	ZLSF  LSF;			// ���������� ����� ��� � ������� ����������� �������. 
	ZMSC  KA;			// ������ �������� (��� ������ ��)
	FILE* fp = nullptr;	// ���� ��� ���������������� ����������� �������
	// ��������� ����������������
	// �������� �� �� ��� ��������� ���������������� ZCPrintManager ��� ����������
	// ��� ���������������� ����������� ��������, �� ������� ��� ���������� �
	// � ���������� �������, � ����� ���������� �������� ���������, ������ 
	// ��������� ���������, ���������������� �� � ���, ���������������� 
	// ���������� � ������� ������� � ��� �������������, ���������������� 
	// ���������������� ������ � �������� �������� (������ ������ � �������� 
	// ����). 
	// � ����� ��������� ���������������� ��� �����!
	ZCPrintManager PrintMng;

	// ������������� �������� ������ ����������� ��� ������� �������� �����
	// ������� ���������� ������������ �������������� - ������� �� ������
	// "�������������" �����. 
	// ������ ����� ����� ���
	// ���� �������� �������� ����������� ��� TaskMagicFly__.
	// � "Prodaction" ������������� � ������ ������ ��������. ��� ��� 
	// �������������� �������� ������� �� GUI 
	// �/��� �� �������� ������������� ���.
	rc = TaskMagicFlyBegin(MS, indexTask, KA, NU, LSF, true, &PrintMng, &fp);
	if (rc) goto end;

	//--------------------------------------------------------------------------
	// ������������ ������� �� ����, "������������" ����� (����������� � 
	// ������������� ������ � �������������� GUI)
	MS->MZ.VITIMP(0) = 2;		// ����� ������� ��������
	MS->MZ.VITIMP(1) = 4;		// ����� ������� ��������
	MS->MZ.VIT() = 5;		// �������� ����� �� ������� �������������� 
	// ������� �������
	// ��������� ��������� ������
	MS->MZ.PARL() = 131.2;		// ������� ��
	MS->MZ.PART() = 5503.0;		// ������������� ������
	MS->MZ.PARh() = 210.0;		// ����������� ������
	MS->MZ.PARBh() = 10;			// ������ ����������� ������
	MS->MZ.PARBv() = 1;			// ���������� �����
	MS->MZ.PARe() = 0.0002;		//	
	MS->MZ.PARw() = 90.0;		//	

	// ������� ���� ��������� ����� ������ ��� ���� �����
	// �� ��������� �������� ���������� �������� ����������� ������ � � ������
	MS->MP.isEstimateOrb = true;

	//--------------------------------------------------------------------------
	// ������� ������ ������� ���������� ��������� �� ������� MZ 
	{
		ZSLoopFact  P1;	// ��������� �������� �� ����� ������� ��� 
		ZSLoopFact  P2;	// ��������� �������� �� ����� ������� ��� 
		ZSLoopFact  PN;	// ��������� ��������� �������� �� �������� �����
		ZSLoopFact  PR;	// ��������� �������� ��������� �������� �� �������� ����� 
		ZSFlightRule U;	// ���������� ����������

		// ��������� ������������� ������� ��������������� ������ ������� �������
		// ������� �����. ������ ��������� �������� ������ ��� ���� �������� �����
		// ����������� ��� TaskMagicFly__. 
		rc = MagicSolution(MS, &PrintMng, fp, LSF, KA, NU, nullptr,
			&P1, &P2, &PN, &PR, &U);
		// �������� ��������� � ������������ �������
		PrintMng.ClosePrint(&fp);
	}
end:;
	//---------------------------------------------------------------------------
	// � ���������� ������� ��������� ����� "������ �������"
	FreeMagicLnsSolution(MS);

	return 0;
}

//-----------------------------------------------------------------------------