#pragma once


// CTHIRD 대화 상자

class CTHIRD : public CDialog
{
	DECLARE_DYNAMIC(CTHIRD)

public:
	CTHIRD(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTHIRD();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_THRESHOLD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_ViewThrd;
	int m_Thrd;
};
