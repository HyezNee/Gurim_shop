// CMyPaper.cpp: 구현 파일
//

#include "stdafx.h"
#include "TeamProjectView.h"
#include "CMyPaper.h"
#include "CHueSaturation.h"
#include "CBlur.h"
#include "CGray.h"
#include "CAlpha.h"
#include "CMosaic.h"

#include "resource.h"

#include "CAddLayerDialog.h"
#include "CEditLayerDialog.h"

// CMyPaper

IMPLEMENT_DYNCREATE(CMyPaper, CScrollView)

CMyPaper::CMyPaper()
	: mouseclick(0), m_rect(CRect(0, 0, 0, 0)), hdc(NULL)
{
	//img.Load(_T(".\\flower.jpg"));
	img = NULL;
	//pDC = CDC::FromHandle(img.GetDC());
	//hdc = img.GetDC();
}

CMyPaper::~CMyPaper()
{
//	if(img != NULL)
//		img->ReleaseDC();
	//if(hdc!=NULL)
	if (img != NULL)
		output.ReleaseDC();
}


BEGIN_MESSAGE_MAP(CMyPaper, CScrollView)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_PAINT()
	ON_COMMAND(ID_32775, &CMyPaper::OnSaturation)
	ON_COMMAND(ID_32773, &CMyPaper::OnBlur)
	ON_COMMAND(ID_32776, &CMyPaper::OnGray)
	ON_COMMAND(ID_32779, &CMyPaper::OnAlpha)
	ON_COMMAND(ID_32772, &CMyPaper::OnMosaic)
	ON_COMMAND(ID_FILE_OPEN, &CMyPaper::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CMyPaper::OnFileSave)
END_MESSAGE_MAP()


// CMyPaper 그리기

void CMyPaper::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	//CRect rect;
	sizeTotal.cx = 800;
	sizeTotal.cy = 554;
	SetScrollSizes(MM_TEXT, sizeTotal);

	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();	//크기를 MainFrm에서 임의로 지정하기 위해 주석처리..하려고 했는데 그냥 안하기로^^

	//pDC->SetBkMode(TRANSPARENT);
}

void CMyPaper::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CMyPaper 진단

#ifdef _DEBUG
void CMyPaper::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyPaper::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyPaper 메시지 처리기


