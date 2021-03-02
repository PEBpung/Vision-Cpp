// CCONST.cpp: 구현 파일
//

#include "pch.h"
#include "PARK.h"
#include "CCONST.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "PARKDoc.h"
#include "PARKView.h"

// CCONST 대화 상자

IMPLEMENT_DYNAMIC(CCONST, CDialog)

CCONST::CCONST(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_CONST, pParent)
	, m_Const(0)
{

}

CCONST::~CCONST()
{
}

void CCONST::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CONST, m_Const);
	DDV_MinMaxInt(pDX, m_Const, 0, 255);
}


BEGIN_MESSAGE_MAP(CCONST, CDialog)
END_MESSAGE_MAP()


// CCONST 메시지 처리기
