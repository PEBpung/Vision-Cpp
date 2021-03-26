
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


void CPARKDoc::Labelstack()
{
	int x, y, top;
	short l_no = 0, r, c, m, n;

	// 스택으로 사용할 메모리 할당
	short* stack_x = new short[256 * 256];
	short* stack_y = new short[256 * 256];

	for (y = 0; y < 256; y++) {
		for (x = 0; x < 256; x++) m_Resultimg[y][x] = 0;
	}

	for (y = 0; y < 256; y++) {
		for (x = 0; x < 256; x++) {
			// 이미 방문한 점이거나 픽셀값이 255가 아니면 처리하지 않음
			if (m_OpenImg[y][x] != 255 || m_Resultimg[y][x] != 0) continue;
			// 새로운 영역에 대한 라벨링 처리
			r = y;
			c = x;
			top = 0;
			l_no++;
			while (1) {
				for (m = r - 1; m <= r + 1; m++) {
					if (m < 0 || m > 255) continue;
					for (n = c - 1; n <= c + 1; n++) {
						if (n < 0 || n > 255) continue;
						if (m_OpenImg[m][n] != 255 || m_Resultimg[m][n] != 0) continue;
						m_Resultimg[m][n] = l_no;
						if (Push(stack_x, stack_y, m, n, &top) == -1) continue;
					}
				}
				if (Pop(stack_x, stack_y, &r, &c, &top) == -1) break;
			}
		}
	}
	// 결과 영상 라벨값 간격 조정
	float l_gap = 250.0F / (float)l_no;
	for (y = 0; y < 256; y++) {
		for (x = 0; x < 256; x++) {
			m_Resultimg[y][x] *= l_gap;
		}
	}
}


int CPARKDoc::Push(short* st_x, short* st_y, short xx, short yy, int* top)
{
	if (*top >= 65536) return(-1);	// 최대 개수 초과
	(*top)++;						// 저장 번지 증가
	st_x[*top] = xx;				// x좌표 저장
	st_y[*top] = yy;				// y좌표 저장
	return 0;
}

int CPARKDoc::Pop(short* st_x, short* st_y, short* xx, short* yy, int* top)
{
	if(*top == 0) return(-1);		// 저장값 없음
	*xx = st_x[*top];				// x좌표 저장
	*yy = st_y[*top];				// y좌표 저장
	(*top)--;						// 저장 번지 감소
	return 0;
}


