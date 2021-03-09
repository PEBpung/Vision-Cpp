// CTHIRD.cpp: 구현 파일
//

#include "pch.h"
#include "PARK.h"
#include "CTHIRD.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "PARKDoc.h"
#include "PARKView.h"

// CTHIRD 대화 상자

IMPLEMENT_DYNAMIC(CTHIRD, CDialog)

CTHIRD::CTHIRD(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_THRESHOLD, pParent)
	, m_Thrd(0)
{

}

CTHIRD::~CTHIRD()
{
}

void CTHIRD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_THRESH, m_Thrd);
	DDV_MinMaxInt(pDX, m_Thrd, 0, 255);
}


BEGIN_MESSAGE_MAP(CTHIRD, CDialog)
END_MESSAGE_MAP()


// CTHIRD 메시지 처리기
