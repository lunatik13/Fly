//---------------------------------------------------------------------------
// ZCNumParser.cpp
// �. ��������� 01.02.2019
// 
// ZNumericParser
// �������� ������ ������� ��������� ������ ���������� ���������������
// ����� ������ (���). �� ������ ����������� ��� ������� ���, ������ �����
// ������������ ����� �������� ��������� �����, ���������� � ��������
// �������� ����������.
// ���� ������� �������� � ������������ ��������� ������ �
// ����� ��������� ��������, ���������� ������ �����, � ��������, ����������
// ������ �������� ���������� � ���� ������ �����. "��������" ������ ����� 
// �������� ������ ����� � ��������� ��� ������������� ������.
//
// ����� ��������� ����� �� ��������� �������� (Execute, ProcessString ��� <<) 
// ����� ������ �� ������ � ��������� ����� ��������� �������� 
// ������ ����� � ������� �����, � ����� ��������� ���������� ���������:
// FindInt, FindDbl, FindStr.
// 
// ����� ��������� ������������ ����������� ����������� ���������� �����
// ����������� ����������� �� ������������ � ����� ������������, � ��� ����� 
// � ����������, �������� � ��� �����. ������� ���� ��� ���������� ������ 
// � ������ ������������������. 
// �������� ����� ���������� X ��������� ���������� Y, � ����� ���������� Z
// � ��� �����...
//
// �� ������ ������ ����� ���������� ��� �������� ������� ������� ����������
// � �������� ������ ����������� ��� �������������� ���������� �������� 
// ��������� ����. ��! ��������� �������� �������� ���� ����� ���� "�������"
// ����� ������� ������� ������������� ��������� ���������� ����� ����������
// � ��������� �������������� ��������� �������� ������ � ������ ������...
// ��������: ��������� ������ ������ ������������������ ���������
//			2,+3.1415926e3,16-11-1973
//			2,     +3.1415926e+3, 16.11.1973
//			2   3.1415926+3, 16 11 1973
// 
//---------------------------------------------------------------------------
#include <stdafx.h>
#pragma   hdrstop
#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ZCNumParser.h>

//---------------------------------------------------------------------------
// !!!! ����� ����� !!!!
// 
// ZNumericParser ���������� ������� ���������� STL, � ����� �������� � ����
// �� �������� ������-�����������, ������� ����� ���� ��� ���������� ������ 
// ��������� ������ �� ��������� ���������� ������������ Mutex-��
// ����� ����� ���������� ������ � ����� ������������ ����, �����������
// ������ ������ ZNumParser, �������� ���� ��������� LoadNU01 � ��� �����
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
ZNumericParser::ZNumericParser() 
{
	DecSeparators.push_back('.') ;
	DecSeparators.push_back(',') ;
	StringSeparators.push_back(';') ;
	StringSeparators.push_back(':') ;
	StringSeparators.push_back('/') ;
	StringSeparators.push_back('\\') ;
	CountItem = 0 ;
	CountString = 0 ;
}

ZNumericParser::~ZNumericParser() 
{
	StringSeparators.clear();
	DecSeparators.clear();
	KeyString.clear() ;
	VPI.clear() ;
}

//---------------------------------------------------------------------------
void operator << (VParserItem& VPI, char* S) 
{
	ZNumericParser Parser ;
	Parser.ProcessString(S, VPI) ;
}

//---------------------------------------------------------------------------
void ZNumericParser::operator << (char* S) 
{
	ProcessString(S, VPI) ;
}

//---------------------------------------------------------------------------
// ������ ������ �� ������������ � ���������, ����� ����� � �����������
int ZNumericParser::ProcessString(char* S, VParserItem& VPI) 
{
	VPI.clear() ;
	ZParserItem PI ;
	while (*S!='\0') {
		int l ;
		// ������� ��������� ��������� ���������
		if (l = GetText(S, PI)) { 
			VPI.push_back(PI) ; 
			S+= l ; 
		}
		// ������� ��������� ���������, ���������� ����� �����
		if (l = GetNum (S, PI)) { 
			VPI.push_back(PI) ; 
			S+= l ; 
		}	
	}

	return 0 ;
}

