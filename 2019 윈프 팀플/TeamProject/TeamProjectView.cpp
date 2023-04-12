
// TeamProjectView.cpp: CTeamProjectView 클래스의 구현
//

#include "framework.h"
#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "TeamProject.h"
#endif

#include "TeamProjectDoc.h"
#include "TeamProjectView.h"

#include "MainFrm.h"

#include "CAddLayerDialog.h"
#include "CEditLayerDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTeamProjectView

IMPLEMENT_DYNCREATE(CTeamProjectView, CFormView)

BEGIN_MESSAGE_MAP(CTeamProjectView, CFormView)
	ON_BN_CLICKED(IDC_PEN, &CTeamProjectView::OnBnClickedPen)
	ON_BN_CLICKED(IDC_FREELINE, &CTeamProjectView::OnBnClickedFreeline)
	ON_WM_PAINT()
	//ON_WM_LBUTTONDOWN()
	//ON_WM_MOUSEMOVE()
	//ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_SELECT, &CTeamProjectView::OnBnClickedSelect)
	ON_BN_CLICKED(IDC_PAINT2, &CTeamProjectView::OnBnClickedPaint2)
	ON_BN_CLICKED(IDC_TEXT, &CTeamProjectView::OnBnClickedText)
	ON_BN_CLICKED(IDC_ERASER, &CTeamProjectView::OnBnClickedEraser)
	ON_BN_CLICKED(IDC_SPOID, &CTeamProjectView::OnBnClickedSpoid)
	ON_BN_CLICKED(IDC_MAGNIFY, &CTeamProjectView::OnBnClickedMagnify)
	ON_BN_CLICKED(IDC_LINE, &CTeamProjectView::OnBnClickedLine)
	ON_BN_CLICKED(IDC_CIRCLE, &CTeamProjectView::OnBnClickedCircle)
	ON_BN_CLICKED(IDC_CHANGECOLOR, &CTeamProjectView::OnBnClickedChangecolor)
	ON_BN_CLICKED(IDC_COLOR1, &CTeamProjectView::OnBnClickedColor1)
	//	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_COLOR2, &CTeamProjectView::OnBnClickedColor2)
	//	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_COMMAND_RANGE(IDC_RADIOCIRCLE, IDC_RADIORECTANGLE, OnShape)
	ON_BN_DOUBLECLICKED(IDC_COLOR1, &CTeamProjectView::OnBnDoubleclickedColor1)
	ON_BN_DOUBLECLICKED(IDC_COLOR2, &CTeamProjectView::OnBnDoubleclickedColor2)
	ON_COMMAND(ID_32775, &CTeamProjectView::OnSaturation)
	ON_COMMAND(ID_32773, &CTeamProjectView::OnBlur)
	ON_COMMAND(ID_32776, &CTeamProjectView::OnGray)
	ON_COMMAND(ID_32779, &CTeamProjectView::OnAlpha)
	ON_COMMAND(ID_32772, &CTeamProjectView::OnMosaic)
	ON_WM_SETFOCUS()
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CTeamProjectView::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, &CTeamProjectView::OnBnClickedButtonRemove)
	ON_LBN_SELCHANGE(IDC_LIST_LAYER, &CTeamProjectView::OnLbnSelchangeListLayer)
	ON_LBN_DBLCLK(IDC_LIST_LAYER, &CTeamProjectView::OnLbnDblclkListLayer)
END_MESSAGE_MAP()

// CTeamProjectView 생성/소멸

CTeamProjectView::CTeamProjectView() noexcept
	//: CFormView(IDD_TEAMPROJECT_FORM), status(0), color1(RGB(0, 0, 0)), color2(RGB(255, 255, 255)), isColor1(true), isColor2(true),
	//mouseclick(0), thickness(1), m_rect(CRect(0, 0, 0, 0)), isSaturation(false)
	: CFormView(IDD_TEAMPROJECT_FORM), status(0), color1(RGB(0, 0, 0)), color2(RGB(255, 255, 255)), isColor1(true), isColor2(true),
	thickness(1), imageEffects(0), colorsel(0)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	//img.Load(_T(".\\flower.jpg"));
}

