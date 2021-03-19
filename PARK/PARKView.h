
// PARKView.h: CPARKView 클래스의 인터페이스
//

#pragma once
class CPARKView : public CView
{
protected: // serialization에서만 만들어집니다.
	CPARKView() noexcept;
	DECLARE_DYNCREATE(CPARKView)

// 특성입니다.
public:
	CPARKDoc* GetDocument() const;

// 작업입니다.
public:
	int			hin;		// 입력 히스토그램 표시 플래그
	int			hout;		// 출력 히스토그램 표시 플래그
	int			hinxy;      // 입력 가로세로 히스토그램 표시 플래그
	int			houtxy;     // 출력 가로세로 히스토그램 표시 플래그
	int			zoom;
	int			zmout;

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CPARKView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnConstsum();
	afx_msg void OnImagesum();
	int m_ViewConst;
	afx_msg void OnSumVarconst();
	afx_msg void OnSlideSum();
	afx_msg void OnHistoin();
	afx_msg void OnHistoOut();
	afx_msg void OnHistoeqal();
	afx_msg void OnStretch();
	afx_msg void OnBinary();
	afx_msg void OnAutoBin();
	afx_msg void OnSlideBin();
	afx_msg void OnHistoinXy();
	afx_msg void OnHistoutXy();
	afx_msg void OnLowpass1();
	afx_msg void OnPrewittX();
	afx_msg void OnPrewittY();
	afx_msg void OnPrewitt();
	afx_msg void OnCanny();
	afx_msg void OnGausslap();
	afx_msg void OnLowpass();
	afx_msg void OnGaussmoth();
	afx_msg void OnMedian();
	afx_msg void OnSobel();
	afx_msg void OnRobert();
	afx_msg void OnLapla1();
	afx_msg void OnLapla2();
	afx_msg void OnEmboss();
	afx_msg void OnSharp();
	afx_msg void OnZminRn();
	afx_msg void OnZminRn2();
	afx_msg void OnZmoutRn1();
	afx_msg void OnZmoutRn2();
};

#ifndef _DEBUG  // PARKView.cpp의 디버그 버전
inline CPARKDoc* CPARKView::GetDocument() const
   { return reinterpret_cast<CPARKDoc*>(m_pDocument); }
#endif