void CMyPaper::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (manager.getLayerCounts() != 0) {
		CClientDC dc(this);
		ClientToScreen(&point);	//현재 좌표를 픽쳐컨트롤에 맞추는 작업
		this->ScreenToClient(&point);
		this->GetClientRect(&m_rect);
		//if (point.x >= 0 && point.x <= m_rect.Width() && point.y >= 0 && point.y <= m_rect.Height()) {
		if (m_parent->imageEffects == 0) {
			mouseclick = 1;
			switch (m_parent->status)
			{
			case 1:  //펜, 자유곡선
				SetCapture();
				start = point;
				//TRACE("%d, %d", point.x, point.y);
				dc.MoveTo(point);
				//img.ReleaseDC();
				break;
			case 2: //직선 그리기
				SetCapture();
				start = point;
				break;
			case 3: //사각형 생성
				SetCapture();
				start = end = point;
				break;
			case 4: //원 생성
				SetCapture();
				start = end = point;
				break;
			case 5: //다각형 생성
				if (nct == 0)
				{
					CPoint *lp = new CPoint[100];
					plp = lp;
				}
				plp[nct] = point;
				nct++;
				break;
			case 6: //지우개
				break;
			case 7:	//선택 영역 이동
				start = point;
				break;
			case 8:	//텍스트 박스
				start = point;
				break;
			case 9:	//스포이드
				if (m_parent->colorsel == 0) {
					m_parent->color1 = dc.GetPixel(point);
					m_parent->m_color1.SetFaceColor(m_parent->color1, true);
				}
				else if (m_parent->colorsel == 1)
				{
					m_parent->color2 = dc.GetPixel(point);
					m_parent->m_color2.SetFaceColor(m_parent->color2, true);
				}
				break;
			case 10: //색 채우기
			{
				start = point;
				//GetClientRect(&m_rect);
				bitmap.GetObject(sizeof(BITMAP), (LPSTR)&bm);
				pData = (BYTE*)malloc(bm.bmWidthBytes * bm.bmHeight);
				memset(pData, 0x00, bm.bmWidthBytes * bm.bmHeight);
				bitmap.GetBitmapBits(bm.bmWidthBytes * bm.bmHeight, pData);
				RGBQUAD *pRgb = (RGBQUAD*)pData;
				for (int y = 0; y < bm.bmHeight; y++)
				{
					for (int x = 0; x < bm.bmWidth; x++)
					{
						int nPos = x + (y * bm.bmWidth);

						BYTE r = pRgb[nPos].rgbRed;
						BYTE g = pRgb[nPos].rgbGreen;
						BYTE b = pRgb[nPos].rgbBlue;

						pRgb[nPos].rgbRed = r;
						pRgb[nPos].rgbGreen = g;
						pRgb[nPos].rgbBlue = b;
					}
				}
				fillcolor = dc.GetPixel(start);
				while (1)
				{
					int x = start.x;
					int y = start.y;
					int nPos = x = x + (y*bm.bmWidth);
					if (pRgb[nPos - bm.bmWidth].rgbRed == GetRValue(fillcolor)
						&& pRgb[nPos - bm.bmWidth - 1].rgbGreen == GetGValue(fillcolor)
						&& pRgb[nPos - bm.bmWidth].rgbBlue == GetBValue(fillcolor)) {
						stack.push_back(start);
						sp++;
						pRgb[nPos - bm.bmWidth].rgbRed = GetRValue(m_parent->color2);
						pRgb[nPos - bm.bmWidth].rgbGreen = GetGValue(m_parent->color2);
						pRgb[nPos - bm.bmWidth].rgbBlue = GetBValue(m_parent->color2);
						bitmap.SetBitmapBits(bm.bmWidthBytes * bm.bmHeight, pData);
						start.x = start.x;
						start.y = start.y - 1;

					}
					else if (pRgb[nPos - 1].rgbRed == GetRValue(fillcolor)
						&& pRgb[nPos - 1].rgbGreen == GetGValue(fillcolor)
						&& pRgb[nPos - 1].rgbBlue == GetBValue(fillcolor)) {
						stack.push_back(start);
						sp++;
						pRgb[nPos - 1].rgbRed = GetRValue(m_parent->color2);
						pRgb[nPos - 1].rgbGreen = GetGValue(m_parent->color2);
						pRgb[nPos - 1].rgbBlue = GetBValue(m_parent->color2);
						bitmap.SetBitmapBits(bm.bmWidthBytes * bm.bmHeight, pData);
						start.x = start.x - 1;
						start.y = start.y;
					}
					else if (pRgb[nPos + 1].rgbRed == GetRValue(fillcolor)
						&& pRgb[nPos + 1].rgbGreen == GetGValue(fillcolor)
						&& pRgb[nPos + 1].rgbBlue == GetBValue(fillcolor)) {
						stack.push_back(start);
						sp++;
						pRgb[nPos + 1].rgbRed = GetRValue(m_parent->color2);
						pRgb[nPos + 1].rgbRed = GetGValue(m_parent->color2);
						pRgb[nPos + 1].rgbRed = GetBValue(m_parent->color2);
						bitmap.SetBitmapBits(bm.bmWidthBytes * bm.bmHeight, pData);
						start.x = start.x + 1;
						start.y = start.y;
					}
					else if (pRgb[nPos + bm.bmWidth].rgbRed == GetRValue(fillcolor)
						&& pRgb[nPos + bm.bmWidth].rgbGreen == GetGValue(fillcolor)
						&& pRgb[nPos + bm.bmWidth].rgbBlue == GetBValue(fillcolor)) {
						stack.push_back(start);
						sp++;
						pRgb[nPos + bm.bmWidth].rgbRed = GetRValue(m_parent->color2);
						pRgb[nPos + bm.bmWidth].rgbRed = GetGValue(m_parent->color2);
						pRgb[nPos + bm.bmWidth].rgbRed = GetBValue(m_parent->color2);
						bitmap.SetBitmapBits(bm.bmWidthBytes * bm.bmHeight, pData);
						start.x = start.x;
						start.y = start.y + 1;
					}
					else {
						if (sp == 0)
							break;
						sp--;
						start = stack.back();
						stack.pop_back();
					}
				}
				dc.BitBlt(0, 0, m_rect.right, m_rect.bottom, &MemDC, 0, 0, SRCCOPY);
				free(pData);
				pData = NULL;
				break;
			}
			default:
				break;
			}
			//}
			BitBlt(hdc, 0, 0, output.GetWidth(), output.GetHeight(), dc.m_hDC, 0, 0, SRCCOPY);
		}
		else {//이미지 효과 적용
			if (point.x <= output.GetWidth() && point.y <= output.GetHeight())
				points.push_back(point);
			else {
				AfxMessageBox(_T("효과 적용 영역이 해당 레이어 크기를 벗어났습니다.\n다시 선택하세요."), MB_OK);
				points.clear();
			}
		}
	}
	else
		AfxMessageBox(_T("레이어를 먼저 추가하세요."), MB_OK);

	CScrollView::OnLButtonDown(nFlags, point);
}