//---------------------------------------------------------------------------
// �������� ������� - ����� ��� ��� 
bool ZNumericParser::IsDigit(char c) 
{
	if (c=='0' || c=='1' || c=='2' || c=='3' || c=='4' || 
		c=='5' || c=='6' || c=='7' || c=='8' || c=='9') {
		return true ;
	} 
	return false ;
}

//---------------------------------------------------------------------------
// �������� �� ������� � ������ �����������
int ZNumericParser::IsSep(char c)  
{
	int r ;
	// �������� �� ���������� �����������
	r = IsDecSep(c) ; if (r) return r ;
	// �������� �� ����������� �����
	r = IsStrSep(c) ; if (r) return r ;
	return 0 ;
}
int ZNumericParser::IsDecSep(char c)  
{
	int i, n=DecSeparators.size() ;
	for (i=0; i<n; i++) { if (c==DecSeparators[i]) return(21+i) ; }
	return 0 ;
}
int ZNumericParser::IsStrSep(char c)  
{
	int i, n=StringSeparators.size() ;
	for (i=0; i<n; i++) { if (c==StringSeparators[i]) return(31+i) ; }
	return 0 ;
}

//---------------------------------------------------------------------------
int ZNumericParser::GetNum (char* S, ZParserItem& PI) 
{
	int i=0, j=0 ;
	// ��������� �������� �������� ������� - ������ ������
	PI.type = 0 ;
	PI.sep = 0 ;
	PI.s = "" ;
	// �������� ������� �������. ���� ����� ���� �� �� ������ 
	// ����������� ������ � ������
	if ((*S=='-' || *S=='+') && IsDigit(*(S+1))) {
		// ����������� ������������ ���� � ������ ��������� � 
		// � �������� �������	
		SS[0] = *S ;
		SS[1] = '\0' ;
		PI.s  = SS ;
		// �������� � �������� ������
		//i++;
		j++;
		S++; 
	}
	// ���� �������
	while(1) {
		// ��������� ������ �������� ������
		char c = *S ;
		// ���� ��� ����� �� ���������� � � �����
		// ����� ��������� ����
		if (IsDigit(c)) SS[j] = c ;
		else {
			// �������� ����������� ������������
			PI.sep = IsSep(c) ;
			if (PI.sep) i++ ;
			break ;
		}
		// �������� � ������� ������ � ���������� ��������
		// ��������� ��������
		j++ ; 
		S++ ;
	}
	// ��������� ����� ��������� ������� �������� � ��������� 
	// ��������� ��������� � �������� �������
	if (j) {
		SS[j] = '\0' ;
		PI.s  = SS ;
	}
	// ������� ��������� - ����� �����, ���� ������� �� ������ ������
	PI.type = j ? 2:0 ;
	// ���������� �������� � �������� ������
	return (j+i) ;
}

//---------------------------------------------------------------------------
int ZNumericParser::GetText(char* S, ZParserItem& PI) 
{
	int   r, i=0, j=0 ;
	char* S0=S ;
	// ��������� �������� �������� ������� - ������ ������
	PI.type = 0 ;
	PI.sep = 0 ;
	PI.s = "" ;
	// ���� �������
	while(1) {
		// ������� ������ ���������
		char c = *S ;
		// �������� �����������
		r = IsSep(c) ;
		if (r) {
			c = '\0' ;
			i++ ; 
			PI.sep = r ;
			break ;
		} 
		if (IsDigit(c)) {
			// ���� ���������� �� ����� �� ��������� ���������
			if (S==S0) {
				// ���� � ������ ������ ������ ���������� �����,
				// ��������� ��������� ����������
				break ;
			} else {
				r = IsSep(*(S-1)) ;
				if (*(S-1)==' ' || r) {
					// ���� �� � ������ ������, ��
					// ��������� ������ � ��� ������ ����� ����� ������
					// ��������� ������ ��� �����������
					break ;
			}	}
		} else {
			if ((c=='+' || c=='-') && IsDigit(*(S+1))) {
				// ������ ���� ����� ������ ==> ������ �������������
				// � ����� ������� � ��������� �����
				c = '\0' ;
				break ;
		}	}
		// ��������� ����� ����������� ������
		if (c == '\0') break;
		// ����� ������ ���������� � ����� ��� ���������
		SS[j] = c;
		//����������� ����� ��������� ��������
		j++; 
		// ��������� � �������� ������
		S++;
	}
	// ��������� ��������� ����
	SS[j]   = '\0' ;
	// �������� ��������� ��������� � �������� �������
	PI.s    = SS ;		
	// ��� �������� - ������ �������� 
	PI.type = PI.s.length()>0 ? 1:0 ;	
	// ���������� �������� � �������� ������
	return(j+i) ;
}

