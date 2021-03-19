
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
#include "CTHIRD.h"
#include "CSliderBinary.h"

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
	ON_COMMAND(ID_BINARY, &CPARKView::OnBinary)
	ON_COMMAND(ID_AUTO_BIN, &CPARKView::OnAutoBin)
	ON_COMMAND(ID_SLIDE_BIN, &CPARKView::OnSlideBin)
	ON_COMMAND(ID_HISTOIN_XY, &CPARKView::OnHistoinXy)
	ON_COMMAND(ID_HISTOUT_XY, &CPARKView::OnHistoutXy)
	ON_COMMAND(ID_LOWPASS1, &CPARKView::OnLowpass1)
	ON_COMMAND(ID_PREWITT_X, &CPARKView::OnPrewittX)
	ON_COMMAND(ID_PREWITT_Y, &CPARKView::OnPrewittY)
	ON_COMMAND(ID_PREWITT, &CPARKView::OnPrewitt)
	ON_COMMAND(ID_CANNY, &CPARKView::OnCanny)
	ON_COMMAND(ID_GAUSSLAP, &CPARKView::OnGausslap)
	ON_COMMAND(ID_LOWPASS, &CPARKView::OnLowpass)
	ON_COMMAND(ID_GAUSSMOTH, &CPARKView::OnGaussmoth)
	ON_COMMAND(ID_MEDIAN, &CPARKView::OnMedian)
	ON_COMMAND(ID_SOBEL, &CPARKView::OnSobel)
	ON_COMMAND(ID_ROBERT, &CPARKView::OnRobert)
	ON_COMMAND(ID_LAPLA1, &CPARKView::OnLapla1)
	ON_COMMAND(ID_LAPLA2, &CPARKView::OnLapla2)
	ON_COMMAND(ID_EMBOSS, &CPARKView::OnEmboss)
	ON_COMMAND(ID_SHARP, &CPARKView::OnSharp)
	ON_COMMAND(ID_ZMIN_RN, &CPARKView::OnZminRn)
	ON_COMMAND(ID_ZMIN_RN2, &CPARKView::OnZminRn2)
	ON_COMMAND(ID_ZMOUT_RN1, &CPARKView::OnZmoutRn1)
	ON_COMMAND(ID_ZMOUT_RN2, &CPARKView::OnZmoutRn2)
	ON_COMMAND(ID_ROTATE, &CPARKView::OnRotate)
	ON_COMMAND(ID_ROTATE_RN, &CPARKView::OnRotateRn)
	ON_COMMAND(ID_MIRROR, &CPARKView::OnMirror)
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

	// 입력 영상 히스토그램
	if (hinxy == 1) {
		// 입력 영상의 수직 라인 그래프를 그려준다.
		pDC->MoveTo(0, 260);
		pDC->LineTo(0, 350);			// Y축
		pDC->MoveTo(0, 350);
		pDC->LineTo(257, 350);			// X축

		for (x = 0; x < 256; x++) {
			y = pDoc->histoinX[x] / 3;
			if (y > 90) y = 260;
			else y = 350 - y;

			pDC->MoveTo(x, 350);
			pDC->LineTo(x, y);
		}

		// 입력 영상의 수평 라인 그래프를 그려준다.
		pDC->MoveTo(260, 0);
		pDC->LineTo(350, 0);			// Y축
		pDC->MoveTo(260, 0);
		pDC->LineTo(260, 257);			// X축

		for (y = 0; y < 256; y++) {
			x = pDoc->histoinY[y] / 3;
			if (x > 90) x = 350;
			else x = 260 + x;

			pDC->MoveTo(260, y);
			pDC->LineTo(x, y);
		}
	}
	// 출력 영상 히스토그램
	if (houtxy == 1) {
		// 출력 영상의 수직 라인 그래프를 그려준다.
		pDC->MoveTo(400, 260);
		pDC->LineTo(400, 350);			// Y축
		pDC->MoveTo(400, 350);
		pDC->LineTo(657, 350);			// X축

		for (x = 0; x < 256; x++) {
			y = pDoc->histoutX[x] / 3;
			if (y > 90) y = 260;
			else y = 350 - y;

			pDC->MoveTo(x + 400, 350);
			pDC->LineTo(x + 400, y);
		}

		// 출력 영상의 수평 라인 그래프를 그려준다.
		pDC->MoveTo(660, 0);
		pDC->LineTo(750, 0);			// Y축
		pDC->MoveTo(660, 0);
		pDC->LineTo(660, 257);			// X축

		for (y = 0; y < 256; y++) {
			x = pDoc->histoutY[y] / 3;
			if (x > 90) x = 350;
			else x = 260 + x;

			pDC->MoveTo(660, y);
			pDC->LineTo(x + 400, y);
		}
	}

	if (zoom == 1) {
		for (y = 0; y < large_y * 256; y++) {
			for (int x = 0; x < large_x * 256; x++) {
				pDC->SetPixel(x + 300, y,
					RGB(pDoc->m_scaleImg[y][x],
						pDoc->m_scaleImg[y][x],
						pDoc->m_scaleImg[y][x]));
			}
		}
	}

	if (zmout == 1) {
		for (y = 0; y < 256 / small_y; y++) {
			for (int x = 0; x < 256 / small_x; x++) {
				pDC->SetPixel(x + 300, y,
					RGB(pDoc->m_scaleImg[y][x],
						pDoc->m_scaleImg[y][x],
						pDoc->m_scaleImg[y][x]));
			}
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


void CPARKView::OnBinary()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int x, y;
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CTHIRD* pTHRED = new CTHIRD;

	if (pTHRED->DoModal() == IDOK) {
		m_ViewConst = pTHRED->m_Thrd;
	}

	for ( y = 0; y < 256; y++)
	{
		for ( x = 0; x < 256; x++)
		{
			if (pDoc->m_OpenImg[y][x] > m_ViewConst)
				pDoc->m_OpenImg[y][x] = 255;
			else pDoc->m_OpenImg[y][x] = 0;
		}
	}
	Invalidate(FALSE);

}


void CPARKView::OnAutoBin()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->AutoBin1();
	Invalidate(FALSE);
}


