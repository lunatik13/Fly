//-------------------------------------------------------------------------------
// ZMSpaceCraft.h
// �. ��������� 03.08.2018
//------------------------------------------------------------------------------- 
#ifndef ModKA_H
#define ModKA_H
//------------------------------------------------------------------------------- 
#include <FlyMMath.h>
#include <FlyTime.h>

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModAstro.h>
#include <FlyCoreSource\\ZMForceGrav.h>
#include <FlyCoreSource\\ModSK.h>
#include <FlyCoreSource\\ModPUManager.h>
#include <FlyCoreSource\\ZCEventManager.h>
#include <FlyCoreSource\\ZCIntegrator.h>
#include <FlyCoreSource\\ZSK.h>
#include <FlyCoreSource\\ZSKNU.h>
#include <FlyCoreSource\\ZPR.h>
#include <FlyCoreSource\\ZNUT.h>
#include <FlyCoreSource\\ZSun.h>
#include <FlyCoreSource\\ZAvrE0.h>

//------------------------------------------------------------------------------- 
//
//------------------------------------------------------------------------------- 
#define COUT_MAIN_EQUATION	8	// ����� �������� ��������� (����� + 6 �������������� ���������� + �����)

// ���������� �������� �� �� �������� ������ �������
// � ��������� �������� ��������� ����������� ����� ������� ���� ��������������
// � �� ����� ���������� �� ���� �������
typedef struct ZSC_SI {
	ZMOD_EVENT E ;				// ������� ��� �������� �������� ������ ������ ���������� ��������
								// �������� ������ �����, ��������� ���, ����������� �����, ����������� ������ � �.�....
	long Vit75 ;				// ����� ��� ��� 1975, 2000 �����, 
	long Vit00 ;				// ������� � ������� ������� �����
	long VitTE ;
	long VitTES ;

	// ��������� ������� � ��, ��/���, ���
	double GSK[6] ;		// ���������� � �������� � ���
	double ASKTES[6] ;	// ���������� � �������� � ��� ������� ����� �������
	double ASKTE[6] ;	// ���������� � �������� � ��� ������� ��������
	double ASK1975[6] ;	// ���������� � �������� � ��� ����� 1975 ����
	double ASK2000[6] ;	// ���������� � �������� � ��� ����� 2000 ����
	double OSK1975[6] ;	// ������������ �������� ������ ������������ �������� ����� 1975 �.
	double OSKTE[6] ;	// ������������ �������� ������ ������������ �������� ������� �������� �����
	double OSKTES[6] ;	// ������������ �������� ������ ������������ �������� ������� ������� �����
	double OSK2000[6] ;	// ������������ �������� ������ ������������ �������� ����� 2000 �.
	double THBLFR[9] ;	// 0 - ������ ���. � ��������, 1 - ������ � ��, 2 - B, 3 - L, 
						// 4 - fi � ��������, 5 - R � ��,
						// 6 - ����������� �� ������ ��/���, 7 - ����������� �� ������� ��/���.
						// 8 - ���������� ��������������� ����� �����

	bool   isOSK1975bpl ;	// �������� ������� ������������ ��������� ��� ��������������
	bool   isOSK2000bpl ;	// ������� ��������� � �������� � ��������� ������
	bool   isOSKTEbpl ;
	bool   isOSKTESbpl ;

	double OSK1975bpl[6] ;	// ������������ �������� ������ ��� �������������� 
	double OSKTEbpl[6] ;	// ������� ��������� � �������� � ��������� ������
	double OSKTESbpl[6] ;
	double OSK2000bpl[6] ;

	// ������������� ��������� ������� ���������������� ���������
	double ExternalEq[EXTEQ_MAXCOUNT] ; // �������� ������� ����������
	int    ExternalEqCount ;// ����������� ������� ���������
	bool   ExternalEqOn ;	// ������� ���������� ������� ��������� ("��������� ��������")

	double SA[25] ;			// �������������� ������������ ("���� ������"). ��� ���� � ��������

	double   m ;			// ������� ����� ��, ��
	double   tmss ;			// ������� ������� ��������� �����, ���
	double   ta ;			// ����� �������, �� ����� �� ������� �������� ������ ������ ����������
	DATE     Data;			// ���� ���������������� (���� ����� � ���)
	TIME     Time ;			// ����� � ����� ������� � �������� (���������� ���������)
	TheTimes T ;			// �� ��� ����� � ������� ;
} _ZSC_SI ;