//---------------------------------------------------------------------------
// ������ ��� ��� ������ ����� ���������� ����� ��� ������� ������
// �� ��������� �������� � ����� �����
// ����������� ���������� ������� ���� �� ����� FileName, ����
// �������� �� ��� ��������� ����� �� ������� ��������� pf,
// ���� �� ��������� ������ Text.
//
// ��������� ����� ����� ��������: ���� ����� ����� (������), ���� ������
// 'C' � ������ ��������� ������, ���� ������ ��������� ������.
// 
// ���������� ������� ���������� � ������� VPI
//---------------------------------------------------------------------------
static int ReadString(FILE* pf, char** Text, char* S)
{
	char ch;
	int  i, n;

	i = 0;
	while (1) {
		if (pf) {
			n = fscanf(pf, "%c", &ch);
			// �������� ������������ ���������� �������
			if (n != 1) ch = '\0';
		}
		else {
			ch = *(*Text);
			if (ch != '\0') (*Text)++;
			n = 1;
		}
		if (ch == '\0' || ch == '\n') break;
		if (ch == '\r') continue;
		S[i] = ch;
		i++;
	}
	S[i] = '\0';
	return (ch == '\0' ? -i : i);
}

int ZNumericParser::Execute(char* FileName, FILE* pf, char* Text,
							vector<string>* rows,
							bool PrintToCout)
{
	int  rc = 0 ;
	char S[512] = ""; 
	VParserItem VSPI; // ������ ��������� ������� ����� ������ ������� �����
	ZParserItem PI;   // ������� �������
	std::string s;

	// �������� ���� ������� ����������� �������
	CountItem = 0 ;
	CountString = 0 ;
	KeyString.clear() ;
	VPI.clear() ;

	//-----------------------------------------------------------------------
	// ������ ���� ������ ���� �� ���������� ������ �����
	if (!FileName && !pf && !Text) { rc = 1 ; goto end ; }
	// ���� ������ ��� �����, �� �������� ������ ����. ��� ���������� 
	// ������������ � ������ �����
	if (FileName) pf = fopen(FileName, "r") ;
	// ������ ��� ����� �� ���� ������ �� ���
	if (FileName && !pf) { rc = 2 ; goto end ; }
	if (!pf && !Text) { rc = 3 ; goto end ; }

	//-----------------------------------------------------------------------
	// ������ ������� ���������� �� ������� - ������������ ������� �����
	// FileName ��� pf ��� Text ===> S 
	// ���� ������ �����. ������ ������ ������������
	int  n, i = 0, k = 0;
	bool f = false;

	// ������� ���������� ����� ������������� ������ ���
	bool isFormFind = false;
	bool isNeedFormFind = WhatToLookFor.lf1.length() || 
						  WhatToLookFor.lf2.length();

	if (rows) rows->clear();
	//-----------------------------------------------------------------------
	// ������������ ������������ ����� ����� �� ��� ��� ���� ��
	// ����� ������� �������� �����
	k = 0;
	while (1){
		n = ReadString(pf, &Text, S);
		if (n < 0) {
			// ��������� ����� ��������� �����
			f = true;
			n = -n;
		}
		if (!n) {
			if (PrintToCout) cout << endl;
			continue;
		}
		if (isNeedFormFind) {
			if (n > 0) {
				s = S;
				k = s.find(WhatToLookFor.lf1, 0);
				if (k != string::npos) break;
				if (WhatToLookFor.lf2.length())	i = s.find(WhatToLookFor.lf2, 0);
				if (k != string::npos) break;
				k = 0;
			}
		}
		if (PrintToCout) cout << S << endl;
		if (f) break;
	}

	while (1) {
		// ������ ������
		// ������ ���������� �� ������� ������
		ProcessString(&S[k], VSPI);
		k = 0;
		n = VSPI.size();
		if (n) {
			// ���������� ������ ����� ��������� ��������� �������
			CountItem += n;
			// ���������� ������ ����� ��������� ����� � ������� �����
			CountString += 1;
			// ������ ������� �������� ������� VSPI[0] ��� ��������� 
			// ������ S � ����� ������� VPI ��������� �������
			n = VPI.size();
			KeyString.push_back(n);
			// ��� ���������� �������� ������� � ������ ������������
			// ������� ����������� - ������ ����� ������
			//VSPI[n-1].sep = 36 ;
			// ��������� ���������� �������� ������� VSPI 
			// � ����� ������ VPI
			VPI.insert(VPI.end(), VSPI.begin(), VSPI.end());
			// ����������� ��� ������������� �� �������
			if (PrintToCout) cout << S << endl;
			// 
			if (rows) rows->push_back(S);
		}

		// ����� ���������
		if (f) break;

		// ������ ����� ������
		n = ReadString(pf, &Text, S);
		if (n < 0) {
			// ��������� ����� ��������� �����
			f = true;
			n = -n;
		}
		if (!n || (n == 1 && S[0] == '\n')) break;
	}
		
end: ; 
	// ���� � �������� ���������� ������ ���������� ���� ��� ������,
	// � ��� ����� ����� ���� ������ ��� ����� FileName,
	// �� ���� ���������� �������.
	if (FileName && pf) { fclose(pf) ; pf = 0 ; }
	return rc ;
}