void CPARKView::OnSlideBin()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSliderBinary pSlider;
	pSlider.DoModal();
}


void CPARKView::OnHistoinXy()
{
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// 입력 영상의 히스토그램 구하는 함수 호출.
	pDoc->HistoInXY();
	hinxy = 1;
	Invalidate(FALSE);
}


void CPARKView::OnHistoutXy()
{
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// 출력 영상의 히스토그램 구하는 함수 호출.
	pDoc->HistoOutXY();
	houtxy = 1;
	Invalidate(FALSE);
}


void CPARKView::OnLowpass1()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->Lowpass1();

	Invalidate(FALSE);
}


void CPARKView::OnPrewittX()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->PrewittX();

	Invalidate(FALSE);
}


void CPARKView::OnPrewittY()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->PrewittY();

	Invalidate(FALSE);
}


void CPARKView::OnPrewitt()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->Prewitt();

	Invalidate(FALSE);
}


void CPARKView::OnCanny()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->Canny();

	Invalidate(FALSE);
}


void CPARKView::OnGausslap()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->GaussLap();

	Invalidate(FALSE);
}


void CPARKView::OnLowpass()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->Lowpass2();

	Invalidate(FALSE);
}


void CPARKView::OnGaussmoth()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->GaussMoth();

	Invalidate(FALSE);
}


void CPARKView::OnMedian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->Median();

	Invalidate(FALSE);
}


void CPARKView::OnSobel()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->Soble();

	Invalidate(FALSE);
}


void CPARKView::OnRobert()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->Robert();

	Invalidate(FALSE);
}


void CPARKView::OnLapla1()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->Lapla1();

	Invalidate(FALSE);
}


void CPARKView::OnLapla2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->Lapla2();

	Invalidate(FALSE);
}


void CPARKView::OnEmboss()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->Emboss();

	Invalidate(FALSE);
}

void CPARKView::OnSharp()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->Sharp();

	Invalidate(FALSE);
}



void CPARKView::OnZminRn()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->ZminRn();

	zoom = 1;
	Invalidate(FALSE);
}


void CPARKView::OnZminRn2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->ZminRn2();

	zoom = 1;
	Invalidate(FALSE);
}


void CPARKView::OnZmoutRn1()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->ZmoutRn1();

	zmout = 1;
	Invalidate(FALSE);
}


void CPARKView::OnZmoutRn2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->ZmoutRn2();

	zmout = 1;
	Invalidate(FALSE);
}


void CPARKView::OnRotate()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->Rotate();

	Invalidate(FALSE);
}


void CPARKView::OnRotateRn()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->RotateRn();

	Invalidate(FALSE);
}


void CPARKView::OnMirror()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPARKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->Mirror();

	Invalidate(FALSE);
}
