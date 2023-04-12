#pragma once
#include "CAdvImg.h"
#include <vector>
#include "LayerManager.h"
#include "CTextBox.h"

class CTeamProjectView;
// CMyPaper 보기

class CMyPaper : public CScrollView
{
	DECLARE_DYNCREATE(CMyPaper)

//protected:
public:
	CMyPaper();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyPaper();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual void OnInitialUpdate();     // 생성된 후 처음입니다.

	DECLARE_MESSAGE_MAP()

public:
	//CAdvImg img;	//작업할 이미지 예시
	CAdvImg* img;	//작업할 이미지 예시
	CTeamProjectView* m_parent;	//팀프로젝트 뷰

	CRgn rgn;		//이미지 필터를 적용할 범위 정보를 담고 있는 리전
	std::vector<CPoint> points;	//마우스를 클릭했을 때 좌표값을 담을 수 있는 벡터

	CRect m_rect;	//작업창 정보를 받아올 객체
	//CClientDC m_dc;
	//CDC* pDC;	//CImage DC
	HDC hdc;	//CImage DC

	//레이어 기능 관련 멤버 변수
	LayerManager manager;
	CImage output;
	CImage currentImg;	//현재 작업중인 레이어를 CImage로 받아온 것

//그림판 기본 기능 멤버 변수들
	CPoint start; // LButton Down 지점
	CPoint end; //LButton UP 지점
	int mouseclick; //마우스 클릭 상태 
	//COLORREF color1; //색1, 도형 외곽선, 직선, 곡선 색깔
	//COLORREF color2; //색2, 도형배경색
	//bool isColor1;	//색 없음 여부
	//bool isColor2;
	CPen pen; //색1을 쥐어줄 객체
	CBrush brush; //색2를 쥐어줄 객체
	//int thickness;  //선 굵기

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSaturation();
	afx_msg void OnBlur();
	afx_msg void OnGray();
	afx_msg void OnAlpha();
	afx_msg void OnMosaic();

	void updateOutput();
	void OnFileOpen();
	void OnFileSave();

	CImage capimage;
	//int isedit;

	//기본기능 추가 구현
	std::vector<CPoint> stack;
	CBitmap bitmap;
	BITMAP bm;
	BYTE *pData = NULL;
	int sp = 0;
	//CRect rect;
	CPoint *plp;
	int textinput = 0;
	int nct = 0; //다각형 각의 갯수
	CTextbox textbox;
	CTextbox **text;
	CTextbox *ptext;
	CString textstr;
	int textcnt = 0;
	int colorsel = 0;
	CAdvImg colorfill;
	//HDC myhdc;
	int width2;
	int height2;
	CDC  MemDC;
	COLORREF fillcolor;
};


