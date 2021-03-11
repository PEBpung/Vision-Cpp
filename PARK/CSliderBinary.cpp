// CSliderBinary.cpp: 구현 파일
//

#include "pch.h"
#include "PARK.h"
#include "CSliderBinary.h"
#include "afxdialogex.h"

#include "ChildFrm.h"
#include "MainFrm.h"
#include "PARKDoc.h"

// CSliderBinary 대화 상자

IMPLEMENT_DYNAMIC(CSliderBinary, CDialogEx)

CSliderBinary::CSliderBinary(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SLIDER_BIN, pParent)
	, m_binaryDisp(0)
{

}

CSliderBinary::~CSliderBinary()
{
}

void CSliderBinary::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BIN, m_binaryDisp);
	DDV_MinMaxInt(pDX, m_binaryDisp, 0, 255);
	DDX_Control(pDX, IDC_SLIDER_BIN, m_sliderBinary);
}


BEGIN_MESSAGE_MAP(CSliderBinary, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_BIN, &CSliderBinary::OnClickedButtonBin)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_BIN, &CSliderBinary::OnCustomdrawSliderBin)
END_MESSAGE_MAP()


// CSliderBinary 메시지 처리기


void CSliderBinary::OnClickedButtonBin()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnOK();
}


BOOL CSliderBinary::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_sliderBinary.SetRange(0, 255);
	m_sliderBinary.SetPos(120);

	m_binaryDisp = m_sliderBinary.GetPos();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CSliderBinary::OnCustomdrawSliderBin(NMHDR* pNMHDR, LRESULT* pResult)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pFrame);
	CChildFrame* pChild = (CChildFrame*)pFrame->GetActiveFrame();
	ASSERT(pChild);
	CPARKDoc* pDoc = (CPARKDoc*)pChild->GetActiveDocument();
	ASSERT(pDoc);

	m_binaryDisp = m_sliderBinary.GetPos();
	UpdateData(FALSE);

	pDoc->m_slideBinarySUM(m_binaryDisp);

}
