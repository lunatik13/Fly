//-------------------------------------------------------------------------------
// RunUpSysBuild.cpp : 
// �. ��������� 11.12.2018
//-------------------------------------------------------------------------------
#include "stdafx.h"

//---------------------------------------------------------------------------
// ���������� Fly
#include <FlyCore.h>		// ����������� ���� ������ ��������
#include <FlyMMath.h>		// �������������� �������

//---------------------------------------------------------------------------
#include "RunUpSysBuild.h"		

//-------------------------------------------------------------------------------
// ������������ ������������ �� ��� 8-� �� ������� "������" (Run Up)
// � ���� ����������, �� 4 �� � ������ ���������. 
int RunUpSysBuild_4_4(ZNU* NU, ZLSF& LSF, int CountKA) 
{
	cout << "\n Build IS...\n";
	//--------------------------------------------------------------------------	
	// ����� ���������� ������ - ���
	// ��������� ������������� ������
	double Havr, Tz, iz, az, pz, ez, e1z, e2z ;
	int    rc ;
	// �������� �������� ������� ������
	Havr = 270 ;
	// �������������� ������ �� �������� ������
	rc = MBuildSSOMIV_hs(Havr, Tz, iz, pz, e1z, e2z, nullptr) ; RRC ;
	// ���������� ��������������� � ������� ������� ������
	ez = sqrt(e1z*e1z+e2z*e2z) ;
	az = pz/(1-ez*ez) ;

	//-------------------------------------------------------------------------
	// ������������ ������ �� 8 �� ���������� � ��������� �� 45 ��������
	ZNU NUK ;
	for (int j=0 ; j<CountKA ; j++) { 
		// "�����" ��
		NU[j].Key.ka = j+1 ; NU[j].Key.nom = 15 ; NU[j].Key.mod = 0 ;
		NU[j].Key.type = 8 ; NU[j].Key.bc = 1 ;
		NU[j].numlsf = 1 ;
		// ���� � ����� ���� �� ���������� - ��� �� ������ �� ���� ������ �������
		DATE dt = DATE(1, 4, 2018);
		TIME tm = TIME(9, 0, 0, 0) ;
		// ������1� �� ��� � ������� �����
		TA_DMB(1, &dt, &tm, &NU[j].t) ;
		// ��� �� ����� ��� 100-�� ����� 
		NU[j].vit  = 100 ;
		// ������� ��������� - ������������ �������� ������������ 
		// �������� ��������� ��������
		NU[j].sk   = _SK_OSKTE ;
		// ��������� ��������� �� ����������� ����������� �������� ��� ���
		NU[j].X[0] = TO_DTKM(az) ;
		NU[j].X[1] = ez ;
		NU[j].X[2] = TO_RAD(iz) ;
		// ��� ��������� 339.555 � 39.551 ���� ���������� �������
		if ((j/2)*2==j)
			// ��� �������� �� � �������� 1,3... (N �� =j+1) ����������
			// ������� ���� ����� 339.555 ���� 
			NU[j].X[3] = TO_RAD(339.555) ;
		else 
			// ��� ������ �� � �������� 2,4... (N �� =j+1) ����������
			// ������� ���� ����� 39.551 ���� 
			NU[j].X[3] = TO_RAD(39.551) ;
		NU[j].X[4] = TO_RAD(0) ;
		// �� � ������ ��������� ��������� �� 90 ����. 
		// � ����������� �� 45 ����. ������������ �� � �������� ���������
		NU[j].X[5] = TO_RAD(1e-15+45.0*((double)(j))) ;
		NU[j].Sb = 0.0005 ;
		NU[j].Kp = 0 ;
		NU[j].M0 = 900 ;
		NU[j].MF = 50 ;

		// �������� ��������� �� � ����� �������� ��� ���� ��
		// ���������� ���������� (������� �������, �������������� � �������) 
		// ��� "�����" �� ������ �����, � ����� ��������� �������� ������-
		// -������������ ������ (270 ��)
		cout << "\n    IS-%d....." << j + 1;
		rc = RunUpSysEstimateNU(LSF, NU[j], NUK, 0, e1z, e2z, 0, Havr) ; RRC ;
		printf("Ok") ;
		NU[j] = NUK ;
	}
	printf("\n\n") ;
	return 0 ;
}

//-------------------------------------------------------------------------------