CTeamProjectView::~CTeamProjectView()
{
}

void CTeamProjectView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PEN, m_pen);
	DDX_Control(pDX, IDC_SELECT, m_select);
	DDX_Control(pDX, IDC_PAINT2, m_paint);
	DDX_Control(pDX, IDC_TEXT, m_text);
	DDX_Control(pDX, IDC_ERASER, m_eraser);
	DDX_Control(pDX, IDC_SPOID, m_spoid);
	DDX_Control(pDX, IDC_MAGNIFY, m_magnify);
	DDX_Control(pDX, IDC_LINE, m_line);
	DDX_Control(pDX, IDC_FREELINE, m_freeline);
	DDX_Control(pDX, IDC_CIRCLE, m_circle);
	DDX_Control(pDX, IDC_CHANGECOLOR, m_changecolor);
	//DDX_Control(pDX, IDC_PAPER, m_paper);
	DDX_Control(pDX, IDC_COLOR1, m_color1);
	DDX_Control(pDX, IDC_COLOR2, m_color2);
	DDX_Control(pDX, IDC_SLIDER1, m_thicknessSlider);
	DDX_Control(pDX, IDC_LIST_LAYER, m_layerList);
}

BOOL CTeamProjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CTeamProjectView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();	//크기를 MainFrm에서 임의로 지정하기 위해 주석처리..하려고 했는데 그냥 안하기로^^

	//도구 모음 항목 -> 버튼에 비트맵 이미지 추가
	m_pen.LoadBitmaps(IDB_PEN1, IDB_PEN2, IDB_PEN3, IDB_PEN1);
	m_pen.SizeToContent();	//m_pen의 사이즈를 컨텐츠에 맞게 맞춤
	m_select.LoadBitmaps(IDB_SELECT1, IDB_SELECT2, IDB_SELECT3, IDB_SELECT1);
	m_select.SizeToContent();
	m_paint.LoadBitmaps(IDB_PAINT1, IDB_PAINT2, IDB_PAINT3, IDB_PAINT1);
	m_paint.SizeToContent();
	m_text.LoadBitmaps(IDB_TEXT1, IDB_TEXT2, IDB_TEXT3, IDB_TEXT1);
	m_text.SizeToContent();
	m_eraser.LoadBitmaps(IDB_ERASER1, IDB_ERASER2, IDB_ERASER3, IDB_ERASER1);
	m_eraser.SizeToContent();
	m_spoid.LoadBitmaps(IDB_SPOID1, IDB_SPOID2, IDB_SPOID3, IDB_SPOID1);
	m_spoid.SizeToContent();
	m_magnify.LoadBitmaps(IDB_MAGNIFY1, IDB_MAGNIFY2, IDB_MAGNIFY3, IDB_MAGNIFY1);
	m_magnify.SizeToContent();
	m_line.LoadBitmaps(IDB_LINE1, IDB_LINE2, IDB_LINE3, IDB_LINE1);
	m_line.SizeToContent();
	m_freeline.LoadBitmaps(IDB_FREELINE1, IDB_FREELINE2, IDB_FREELINE3, IDB_FREELINE1);
	m_freeline.SizeToContent();
	m_circle.LoadBitmaps(IDB_CIRCLE1, IDB_CIRCLE2, IDB_CIRCLE3, IDB_CIRCLE1);
	m_circle.SizeToContent();
	m_changecolor.LoadBitmaps(IDB_CHANGECOLOR1, IDB_CHANGECOLOR2, IDB_CHANGECOLOR3, IDB_CHANGECOLOR1);
	m_changecolor.SizeToContent();

	//세부 설정 항목 -> 버튼 배경색 초기 설정
	m_color1.SetTextColor(RGB(255, 255, 255));
	m_color1.SetFaceColor(color1, true);
	m_color2.SetTextColor(RGB(0, 0, 0));
	m_color2.SetFaceColor(color2, true);
	//세부 설정 항목 -> 획 굵기 슬라이더 초기 설정
	m_thicknessSlider.SetRange(1, 5, TRUE);
	m_thicknessSlider.SetPos(1);
	//세부 설정 항목 -> 원, 사각형 초기 설정
	((CButton*)GetDlgItem(IDC_RADIOCIRCLE)) ->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_RADIOCIRCLE))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_RADIORECTANGLE))->EnableWindow(FALSE);

	//m_paper.m_parent = this;
	m_paper = (CMyPaper*)((CMainFrame*)AfxGetMainWnd())->m_splitwnd.GetPane(0, 1);
	m_paper->m_parent = this;
}


