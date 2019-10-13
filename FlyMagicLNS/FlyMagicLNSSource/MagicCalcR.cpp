//-------------------------------------------------------------------------------
// MagicCalcR.cpp: 
// �. ��������� 23.08.2018
// ������ �������� � ������� ������ ������ ��� ������ ������� �����
//-------------------------------------------------------------------------------
#include <stdafx.h>

//---------------------------------------------------------------------------
// ���������� Fly
#include <FlyMMath.h>		// �������������� �������
#include <FlyCore.h>		// �������������� �������
#include <FlyMagicLNS.h>	//

//------------------------------------------------------------------------------
// ������� ������� �������� ������ � ������� �����
int MagicCalcR(double i, double p, double e1, double e2, double* R)
{
    int rc = 0 ;
    double ski = pow(Sin(i),2) ;
    R[0]= 2.0*(p*e1/(1.0-e1*e1)-k_eps/k_mu_kms/p*(1.0-2.0/3.0*ski)) ;				//r1
    R[1]= 2.0*p*e2/(1.0-e2*e2) ;													//r2
    R[2]= 0.5*p*(1.0/(1.0-e1*e1)+1.0/(1.0-e2*e2))-k_eps/k_mu_kms/p*(1.0-0.5*ski) ;	//rcp
	R[3]= 0 ; 
    return rc ;
}

//------------------------------------------------------------------------------