void CMyPaper::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CClientDC dc(this);
	ClientToScreen(&point);
	this->ScreenToClient(&point);
	//	if (point.x >= 0 && point.x <= m_rect.Width() && point.y >= 0 && point.y <= m_rect.Height()) {
	if (mouseclick == 1) {
		switch (m_parent->status)
		{
		case 1:  //펜, 자유곡선
			if (m_parent->isColor1) {
				pen.CreatePen(PS_SOLID, m_parent->thickness, m_parent->color1);
				dc.SelectObject(&pen);
			}
			else
				dc.SelectStockObject(NULL_PEN);
			dc.MoveTo(start);
			//if (point.x >= 0 && point.x <= m_rect.Width() && point.y >= 0 && point.y <= m_rect.Height())
			dc.LineTo(point);
			if (m_parent->isColor1)
				pen.DeleteObject();
			start = point;
			//img.ReleaseDC();
			break;
		case 2: //직선 그리기
			/*dc.SelectStockObject(NULL_PEN);
			dc.MoveTo(start);
			dc.LineTo(point);
			end = point;
			dc.SetROP2(R2_NOT);
			dc.LineTo(end);*/
			//지우개 기능이 있어야 할 듯
			//img.ReleaseDC();
			break;
		case 3: //사각형 생성
			dc.SelectStockObject(NULL_BRUSH);
			dc.SetROP2(R2_NOT);
			dc.Rectangle(start.x, start.y, end.x, end.y);
			end = point;	//좌표 갱신하고
			dc.Rectangle(start.x, start.y, end.x, end.y);
			//img.Releasedc();
			break;
		case 4: //원 생성
			dc.SelectStockObject(NULL_BRUSH);
			dc.SetROP2(R2_NOT);
			dc.Ellipse(start.x, start.y, end.x, end.y);	//이전 위치에 있던 원을 지우고
			end = point;	//좌표 갱신하고
			dc.Ellipse(start.x, start.y, end.x, end.y);	//다시 그리기
			//img.ReleaseDC();
			break;
		case 5: //다각형 생성
			//img.ReleaseDC();
			break;
		case 6: //지우개
			//pen.CreatePen(PS_SOLID, 1, manager.getLayer()->getTransparentColor());
			pen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
			brush.CreateSolidBrush(RGB(255, 255, 255));
			dc.SelectObject(&pen);
			dc.SelectObject(&brush);
			dc.SetROP2(R2_COPYPEN);
			dc.Rectangle(point.x - 5, point.y - 5, point.x + 5, point.y + 5);
			pen.DeleteObject();
			brush.DeleteObject();
			//img.ReleaseDC();
			break;
		default:
			break;
		}
		//		}
				/*else {	//도중에 픽쳐컨트롤을 벗어났을 때

					switch (m_parent->status)
					{
					case 1:  //펜, 자유곡선
						if (m_parent->isColor1)
							pen.DeleteObject();
						break;
					case 6: //지우개
						pen.DeleteObject();
						brush.DeleteObject();
						break;
					default:
						break;
					}
				}*/
		BitBlt(hdc, 0, 0, output.GetWidth(), output.GetHeight(), dc.m_hDC, 0, 0, SRCCOPY);
	}

	CScrollView::OnMouseMove(nFlags, point);
}