// CTeamProjectView 진단

#ifdef _DEBUG
void CTeamProjectView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTeamProjectView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CTeamProjectDoc* CTeamProjectView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTeamProjectDoc)));
	return (CTeamProjectDoc*)m_pDocument;
}
#endif //_DEBUG


// CTeamProjectView 메시지 처리기


void CTeamProjectView::OnBnClickedPen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	status = 1;
	((CButton*)GetDlgItem(IDC_RADIOCIRCLE))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_RADIORECTANGLE))->EnableWindow(FALSE);
}


void CTeamProjectView::OnBnClickedFreeline()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	status = 5;
	((CButton*)GetDlgItem(IDC_RADIOCIRCLE))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_RADIORECTANGLE))->EnableWindow(FALSE);
}


void CTeamProjectView::OnPaint()
{
	CPaintDC dc2(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CFormView::OnPaint()을(를) 호출하지 마십시오.
	//CClientDC dc(&m_paper);
	//img.BitBlt(dc, 0, 0, img.GetWidth(), img.GetHeight(), 0, 0);
	//m_paper->OnPaint();
}


BOOL CTeamProjectView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	/*if (pMsg->hwnd == GetDlgItem(IDC_PAPER)->GetSafeHwnd()) {
		//TRACE(_T("ccc\r\n"));
		if (isSaturation && pMsg->message == WM_LBUTTONDOWN) {
			CString str;
			CPoint pt = pMsg->pt;
			GetDlgItem(IDC_PAPER)->ScreenToClient(&pt);
			points.push_back(pt);
			str.Format(_T("%d %d \n"), pt.x, pt.y);
			TRACE(str);
			return true;
		}
		else if (isSaturation && pMsg->message == WM_RBUTTONDOWN) {
			if (points.size() > 2) {
				rgn.DeleteObject();
				rgn.CreatePolygonRgn(&points[0], points.size(), ALTERNATE);
				CCutPrevDialog dlg(&img, &rgn);
				int a = dlg.DoModal();
				if (a == -1) {
					TRACE(_T("ERR MAKING dig. \n\r"));
				}
				points.clear();
				Invalidate(false);
				return true;
			}
		}
	}*/
	return CFormView::PreTranslateMessage(pMsg);
}

//void CTeamProjectView::OnLButtonDown(UINT nFlags, CPoint point)
//{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//CClientDC dc(&m_paper);
	//ClientToScreen(&point);	//현재 좌표를 픽쳐컨트롤에 맞추는 작업
	//GetDlgItem(IDC_PAPER)->ScreenToClient(&point);
	//GetDlgItem(IDC_PAPER)->GetClientRect(&m_rect);
	//if (point.x >= 0 && point.x <= m_rect.Width() && point.y >= 0 && point.y <= m_rect.Height()) {
	//	mouseclick = 1;
	//	switch (status)
	//	{
	//	case 1:  //펜, 자유곡선
	//			start = point;
	//			//TRACE("%d, %d", point.x, point.y);
	//			dc.MoveTo(point);
	//		break;
	//	case 2: //직선 그리기
	//			start = point;
	//		break;
	//	case 3: //사각형 생성
	//			SetCapture();
	//			start = end = point;
	//		break;
	//	case 4: //원 생성
	//			SetCapture();
	//			start = end = point;
	//		break;
	//	case 5: //다각형 생성
	//		break;
	//	case 6: //지우개
	//		break;
	//	}
	//}