//---------------------------------------------------------------------------
// ��������� �� �������� � ��������� �������� �����, 
// ����� ����� � �������.
// indStr � IndItem ��������� �� ������ � ���������, 
// ������� � ������� ���������� ������ �����
// ������� 0 - ����� (������) ������ 
//		   1 - ������ 
//		  -1 - ����� (������) �� ������� 
// ����� �������� IndItem ��������� �� ��������� �������������
// ��������� � ������� ������ indStr
//---------------------------------------------------------------------------
// ��������� �� �������� � ��������� �������� ����� �����. 
int ZNumericParser::FindInt(int indStr, int& IndItem, int& D)  
{
	int n = GetCountItems(indStr) ;	// ����� �������� � ������
	int i0= KeyString[indStr] ;		// ������ ������ � ����� �������
	int i1= i0+IndItem ;			// ������ �������� � ����� �������	
	int N = i0+n ;					// ����� �������� � ����� �������
	int rc= 0 ;
	int i ;
	// ��������� �������� �������� �����
	D = 0 ;
	// ������� ��������
	for (i=i1; i<N; i++) { 
		// �������� ���� - ������ ���� ����� �����
		ZParserItem I = VPI[i] ;
		if (I.type==2) {
			// ������ ��������� ����������� ���������
			IndItem = i-i0+1 ;
			// �������������� ������ � �����
			// � ���������� ��������� �������������� ��������
			try { 
				D = atoi(I.s.c_str()) ; 
				rc = 0 ; goto end ;
			} catch(...) { 
				// ���� ����� ����� �������������� ��������, 
				// �� ������������ ��� ������ 
				rc = 1 ; goto end ;
	}	}	}	
	// ������ ��������� ����������� ���������
	IndItem = N-i0 ;
	// ����� ����� � ������ �� �������
	rc =-1 ; 
end: ;
	return rc ;
}

// ���������� ������ ��� ����������� ������������ ��� short � long
int ZNumericParser::FindInt(int indStr, int& IndItem, short& D)  
{
	int DS ; 
	int rc = FindInt(indStr, IndItem, DS) ;
	D = DS ;
	return rc ;   
}

int ZNumericParser::FindInt(int indStr, int& IndItem, long& D)  
{
	int DS ; 
	int rc = FindInt(indStr, IndItem, DS) ;
	D = DS ;
	return rc ;   
}

