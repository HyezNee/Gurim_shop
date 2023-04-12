
// TeamProjectView.h: CTeamProjectView 클래스의 인터페이스
//

#pragma once
#include "CAdvImg.h"
//#include <vector>
//#include "CMyStatic.h"
#include "CMyPaper.h"

class CTeamProjectDoc;
class CTeamProjectView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CTeamProjectView() noexcept;
	DECLARE_DYNCREATE(CTeamProjectView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_TEAMPROJECT_FORM };
#endif

// 특성입니다.
public:
	CTeamProjectDoc* GetDocument() const;
	//CAdvImg img;	//작업할 이미지 예시
	//CRgn rgn;		//이미지 필터를 적용할 범위 정보를 담고 있는 리전
	//std::vector<CPoint> points;	//마우스를 클릭했을 때 좌표값을 담을 수 있는 벡터

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CTeamProjectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	//도구 버튼 멤버 변수들
	CBitmapButton m_pen;
	CBitmapButton m_select;
	CBitmapButton m_paint;
	CBitmapButton m_text;
	CBitmapButton m_eraser;
	CBitmapButton m_spoid;
	CBitmapButton m_magnify;
	CBitmapButton m_line;
	CBitmapButton m_freeline;
	CBitmapButton m_circle;
	CBitmapButton m_changecolor;

	//상세 설정 부분 멤버 변수들
	CMFCButton m_color1;
	CMFCButton m_color2;
	CSliderCtrl m_thicknessSlider;

	//상세 설정 부분 멤버 함수들
	afx_msg void OnBnClickedColor1();
	//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedColor2();

	//도구 버튼 이벤트 처리 함수들
	afx_msg void OnBnClickedPen();
	afx_msg void OnBnClickedFreeline();
	
	//CStatic m_paper;	//작업창 멤버변수
	//CMyStatic m_paper;	//작업창 멤버변수
	CMyPaper* m_paper;	//작업창 멤버변수
	//CRect m_rect;	//작업창 정보를 받아올 객체

	//그림판 기본 기능 멤버 변수들
	int status;  // 선택한 기능 종류
	//CPoint start; // LButton Down 지점
	//CPoint end; //LButton UP 지점
	//int mouseclick; //마우스 클릭 상태 
	COLORREF color1; //색1, 도형 외곽선, 직선, 곡선 색깔
	COLORREF color2; //색2, 도형배경색
	bool isColor1;	//색 없음 여부
	bool isColor2;
	//CPen pen; //색1을 쥐어줄 객체
	//CBrush brush; //색2를 쥐어줄 객체
	int thickness;  //선 굵기
	int colorsel;
	
	//이미지 효과 기능 멤버 변수들
	int imageEffects;	//0 : 효과X, 1: 색조/채도, 2 : 블러, 3 : 흑백, 4 : 명도/대비, 5 : 모자이크, 6 : 지우개, 7 : 선택 영역, 8 : 텍스트 박스, 9 : 스포이드, 10 : 페인트통

	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedSelect();
	afx_msg void OnBnClickedPaint2();
	afx_msg void OnBnClickedText();
	afx_msg void OnBnClickedEraser();
	afx_msg void OnBnClickedSpoid();
	afx_msg void OnBnClickedMagnify();
	afx_msg void OnBnClickedLine();
	afx_msg void OnBnClickedCircle();
	afx_msg void OnBnClickedChangecolor();
//	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void OnShape(UINT nID);
	afx_msg void OnBnDoubleclickedColor1();
	afx_msg void OnBnDoubleclickedColor2();
	afx_msg void OnSaturation();
	afx_msg void OnBlur();
	afx_msg void OnGray();
	afx_msg void OnAlpha();
	afx_msg void OnMosaic();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	CListBox m_layerList;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonRemove();
	afx_msg void OnLbnSelchangeListLayer();
	afx_msg void OnLbnDblclkListLayer();
};

#ifndef _DEBUG  // TeamProjectView.cpp의 디버그 버전
inline CTeamProjectDoc* CTeamProjectView::GetDocument() const
   { return reinterpret_cast<CTeamProjectDoc*>(m_pDocument); }
#endif

