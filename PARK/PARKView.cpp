
// PARKView.cpp: CPARKView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "PARK.h"
#endif

#include "PARKDoc.h"
#include "PARKView.h"
#include "CCONST.h"
#include "CConstCntrDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPARKView

IMPLEMENT_DYNCREATE(CPARKView, CView)

BEGIN_MESSAGE_MAP(CPARKView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPARKView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(IID_CONSTSUM, &CPARKView::OnConstsum)
	ON_COMMAND(ID_IMAGESUM, &CPARKView::OnImagesum)
	ON_COMMAND(ID_SUM_VARCONST, &CPARKView::OnSumVarconst)
	ON_COMMAND(ID_SLIDE_SUM, &CPARKView::OnSlideSum)
	ON_COMMAND(ID_HISTOIN, &CPARKView::OnHistoin)
	ON_COMMAND(ID_HISTO_OUT, &CPARKView::OnHistoOut)
	ON_COMMAND(ID_HISTOEQAL, &CPARKView::OnHistoeqal)
	ON_COMMAND(ID_STRETCH, &CPARKView::OnStretch)
END_MESSAGE_MAP()

// CPARKView 생성/소멸

CPARKView::CPARKView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CPARKView::~CPARKView()
{
}

BOOL CPARKView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CPARKView 그리기

void CPARKView::OnDraw(CDC* pDC)
{
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	int x, y;

	for (y = 0; y < 256; y++) {
		for (x = 0; x < 256; x++) {
			pDC->SetPixel(x, y, RGB(pDoc->m_OpenImg[y][x],
			pDoc->m_OpenImg[y][x], pDoc->m_OpenImg[y][x]));
		}
	}

	for (y = 0; y < 256; y++) {
		for (x = 0; x < 256; x++) {
			pDC->SetPixel(x + 300, y, RGB(pDoc->m_Resultimg[y][x],
				pDoc->m_Resultimg[y][x], pDoc->m_Resultimg[y][x]));
		}
	}

	if (hin == 1) {
		// Histgram을 나타낼 그래프를 그려준다.
		pDC->MoveTo(0, 260);
		pDC->LineTo(0, 350);			// Y축
		pDC->MoveTo(0, 350);	
		pDC->LineTo(257, 350);			// X축

		for (x = 0; x < 256; x++) {
			y = pDoc->histoin[x] / 20;
			if (y > 90) y = 260;
			else y = 350 - y;

			pDC->MoveTo(x, 350);
			pDC->LineTo(x, y);
		}
	}
	if (hout == 1) {
		for (y = 256; y < 360; y++) {
			for (x = 300; x < 560; x++) {
				pDC->SetPixel(x, y, RGB(255, 255, 255));
			}
		}
		pDC->MoveTo(300, 260);
		pDC->LineTo(300, 350);			// Y축
		pDC->MoveTo(300, 350);
		pDC->LineTo(557, 350);			// X축

		for (x = 0; x < 256; x++) {
			y = pDoc->histoout[x] / 20;
			if (y > 90) y = 260;
			else y = 350 - y;

			pDC->MoveTo(x + 300, 350);
			pDC->LineTo(x + 300, y);
		}
	}
}


// CPARKView 인쇄


void CPARKView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CPARKView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CPARKView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CPARKView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CPARKView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPARKView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CPARKView 진단

#ifdef _DEBUG
void CPARKView::AssertValid() const
{
	CView::AssertValid();
}

void CPARKView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPARKDoc* CPARKView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPARKDoc)));
	return (CPARKDoc*)m_pDocument;
}
#endif //_DEBUG


// CPARKView 메시지 처리기


void CPARKView::OnConstsum()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int x, y, data;
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	for (y = 0; y < 256; y++) {
		for (x = 0; x < 256; x++) {
			data = pDoc->m_OpenImg[y][x] + 100;
			
			if (data > 255) pDoc->m_Resultimg[y][x] = 255;
			else pDoc->m_Resultimg[y][x] = data;
		}	
	}
	Invalidate(FALSE); // 화면 갱신
}


void CPARKView::OnImagesum()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int			x, y, data;
	// Document 파일에 사용된 변수를 사용하기 위해서 포인터 선언
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// Document 파일의 DoubleLoad 함수 호출
	pDoc->DoubleLoad();

	for(y = 0; y < 256; y++) {
		for (x = 0; x < 256; x++) {
			data = pDoc->m_ImageBuf1[y][x] + pDoc->m_ImageBuf2[y][x];

			// Seturateion Operating
			if (data > 255) pDoc->m_Resultimg[y][x] = 255;
			else pDoc->m_Resultimg[y][x] = data;

			// Wrap Operating 
			pDoc->m_OpenImg[y][x] = data;
		}
	}
	Invalidate(FALSE); // 화면 갱신
}


void CPARKView::OnSumVarconst()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int x, y, data;
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CCONST* pCONST = new CCONST;

	if (pCONST->DoModal() == IDOK) {
		m_ViewConst = pCONST->m_Const;
	}

	for (y = 0; y < 256; y++) {
		for (x = 0; x < 256; x++) {
			data = pDoc->m_OpenImg[y][x] + m_ViewConst;

			//Saturation Operating
			if (data > 255) pDoc->m_Resultimg[y][x] = 255;
			else pDoc->m_Resultimg[y][x] = data;
		}
	}
	Invalidate(FALSE); // 화면갱신
}

void CPARKView::OnSlideSum()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CConstCntrDlg psumCntrDlg;
	psumCntrDlg.DoModal();
}


void CPARKView::OnHistoin()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->HistoIn();   // 도큐먼트 함수 호출
	hin = 1;
	Invalidate(FALSE);
}


void CPARKView::OnHistoOut()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->HistoOut();
	hout = 1;
	Invalidate(FALSE);
}


void CPARKView::OnHistoeqal()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->Histoequal();
	Invalidate(FALSE);
}


void CPARKView::OnStretch()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->Stretch();
	Invalidate(FALSE);
}
