#pragma once


// CCONST 대화 상자

class CCONST : public CDialog
{
	DECLARE_DYNAMIC(CCONST)

public:
	CCONST(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CCONST();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CONST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_Const;
};