//	CFormView::OnLButtonDown(nFlags, point);
//}


//void CTeamProjectView::OnMouseMove(UINT nFlags, CPoint point)
//{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//CClientDC dc(&m_paper);
	//ClientToScreen(&point);
	//GetDlgItem(IDC_PAPER)->ScreenToClient(&point);
	//if (mouseclick == 1 && point.x >= 0 && point.x <= m_rect.Width() && point.y >= 0 && point.y <= m_rect.Height()) {
	//	switch (status)
	//	{
	//	case 1:  //펜, 자유곡선
	//		if (isColor1) {
	//			pen.CreatePen(PS_SOLID, thickness, color1);
	//			dc.SelectObject(&pen);
	//		}
	//		else
	//			dc.SelectStockObject(NULL_PEN);
	//		dc.MoveTo(start);
	//		dc.LineTo(point);
	//		if (isColor1)
	//			pen.DeleteObject();
	//		start = point;
	//		break;
	//	case 2: //직선 그리기
	//			//dc.MoveTo(start);
	//			//dc.SetROP2(R2_COPYPEN);
	//			//dc.LineTo(point);
	//		break;
	//	case 3: //사각형 생성
	//		dc.SelectStockObject(NULL_BRUSH);
	//		dc.SetROP2(R2_NOT);
	//		dc.Rectangle(start.x, start.y, end.x, end.y);
	//		end = point;	//좌표 갱신하고
	//		dc.Rectangle(start.x, start.y, end.x, end.y);
	//		break;
	//	case 4: //원 생성
	//		dc.SelectStockObject(NULL_BRUSH);
	//		dc.SetROP2(R2_NOT);
	//		dc.Ellipse(start.x, start.y, end.x, end.y);	//이전 위치에 있던 원을 지우고
	//		end = point;	//좌표 갱신하고
	//		dc.Ellipse(start.x, start.y, end.x, end.y);	//다시 그리기
	//		break;
	//	case 5: //다각형 생성
	//		break;
	//	case 6: //지우개
	//		pen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	//		brush.CreateSolidBrush(RGB(255, 255, 255));
	//		dc.SelectObject(&pen);
	//		dc.SelectObject(&brush);
	//		dc.SetROP2(R2_COPYPEN);
	//		dc.Rectangle(point.x - 5, point.y - 5, point.x + 5, point.y + 5);
	//		pen.DeleteObject();
	//		brush.DeleteObject();
	//		break;
	//	}
	//}
	//else {	//도중에 픽쳐컨트롤을 벗어났을 때
	//	mouseclick = 0;
	//}
//	CFormView::OnMouseMove(nFlags, point);
//}