// ��������� �������� �� � ����������� ����� �����, � ������
// � ������ ������������/����������� ������ ��� �������
typedef struct ZSC_VP {
	bool   isFind ; 
	double tmssvv ;	//
	double R ;		// 
	double H ;		// ������
	double B ;		// ������
	double L ;		// �������
	double t ;		// �����
	double ute ;	// �������� ������ ����������� �� �������� ������� �����
} _ZSC_VP ;

// ������������ ��������� �������� �� �� �����
typedef struct ZSC_VI {
	double Td ;		// ������ ������������� � ��������
	double dL ;		// ����������� ����� � ��������
	double dLs ;	// �������� �������� ����� � ��������
	int    Ks ;		// ����� �������� ������
	double HAvr ;	// ������� ������ ������ � ��
	double exAvr ;	// ������� ���������� ������� ��������������� 
	double eyAvr ;	// e*cos(w)   e*sin(w)
	double ixAvr ;	// ������� ���������� ������� ����������
	double iyAvr ;	// cos(W)*tg(i/2)   sin(W)*tg(i/2) 

	ZSC_VP UN ;			 // ������������ ��������� � ��
	ZSC_VP DN ;			 // .... � ��
	ZSC_VP Hmax, Hmax2, Hmax3 ; // .... � ������ ������������ ������
	ZSC_VP Hmin, Hmin2, Hmin3 ; // .... � ������ ����������� ������
	ZSC_VP Rmax, Rmax2, Rmax3 ; // .... � ������ ������������� �������
	ZSC_VP Rmin, Rmin2, Rmin3 ; // .... � ������ ������������ �������
	ZSC_SI FSUN ;// �������������� ��������� � �� (������ �����)
	ZSC_SI FSDN ;// �������������� ��������� � ��
	ZSC_SI FSTE ;// �������������� ��������� � ����� ����� (� ��)
} _ZSC_VI ;

// ����� ����������� �������� ��� ���������� �� ����� �� ���������
// ��������� ��� ���������� ������ ��� ������� ������
// !!!! � ����� ������ �� ����� ����� ���� �� 1-�� �� 3-� ���������
// ��������� ��� ���������� ������� ��������� !!!!
FLYCORE_API ZSC_VP* HMIN_VI(ZSC_VI& I);
FLYCORE_API ZSC_VP* HMAX_VI(ZSC_VI& I);
FLYCORE_API ZSC_VP* RMIN_VI(ZSC_VI& I);
FLYCORE_API ZSC_VP* RMAX_VI(ZSC_VI& I);

//------------------------------------------------------------------------------- 
class ZMAtmSMA81 ;
class ZMAtmSMA62 ;
class ZMAtmSMA440181 ;
class ZCISAManager ;

//-----------------------------------------------------------------------------
// ����� ������ �������� ��
// "Z" Model of SpaceCraft
//-----------------------------------------------------------------------------
class FLYCORE_API ZMSC : public ZCIntegrator {
public:
	friend class ZModNotice;
	friend class ZCEventManager;
	//-------------------------------------------------------------------------
	// ��������� �������� ���������� 
	// ��������� ��������� ������������� ������������ �������� ���������� 
	// ��������� ������ ���� ��, � ��� �� ������������� ������ 
	// ���������� ��������� ������������� ������� ��� ��� 
	// ���� ������������ ��������� ��
	//-------------------------------------------------------------------------
	ZPUManager MPU;

