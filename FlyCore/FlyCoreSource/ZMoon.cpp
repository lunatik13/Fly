//---------------------------------------------------------------------------
// ZMoon.cpp
// ���������� ��������� ���� � ���-2000 �� �������� ������ ������� 
// 
// �. ��������� 03.08.2018
//---------------------------------------------------------------------------
#include <stdafx.h>
#pragma   hdrstop

//---------------------------------------------------------------------------
// ���������� Fly
#include <FlyTime.h>	// �� ��� �����

#include <FlyCoreSource\\ZMoon.h>
#include <FlyCoreSource\\ZCMiniStackKeeper.h>

// �������� ��� �������� ��������� ����������� ��������� ����,
// ����������� ��� ���������� ���������� ������������ ����������
// ��� ������ � ���� �� ������� �������.
static ZCKeepper3D MoonStack(1e-3 / k_cbc);

//----------------------------------------------------------------------------
// ������ ��������� ���� 
// ��������� � ���� ������������� ��������� � ��� J2000 
// Scale:
//		TIME2000ASD  � ��� �� 2000 (0h 01.01.2000)
//		TIME1975ASD  � ��� �� 1975 (0h 01.01.1975)
//		TIMEJ2000JC	 � ��������� ��������� �� J2000 (12h 01.01.2000)
//----------------------------------------------------------------------------
int ZMoonJ2000(
	double	  ta,		// ����� �������� � ����� Scale
	double	 *Moon,		// ������������� ���������� ���� � ���-2000	
	TIMESCALE Scale,	// ����� ��������� ������� t
	bool	  notStory)	// ������ �� ����������� ����������� �������
{
	// ���������� ������� � ��������� ��������� �� ����� J2000
	double t = Astro2000Time(ta, Scale, &ta) ;

	// �������� ������������� ������� 
	if (MoonStack.IsExistResultFor(ta, Moon)) return 0;

	// ��������� ������ ��������� ����
	int rc = AEJG(ta, 1); if (rc) return rc;

	// ����������� ������ � ������ �����������
	memcpy(Moon, Wae->qlu, 3 * sizeof(double));

	// ��� ������������� ���������� ����������� ������ � "���������"
	if (!notStory) MoonStack.Story(ta, Moon);

	return 0 ; 
}

//----------------------------------------------------------------------------