void CMyPaper::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CClientDC dc(this);
	ClientToScreen(&point);
	this->ScreenToClient(&point);
	//	if (point.x >= 0 && point.x <= m_rect.Width() && point.y >= 0 && point.y <= m_rect.Height()) {
	if (m_parent->imageEffects == 0) {
		switch (m_parent->status)
		{
		case 1:  //펜, 자유곡선
			if (m_parent->isColor1 && point.x >= 0 && point.x <= m_rect.Width() && point.y >= 0 && point.y <= m_rect.Height())
				pen.DeleteObject();
			//img.ReleaseDC();
			ReleaseCapture();
			break;
		case 2: //직선 그리기
			if (m_parent->isColor1) {
				pen.CreatePen(PS_SOLID, m_parent->thickness, m_parent->color1);
				dc.SelectObject(&pen);
			}
			else
				dc.SelectStockObject(NULL_PEN);
			end = point;
			dc.MoveTo(start);
			dc.LineTo(end);
			//img.ReleaseDC();
			if (m_parent->isColor1)
				pen.DeleteObject();
			ReleaseCapture();
			break;
		case 3: //사각형 생성
			dc.SelectStockObject(NULL_BRUSH);
			dc.SetROP2(R2_NOT);
			dc.Rectangle(start.x, start.y, end.x, end.y);
			dc.SetROP2(R2_COPYPEN);
			end = point;
			if (m_parent->isColor1) {
				pen.CreatePen(PS_SOLID, m_parent->thickness, m_parent->color1);
				dc.SelectObject(&pen);
			}
			else
				dc.SelectStockObject(NULL_PEN);
			if (m_parent->isColor2) {
				brush.CreateSolidBrush(m_parent->color2);
				dc.SelectObject(&brush);
			}
			else
				dc.SelectStockObject(NULL_BRUSH);
			dc.Rectangle(start.x, start.y, end.x, end.y);
			//img.ReleaseDC();
			if (m_parent->isColor1)
				pen.DeleteObject();
			if (m_parent->isColor2)
				brush.DeleteObject();
			ReleaseCapture();
			break;
		case 4: //원 생성
			dc.SelectStockObject(NULL_BRUSH);
			dc.SetROP2(R2_NOT);
			dc.Ellipse(start.x, start.y, end.x, end.y);
			dc.SetROP2(R2_COPYPEN);
			end = point;
			if (m_parent->isColor1) {
				pen.CreatePen(PS_SOLID, m_parent->thickness, m_parent->color1);
				dc.SelectObject(&pen);
			}
			else
				dc.SelectStockObject(NULL_PEN);
			if (m_parent->isColor2) {
				brush.CreateSolidBrush(m_parent->color2);
				dc.SelectObject(&brush);
			}
			else
				dc.SelectStockObject(NULL_BRUSH);
			dc.Ellipse(start.x, start.y, end.x, end.y);
			//img.ReleaseDC();
			if (m_parent->isColor1)
				pen.DeleteObject();
			if (m_parent->isColor2)
				brush.DeleteObject();
			ReleaseCapture();
			break;
		case 5: //다각형 생성
			//img.ReleaseDC();
			break;
		case 6: //지우개
			//img.ReleaseDC();
			break;
		case 8:	//텍스트박스
			if (textinput == 0) {
				CTextbox *textbox = new CTextbox;
				//text[textcnt]->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE, CRect(start.x, start.y, point.x, point.y), this, IDC_TEXTBOX1+textcnt);
				//ptext = text[textcnt];
				//text[textcnt]->textcnt = textcnt;
				//textcnt++;
				textbox->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE, CRect(start.x, start.y, point.x, point.y), this, 1);
				textbox->color1 = m_parent->color1;
				textbox->color2 = m_parent->color2;
				ptext = textbox;
				//textbox->color1 = color1;
				//textbox->color2 = color2;
				//textbox->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE, CRect(start.x, start.y, point.x, point.y), this, IDC_TEXTBOX);
				textinput = 1;
				ptext->SetFocus();
			}
			else if (textinput == 1)
			{
				//CMyStatic *stat = new CMyStatic;
				//ptext->GetWindowTextW(str);
				/*ptext->SetSel(0, -1);
				ptext->Clear();*/
				//delete ptext;
				ptext->SetReadOnly(true);
				//stat->Create(_T("sadasd"), ES_MULTILINE | WS_CHILD | WS_VISIBLE, CRect(start.x , start.y , point.x  , point.y ), this,IDC_TEXTBOX);
				textinput = 0;
				//textbox.SetSel(0, -1);
				//textbox.Clear();
				//textbox.Undo();
				//pedit->Clear();
			}
			break;
		default:
			break;
		}
		if (m_parent->status != 7)
			BitBlt(hdc, 0, 0, output.GetWidth(), output.GetHeight(), dc.m_hDC, 0, 0, SRCCOPY);
		else {
			end = point;
			int w = 0, h = 0;
			w = end.x - start.x;
			h = end.y - start.y;
			output.ReleaseDC();
			output.Destroy();

			output.Create(w, h, 32);
			hdc = output.GetDC();
			BitBlt(hdc, 0, 0, w, h, dc.m_hDC, start.x, start.y, SRCCOPY);
			//BitBlt(hdc, 0, 0, w, h, dc.m_hDC, start.x, start.y, SRCCOPY);
			//BitBlt(dc.m_hDC, start.x, start.y, w, h, hdc, 0,0, SRCCOPY);
			start.x = -1;
			end.x = -1;
			//m_parent->status = 0;
			RedrawWindow();
		}

		mouseclick = 0;
		//	}
	}
	//Invalidate(false);
	//img.ReleaseDC();
	//pDC = CDC::FromHandle(img.GetDC());
	CScrollView::OnLButtonUp(nFlags, point);
}


