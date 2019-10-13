//-----------------------------------------------------------------------
// LeapSeconds.h
// �. ��������� 11.06.2019
//-----------------------------------------------------------------------
#ifndef LEAP_SECONDS_H
#define LEAP_SECONDS_H

//-----------------------------------------------------------------------
// �������� �������� ������ �������� ������� ��� �������������
//-----------------------------------------------------------------------
struct ZSUtcIatItem {
	// ���� �������� � ���
	double IatTime;
	// �������� �������� � ���
	double dUtcIat;

	ZSUtcIatItem() { Clear(); }
	void Clear() { IatTime = dUtcIat = 0.; }
};

typedef vector<ZSUtcIatItem> VUtcIatItems;

//-----------------------------------------------------------------------
// �������� ������ �������� �������
//-----------------------------------------------------------------------
struct ZSLeapSeconds {
	bool isInit ;			// ���� ������������� ������ ��������
	VUtcIatItems IatList;	// ������ ��������
	ZSUtcIatItem Cur, Next; // ����������� ���������

	ZSLeapSeconds()  { Clear(); }
	~ZSLeapSeconds() { Clear(); }
	void Clear() { isInit = false; IatList.clear(); }
};

//---------------------------------------------------------------------------
// ������������� ������� ������ �����������
// forSureLoad - ������� �������������� �������� (true)
//				 ���� false �� ��� ���������� ����� ���������� ������������
//				 �� �����������
//		������ ������� ����� � �������� ������ �����������
//		3653.0 - 23.0
//		4748.0 - 24.0
//		5479.0 - 25.0
//		5844.0 - 26.0
//		6391.0 - 27.0
//		6756.0 - 28.0
//		7121.0 - 29.0
//		7670.0 - 30.0
//		8217.0 - 31.0
//		8766.0 - 32.0
//		11323.0 - 33.0
//		12419.0 - 34.0
//		13696.0 - 35.0
//		14791.0 - 36.0
//		14975.0 - 37.0
//
//		������� ����� ������ ������� ����������� � ��� �� 1975 ���� �
//		�������� ����� ������� �����������
FLYTIME_API int     LeapSecondsInit(bool forSureLoad=true);

//---------------------------------------------------------------------------
// ����������� �������� ������ ����� ����������� �� �������� ������ IAT
// �������� ������� - dTime, ��������� � ��� (���� �������������� �� 1975 ����)
FLYTIME_API double  GetLeapSeconds (double dTime);

// ����������� �������� ����� � ��������� ������ �����������
FLYTIME_API void    SetLeapSecondsPath  (const char* path);
// ����������� ����� ����� � ��������� ������ �����������
FLYTIME_API void    SetLeapSecondsSource(const char* source);
// ������� ��� �������� � ����� � ������� � ������� ������� �����������
FLYTIME_API char*   GetLeapSecondsPath  ();
FLYTIME_API char*   GetLeapSecondsSource();

#endif //#ifndef LEAP_SECONDS_H