void CPARKDoc::TrackBorder()
{
	// 영역의 경계정보를 저장하기 위한 구조체 메모리
	typedef struct tagBORDERINFO { 
		short *x, * y; 
		short n, dn; 
	} BORDERINFO;

	BORDERINFO stBorderInfo[1000];

	// 추적점을 임시로 저장하기 위한 메모리
	short *xchain = new short[10000];
	short* ychain = new short[10000];

	// 관심 픽셀의 시계방향으로 주위점을 나타내기 위한 좌표 설정
	const POINT nei[8] = {
		{0, 1}, {-1, 1}, {-1, 0}, {-1,-1}, {0, -1}, {1, -1}, {1, 0}, {1,1} };
	int x0, y0, x, y, k, n;
	int numberBorder = 0, border_count, diagonal_count;
	unsigned char c0, c1;

	// 영상에 있는 픽셀이 방문된 점인지를 마크하기 위해 영상 메모리 할당
	for (y = 0; y < 256; y++) {
		for (x = 0; x < 256; x++) m_Resultimg[y][x] = 0;
	}

	for ( y = 0; y < 256; y++)
	{
		for ( x = 0; x < 256; x++)
		{
			// 기준 점
			c0 = m_OpenImg[y][x];
			// 바로 위의 점
			c1 = m_OpenImg[y - 1][x];
			// 경계 판단
			if (c1 == 0 && c0 != 0 && m_Resultimg[y][x] == 0)
			{
				border_count = 0;		// 경계점의 개수를 세기 위한 카운트
				diagonal_count = 0;		// 대각 방향 연결 화소수

				// 시작점
				y0 = y;
				x0 = x;
				// 시작점에서의 조사 시작 방향
				n = 6;
				do
				{
					// 관심점 주의에서 같은 색을 가진 경계점을 찾기 위함
					for ( k = 0; k < 8; k++, n = ((n+1) % 8)) // 01234567 -> 12345670
					{
						// 주위 영역으로 변환, v, u
						short v = (short)(y + nei[n].y);
						short u = (short)(x + nei[n].x);
						// 영역에서 벗어나면 재귀.
						if (u < 0 || u >= 256 || v < 0 || v >= 256)
							continue;
						// 초기 좌표와 같으면 탈출.
						if (m_OpenImg[v][u] == c0) 
							break;
					}
					// MARK 1:: 고립점이라면 탈출
					if (k == 8)
						break;
					// 방문한 점 표시
					m_Resultimg[y][x] = 255;
					// 경계점의 좌표를 저장하고 카운트를 늘림.
					xchain[border_count] = x;
					ychain[border_count++] = y;
					// 경계가 10000 이상이면 탈출
					if (border_count >= 10000) 
						break;
					// 다음 방문할 점으로 값 교체
					x = x + nei[n].x;
					y = y + nei[n].y;
					// 대각선 방향 연결 화소수 체크
					if (n % 2 == 1) 
						diagonal_count++;
					// 01234567 -> 56701234, 다음 관심에 대한 순서
					n = (n + 5) % 8;
				// 초기 위치로 돌아오게 되면 스탑
				} while (!(x == x0 && y == y0));
				// 고립점인 경우 다시 루프를 돈다. (MARK 1에서 왔음)
				if (k == 8)
					continue;

				// ---------------------------------
				// 고립점이 아니라면 영역의 경계정보를 저장
				// ---------------------------------
				// 하지만 너무 작은 영역이면 무시한다.
				if (border_count < 10)
					continue;

				// 영역 정보를 저장하기 위해서 구조체 속의 변수에 경계 수 만큼 메모리 할당
				stBorderInfo[numberBorder].x = new short[border_count];
				stBorderInfo[numberBorder].y = new short[border_count];

				// 위에서 구한 경계수 만큼 돌면서 x와 y좌표를 구조체 배열에 할당.
				for ( k = 0; k < border_count; k++)
				{
					stBorderInfo[numberBorder].x[k] = xchain[k];
					stBorderInfo[numberBorder].y[k] = ychain[k];
				}
				// 마찬가지로 경계점 개수와 대각선 개수를 저장시킴.
				stBorderInfo[numberBorder].n = border_count;
				stBorderInfo[numberBorder++].dn = diagonal_count;

				// 영역의 총 개수는 1000이하로 설정시킴 초과되면 탈출.
				if (numberBorder >= 1000)
					break;
			}
		}
	}
	// 영역의 면적을 Text 정보로 출력한다. 디버깅시에만 동작
	CString tempStr;
	TRACE("\r\n\r\n[ Border Following ]\r\n--------------------\r\n");

	// 화면에 경계를 출력하기 위해 m_ResultImg 배열을 이용.
	memset(m_Resultimg, 255, 256 * 256 * sizeof(unsigned char));

	for ( k = 0; k < numberBorder; k++)
	{
		TRACE("(%d: %d %d, %d)\r\n", k, stBorderInfo[k].n, stBorderInfo[k].dn,
			(int)(1.4142F * stBorderInfo[k].dn) + (stBorderInfo[k].n - stBorderInfo[k].dn));

		// 결과영상 출력처리
		for (int i = 0; i < stBorderInfo[k].n; i++) {
			x = stBorderInfo[k].x[i];
			y = stBorderInfo[k].y[i];
			m_Resultimg[y][x] = 0;
		}
	}
	// 사용이 끝난 동적 메모리를 닫아준다. 
	for (k = 0; k < numberBorder; k++) {
		delete[]stBorderInfo[k].x;
		delete[]stBorderInfo[k].y;
	}
	delete[]xchain;
	delete[]ychain;
}