void CMyPaper::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CClientDC dc(this);
	if (m_parent->status == 5)	//다각형 생성일 때
	{
		if (m_parent->isColor1) {
			pen.CreatePen(PS_SOLID, m_parent->thickness, m_parent->color1);
			dc.SelectObject(&pen);
		}
		else
			dc.SelectStockObject(NULL_PEN);
		if (m_parent->isColor2) {
			brush.CreateSolidBrush(m_parent->color2);
			dc.SelectObject(&brush);
		}
		else
			dc.SelectStockObject(NULL_BRUSH);
		dc.Polygon(plp, nct);
		nct = 0;
		delete[]plp;
		//img.ReleaseDC();
		if (m_parent->isColor1)
			pen.DeleteObject();
		if (m_parent->isColor2)
			brush.DeleteObject();
		ReleaseCapture();
		BitBlt(hdc, 0, 0, output.GetWidth(), output.GetHeight(), dc.m_hDC, 0, 0, SRCCOPY);
		return;
	}
	if (points.size() > 2) {
		if (m_parent->imageEffects == 1) {
			rgn.DeleteObject();
			rgn.CreatePolygonRgn(&points[0], points.size(), ALTERNATE);
			CHueSaturation dlg(img, &rgn);
			int a = dlg.DoModal();
			if (a == -1) {
				TRACE(_T("ERR MAKING dig. \n\r"));
			}
			points.clear();
			m_parent->imageEffects = 0;
			Invalidate(false);
		}
		else if (m_parent->imageEffects == 2) {
			rgn.DeleteObject();
			rgn.CreatePolygonRgn(&points[0], points.size(), ALTERNATE);
			CBlur dlg(img, &rgn);
			int a = dlg.DoModal();
			if (a == -1) {
				TRACE(_T("ERR MAKING dig. \n\r"));
			}
			points.clear();
			m_parent->imageEffects = 0;
			Invalidate(false);
		}
		else if (m_parent->imageEffects == 3) {
			rgn.DeleteObject();
			rgn.CreatePolygonRgn(&points[0], points.size(), ALTERNATE);
			CGray dlg(img, &rgn);
			int a = dlg.DoModal();
			if (a == -1) {
				TRACE(_T("ERR MAKING dig. \n\r"));
			}
			points.clear();
			m_parent->imageEffects = 0;
			Invalidate(false);
		}
		else if (m_parent->imageEffects == 4) {
			rgn.DeleteObject();
			rgn.CreatePolygonRgn(&points[0], points.size(), ALTERNATE);
			CAlpha dlg(img, &rgn);
			int a = dlg.DoModal();
			if (a == -1) {
				TRACE(_T("ERR MAKING dig. \n\r"));
			}
			points.clear();
			m_parent->imageEffects = 0;
			Invalidate(false);
		}
		else if (m_parent->imageEffects == 5) {
			rgn.DeleteObject();
			rgn.CreatePolygonRgn(&points[0], points.size(), ALTERNATE);
			CMosaic dlg(img, &rgn);
			int a = dlg.DoModal();
			if (a == -1) {
				TRACE(_T("ERR MAKING dig. \n\r"));
			}
			points.clear();
			m_parent->imageEffects = 0;
			Invalidate(false);
		}
	}

	CScrollView::OnRButtonDown(nFlags, point);
}