	//-------------------------------------------------------------------------
	// �������� ������� ������ ��������
	// ������������� ����� ��������� ������� � �������� ���� 
	// �������������� ���� ��� ����� ����������� �������.
	// ����������� ��������� �������� ������� ���c/��� ������ � ������� 
	// � ������ ������������� ����������� ������� GoAlongVit, � ��� �� 
	// ������� ��� � ������ ������ ��� � ������� �� ������������� 
	// (��� ������ ����� RunUpZRV) . 
	//-------------------------------------------------------------------------
	ZCEventManager EventMng;

protected:
	ZKA    KA;				// ������ � �� 
	ZNU	   NU;				// �������� ��
	ZLSF   LSF;				// ���������� ����� ���
	int	   HStoryClarify;	// ���� ��� ���������� � �������������� 
	// ��������� ������ �������� � �������� 
	// �������������� �������� ��������� ��������� 
	// �������� ���������� ��������. ��������: 
	// ��������� ������� ��� ��� ���, �������� 
	// ������������ ��� ����������� ������...
	long   Vitok75;			// ����� � ��� 1975
	double TestVit75Z0;

private:
	//
	ZModNotice*	    m_pNotice;
	// ��������� �������� ��������� ����������
	ZCISAManager*   m_ISAManager;
	// ������ ������������ ��������� ����������� 81 ����
	ZMAtmSMA81*     m_AtmSMA81;
	// ������ ������������ ��������� ����������� 62 ����
	ZMAtmSMA62*     m_AtmSMA62;
	//
	ZMAtmSMA440181* m_AtmSMA440181;
	// �������������� ���� �����
	ZMForceGrav	    GPZ;

	// ������� ��������� �������� (�� ���� ��� �������� ����)
	// ���� ��� �������� ��������� ���������� �������� � ������ ��
public:
	ZSC_SI FSC;	// ��� �������� ���� (����������)
	ZSC_SI FSP;	// ��� ����������� ���� (��������������)
	ZSC_SI FSR;	// ��� ����������� �������� - ����� �� �����, 
	// �� �������� ��� �� �����
	// !!!! ����� ������ ����! ��� ��������� ���������� 
	// ��������������� ���������� ����������� �-��:
	// GoAlongVit, GoToVit, GoToVitU, GoToU, GoToDU
	// GoToTime, GoDTime. !!!! 
	ZSC_SI FSE;	// ��� ���������� ����������� ������� � �������� 
	// ���������� ���� (�� FSP.ta �� FSC.ta)
	ZSC_VI FV;	// ���� ��� �������� ���������� ����� ��� ��������������� 
	// �������� �� ������. ����������� ���������� 
	// ������������� �������� �� ����� 
	// ��������� ����� GoAlongVit.

public :
	ZMSC ( ) ;
	~ZMSC( ) ;

	//-------------------------------------------------------------------------
	// ������������� ������
	//		Nu		- ��������� �������.
	//		NuKey	- ����� ��������� �������.
	//		lsf_	- ���������� ����� ��� � ������� ����� ����������� 
	//				  �������������.
	//		EED		- �������� ������� ������� ����������.
	int Init(ZNU& Nu, ZLSF* lsf_=nullptr, 
			 ZCExternalEquationsDefinition* EED=nullptr) ;
	int Init(ZKEYNU& NuKey, 
			 ZCExternalEquationsDefinition* EED=nullptr) ;

	// ��������� ��� ��������������� ��������� ���������� � �������������
	// ������ �������� ������� ����� ��� � �������������. �������� ���� 
	// ��������� ����� ��������� ��������� ������ ����� ������� ������
	// Init. ����� ������ ������� ������ �������� ����� ��������� 
	// �� �� ��� �� ������
	struct {
		// �������� ������������� ��� �������������� ���. 
		// ���� �������� ������� ���� ������ ����, �� ��� �������������� 
		// ��� ������������� ��� �������������� ��� � ��������. 
		// ���� defautFixedStep>0, �� ���������� ��� �� �������� ������������� 
		// (�� ����������� � �� ���������), ������ ����� ����������� 
		// � ���������� ����� defautFixedStep.
		double defautFixedStep;
	} PreInitInfo;

