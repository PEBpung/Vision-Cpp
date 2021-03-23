
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


void CPARKDoc::AutoBin1()
{
	// TODO: 여기에 구현 코드 추가.
	int x, y, i, t;
	float prob[256];
	// 변수 초기화
	for ( i = 0; i < 256; i++)
	{
		histoin[i] = 0;
		prob[i] = 0.0f;
	}
	// 히스토그램 생성
	for ( y = 0; y < 256; y++)
	{
		for ( x = 0; x < 256; x++)
		{
			t = m_OpenImg[y][x];
			histoin[t]++;
		}
	}
	// 밝기 확률값 P(i) 생성
	for (i = 0; i < 256; i++) prob[i] = (float)histoin[i] / 65536.0F;

	float wsv_min = 1000000.0f;
	float wsv_u1, wsv_u2, wsv_s1, wsv_s2;
	int wsv_t;

	for ( t = 0; t < 256; t++)
	{
		// 확률값 q1, q2 계산.
		float q1 = 0.0f, q2 = 0.0f; 
		for (i = 0; i < t; i++) q1 += prob[i];
		for (i = t; i < 256; i++) q2 += prob[i];
		if (q1 == 0 || q2 == 0) continue;

		// 평균값 u1, u2 계산.
		float u1 = 0.0f, u2 = 0.0f;
		for (i = 0; i < t; i++) u1 += i * prob[i];
		u1 /= q1;
		for (i = t; i < 256; i++) u2 += i * prob[i];
		u2 /= q2;

		// 분산값 s1, s2 계산.
		float s1 = 0.0f, s2 = 0.0f;
		for (i = 0; i < t; i++) s1 += (i - u1) * (i - u1) * prob[i];
		s1 /= q1;

		for (i = t; i < 256; i++) s2 += (i - u2) * (i - u2) * prob[i];
		s2 /= q2;
		float wsv = q1 * s1 + q2 * s2;
		// 최소값을 갱신하는 t값 저장.
		if (wsv < wsv_min) {
			wsv_min = wsv;
			wsv_t = t;
			wsv_u1 = u1;
			wsv_u2 = u2;
			wsv_s1 = s1;
			wsv_s2 = s2;
		}
	}
	// 결과값 출력
	CString strTemp;
	strTemp.Format(_T("Optimal Threshold : % 3d\nMean : \
		% 7.3f, % 7.3f\nVariance : % 7.3f, % 7.3f"), \
		wsv_t, wsv_u1, wsv_u2, wsv_s1, wsv_s2);
	AfxMessageBox(strTemp);

	// 2치화
	for (y = 0; y < 256; y++) {
		for (x = 0; x < 256; x++) {
			if (m_OpenImg[y][x] < wsv_t) m_Resultimg[y][x] = 0;
			else m_Resultimg[y][x] = 255;
		}
	}
}


void CPARKDoc::m_slideBinary(int slidebinary)
{
	// TODO: 여기에 구현 코드 추가.
	int x, y;
	
	for ( y = 0; y < 256; y++)
	{
		for ( x = 0; x < 256; x++)
		{
			if (m_OpenImg[y][x] > slidebinary) m_Resultimg[y][x] = 255;
			else m_Resultimg[y][x] = 0;
		}
	}
	UpdateAllViews(FALSE);
}


void CPARKDoc::HistoInXY()
{
	int x, y;
	// 히스토그램 배열 초기화.
	for (x = 0; x < 256; x++) histoinX[x] = 0;
	for (y = 0; y < 256; y++) histoinY[y] = 0;

	// 입력 영상의 수직 라인에 대한 히스토그램을 구한다.
	for (x = 0; x < 256; x++)
	{
		for (y = 0; y < 256; y++)
		{
			if (m_OpenImg[y][x] == 255) histoinX[x] ++;
		}
	}
	
	// 입력 영상의 수평 라인에 대한 히스토그램을 구한다.
	for (y = 0; y < 256; y++)
	{
		for (x = 0; x < 256; x++)
		{
			if (m_OpenImg[y][x] == 255) histoinY[y] ++;
		}
	}
	
	UpdateAllViews(FALSE);
}