void CPARKDoc::Filling()
{
	// 영역의 경계정보를 저장하기 위한 구조체 메모리
	typedef struct tagBORDERINFO {
		short* x, * y;
		short n, dn, center_x, center_y;
	} BORDERINFO;

	BORDERINFO stBorderInfo[1000];

	// 추적점을 임시로 저장하기 위한 메모리
	int* xchain = new int[50000];
	int* ychain = new int[50000];

	// 관심 픽셀의 시계방향으로 주위점을 나타내기 위한 좌표 설정
	const POINT nei[8] = {
		{0, 1}, {-1, 1}, {-1, 0}, {-1,-1}, {0, -1}, {1, -1}, {1, 0}, {1,1} };
	int x0, y0, x, y, k, n;
	int numberBorder = 0, border_count, diagonal_count;
	unsigned char c0, c1;

	// 영상에 있는 픽셀이 방문된 점인지를 마크하기 위해 영상 메모리 할당
	for (y = 0; y < 256; y++) {
		for (x = 0; x < 256; x++) m_ImageBuf1[y][x] = 0;
	}

	for (y = 0; y < 256; y++)
	{
		for (x = 0; x < 256; x++)
		{
			// 기준 점
			c0 = m_OpenImg[y][x];
			// 바로 위의 점
			c1 = m_OpenImg[y - 1][x];
			// 경계 판단
			if (c1 == 0 && c0 != 0 && m_ImageBuf1[y][x] == 0)
			{
				border_count = 0;		// 경계점의 개수를 세기 위한 카운트
				diagonal_count = 0;		// 대각 방향 연결 화소수

				// 시작점
				y0 = y;
				x0 = x;
				// 시작점에서의 조사 시작 방향
				n = 6;
				do
				{
					// 관심점 주의에서 같은 색을 가진 경계점을 찾기 위함
					for (k = 0; k < 8; k++, n = ((n + 1) % 8)) // 01234567 -> 12345670
					{
						// 주위 영역으로 변환, v, u
						short v = (short)(y + nei[n].y);
						short u = (short)(x + nei[n].x);
						// 영역에서 벗어나면 재귀.
						if (u < 0 || u >= 256 || v < 0 || v >= 256)
							continue;
						// 초기 좌표와 같으면 탈출.
						if (m_OpenImg[v][u] == c0)
							break;
					}
					// MARK 1:: 고립점이라면 탈출
					if (k == 8)
						break;
					// 방문한 점 표시
					m_ImageBuf1[y][x] = 255;
					// 경계점의 좌표를 저장하고 카운트를 늘림.
					xchain[border_count] = x;
					ychain[border_count++] = y;
					// 경계가 10000 이상이면 탈출
					if (border_count >= 10000)
						break;
					// 다음 방문할 점으로 값 교체
					x = x + nei[n].x;
					y = y + nei[n].y;
					// 대각선 방향 연결 화소수 체크
					if (n % 2 == 1)
						diagonal_count++;
					// 01234567 -> 56701234, 다음 관심에 대한 순서
					n = (n + 5) % 8;
					// 초기 위치로 돌아오게 되면 스탑
				} while (!(x == x0 && y == y0));
				// 고립점인 경우 다시 루프를 돈다. (MARK 1에서 왔음)
				if (k == 8)
					continue;

				// ---------------------------------
				// 고립점이 아니라면 영역의 경계정보를 저장
				// ---------------------------------
				// 하지만 너무 작은 영역이면 무시한다.
				if (border_count < 10)
					continue;

				// 영역 정보를 저장하기 위해서 구조체 속의 변수에 경계 수 만큼 메모리 할당
				stBorderInfo[numberBorder].x = new short[border_count];
				stBorderInfo[numberBorder].y = new short[border_count];

				int sum_x = 0;
				int sum_y = 0;

				// 위에서 구한 경계수 만큼 돌면서 x와 y좌표를 구조체 배열에 할당.
				for (k = 0; k < border_count; k++)
				{
					sum_x += xchain[k];
					sum_y += ychain[k];
					stBorderInfo[numberBorder].x[k] = xchain[k];
					stBorderInfo[numberBorder].y[k] = ychain[k];
				}
				// 마찬가지로 경계점 개수와 대각선 개수를 저장시킴.
				stBorderInfo[numberBorder].center_x = (short)(sum_x / border_count);
				stBorderInfo[numberBorder].center_y = (short)(sum_y / border_count);
				stBorderInfo[numberBorder].n = border_count;
				stBorderInfo[numberBorder++].dn = diagonal_count;

				// 영역의 총 개수는 1000이하로 설정시킴 초과되면 탈출.
				if (numberBorder >= 1000)
					break;
			}
		}
	}

	int top = 0;
	short r, c;
	r = 1;
	c = 1;

	/*while (true)
	{
		// 스택으로 사용할 메모리 할당
		const POINT pos[4] = { { 0, 1 }, { -1, 0 }, { 0, -1 },  { 1, 0 } };

		short* d_x = new short[256 * 256];
		short* d_y = new short[256 * 256];

		for (int i = 0; i < 4; i++) {
			short m = (short)(r + pos[i].y);
			short n = (short)(c + pos[i].x);

			if (n < 0 || n >= 255 || m < 0 || m >= 255 || m_Resultimg[m][n] == 255)
				continue;

			xchain[count] = n;
			ychain[count++] = m;

			if (Push(d_x, d_y, m, n, &top) == -1) continue;
		}

		if (Pop(d_x, d_y, &r, &c, &top) == -1) break;
	}*/
	memset(xchain, 0, 50000);
	memset(ychain, 0, 50000);
	int temp_x, temp_y;
	int count;


	// 화면에 경계를 출력하기 위해 m_ResultImg 배열을 이용.
	memset(m_Resultimg, 255, 256 * 256 * sizeof(unsigned char));

	for (int i = 0; i < numberBorder; i++) {
		count = 0;

		temp_x = stBorderInfo[i].center_x;
		temp_y = stBorderInfo[i].center_y;

		floodfill(temp_x, temp_y, m_ImageBuf1, xchain, ychain, &count);

		for (k = 0; k < count; k++)
		{
			x = xchain[k];
			y = ychain[k];
			if (x < 0 || x >= 255 || y < 0 || y >= 255)
				continue;
			m_Resultimg[y][x] = 0;
		}
	}
	


	// 사용이 끝난 동적 메모리를 닫아준다. 
	for (k = 0; k < numberBorder; k++) {
		delete[]stBorderInfo[k].x;
		delete[]stBorderInfo[k].y;
	}
	delete[]xchain;
	delete[]ychain;

	
}

int CPARKDoc::floodfill(int x, int y, unsigned char image[][256], int* xchain, int* ychain, int* count)
{
	if (x < 0 || x >= 256 || y < 0 || y >= 256 || image[y][x] == 255)
		return NULL;

	else
	{
		if (*count >= 4000)
			return NULL;	// 최대 개수 초과
		image[y][x] = 255;

		xchain[*count] = x;
		ychain[(*count)++] = y;
	}

	floodfill(x + 1, y, image, xchain, ychain, count);
	floodfill(x - 1, y, image, xchain, ychain, count);
	floodfill(x, y + 1, image, xchain, ychain, count);
	floodfill(x, y - 1, image, xchain, ychain, count);

	/*floodfill(x + 1, y + 1, color, image, xchain, ychain, count);
	floodfill(x - 1, y - 1, color, image, xchain, ychain, count);
	floodfill(x - 1, y + 1, color, image, xchain, ychain, count);
	floodfill(x + 1, y - 1, color, image, xchain, ychain, count);*/
}