	//-------------------------------------------------------------------------
	int GetNumKA() const { return KA.num ; } ;
	int GetNumNU() const { return NU.Key.nom; };
	ZKEYNU GetKeyNU() const { return NU.Key; };
	// ��������� ������� �� � ���������� ��������
	void   GetLSF(ZLSF& lsf) const;
	void   GetKA(ZKA& ka) const;
	void   GetNU(ZNU& nu) const;
	int    SetNU(ZNU& nu);
	double GetTime() const;
	long   GetVitok(SK_TYPE sk=_SK_ASKTE, ZSC_SI* F=nullptr) const;
	double GetU    (SK_TYPE sk=_SK_ASKTE, ZSC_SI* F=nullptr, 
					bool isbpl=false) const;
	int    GetVector(double* X, long* Vit=nullptr, double* t=nullptr, 
					double* m=nullptr, SK_TYPE sk=_SK_ASK1975) const;
	// ������ �������� �������� ������� ��������� ��
	int    SetVector(double* X, long Vit, double t, double m, 
					SK_TYPE sk=_SK_ASK1975) ;
	// ��������� ��� �� (����� � � ��������)
	double WeightNU() const;
	double WeightFullNU() const;
	// ������� ��� �� � ��������
	double WeightFull() const;
	// ������� ��� �������
	double WeightFuel() const;

	//-------------------------------------------------------------------------
	// ��������������� �� �������
	// ������� �� �������� ������ �������
	virtual int GoToTime(double  t, ZSC_SI* SI=nullptr) ;
	// ������� �� �������� �������� �� �������� ������� �������
	virtual int GoDTime (double dt, ZSC_SI* SI=nullptr);
	// ���� ��� �������������� � �������������� �������
	// ���� ������ �������� notFurtherTime, �� ��� ����� �������� ����� �������
	// ��� ���������� ������� ����� ������ ���� ����� notFurtherTime
	// ������������� ������������ ��� ���������� ������������ ���������� ������ 
	// �� �������� ������ ������� - ������������ ������� GoToTime() 
	virtual int GoStepA (ZSC_SI* SI=nullptr, int dir=1, 
					double* notFurtherTime=nullptr, 
					ModEventName* BreakKey=nullptr) ;

	//-------------------------------------------------------------------------
	// ��������������� �� ��������� ������ � �� ������
	// ������� �������� �� ����� � ������������� ����������� �� ����� �����
	// � �������� ������������ ������������� �����
	virtual int GoAlongVit(long vit, SK_TYPE sk=_SK_ASKTE) ; 
	// ������� �� ��������� ����� 
	// (���������� ��� ���������� ����) � �������� �����
	virtual int GoToVit (long  vit, ZSC_SI* SI = nullptr, bool fronNUZ = false, 
					SK_TYPE sk = _SK_ASKTE);
	// ������� �� ��������� ����� � ��������� ������ (����) � �������� �����
	virtual int GoToVitU(long  vit, double u, ZSC_SI* SI = nullptr, 
					SK_TYPE sk = _SK_ASKTE);
	// ������� �� ��������� ��������� ������ (����) � �������� ����� �� 
	// ������� �����
	virtual int GoToU(double  u, ZSC_SI* SI = nullptr, 
					SK_TYPE sk = _SK_ASKTE);
	// ������� �� �������� ������� ���������� (����) ����� ������ �� �������� ���������
	virtual int GoToDU(double du, ZSC_SI* SI = nullptr);

	//-------------------------------------------------------------------------
	// ���������� ��������� ������������� ���������� ������������� �������
	// ���������������� ��������� - ����������� � �������� ���������� ��������
	// �������������� (�������) ��������� - ���������� ����������� ������ ����
	// 	
	// ������� ���������� ����������� ���������� 
	// ZExternalEquationsDefinition* EED
	// ��� ������ ������ ������������� ������ �������� Init. ����� ������� 
	// ������ ������� ��������� ������������ ������ � ������ �������� ������ 
	// �������� � � ���������� "�� ����" ������� ���� �� �����. ��� ��������� 
	// ������� ������� ��������� ������� ����������� ������ ������ ��������.
	// !!!! � ��������� ���� ���������� ����������� ��������� !!!!
	//-------------------------------------------------------------------------
	// �������� �������������� ������� ���������
	//		NU - ��������� ������� ��� ������� ���������� 
	virtual void ExternalEquationsOn (double* NU=nullptr) ;
	// ��������� �������������� ������� ���������
	//		X  - �������� ������� ���������� � ������ ���������� 
	//			 ������� ��������� 
	virtual void ExternalEquationsOff(double* X=nullptr) ;
	// ����������� ������� ��� ���������� ������� ������ ������ 
	// ������� ���������
	//		t	  - �����,
	//		mainX - �������� ������ (�������������� ��������� � ����� ��),
	//		Xexe  - ������� ����������,
	//		Yexe  - ����������� ������� ���������.
	virtual int  ExternalSph(double t, double* mainX, double* Xexe, 
					double* Yexe) ; 