void CPARKDoc::HistoOutXY()
{
	int x, y;
	// 히스토그램 배열 초기화.
	for (x = 0; x < 256; x++) histoutX[x] = 0;
	for (y = 0; y < 256; y++) histoutY[y] = 0;

	// 출력 영상의 수직 라인에 대한 히스토그램을 구한다.
	for (x = 0; x < 256; x++)
	{
		for (y = 0; y < 256; y++)
		{
			if (m_Resultimg[y][x] == 255) histoutX[x] ++;
		}
	}

	// 출력 영상의 수평 라인에 대한 히스토그램을 구한다.
	for (y = 0; y < 256; y++)
	{
		for (x = 0; x < 256; x++)
		{
			if (m_Resultimg[y][x] == 255) histoutY[y] ++;
		}
	}

	UpdateAllViews(FALSE);
}


void CPARKDoc::Lowpass1()
{
	int x, y, p, q, div;
	int sum, filter1[3][3] = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };

	div = 0;

	for (q = 0; q <= 2; q++)
		for (p = 0; p <= 2; p++)
			div += filter1[q][p];

	for ( y = 0; y < 255; y++)
	{
		for ( x = 0; x < 255; x++)
		{
			sum = 0;
			for ( q = 0; q <= 2; q++)
				for ( p = 0; p <= 2; p++)
					// 이미지와 필터를 차례대로 합성곱 해준다.
					sum += filter1[q][p] * m_OpenImg[y + q - 1][x + p - 1];
			m_Resultimg[y][x] = sum / div;
		}
	}
}


void CPARKDoc::PrewittX()
{
	int x, y, p, q;
	int sum, filter1[3][3] = { {-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1} };

	for (y = 0; y < 255; y++)
	{
		for (x = 0; x < 255; x++)
		{
			sum = 0;
			for (q = 0; q <= 2; q++)
				for (p = 0; p <= 2; p++)
					// 이미지와 필터를 차례대로 합성곱 해준다.
					sum += filter1[q][p] * m_OpenImg[y + q - 1][x + p - 1];
			
			sum = abs(sum);
			if (sum > 255) m_Resultimg[y][x] = 255;
			else m_Resultimg[y][x] = sum;
		}
	}
}


void CPARKDoc::PrewittY()
{
	int x, y, p, q;
	int sum, filter2[3][3] = { {1, 1, 1}, {0, 0, 0}, {-1, -1, -1} };

	for (y = 0; y < 255; y++)
	{
		for (x = 0; x < 255; x++)
		{
			sum = 0;
			for (q = 0; q <= 2; q++)
				for (p = 0; p <= 2; p++)
					// 이미지와 필터를 차례대로 합성곱 해준다.
					sum += filter2[q][p] * m_OpenImg[y + q - 1][x + p - 1];

			sum = abs(sum);
			if (sum > 255) m_Resultimg[y][x] = 255;
			else m_Resultimg[y][x] = sum;
		}
	}
}


void CPARKDoc::Prewitt()
{
	int x, y, p, q;
	int px, ft_x[3][3] = { {-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1} };
	int py, ft_y[3][3] = { {1, 1, 1}, {0, 0, 0}, {-1, -1, -1} };


	for (y = 0; y < 255; y++)
	{
		for (x = 0; x < 255; x++)
		{
			px = 0;
			for (q = 0; q <= 2; q++)
				for (p = 0; p <= 2; p++)
					px += ft_x[q][p] * m_OpenImg[y + q - 1][x + p - 1];

			py = 0;
			for (q = 0; q <= 2; q++)
				for (p = 0; p <= 2; p++)
					py += ft_y[q][p] * m_OpenImg[y + q - 1][x + p - 1];

			py = abs(px) + abs(py);
			if (py > 255) m_Resultimg[y][x] = 255;
			else m_Resultimg[y][x] = py;
		}
	}
}