//void CTeamProjectView::OnLButtonUp(UINT nFlags, CPoint point)
//{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//CClientDC dc(&m_paper);
	//ClientToScreen(&point);
	//GetDlgItem(IDC_PAPER)->ScreenToClient(&point);
	//if (point.x >= 0 && point.x <= m_rect.Width() && point.y >= 0 && point.y <= m_rect.Height()) {
	//	switch (status)
	//	{
	//	case 1:  //펜, 자유곡선
	//		pen.DeleteObject();
	//		break;
	//	case 2: //직선 그리기
	//		if (isColor1) {
	//			pen.CreatePen(PS_SOLID, thickness, color1);
	//			dc.SelectObject(&pen);
	//		}
	//		else
	//			dc.SelectStockObject(NULL_PEN);
	//		end = point;
	//		dc.MoveTo(start);
	//		dc.LineTo(end);
	//		if (isColor1)
	//			pen.DeleteObject();
	//		break;
	//	case 3: //사각형 생성
	//		dc.SelectStockObject(NULL_BRUSH);
	//		dc.SetROP2(R2_NOT);
	//		dc.Rectangle(start.x, start.y, end.x, end.y);
	//		dc.SetROP2(R2_COPYPEN);
	//		end = point;
	//		if (isColor1) {
	//			pen.CreatePen(PS_SOLID, thickness, color1);
	//			dc.SelectObject(&pen);
	//		}
	//		else
	//			dc.SelectStockObject(NULL_PEN);
	//		if (isColor2) {
	//			brush.CreateSolidBrush(color2);
	//			dc.SelectObject(&brush);
	//		}
	//		else
	//			dc.SelectStockObject(NULL_BRUSH);
	//		dc.Rectangle(start.x, start.y, end.x, end.y);
	//		if (isColor1)
	//			pen.DeleteObject();
	//		if (isColor2)
	//			brush.DeleteObject();
	//		ReleaseCapture();
	//		break;
	//	case 4: //원 생성
	//		dc.SelectStockObject(NULL_BRUSH);
	//		dc.SetROP2(R2_NOT);
	//		dc.Ellipse(start.x, start.y, end.x, end.y);
	//		dc.SetROP2(R2_COPYPEN);
	//		end = point;
	//		if (isColor1) {
	//			pen.CreatePen(PS_SOLID, thickness, color1);
	//			dc.SelectObject(&pen);
	//		}
	//		else
	//			dc.SelectStockObject(NULL_PEN);
	//		if (isColor2) {
	//			brush.CreateSolidBrush(color2);
	//			dc.SelectObject(&brush);
	//		}
	//		else
	//			dc.SelectStockObject(NULL_BRUSH);
	//		dc.Ellipse(start.x, start.y, end.x, end.y);
	//		if (isColor1)
	//			pen.DeleteObject();
	//		if (isColor2)
	//			brush.DeleteObject();
	//		ReleaseCapture();
	//		break;
	//	case 5: //다각형 생성
	//		break;
	//	case 6: //지우개
	//		break;
	//	}
	//	mouseclick = 0;
	//}
//	CFormView::OnLButtonUp(nFlags, point);
//}


void CTeamProjectView::OnBnClickedSelect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	((CButton*)GetDlgItem(IDC_RADIOCIRCLE))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_RADIORECTANGLE))->EnableWindow(FALSE);
	status = 7;
	AfxMessageBox(_T("자를 사각형을 드래그하세요"));
}


void CTeamProjectView::OnBnClickedPaint2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	((CButton*)GetDlgItem(IDC_RADIOCIRCLE))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_RADIORECTANGLE))->EnableWindow(FALSE);
	status = 10;
}

//7 : 선택 영역, 8 : 텍스트 박스, 9 : 스포이드, 10 : 페인트통
void CTeamProjectView::OnBnClickedText()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	((CButton*)GetDlgItem(IDC_RADIOCIRCLE))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_RADIORECTANGLE))->EnableWindow(FALSE);
	status = 8;
}


void CTeamProjectView::OnBnClickedEraser()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	status = 6;
	((CButton*)GetDlgItem(IDC_RADIOCIRCLE))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_RADIORECTANGLE))->EnableWindow(FALSE);
}


void CTeamProjectView::OnBnClickedSpoid()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	((CButton*)GetDlgItem(IDC_RADIOCIRCLE))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_RADIORECTANGLE))->EnableWindow(FALSE);
	status = 9;
}


void CTeamProjectView::OnBnClickedMagnify()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	((CButton*)GetDlgItem(IDC_RADIOCIRCLE))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_RADIORECTANGLE))->EnableWindow(FALSE);
}


void CTeamProjectView::OnBnClickedLine()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	status = 2;
	((CButton*)GetDlgItem(IDC_RADIOCIRCLE))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_RADIORECTANGLE))->EnableWindow(FALSE);
}