	//-------------------------------------------------------------------------
	// ���������� � �������������� ��������� ������ ��������
	// ��� ���������� ������ �������� ���������� ������������ ��������� ������:
	//		- ������� ������ ������� ZModIntegrator::m_a[0], 
	//		- ������ �������������� ���������� � �� �������������� ��� 
	//		  (���-1975) ZModIntegrator::m_a[1, 2, 3, 4, 5, 6],
	//		- ������� ����� � �� �������������� Vitok75, 
	//		- ��������� ��������� ���� FSC, 
	//		- ��������� ��������� ����������� ���� FSP, 
	//		- ��������� ��������� ���������� ���������� ������� FSE, 
	//		- ��������� ��������� ����� FV
	//-------------------------------------------------------------------------
	virtual int  Story    (int  HStory=0) ; 
	virtual int  ReStory  (int  HStory ) ; 
	virtual void FreeStory(int* HStory=nullptr) ; 

	// ������ � ��������� ��������������� ����� (��� ������������� �������� 
	// �� ������) MKA_VI* GetFV() { return &FV ; }
	// ������ � ��������� ���������� �������� � ������� ������ ������� 
	// (��� ������������� �������� �� �������)
	// ������� ���������� ��������� ��������� ���� �� ������ ���������� ����,
	//ZSC_SI* GetFS() { return (FSE.E.ID>0 ? &FSE:&FSC) ; }
	// ������� ���������� ��������� ��������� �������� �� ������ 
	// ����������� ���� ����,
	//ZSC_SI* GetFSP() { return &FSP ; }

	//-------------------------------------------------------------------------
	// ������ ��� �������� ���, ������ ������������ �� ����, �� ���� 
	// ������������ � ������� ��������� ���������� ��������, ������� 
	// ��� (����, �����, ��������) 
	//-------------------------------------------------------------------------
	// ������������� ������ ������������ ���
	int RunUpZRV(int CountNip, int* NipIndexes ) ;
	// ���������� ������������ ���
	void CompletZRV() ;

	//-------------------------------------------------------------------------
	// ������ ������������ ������� �����
	// ������������� ������ ������������ ������� �����
	int RunUpVit() ;
	// ���������� ������������ ������� �����
	void CompletVit() ;

protected :
	// ���� ��� �������������� - ��� ����� (������������ ��� ��������� 
	// ������������� �������)
	virtual int BackStep(ZSC_SI* SI, double h, 
						 ModEventName idevent=ModEvent_EMPTY) ;
	// ���������� �� ������� ��� ������� �� ��� (� ������ ���� ������ ����� ��, 
	// ��������� ������� ��������� ��� �������������� ����������� �������)
	int RestartFromEvent() ; 
	int CheckVitEvent(ZSC_SI* SI=nullptr, bool fronNUZ=false, SK_TYPE sk=_SK_ASKTE) ;  

	void Clear( ) ;
	// ��������� �������
	virtual int StepBreak(double t, double* X) ;
	// ��������� ������� ������
	virtual int OnMessage   (long ModMsgID, double* t, double* X) ;
	virtual int OnEvent     (ZMOD_EVENT& E, ZSC_SI* PSE) ;
	virtual int OnVitokEvent(ZMOD_EVENT& E, ZSC_SI* PSE) ;
	virtual int OnUserEvent (ZMOD_EVENT& E, ZSC_SI* PSE) ;
	virtual int OnZRVEvent  (ZMOD_EVENT& E, ZSC_SI* FSC) ;
	virtual int OnFullZRV   (ZZRV* ZRV, ZSC_SI* FSC) ;

	virtual int Sph(double* X, double* Y) ;
	virtual int StepCalc(char ForTheCase=1, ModEventName idevent=ModEvent_EMPTY, 
						 ZSC_SI* SI=nullptr) ;
	virtual int CalcAstroKoord(double t, int NP) ; 