void CPARKDoc::Canny()
{
	int x, y, p, q, div;
	int sum, Gaussian[3][3] = { {1, 2, 1}, {2, 4, 2}, {1, 2, 1} };
	int sx, ft_x[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
	int sy, ft_y[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };

	div = 0;
	for (q = 0; q <= 2; q++)
		for (p = 0; p <= 2; p++)
			div += Gaussian[q][p];
	for ( y = 0; y < 255; y++)
	{
		for ( x = 0; x < 255; x++)
		{
			sum = 0;
			for (q = 0; q <= 2; q++)
				for (p = 0; p <= 2; p++)
					sum += Gaussian[q][p] * m_OpenImg[y + q - 1][x + p - 1];
			m_ImageBuf1[y][x] = sum / div;
		}
	}
	for (y = 0; y < 255; y++)
	{
		for (x = 0; x < 255; x++)
		{
			sx = 0;
			for (q = 0; q <= 2; q++)
				for (p = 0; p <= 2; p++)
					sx += ft_x[q][p] * m_ImageBuf1[y + q - 1][x + p - 1];

			sy = 0;
			for (q = 0; q <= 2; q++)
				for (p = 0; p <= 2; p++)
					sy += ft_y[q][p] * m_ImageBuf1[y + q - 1][x + p - 1];

			sy = abs(sx) + abs(sy);
			if (sy > 255) m_Resultimg[y][x] = 255;
			else m_Resultimg[y][x] = sy;
		}
	}

}


void CPARKDoc::GaussLap()
{
	int x, y, p, q, div;
	int sum, Gaussian[3][3] = { {1, 2, 1}, {2, 4, 2}, {1, 2, 1} };
	int ft[3][3] = { {0, 1, 0}, {1, -4, 1}, {0, 1, 0} };

	div = 0;
	for (q = 0; q <= 2; q++)
		for (p = 0; p <= 2; p++)
			div += Gaussian[q][p];

	for ( y = 0; y < 255; y++)
	{
		for ( x = 0; x < 255; x++)
		{
			sum = 0;
			for (q = 0; q <= 2; q++)
				for (p = 0; p <= 2; p++)
					sum += Gaussian[q][p] * m_OpenImg[y + q - 1][x + p - 1];
			m_ImageBuf1[y][x] = sum / div;
		}
	}

	for (y = 0; y < 255; y++)
	{
		for (x = 0; x < 255; x++)
		{
			sum = 0;
			for (q = 0; q <= 2; q++)
				for (p = 0; p <= 2; p++)
					sum += ft[q][p] * m_ImageBuf1[y + q - 1][x + p - 1];
			if (sum < 0) sum = 0;
			sum = abs(sum) * 5;
			if (sum > 255) sum = 255;
			m_Resultimg[y][x] = sum;
		}
	}
}


void CPARKDoc::Lowpass2()
{
	int x, y, q, p, div;
	int sum, filter2[3][3] = { {0, 1, 0}, {1, 5, 1}, {0, 1, 0} };

	div = 0;
	for (q = 0; q <= 2; q++)
		for (p = 0; p <= 2; p++)
			div += filter2[q][p];

	for ( y = 0; y < 255; y++)
	{
		for ( x = 0; x < 255; x++)
		{
			sum = 0;
			for (q = 0; q <= 2; q++)
				for (p = 0; p <= 2; p++)
					sum += filter2[q][p] * m_OpenImg[y + q - 1][x + p - 1];
			m_Resultimg[y][x] = sum / div;
		}
	}
}


void CPARKDoc::GaussMoth()
{
	// TODO: 여기에 구현 코드 추가.
	int x, y, q, p, div;
	int sum, Gaussian[3][3] = { {1, 2, 1}, {2, 4, 2}, {1, 2, 1} };

	div = 0;
	for (q = 0; q <= 2; q++)
		for (p = 0; p <= 2; p++)
			div += Gaussian[q][p];

	for (y = 0; y < 255; y++)
	{
		for (x = 0; x < 255; x++)
		{
			sum = 0;
			for (q = 0; q <= 2; q++)
				for (p = 0; p <= 2; p++)
					sum += Gaussian[q][p] * m_OpenImg[y + q - 1][x + p - 1];
			m_Resultimg[y][x] = sum / div;
		}
	}
}


int CPARKDoc::Align(int N[], int size)
{
	int temp;
	int mid;
	mid = size / 2;   // 마스크의 중앙 위치 계산

	for (int k = 1; k < size; k++) {
		for (int i = 0; i < size - 1; i++)
		{
			if (N[i] > N[i + 1]) {
				temp = N[i];
				N[i] = N[i + 1];
				N[i + 1] = temp;
			}
		}
	}

	return N[mid];
}

void CPARKDoc::Median()
{
	int x, y, q, p, d;
	int median, filter3[3][3] = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };
	int N[9];

	for (y = 0; y < 255; y++)
	{
		for (x = 0; x < 255; x++)
		{
			for (q = 0; q <= 2; q++)
				for (p = 0; p <= 2; p++)
					N[q * 3 + p] = filter3[q][p] * m_OpenImg[y + q - 1][x + p - 1];
			median = Align(N, 9);
			m_Resultimg[y][x] = median;
		}
	}

}


