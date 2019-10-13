//-------------------------------------------------------------------------------
// ZSiderealTime.h
// �. ��������� 03.08.2018
//-------------------------------------------------------------------------------
#ifndef _ZSIDEREALTIME_H
#define _ZSIDEREALTIME_H

// ������� ������� �������� ��������� ������� �� ���������� ������� UT1
// � ������ �� 0 ����� 01.01.2000.
// ���������  - � c����� (���).
FLYCORE_API double ZAverageSiderealTime(double UT2000);
// ������ ������� ��� � �� �����������
FLYCORE_API int ZMER(double t, double* M, double* Ms);
// ������ ��������� ��������� ������� �� ���������� 
// ����������������� ������ (UTC) � ������ �� 0h 01.01.1975 �. 
// �� ���� �������� �������� ����� � ��� �� 0h 01.01.1975 �.
// ��������� - �� � ��������.
FLYCORE_API int ZTrueSiderealTime(double t, double& zw);

//-------------------------------------------------------------------------------
#endif _ZSIDEREALTIME_H