	long   VitSK	 (double* Ask1, long V1, double* Ask2) ; 
	int    ClarifyVit(ZSC_SI* SI, double tp, double Zp, double tt, double Zt, 
					  bool fronNUZ=false, SK_TYPE sk=_SK_ASKTE) ;

	// ������� ���������� (�� ��������� �������) 
	// � ������� �������������� (����� ��������� �������) ��������� �����������
	// !!!! ������ ����������� � �� ���� ������ �������� !!!!
	void   FastStory  () ; 
	void   FastReStory() ; 
	double FastX[EXTEQ_MAXCOUNT+COUT_MAIN_EQUATION] ;
	double FastGI[2][EXTEQ_MAXCOUNT+COUT_MAIN_EQUATION] ;

	void ASKtoASKbpl(double* X, double* Xbpl) ;

private :
	void   PreTestVit () ;
	void   TestVit    () ;
	double KoordVit   (double* X=nullptr) ;
	double KoordVVit  (double* X=nullptr) ;
	double KoordVitSK (ZSC_SI* SI=nullptr, SK_TYPE sk=_SK_ASKTE) ;
	double KoordVVitSK(ZSC_SI* SI=nullptr, SK_TYPE sk=_SK_ASKTE) ;
	int    TestEpsVit (SK_TYPE sk1, double* Ask1, long Vit1, 
					   SK_TYPE sk2, double* Ask2, long& Vit2) ; 

	// ������� FillStep... ���������� ������� ����� ������� ���� ���
	// ������� ���� ��������� ���������� �������� � ������ �� �������� 
	// � ��������� ZSC_SI* SI.

	// ������������ �������������� ���������� �������� � ���-1975 ���� 
	// � ������������ ��������� � ���� �����
	int FillStepInfo75 (ZSC_SI* SI, int var) ;  
	// ������������ �������������� ���������� �������� � ���-2000 ����
	// � ������������ ��������� � ���� �����
	int FillStepInfo00 (ZSC_SI* SI, int var) ;  
	// ������������ �������������� ���������� �������� ������������
	// ��������� � ��� ������� �����
	int FillStepInfoTE (ZSC_SI* SI, int var) ;  
	// ������������ �������������� ���������� �������� ������������
	// ��������� � ��� ������� ������� �����
	int FillStepInfoTES(ZSC_SI* SI, int var) ;  
	// ������������ �������������� ���������� �������� � ���
	int FillStepInfoGSK(ZSC_SI* SI) ;  
	// ������ ������������� �������, ������, ������������� ������ � �������
	int FillStepInfoTHBL(ZSC_SI* SI) ;  
} ;

//-----------------------------------------------------------------------------
// !!!! ������� ������ ZMSC ��� �� �������� � ����� �������� ���������� ����� 
// ����� � ��� ��������� �� ���������� � ������� ������ �����. �������
// ������������� ��������� ������ ������� � ����. ��! ����� � ����������� ����
// ���������� ������� �� ��������� �������� �������� �� ������. ��� �� ����� 
// ������ � ������������ ����������� ���. �������� ����� ���������
// �����-���������, ��� ��������� �� ������� ... ��������
// ������ ZTSelfRemovPointer, ������� ����������� ��������� �� ����������� 
// ������, ��� ������ �������� ������������� ��� �������� ����������
// ZTSelfRemovPointer � ��� �� ������������� ��������� ��� �������� 
// ZTSelfRemovPointer, �������� ��� ������ �� ������� ��������� ������������
// ���� � ������� ������ ZTSelfRemovPointer
//-----------------------------------------------------------------------------
#define PZMSC ZTSelfRemovPointer<ZMSC>

