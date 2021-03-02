#pragma once


// CConstCntrDlg 대화 상자

class CConstCntrDlg : public CDialog
{
	DECLARE_DYNAMIC(CConstCntrDlg)

public:
	CConstCntrDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CConstCntrDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SLIDE_CONST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedButton1();
	int m_constDisp;
	CSliderCtrl m_constSlider;
	virtual BOOL OnInitDialog();
	afx_msg void OnCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult);
};
