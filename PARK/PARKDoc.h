
// PARKDoc.h: CPARKDoc 클래스의 인터페이스
//


#pragma once


class CPARKDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CPARKDoc() noexcept;
	DECLARE_DYNCREATE(CPARKDoc)

// 특성입니다.
public:

// 작업입니다.
public:
	unsigned char m_OpenImg[256][256];
	unsigned char m_Resultimg[256][256];
	// 1. 두 영상의 데이터를 저장하기 위해 사용.
	// 2. 연산 중간 결과를 저장 할 때도 사용.
	unsigned char m_ImageBuf1[256][256]; 
	unsigned char m_ImageBuf2[256][256];
	unsigned int histoin[256];  // 입력 영상의 히스토그램 저장

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CPARKDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	void DoubleLoad();
	void m_slideSUM(int slideconst);
	void HistoIn();
};