//-----------------------------------------------------------------------------
// ������
// � ����������� ���� ����� ��������  
//  1)
//		ZMSC KA;
//		KA.GoTo...
//		. . . .
//		�� ��� �� ������� � ����� ������ ������������� �����
//  2)
//		ZMSC* KA;
//		KA.GoTo...
//		. . . .
//		delete KA;
//		� ����� ������ �������� � ���� ��������� �������
//		����������� �������� delete KA ���� �������������� ������� 
//		����� ����� �������
//	3)
//		PZMSC KA;
//		KA->GoTo...
//		. . . .
//		� ����� ������ ���� ������� ������ �������� �� ������ �� �����
//		� ������� ����� ����������� �� �����.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ������� ������������ ������, ����������� �� ZModKA � ����� "���������������"
// � (���) ������������� ���������� ������� ���������������� ���������.
// 
// "������������" ����������	- ���������� ����� ��������� � �������
// "��������������" ����������	- ���������� ����� ��������� � ������ �����
//								  ���������������� ��������� �������
// ������
// ZMSCvex(ZModAvr)	-	���������� ������ ZModAvr ������������ ��������� ������
//						������������� ���������������� ���������
// ��� ���������� ������ ���������� ����������� ����������� ������� 
// int ZModAvr::ExternalSph(double t, double* Xmain, double* Xexe, double* Yexe) 
// ���:		t		-	������� �����,	 
//			Xmain	-	������ �������� ���������� �������� 
//							(X,Y,Z, Vx,Vy,Vz, m),	 
//			Xexe	-	������ �������������� ���������, ��� ����������� ����� 
//						����������� �������������� ��������� � ���������������
//						������� ������ �������� 
//							SetExternalEquationsCount(int)
//						�� ������� ������������� ������ (�� ���� �� ������ ������
//							Init)
//			Yexe	-	������ ����������� �������������� ��������� ��� ������� 
//						�������	
//							Yexe[i] = d(Xexe[i])/d(t).
//
// �������������� �������������� ���������� ����� "��������" � "���������" 
// � ������ RunTime. ������������ (����������) �������������� ��������������� 
// ��� ������ ������
//							ExternalEquationsOff(double* X=nullptr) 
// ��� � ���������� X ����� ��� ������������� �������� ������� �������� 
// �������������� ����������.
// ������������� �������������� �������������� ���������� ����������� 
// ����������� ������ ������ 
//							ExternalEquationsOn (double* NU=nullptr) 
// ��� � ���������� NU ����� ������ ����� ��������� �������� �������������� 
// ����������, ���� ��������� �������� �� ��������� �� � �������� ������� 
// ������ �������� ����� ������������ ��������� ��������� ���������� ��������.
// 
// ��� ������������� ������ �������� � �������� ��������� �������� 
// �������������� ���������� ����������� ������� ��������.
// 
// �������� �������� �������������� ���������� � ����� ������ ������� �����, 
// ������ ����� ����������� ZIntegrator 
//							void  GetVectorExe(double*)
// ������ �������� ����� ��� ������ �������
//							int   SetVectorExe(double*) 
// ������� �� ��� �������������� ������� �������� �������������� ����������
// ����� ����� ����� ������ ������ ������ ��� � ��� ��������� "����������"
// ���������� ��������� �������������� ��� �� ����� ����������� �� ����� 
// �������������...
//
//-----------------------------------------------------------------------------
// ������� ���������� 
// 
// ����� ZModelExVar ����������� �� ZMSC ��� �������������� 
// N �������������� ���������
//
// ���������� ������ � ������������� ���������� ������ ������� ������ ������
// �������������� ���������������� ��������� 
//
//			ZMSC_EXV_BEG(ZModelExVar)
//				// ������ ����� ������� ���������������� ���������
//				// ������� ���������:  t, Xmain, Xexe.
//				// ���������� �������: Yexe. 
//				.............
//				Yexe[0] = ... ;
//				Yexe[1] = ... ;
//				.............
//				Yexe[N] = ... ;
//				.............
//			ZMSC_END
//
// ����� ZModelExForce ����������� �� ZMSC ��� ���������� ������� ����������� 
// �������� ������� ���������� ��������� ��� ������������� �������� ��
//
// ���������� ������ � ������������� ���������� ������ ������� ������ ������
// �������������� ���������������� ��������� 
//
//			ZMSC_EXH_BEG(ZModelExForce)
//				// ������ ����� ���������������� ���������
//				// ������� ���������:  t, X, N, Y.
//				// ���������� �������: Y. 
//				// N - ����� ���������, �������� ����� 7(��� X,Y,Z,Vx,Vy,Vz,m)+ 
//				//     + ����������� ������� ��������� 
//				// �� ����� � ������� Y �������� ����������� ���������� 
//				// �������� (� ��� ����� � ��������������) �� �������, 
//				// ������������ � ������������ � ������� �������� �� ZMSC �
//				// ������� ExternalSph. 
//				// ������ ����������� ����� ���� ��������� ������ ����������.
//				..........
//				Y[0]+= ... ;
//				Y[1]+= ... ;
//				Y[2]+= ... ;
//				..........
//				Y[7]+= ... ;
//				..........
//			ZMSC_END
//
// ����� ZModelEx, ����������� ��� ������������ ��� � �������������� ����������
// ���������� ������ � ������������� ���������� �������
//
//			ZMSC_EXHV_BEG(ZModelEx)
//				// ���������� �������������� ���������������� ���������
//				.............
//				Yexe[0] = ... ;
//				Yexe[1] = ... ;
//				.............
//				Yexe[N] = ... ;
//				.............
//			ZMSC_EXTFORCE(ZModelEx)
//				// ���������� �������������� ����������� ��������
//				..........
//				Y[0]+= ... ;
//				Y[1]+= ... ;
//				Y[2]+= ... ;
//				..........
//				Y[7]+= ... ;
//				.......... ;
//				Y[N]+= ... ;
//				..........
//			ZMSC_END
//
//-----------------------------------------------------------------------------
// ����� �������� ��� ����� ������������ �������.
// � ����� ���������: 
//
//			ZMSC_EXHV_DEF(ZModelEx)
//
// � ����� ���������� ������:
//
//			int ZModelEx::ExternalSph(double t, double* Xmain, 
//									  double* Xexe, double* Yexe)
//			{ 
//			...........	
//
//			...........
//			return 0 ;
//			}
//
//			int ZModelEx::ExternalForces(double t, double* X,  
//										 double* Y, int N)
//			{ 
//			...........	
//
//			...........
//			return 0 ;
//			}
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// �������������� ����������
#define ZMSC_EXH_DEF(ClassName)\
class ClassName : public ZMSC {\
public:\
	ClassName() {} ;\
	~ClassName(){} ;\
	virtual int ExternalForces(double t, double* X, double* Y, int N) ;\
} ;