void CMyPaper::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CScrollView::OnPaint()을(를) 호출하지 마십시오.
	//img.ReleaseDC();
	//img.BitBlt(dc, 0, 0, img.GetWidth(), img.GetHeight(), 0, 0);
	//if (img != NULL) {
		//updateOutput();
		//img->BitBlt(dc.m_hDC, 0, 0, img->GetWidth(), img->GetHeight(), 0, 0);
		//img->BitBlt(hdc, 0, 0, img->GetWidth(), img->GetHeight(), 0, 0);
	//}
	//pDC = CDC::FromHandle(img.GetDC());

	//CClientDC pdc(&m_picture);
	if (img != NULL) {
		//if (m_parent->isLayer) {
		if(m_parent->status == 0)
			updateOutput();
		//m_parent->isLayer = false;
		//}
		output.BitBlt(dc, 1, 1, output.GetWidth(), output.GetHeight(), 0, 0);
		dc.SelectStockObject(NULL_BRUSH);
		dc.Rectangle(0, 0, output.GetWidth()+2, output.GetHeight()+2);
	}
}


void CMyPaper::OnSaturation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_parent->imageEffects = 1;
}


void CMyPaper::OnBlur()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_parent->imageEffects = 2;
}


void CMyPaper::OnGray()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_parent->imageEffects = 3;
}


void CMyPaper::OnAlpha()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_parent->imageEffects = 4;
}


void CMyPaper::OnMosaic()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_parent->imageEffects = 5;
}

void CMyPaper::updateOutput()
{
	/*CDC* dc = CDC::FromHandle(img->GetDC());
	//CDC* dc = CDC::FromHandle(output.GetDC());
	dc->FillSolidRect(0, 0, img->GetWidth(), img->GetHeight(), RGB(255, 255, 255));
	//dc->FillSolidRect(0, 0, output.GetWidth(), output.GetHeight(), RGB(255, 255, 255));
	img->ReleaseDC();
	manager.getAllBlendedImage(&output);*/

	CDC* dc = CDC::FromHandle(output.GetDC());
	dc->FillSolidRect(0, 0, output.GetWidth(), output.GetHeight(), RGB(255, 255, 255));
	output.ReleaseDC();
	manager.getAllBlendedImage(&output);
}

void CMyPaper::OnFileOpen()
{
	CFileDialog dlg(true, _T("jpg file(*.jpg)"), _T("*.jpg"),
		OFN_READONLY, _T("jpg file(*.jpg)|*.jpg|bmp file(*.bmp)|*.bmp|png file(*.png)|*png.jpg|모든파일(*.*)|*.*|"));
	if (dlg.DoModal() == IDOK) {
		CString file = dlg.GetPathName();

		output.ReleaseDC();
		output.Destroy();

		output.Load(file);
		hdc = output.GetDC();

		RedrawWindow();
	}
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CMyPaper::OnFileSave()
{
	CRect rect;
	//AfxGetMainWnd()->m_splitwnd.GetPane(0, 1)
	/*GetClientRect(&rect);
	int x = rect.Width();
	int y = rect.Height();*/
	int x = output.GetWidth();
	int y = output.GetHeight();


	CClientDC dc(this);

	capimage.Create(x, y, 32);			//캡처할 이미지의 크기를 지정해준다. 그 크기는 클라이언트 영역의 크기와 같다.
	HDC hdc = capimage.GetDC();			//캡처할 이미지의 dc를 받아준다.
	BitBlt(hdc, 0, 0, x, y, dc.m_hDC, 1, 1, SRCCOPY);			//hdc에 dc의 0,0부터의 bit를 받아온다.


	CFileDialog dlg(false, _T("jpg file(*.jpg)"), _T("*.jpg"),
		OFN_READONLY, _T("jpg file(*.jpg)|*.jpg|bmp file(*.bmp)|*.bmp|png file(*.png)|*png.jpg|모든파일(*.*)|*.*|"));
	if (dlg.DoModal() == IDOK) {
		CString filename = dlg.GetFileName();
		capimage.Save(filename, Gdiplus::ImageFormatJPEG);
	}
	capimage.ReleaseDC();
}