void CTeamProjectView::OnBnClickedCircle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	((CButton*)GetDlgItem(IDC_RADIOCIRCLE))->EnableWindow(TRUE);	//세부 설정 원/사각형 라디오버튼 활성화
	((CButton*)GetDlgItem(IDC_RADIORECTANGLE))->EnableWindow(TRUE);
	if (((CButton*)GetDlgItem(IDC_RADIOCIRCLE))->GetCheck() == BST_CHECKED)
		status = 4;
	else
		status = 3;
}


void CTeamProjectView::OnBnClickedChangecolor()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	((CButton*)GetDlgItem(IDC_RADIOCIRCLE))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_RADIORECTANGLE))->EnableWindow(FALSE);
}

void CTeamProjectView::OnBnClickedColor1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다
	colorsel = 0;
	if(isColor1) {
		isColor1 = false;
		m_color1.SetTextColor(RGB(200, 200, 200));
		m_color1.SetFaceColor(RGB(200, 200, 200), true);
	}
	else {
		isColor1 = true;
		CColorDialog dlg(color1, CC_FULLOPEN);
		dlg.DoModal();
		color1 = dlg.GetColor();
		m_color1.SetTextColor(RGB(255, 255, 255));
		m_color1.SetFaceColor(color1, true);
	}
}


void CTeamProjectView::OnBnClickedColor2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	colorsel = 1;
	if (isColor2) {
		isColor2 = false;
		m_color2.SetTextColor(RGB(200, 200, 200));
		m_color2.SetFaceColor(RGB(200, 200, 200), true);
	}
	else {
		isColor2 = true;
		CColorDialog dlg(color2, CC_FULLOPEN);
		dlg.DoModal();
		color2 = dlg.GetColor();
		m_color2.SetTextColor(RGB(255, 255, 255));
		m_color2.SetFaceColor(color2, true);
	}
}




void CTeamProjectView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	thickness = m_thicknessSlider.GetPos();

	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CTeamProjectView::OnShape(UINT nID)	//아마 메세지처리기없이 직접 만들었는듯
{
	switch (nID) {
	case IDC_RADIOCIRCLE:
		status = 4;
		break;
	case IDC_RADIORECTANGLE:
		status = 3;
		break;
	}
}

void CTeamProjectView::OnBnDoubleclickedColor1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	colorsel = 0;
	m_color1.SetTextColor(RGB(255, 255, 255));
	m_color1.SetFaceColor(color1, true);
	isColor1 = true;
	CColorDialog dlg(color1, CC_FULLOPEN);
	dlg.DoModal();
	color1 = dlg.GetColor();
	m_color1.SetTextColor(RGB(255, 255, 255));
	m_color1.SetFaceColor(color1, true);
}


void CTeamProjectView::OnBnDoubleclickedColor2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	colorsel = 1;
	m_color2.SetTextColor(RGB(0, 0, 0));
	m_color2.SetFaceColor(color2, true);
	isColor2 = true;
	CColorDialog dlg(color2, CC_FULLOPEN);
	dlg.DoModal();
	color2 = dlg.GetColor();
	m_color2.SetTextColor(RGB(0, 0, 0));
	m_color2.SetFaceColor(color2, true);
}


void CTeamProjectView::OnSaturation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	imageEffects = 1;
}


void CTeamProjectView::OnBlur()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	imageEffects = 2;
}


void CTeamProjectView::OnGray()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	imageEffects = 3;
}


void CTeamProjectView::OnAlpha()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	imageEffects = 4;
}


void CTeamProjectView::OnMosaic()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	imageEffects = 5;
}


