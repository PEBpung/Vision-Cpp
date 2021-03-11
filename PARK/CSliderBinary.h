#pragma once


// CSliderBinary 대화 상자

class CSliderBinary : public CDialogEx
{
	DECLARE_DYNAMIC(CSliderBinary)

public:
	CSliderBinary(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CSliderBinary();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SLIDER_BIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedButtonBin();
	int m_binaryDisp;
	CSliderCtrl m_sliderBinary;
	virtual BOOL OnInitDialog();
	afx_msg void OnCustomdrawSliderBin(NMHDR* pNMHDR, LRESULT* pResult);
};