//---------------------------------------------------------------------------
// ��������� �� �������� � ��������� �������� ������� �����.
int ZNumericParser::FindDbl(int indStr, int& IndItem, double& D)  
{
	double R ;
	int n = GetCountItems(indStr) ;	// ����� �������� � ������
	int i0= KeyString[indStr] ;		// ������ ������ � ����� �������
	int i1= i0+IndItem ;			// ������ �������� � ����� �������	
	int N = i0+n ;					// ����� �������� � ����� �������
	int i ;
	int rc= 0 ;
	// ��������� �������� �������� �����
	D = 0 ;
	//-----------------------------------------------------------------------
	// ������� ����� ������ � ���� ����
	// 99999.999, ���� 999.99999e-(+)999, ���� 999.99999-(+)999.
	// ��������� ������ ���� ������������� � ���������������� ���� ��
	// ������ ��������� ����� "e".
	// ���������� �������� ��� ���� � ����� ����������� ����� ����
	// 999999 �� ������� ������� �� ���������� �����������, �� ����  
	// ���������� ����� �����, �� ��� ������ �������� ����� ���
	// ����� (�������� �����) ������������ ��� ����� � �������������
	// ������ � � ������� ������ ������ ����� �������.
	// ����� ������� ������� ����� ����� ���� ������������ �� ����� 
	// �� 4-� ��������. ��������:
	//	1 ���������� - "9999" 
	//				   ����� 9999.0
	//	2 ���������� - "9999" � "99", ��� ����� ������ ������ 
	//				   ����� ����������� ���������� �����������,
	//				   ����� 9999.99
	//	4 ���������  - "999", "99999", "e" � "-999", ��� ����������� 
	//				   ����� ������ ������ ����� ���������� �����������,
	//				   ����� ������ � ������� ����� ����������� �����������,
	//				   ����� 999.99999�-999
	//	3 ���������  - "999", "99999" � "-999", ��� ����������� 
	//				   ����� ������ ������ ����� ���������� �����������,
	//				   ����� ������ ������ ����������� �����������,
	//				   ����� 999.99999�-999
	//-----------------------------------------------------------------------
	for (i=i1; i<N; i++) { 
		try {
			ZParserItem I = VPI[i] ;
			if (I.type==2) {
				// ���� ����� ����� �� ��� ����� ����� ��������
				D = (double)atof(I.s.c_str()) ; 
				IndItem = i-i0+1 ;
				// ���� ������� � ������ �������� (����� ��������) ������ 
				// ���, �� ����������� ����� �����
				if (i==N-1) { rc = 0 ; goto end ; }
				// ���� ���������� �����������
				if (I.sep==21 || I.sep==22) {
					// ������ ��������� ������
					i++ ; 
					I = VPI[i] ;
					// ���� ����� �����
					if (I.type==2) {
						R = (double)atof(I.s.c_str()) ; 
						R/= pow(10.0, (double)I.s.length()) ;
						D = (fabs(D)+R)*(D<0 ? -1.0:1.0) ;
						IndItem++ ;
						//---------------------------------------------------
						// ������ ����������� ������ �������� �� �������
						// ���������� �����
m_exp: ;
						// ���� ������� � ������ �������� (����� ��������) 
						// ������ ���, �� ����������� ����� �����
						if (i==N-1) { rc = 0 ; goto end ; }
						// ���� ����� ������� ����� ����������� ���,
						// �� �������� ����� ������� ���������� �������
						// +(-)e999 ��� e+(-)999 ��� +(-)999
						if (I.sep==0) {
							// ������ ��������� ������
							i++ ; 
							I = VPI[i] ;
							if (I.type==2) {
								// ����� ����� ���������� ������� � �������
								// +(-)999
								R = (double)atoi(I.s.c_str()) ; 
								D*= pow(10.0, R) ;
								IndItem++ ;
								// ������� �����, ������� ����� � 
								// ���������� �������
								rc = 0 ; goto end ; 
							} else {
								// ��������� ��������� - ��������� ����������
								// ����� ���� ������ ���������� "e" ��� "E"
								if (I.type==1 && 
								   (I.s== "e" || I.s== "E" || 
								    I.s=="+e" || I.s=="+E" || 
									I.s=="-e" || I.s=="-E") &&
								    I.sep==0) {
									// ������ ��������� ������
									i++ ; IndItem++ ;
									I = VPI[i] ;
									if (I.type==2) {
										R = (double)atoi(I.s.c_str()) ; 
										D*= pow(10.0, R) ;
										IndItem++ ;
									} else {
										assert(0) ;
									}
								} else {
									// ���� �� �������� � �� ��������� 
									// ����������, ������ ������ ������,
									// ���� ������ �� �������� ������ 
									// ���������� ������� �����.
									// ������ ���������� ������� ��� � ����� 
									// �������� ������ ����� � ������� ������
									rc = 0 ; goto end ; 
						//---------------------------------------------------
						}	}	}
					} else {
						// ���� ����� ����������� ����������� ��� 
						// ��������� ����������, ������ ����� ������ ���
						// � ����� ���������� ����� ������
						rc = 0 ; goto end ; 
					}
				} else {
					if (I.sep>22) {
						// ����� ����� ����� ����� ��������� �����������
						// ������ ����� ���������� ����� ������	
						rc = 0 ; goto end ;
					}
					// ���� ����� ����� ����� ��� �� �����������, 
					// �� ������ ������� ����������� (VPI[i].sep=0),
					// �����, �������� ����� ������� +)-)e999
					goto m_exp ;
			}	}
		} catch(...) { return 1 ; }
	}
	// ����� � ������ �� �������
	rc =-1 ;

end: ;
	return rc ;
}