void CTeamProjectView::OnSetFocus(CWnd* pOldWnd)	//이미지효과 선택해놓고 중간에 딴거할때
{
	CFormView::OnSetFocus(pOldWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (imageEffects != 0) {
		imageEffects = 0;
		m_paper->points.clear();
	}
}


void CTeamProjectView::OnBnClickedButtonAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/*CAddLayerDialog dlg(&(m_paper->manager));
	if (dlg.DoModal() == IDOK) {
		if (m_paper->manager.getLayerCounts() == 1) {
			CAdvImg* tmp = m_paper->manager.getLayerImage(0);	//0에서 바꿈	m_paper->manager.getLayerCounts()-1
			m_paper->output.Destroy();
			m_paper->output.Create(tmp->GetWidth(), tmp->GetHeight(), tmp->GetBPP());
			m_paper->img = tmp;

			if (m_paper->hdc != NULL)
				m_paper->img->ReleaseDC();
			//m_paper->currentImg.ReleaseDC();
			m_paper->hdc = m_paper->img->GetDC();
		//m_paper->manager.getOneBlendedImage(&(m_paper->currentImg), m_paper->manager.getLayerCounts() - 1);
		//m_paper->hdc = m_paper->currentImg.GetDC();	//DC 위치 바꿔주고
		}
		m_layerList.AddString(m_paper->manager.getLayer(m_paper->manager.getLayerCounts() - 1)->name);

		//Invalidate(FALSE);
		m_paper->OnPaint();
	}*/

	CAddLayerDialog dlg(&(m_paper->manager));
	if (dlg.DoModal() == IDOK) {
		if (m_paper->manager.getLayerCounts() == 1) {
			CAdvImg* tmp = m_paper->manager.getLayerImage(0);
			m_paper->output.Destroy();
			m_paper->output.Create(tmp->GetWidth(), tmp->GetHeight(), tmp->GetBPP());
			m_paper->img = tmp;
		}
		m_layerList.AddString(m_paper->manager.getLayer(m_paper->manager.getLayerCounts() - 1)->name);
		m_paper->output.GetDC();	//추가

		m_paper->Invalidate(FALSE);
	}
}


void CTeamProjectView::OnBnClickedButtonRemove()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/*if (m_paper->manager.getLayerCounts() > 0) {
		int i = m_layerList.GetCurSel();
		if (i < 0)
			return;
		m_paper->img->ReleaseDC();
		//m_paper->currentImg.ReleaseDC();
		m_paper->manager.remove(i);
		m_layerList.DeleteString(i);
		--i;
		if (i < 0) { // no layer left
			m_paper->img = NULL;
		}
		else {
			m_layerList.SetCurSel(i);
		}
	}
	Invalidate();*/

	if (m_paper->manager.getLayerCounts() > 0) {
		int i = m_layerList.GetCurSel();
		if (i < 0)
			return;
		m_paper->manager.remove(i);
		m_layerList.DeleteString(i);
		--i;
		if (i < 0) { // no layer left
			m_paper->img = NULL;
			m_paper->output.ReleaseDC();
		}
		else {
			m_layerList.SetCurSel(i);
		}
	}
	m_paper->Invalidate();
}


void CTeamProjectView::OnLbnSelchangeListLayer()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/*m_paper->img->ReleaseDC();
	//m_paper->currentImg.ReleaseDC();
	//m_paper->manager.getOneBlendedImage(&(m_paper->currentImg), m_layerList.GetCurSel());
	//m_paper->hdc = m_paper->currentImg.GetDC();	//DC 위치 바꿔주고
	m_paper->img = m_paper->manager.getLayerImage(m_layerList.GetCurSel());	//이미지 바꿔주기
	m_paper->hdc = m_paper->img->GetDC();	//DC 위치 바꿔주고*/

	int i = m_layerList.GetCurSel();
	m_paper->img = m_paper->manager.getLayerImage(i);
}


void CTeamProjectView::OnLbnDblclkListLayer()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int i = m_layerList.GetCurSel();
	CEditLayerDialog dlg(&(m_paper->manager), i);
	if (dlg.DoModal() == IDOK) {
		m_layerList.InsertString(i, m_paper->manager.getLayer(i)->name);
		m_layerList.DeleteString(++i);
		m_paper->Invalidate(false);
	}
}
