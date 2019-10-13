//-------------------------------------------------------------------------------
// TaskCalcTargDir.h
// �. ��������� 18.12.2018
//
// ������ � ���������������� �� ��� ������� � �������� ��� ��-���
//------------------------------------------------------------------------------- 
#ifndef TASK_CALC_TARG_DIR_H
#define TASK_CALC_TARG_DIR_H

//-------------------------------------------------------------------------------
// �������� ���� ��������� ���������� ��������� ��� ���������������� ��
void  TargDirFileCloseAll() ;
// �������� ����� ����� ��� ���������������� �� ��� ��������� ��
void  TargDirFileName(int NumKA, int NipInd, long Vit, char* FN) ;
// �������� ��� �������� ���������� ��������� ��� ���������������� ��
FILE* TargDirFileOpen(int NumKA, int NipInd, long Vit, double t1, double t2) ;
// ������ � ���������������� �� ��� ��������� ��� - NipInd, �� - NumKA, 
// �� ��������� ������� [t1, t2], � ���������� ��������� � ���1975 - Xasc1975
int   CalcTargDir(int NumKA, int NipInd, long Vit, double* Xasc1975, double t1, double t2) ;

//------------------------------------------------------------------------------- 
#endif // #ifndef TASK_CALC_TARG_DIR_H