void CPARKDoc::Soble()
{
	int x, y, p, q;
	int sx, sb_x[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
	int sy, sb_y[3][3] = { {1, 2, 1}, {0, 0, 0}, {-1, -2, -1} };


	for (y = 0; y < 255; y++)
	{
		for (x = 0; x < 255; x++)
		{
			sx = 0;
			for (q = 0; q <= 2; q++)
				for (p = 0; p <= 2; p++)
					sx += sb_x[q][p] * m_OpenImg[y + q - 1][x + p - 1];

			sy = 0;
			for (q = 0; q <= 2; q++)
				for (p = 0; p <= 2; p++)
					sy += sb_y[q][p] * m_OpenImg[y + q - 1][x + p - 1];

			sy = abs(sx) + abs(sy);
			if (sy > 255) m_Resultimg[y][x] = 255;
			else m_Resultimg[y][x] = sy;
		}
	}
}


void CPARKDoc::Robert()
{
	int x, y, p, q;
	int rx, rb_x[3][3] = { {0, 0, -1}, {0, 1, 0}, {0, 0, 0} };
	int ry, rb_y[3][3] = { {-1, 0, 0}, {0, 1, 0}, {0, 0, 0} };


	for (y = 0; y < 255; y++)
	{
		for (x = 0; x < 255; x++)
		{
			rx = 0;
			for (q = 0; q <= 2; q++)
				for (p = 0; p <= 2; p++)
					rx += rb_x[q][p] * m_OpenImg[y + q - 1][x + p - 1];

			ry = 0;
			for (q = 0; q <= 2; q++)
				for (p = 0; p <= 2; p++)
					ry += rb_y[q][p] * m_OpenImg[y + q - 1][x + p - 1];

			ry = abs(rx) + abs(ry);
			if (ry > 255) m_Resultimg[y][x] = 255;
			else m_Resultimg[y][x] = ry;
		}
	}
}


void CPARKDoc::Lapla1()
{
	int x, y, q, p;
	int sum, ft[3][3] = { {0, 1, 0}, {1, -4, 1}, {0, 1, 0} };

	for (y = 0; y < 255; y++)
	{
		for (x = 0; x < 255; x++)
		{
			sum = 0;
			for (q = 0; q <= 2; q++)
				for (p = 0; p <= 2; p++)
					sum += ft[q][p] * m_OpenImg[y + q - 1][x + p - 1];
			if (sum < 0) sum = 0;
			sum = abs(sum);
			if (sum > 255) sum = 255;
			m_Resultimg[y][x] = sum;
		}
	}
}


void CPARKDoc::Lapla2()
{
	// TODO: 여기에 구현 코드 추가.
	int x, y, q, p;
	int sum, ft[3][3] = { {1, 1, 1}, {1, -8, 1}, {1, 1, 1} };

	for (y = 0; y < 255; y++)
	{
		for (x = 0; x < 255; x++)
		{
			sum = 0;
			for (q = 0; q <= 2; q++)
				for (p = 0; p <= 2; p++)
					sum += ft[q][p] * m_OpenImg[y + q - 1][x + p - 1];
			if (sum < 0) sum = 0;
			sum = abs(sum);
			if (sum > 255) sum = 255;
			m_Resultimg[y][x] = sum;
		}
	}
}

void CPARKDoc::Emboss()
{
	// TODO: 여기에 구현 코드 추가.
	int x, y, q, p;
	int sum, ft[3][3] = { {-1, 0, 0}, {0, 0, 0}, {0, 0, 1} };

	for (y = 0; y < 255; y++)
	{
		for (x = 0; x < 255; x++)
		{
			sum = 0;
			for (q = 0; q <= 2; q++)
				for (p = 0; p <= 2; p++)
					sum += ft[q][p] * m_OpenImg[y + q - 1][x + p - 1];
			sum = sum + +128;
			if (sum > 255) sum = 255;
			m_Resultimg[y][x] = sum;
		}
	}
}


void CPARKDoc::Sharp()
{
	// TODO: 여기에 구현 코드 추가.
	int x, y, q, p;
	int sum, ft[3][3] = { {0, -1, 0}, {-1, 5, -1}, {0, -1, 0} };

	for (y = 0; y < 255; y++)
	{
		for (x = 0; x < 255; x++)
		{
			sum = 0;
			for (q = 0; q <= 2; q++)
				for (p = 0; p <= 2; p++)
					sum += ft[q][p] * m_OpenImg[y + q - 1][x + p - 1];
			if (sum < 0) sum = 0;
			sum = abs(sum);
			if (sum > 255) sum = 255;
			m_Resultimg[y][x] = sum;
		}
	}
}


void CPARKDoc::ZminRn()
{
	// TODO: 여기에 구현 코드 추가.
	int x, y, x1, y1;
	float fx, fy;
	for (y = 0; y < 256 * large_y; y++) {
		for (x = 0; x < 256 * large_x; x++) {
			m_scaleImg[y][x] = 255;
		}
	}
	for (y = 0; y < 256 * zmin_y; y++) {
		fy = (float)y / (float)zmin_y;
		y1 = (int)(fy + 0.5);
		for (x = 0; x < 256 * zmin_x; x++) {
			fx = (float)x / (float)zmin_x;
			x1 = (int)(fx + 0.5);
			m_scaleImg[y][x] = m_OpenImg[y1][x1];
		}
	}
}


void CPARKDoc::ZminRn2()
{
	// TODO: 여기에 구현 코드 추가.
	int x, y, x1, y1, x2, y2;
	float fx, fy, p, q, fd, dd;
	// 출력 영상 흰색 클리어
	for (y = 0; y < 256 * large_y; y++) { 
		for (x = 0; x < 256 * large_x; x++) {
			m_scaleImg[y][x] = 255;
		}
	}
	for (y = 0; y < 256 * zmin_y; y++) {
		// 역변환 fy
		fy = (float)y / (float)zmin_y;
		y1 = (int)fy;
		y2 = y1 + 1;
		q = fy - (float)y1;
		for (x = 0; x < 256 * zmin_x; x++) {
			// 역변환 fx
			fx = (float)x / (float)zmin_x;
			x1 = (int)fx;
			x2 = x1 + 1;
			p = fx - (float)x1;
			// 보간처리
			fd = 0;
			dd = (float)m_OpenImg[y1][x1];
			fd = (1.0 - q) * (1.0 - p) * dd;
			dd = (float)m_OpenImg[y1][x2];
			fd = fd + (1.0 - q) * p * dd;
			dd = (float)m_OpenImg[y2][x1];
			fd = fd + q * (1.0 - p) * dd;
			dd = (float)m_OpenImg[y2][x2];
			fd = fd + q * p * dd;
			if (fd > 255.0) fd = 255.0;
			m_scaleImg[y][x] = (unsigned char)fd;
		}
	}
}


void CPARKDoc::ZmoutRn1()
{
	// TODO: 여기에 구현 코드 추가.
	int x, y, x1, y1;
	float fx, fy;
	for (y = 0; y < 256 / small_y; y++) {
		for (x = 0; x < 256 / small_x; x++) {
			m_scaleImg[y][x] = 255;
		}
	}
	for (y = 0; y < 256 / small_y; y++) {
		fy = (float)y / (float)zmout_y;
		y1 = (int)(fy + 0.5);
		for (x = 0; x < 256 / zmout_x; x++) {
			fx = (float)x / (float)0.5;
			x1 = (int)(fx + 0.5);
			m_scaleImg[y][x] = m_OpenImg[y1][x1];
		}
	}
}


void CPARKDoc::ZmoutRn2()
{
	// TODO: 여기에 구현 코드 추가.
	int x, y, x1, y1, x2, y2;
	float fx, fy, p, q, fd, dd;
	// 출력 영상 흰색 클리어
	for (y = 0; y < 256 / small_y; y++) {
		for (x = 0; x < 256 / small_x; x++) {
			m_scaleImg[y][x] = 255;
		}
	}
	for (y = 0; y < 256 / small_y; y++) {
		// 역변환 fy
		fy = (float)y / (float)zmout_y;
		y1 = (int)fy;
		y2 = y1 + 1;
		q = fy - (float)y1;
		for (x = 0; x < 256 / small_x; x++) {
			// 역변환 fx
			fx = (float)x / (float)zmout_x;
			x1 = (int)fx;
			x2 = x1 + 1;
			p = fx - (float)x1;
			// 보간처리
			fd = 0;
			dd = (float)m_OpenImg[y1][x1];
			fd = (1.0 - q) * (1.0 - p) * dd;
			dd = (float)m_OpenImg[y1][x2];
			fd = fd + (1.0 - q) * p * dd;
			dd = (float)m_OpenImg[y2][x1];
			fd = fd + q * (1.0 - p) * dd;
			dd = (float)m_OpenImg[y2][x2];
			fd = fd + q * p * dd;
			if (fd > 255.0) fd = 255.0;
			m_scaleImg[y][x] = (unsigned char)fd;
		}
	}
}


void CPARKDoc::Rotate()
{
	// TODO: 여기에 구현 코드 추가.
	int x, y, x1, y1;
	float fx, fy, ct, sn;

	ct = cos(DEG * 3.14159 / 180.0);
	sn = sin(DEG * 3.14159 / 180.0);

	for (y = 0; y < 256; y++) {
		for (x = 0; x < 256; x++) {
			// 역변환
			fx = (float)(x - 128) * ct + (float)(y - 128) * sn;
			fy = (float)(y - 128) * ct - (float)(x - 128) * sn;
			// 근방처리
			x1 = (int)(fx + 0.5) + 128;
			y1 = (int)(fy + 0.5) + 128;
			// 역변환 좌표 영역내 조사
			if (x1 >= 0 && x1 < 256 && y1 >= 0 && y1 < 256)
				m_Resultimg[y][x] = m_OpenImg[y1][x1];
			else m_Resultimg[y][x] = 0;
		}
	}
}

void CPARKDoc::RotateRn()
{
	// TODO: 여기에 구현 코드 추가.
	int x, y, x1, y1, x2, y2;
	float fx, fy, p, q, fd, dd, ct, sn;
	// 출력 영상 흰색 클리어
	ct = cos(DEG * 3.14159 / 180.0);
	sn = sin(DEG * 3.14159 / 180.0);

	for (y = 0; y < 256; y++) {
		for (x = 0; x < 256; x++) {
			// 역변환
			fx = (float)(x - 128) * ct + (float)(y - 128) * sn + 128;
			x1 = (int)fx;
			x2 = x1 + 1;
			p = fx - (float)x1;
			fy = (float)(y - 128) * ct - (float)(x - 128) * sn + 128;
			y1 = (int)fy;
			y2 = y1 + 1;
			q = fy - (float)y1;

			fd = 0;
			if (x1 >= 0 && x1 < 256 && y1 >= 0 && y1 < 256)
				dd = (float)m_OpenImg[y1][x1];
			else dd = 0;
			fd = (1.0 - q) * (1.0 - p) * dd;

			if (x2 >= 0 && x2 < 256 && y1 >= 0 && y1 < 256)
				dd = (float)m_OpenImg[y1][x2];
			else dd = 0;
			fd = fd + (1.0 - q) * p * dd;

			if (x1 >= 0 && x1 < 256 && y2 >= 0 && y2 < 256)
				dd = (float)m_OpenImg[y2][x1];
			else dd = 0;
			fd = fd + q * (1.0 - p) * dd;

			if (x2 >= 0 && x2 < 256 && y2 >= 0 && y2 < 256)
				dd = (float)m_OpenImg[y2][x2];
			else dd = 0;

			fd = fd + q * p * dd;

			if (fd > 255.0) fd = 255.0;
			if (fd < 0) fd = 0;
			m_Resultimg[y][x] = (unsigned char)fd;
		}
	}
}


void CPARKDoc::Mirror()
{
	// TODO: 여기에 구현 코드 추가.
	int x, y;
	for (y = 0; y < 256; y++) {
		for (x = 0; x < 256; x++) {
			m_Resultimg[y][x] = m_OpenImg[y][255 - x];
		}
	}
}


void CPARKDoc::Flip()
{
	// TODO: 여기에 구현 코드 추가.
	int x, y;
	for (y = 0; y < 256; y++) {
		for (x = 0; x < 256; x++) {
			m_Resultimg[y][x] = m_OpenImg[255 - y][x];
		}
	}
}


void CPARKDoc::Erosion()
{
	int x, y, p, q, sum;
	for (y = 0; y < 256; y++) {
		for (x = 0; x < 256; x++) m_Resultimg[y][x] = 0;
	}
	
	for (y = 1; y < 255; y++) {
		for (x = 1; x < 255; x++) {
			// 배경영역이면 0으로 처리
			if (m_OpenImg[y][x] == 0) {
				m_Resultimg[y][x] = 0;
				continue;
			}
			sum = 0;
			// 물체영역이면 3x3 영역 내 물체의 점들 조사
			for (q = 0; q <= 2; q++) {
				for (p = 0; p <= 2; p++) {
					if (m_OpenImg[y + q - 1][x + p - 1] != 0) sum++;
				}
			}
			// 내부점
			if (sum == 9) m_Resultimg[y][x] = 255;
			// 경계점 또는 고립점
			else m_Resultimg[y][x] = 0;
		}
	}
}


void CPARKDoc::Opening()
{
	int x, y, p, q;
	int sum;
	
	for (y = 0; y < 256; y++) {
		for (x = 0; x < 256; x++) {
			m_Resultimg[y][x] = 0;
			m_ImageBuf1[y][x] = 0;
		}
	}
	// STEP1: 침식 연산 수행
	for (y = 1; y < 255; y++) {
		for (x = 1; x < 255; x++) {
			if (m_OpenImg[y][x] == 0) {
				m_ImageBuf1[y][x] = 0;
				continue;
			}
			sum = 0;
			for (q = 0; q <= 2; q++) {
				for (p = 0; p <= 2; p++) {
					if (m_OpenImg[y + q - 1][x + p - 1] != 0) sum++;
				}
			}
			// 내부점
			if (sum == 9) m_ImageBuf1[y][x] = 255;
			// 경계점 또는 고립점
			else m_ImageBuf1[y][x] = 0;
		}
	}

	// STEP2: 팽창 연산 수행
	for (y = 1; y < 255; y++) { 
		for (x = 1; x < 255; x++) {
			// 물체영역이면 255로 처리
			if (m_ImageBuf1[y][x] == 255) {
				m_Resultimg[y][x] = 255;
				continue;
			}
			sum = 0;
			// 배경영역이면 3x3 영역 내 물체의 점들 조사
			for (q = 0; q <= 2; q++) {
				for (p = 0; p <= 2; p++) {
					if (m_ImageBuf1[y + q - 1][x + p - 1] != 0) sum++;
				}
			}
			if (sum == 0) m_Resultimg[y][x] = 0;
			// 주변에 하나라도 물체가 있으면 물체로 변환
			else m_Resultimg[y][x] = 255;
		}
	}
}


void CPARKDoc::Dilation()
{
	int x, y, p, q;
	int sum;

	for (y = 0; y < 256; y++) {
		for (x = 0; x < 256; x++) {
			m_Resultimg[y][x] = 0;
		}
	}
	for (y = 1; y < 255; y++) {
		for (x = 1; x < 255; x++) {
			if (m_OpenImg[y][x] == 255) {
				m_Resultimg[y][x] = 255;
				continue;
			}
			sum = 0;
			for (q = 0; q <= 2; q++) {
				for (p = 0; p <= 2; p++) {
					if (m_OpenImg[y + q - 1][x + p - 1] != 0) sum++;
				}
			}
			if (sum == 0) m_Resultimg[y][x] = 0;
			// 주변에 하나라도 물체가 있으면 물체로 변환
			else m_Resultimg[y][x] = 255;
		}
	}
}


void CPARKDoc::Closing()
{
	int x, y, p, q;
	int sum;

	for (y = 0; y < 256; y++) {
		for (x = 0; x < 256; x++) {
			m_Resultimg[y][x] = 0;
			m_ImageBuf1[y][x] = 0;
		}
	}
	// STEP1: 팽창 연산 수행
	for (y = 1; y < 255; y++) {
		for (x = 1; x < 255; x++) {
			if (m_OpenImg[y][x] == 255) {
				m_ImageBuf1[y][x] = 255;
				continue;
			}
			sum = 0;
			for (q = 0; q <= 2; q++) {
				for (p = 0; p <= 2; p++) {
					if (m_OpenImg[y + q - 1][x + p - 1] != 0) sum++;

				}
			}
			if (sum == 0) m_ImageBuf1[y][x] = 0;
			// 주변에 하나라도 물체가 있으면 물체로 변환
			else m_ImageBuf1[y][x] = 255;
		}
	}
	// STEP2: 침식 연산 수행
	for (y = 1; y < 255; y++) {
		for (x = 1; x < 255; x++) {
			if (m_ImageBuf1[y][x] == 0) {
				m_Resultimg[y][x] = 0;
				continue;
			}
			sum = 0;
			// 배경 영역이 아닌 경우
			for (q = 0; q <= 2; q++) {
				for (p = 0; p <= 2; p++) {
					if (m_ImageBuf1[y + q - 1][x + p - 1] != 0) sum++;
				}
			}
			// 내부점인 경우 그대로 둠.
			if (sum == 9) m_Resultimg[y][x] = 255;
			else m_Resultimg[y][x] = 0;
		}
	}
}


void CPARKDoc::Labeling()
{
	int x, y, l_no = 100;
	for (y = 0; y < 256; y++) {
		for (x = 0; x < 256; x++) {
			if (m_OpenImg[y][x] == 0) m_Resultimg[y][x] = 0;
			else m_Resultimg[y][x] = 1;
		}
	}
	for (y = 0; y < 256; y++) {
		for (x = 0; x < 256; x++) {
			if (m_Resultimg[y][x] != 1) continue;
			Label_p(x, y, l_no);
			l_no += 5;
		}
	}
}


void CPARKDoc::Label_p(int x, int y, int lable_no)
{
	int p, q;
	m_Resultimg[y][x] = lable_no;
	for (q = y - 1; q <= y + 1; q++) {
		for (p = x - 1; p <= x + 1; p++) {
			if (p < 0 || p > 255) continue;
			if (q < 0 || q > 255) continue;
			if (m_Resultimg[q][p] == 1) Label_p(p, q, lable_no);
		}
	}
}
