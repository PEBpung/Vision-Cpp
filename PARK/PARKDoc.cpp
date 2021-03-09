
// PARKDoc.cpp: CPARKDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "PARK.h"
#endif

#include "PARKDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPARKDoc

IMPLEMENT_DYNCREATE(CPARKDoc, CDocument)

BEGIN_MESSAGE_MAP(CPARKDoc, CDocument)
END_MESSAGE_MAP()


// CPARKDoc 생성/소멸

CPARKDoc::CPARKDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CPARKDoc::~CPARKDoc()
{
}

BOOL CPARKDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CPARKDoc serialization

void CPARKDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
		ar.Write(m_Resultimg, 256 * 256);
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
		CFile* fp = ar.GetFile();

		if (fp->GetLength() != 256 * 256) {
			AfxMessageBox(_T("256 * 256 이미지가 아님!"));
			return;
		}
		ar.Read(m_OpenImg, fp->GetLength());
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CPARKDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CPARKDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CPARKDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CPARKDoc 진단

#ifdef _DEBUG
void CPARKDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPARKDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CPARKDoc 명령

// 두개 영상 파일을 오픈하는 함수
void CPARKDoc::DoubleLoad()
{
	// TODO: 여기에 구현 코드 추가.
	// 첫 번째 영상을 읽기 위한 대화상자 변수
	CFileDialog dlg1(TRUE); 
	AfxMessageBox(_T("Select First Image"));

	if (dlg1.DoModal() == IDOK) {
		CFile file;
		file.Open(dlg1.GetPathName(), CFile::modeRead); //파일 열기
		file.Read(m_ImageBuf1, 256 * 256); // 파일 읽기
		file.Close();
	}

	// 두 번째 영상을 읽기 위한 대화상자 변수
	CFileDialog dlg2(TRUE);
	AfxMessageBox(_T("Select Second Image"));

	if (dlg2.DoModal() == IDOK) {
		CFile file;
		file.Open(dlg2.GetPathName(), CFile::modeRead); //파일 열기
		file.Read(m_ImageBuf2, sizeof(m_ImageBuf2));
		file.Close();
	}
}


void CPARKDoc::m_slideSUM(int slideconst)
{
	int		x, y, data;

	for(y = 0; y < 255; y++) {
		for (x = 0; x < 255; x++) {
			data = m_OpenImg[y][x] + slideconst;
			if (data > 255) m_Resultimg[y][x] = 255;
			else m_Resultimg[y][x] = data;
		}
	}
	UpdateAllViews(FALSE);
}


void CPARKDoc::HistoIn()
{
	// TODO: 여기에 구현 코드 추가.
	int			x, y, d;

	for (y = 0; y < 256; y++) histoin[y] = 0; // 배열값 초기화

	for (y = 0; y < 256; y++){
		for (x = 0; x < 256; x++){
			d = (int)m_OpenImg[y][x];
			histoin[d] ++; // 해당 빈도수 값을 1씩 증가
		}
	}
}


void CPARKDoc::HistoOut()
{
	// TODO: 여기에 구현 코드 추가.
	int			x, y, d;
	
	for (y = 0; y < 256; y++) histoout[y] = 0;

	for (y = 0; y < 256; y++) {
		for (x = 0; x < 256; x++) {
			d = (int)m_Resultimg[y][x];
			histoout[d]++;
		}
	}
}


void CPARKDoc::Histoequal()
{
	// TODO: 여기에 구현 코드 추가.
	int x, y, d, nn;
	int k = 0, sum = 0, total_pix = 0;
	int hist[256];
	int sum_hist[256];

	// 변수 초기화
	for ( x = 0; x < 256; x++)
	{
		hist[x] = 0;
		sum_hist[x] = 0;
	}
	// 히스토그램 생성
	for ( y = 0; y < 256; y++)
	{
		for ( x = 0; x < 256; x++)
		{
			k = m_OpenImg[y][x];
			hist[k]++; 
		}
	}
	// 누적 히스토그램 구함.
	for ( x = 0; x < 256; x++)
	{
		sum = sum + hist[x];
		sum_hist[x] = sum;
	}
	// 히스토그램 평활화
	total_pix = 256 * 256;
	nn = total_pix / 256; // 균일 분포시 화소 수 (nt / bm)
	for ( y = 0; y < 256; y++)
	{
		for ( x = 0; x < 256; x++)
		{
			k = m_OpenImg[y][x];
			if (sum_hist[k] == 0) d = 0;
			else d = (sum_hist[k] - 1) / nn;

			m_Resultimg[y][x] = (unsigned char)d;
		}
	}
}


void CPARKDoc::Stretch()
{
	// TODO: 여기에 구현 코드 추가.
	int x, y, low, high, n, m;
	int hist[256];
	unsigned char LUT[256];
	float scale;

	// 변수 초기화
	for (x = 0; x < 256; x++) hist[x] = 0;
	
	// 히스토그램 생성
	for (y = 0; y < 256; y++)
	{
		for (x = 0; x < 256; x++)
		{
			n = m_OpenImg[y][x];
			hist[n]++;
		}
	}
	// 가장 낮은 농도값 추출
	for (x = 0; x < 256; x++)
	{
		if (hist[x] != 0)
		{
			low = x;
			break;
		}
	}
	// 가장 높은 농도값 추출
	for (x = 255; x > 0; x--)
	{
		if (hist[x] != 0)
		{
			high = x;
			break;
		}
	}
	
	// 룩업 테이블 작성
	for (x = 0; x < low; x++) LUT[x] = 0;
	for (x = high; x < 256; x++) LUT[x] = 255;

	scale = 255.0 / (float)(high - low);
	for ( x = low; x < high; x++)
	{
		LUT[x] = (unsigned char)((x - low) * scale);
	}

	for ( y = 0; y < 256; y++)
	{
		for ( x = 0; x < 256; x++)
		{
			m = m_OpenImg[y][x];
			m_Resultimg[y][x] = LUT[m];
		}
	}
}
