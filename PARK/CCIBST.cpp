// CCIBST.cpp: 구현 파일
//

#include "pch.h"
#include "PARK.h"
#include "CCIBST.h"
#include "afxdialogex.h"


// CCIBST 대화 상자

IMPLEMENT_DYNAMIC(CCIBST, CDialog)

CCIBST::CCIBST(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_CONST, pParent)
{

}

CCIBST::~CCIBST()
{
}

void CCIBST::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCIBST, CDialog)
END_MESSAGE_MAP()


// CCIBST 메시지 처리기
