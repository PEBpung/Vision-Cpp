﻿
// PARKDoc.h: CPARKDoc 클래스의 인터페이스
//


#pragma once
#define large_x 2 
#define large_y 2 
#define zmin_x 1.7 
#define zmin_y 1.7 
#define small_x 2 
#define small_y 2 
#define zmout_x 0.5
#define zmout_y 0.5 

#include "math.h"
#define DEG	30

class CPARKDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CPARKDoc() noexcept;
	DECLARE_DYNCREATE(CPARKDoc)

// 특성입니다.
public:

// 작업입니다.
public:
	unsigned char m_OpenImg[256][256];
	unsigned char m_Resultimg[256][256];
	// 1. 두 영상의 데이터를 저장하기 위해 사용.
	// 2. 연산 중간 결과를 저장 할 때도 사용.
	unsigned char m_ImageBuf1[256][256]; 
	unsigned char m_ImageBuf2[256][256];
	unsigned int histoin[256];  // 입력 영상의 히스토그램 저장
	unsigned int histoout[256]; // 출력 영상의 히스토그램 저장

	unsigned int histoinX[256];  // 입력 영상의 세로축 히스토그램 
	unsigned int histoinY[256];  // 입력 영상의 가로축 히스토그램
	unsigned int histoutX[256];  // 출력 영상의 세로축 히스토그램
	unsigned int histoutY[256];  // 출력 영상의 가로축 히스토그램

	unsigned char m_scaleImg[256 * large_y][256 * large_x];

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CPARKDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	void DoubleLoad();
	void m_slideSUM(int slideconst);
	void HistoIn();
	void HistoOut();
	void Histoequal();
	void Stretch();
	void AutoBin1();
	void m_slideBinary(int slidebinary);
	void HistoInXY();
	void HistoOutXY();
	void Lowpass1();
	void PrewittX();
	void PrewittY();
	void Prewitt();
	void Canny();
	void GaussLap();
	void Lowpass2();
	void GaussMoth();
	void Median();
	int Align(int N[], int size);
	void Soble();
	void Robert();
	void Lapla1();
	void Lapla2();
	void Emboss();
	void Sharp();
	void ZminRn();
	void ZminRn2();
	void ZmoutRn1();
	void ZmoutRn2();
	void Rotate();
	void RotateRn();
	void Mirror();
	void Flip();
	void Erosion();
	void Opening();
	void Dilation();
	void Closing();
	void Labeling();
	void Label_p(int x, int y, int lable_no);
	void Labelstack();
	int Push(short* st_x, short* st_y, short xx, short yy, int* top);
	int Pop(short* st_x, short* st_y, short* xx, short* yy, int* top);
	void TrackBorder();
	void Filling();
	int floodfill(int x, int y, unsigned char image[][256], int* xchain, int* ychain, int* count);
};