// ���������� ������ ��� ����������� ������������ ��� float
int ZNumericParser::FindDbl(int indStr, int& IndItem, float& D)  
{
	double DS ; 
	int rc = FindDbl(indStr, IndItem, DS) ;
	D = (float)DS ;
	return rc ;   
}

int ZNumericParser::FindStr(int indStr, int& IndItem, std::string& S, char* key)
{
	int n = GetCountItems(indStr);	// ����� �������� � ������
	int i0 = KeyString[indStr];		// ������ ������ � ����� �������
	int i1 = i0 + IndItem;			// ������ �������� � ����� �������	
	int N = i0 + n;					// ����� �������� � ����� �������
	int rc = 0;
	int i;
	// ����� ��������� ���������� � ���������� ��������� 
	// �������������� ��������
	try {
		// ������������� ������ ������ ������
		S[0] = '\0';
		// ������� ��������
		for (i = i1; i<N; i++) {
			// �������� ���� - ������ ���� ������
			ZParserItem I = VPI[i];
			if (I.type == 1) {
				int ikey = -1;
				if (key) {
					// ���� ������ �������� ��������� �� ����������� �� �������
					ikey = I.s.find(key, 0);
					if (ikey == string::npos) {
						// ���� ���� �� ��������� �� ������� � ����������
						// �������� �������
						continue;
				}	}
				// ������ ��������� ����������� ���������
				IndItem = i - i0 + 1;
				// ����������� ��������� ��������� ����������
				S = I.s;
				rc = 0; goto end;
	}	}	}
	// ���� ����� ����� �������������� ��������, �� ������������ ��� ������ 
	catch (...) {
		rc = 1; goto end;
	}
	// ������ ��������� ����������� ���������
	IndItem = N - i0;
	// ��������� ���������� � ������ �� �������
	rc = -1;

end:;
	return rc;
}

//---------------------------------------------------------------------------
// ��������� �� �������� � ��������� �������� ��������� ���������.
int ZNumericParser::FindStr(int indStr, int& IndItem, char* s, char* key)  
{
	std::string S;
	int rc = FindStr(indStr, IndItem, S, key); if (rc) return rc;
	sprintf(s, "%s\0", S.c_str()); 
	return 0; 
}

//---------------------------------------------------------------------------
// �������� ��������� ����� � ��������� �������
bool ZNumericParser::IsText(int indStr, int IndItem, const char* Text)
{
	int i0= KeyString[indStr] ;		// ������ ������ � ����� �������
	int i = i0+IndItem ;			// ������ � ����� �������	
	ZParserItem I = VPI[i] ;

	return (I.s==Text ? true:false) ;
}

//---------------------------------------------------------------------------