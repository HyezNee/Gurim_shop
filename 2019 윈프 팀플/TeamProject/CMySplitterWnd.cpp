// CMySplitterWnd.cpp: 구현 파일
//

#include "stdafx.h"
#include "TeamProject.h"
#include "CMySplitterWnd.h"


// CMySplitterWnd

IMPLEMENT_DYNAMIC(CMySplitterWnd, CSplitterWnd)

CMySplitterWnd::CMySplitterWnd()
{

}

CMySplitterWnd::~CMySplitterWnd()
{
}


BEGIN_MESSAGE_MAP(CMySplitterWnd, CSplitterWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CMySplitterWnd 메시지 처리기




void CMySplitterWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	//CWnd::OnLButtonDown(nFlags, point);
}


void CMySplitterWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	//CWnd::OnMouseMove(nFlags, point);
}