#define ZMSC_EXH_BEG(ClassName)\
ZMSC_EXH_DEF(ClassName)\
int ClassName::ExternalForces(double t, double* X, double* Y, int N) ;\
{

//-----------------------------------------------------------------------------
// ������������ ����������
#define ZMSC_EXV_DEF(ClassName)\
class ClassName : public ZMSC {\
public:\
	ClassName() {};\
	~ClassName(){};\
	virtual int ExternalSph(double t, double* Xmain, double* Xexe, double* Yexe);\
} ;

#define ZMSC_EXV_BEG(ClassName)\
ZMSC_EXV_DEF(ClassName)\
int ClassName::ExternalSph(double t, double* Xmain, double* Xexe, double* Yexe)\
{

//-----------------------------------------------------------------------------
// ������������� �������������� � ������������ ����������
#define ZMSC_EXHV_DEF(ClassName)\
class ClassName : public ZMSC {\
public:\
	ClassName() {} ;\
	~ClassName(){} ;\
	virtual int ExternalSph(double t, double* Xmain, double* Xexe, double* Yexe);\
	virtual int ExternalForces(double t, double* X, double* Y, int N);\
} ;

#define ZMSC_EXHV_BEG(ClassName)\
ZMSC_EXHV_DEF(ClassName)\
int ClassName::ExternalSph(double t, double* Xmain, double* Xexe, double* Yexe)\
{

#define ZMSC_EXTFORCE(ClassName)\
	return 0 ;\
}\
int ClassName::ExternalForces(double t, double* X, double* Y, int N)\
{

//-----------------------------------------------------------------------------
// ���������� �������� ZMSC_EXH_BEG  ZMSC_EXV_BEG  ZMSC_EXHV_BEG 
#define ZMSC_END\
	return 0 ;\
}

//-----------------------------------------------------------------------------
#endif // #ifndef ModKA_H
