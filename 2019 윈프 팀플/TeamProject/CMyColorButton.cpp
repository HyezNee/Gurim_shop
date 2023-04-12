#include "stdafx.h"
#include "CMyColorButton.h"
#include "resource.h"

class TeamProjectView;
CMyColorButton::CMyColorButton()
	:m_color(RGB(0,0,0))
{
}


CMyColorButton::~CMyColorButton()
{
}
BEGIN_MESSAGE_MAP(CMyColorButton, CButton)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()


void CMyColorButton::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDCtl == IDC_COLOR1)
	{
		CDC dc;
		RECT rect;
		dc.Attach(lpDrawItemStruct->hDC);   // Get the Button DC to CDC

		rect = lpDrawItemStruct->rcItem;     //Store the Button rect to our local rect.
		dc.Draw3dRect(&rect, RGB(255, 255, 255), RGB(0, 0, 0));
		dc.FillSolidRect(&rect, m_color);	//Here you can define the required color to appear on the Button.

		UINT state = lpDrawItemStruct->itemState;  //This defines the state of the Push button either pressed or not. 
		if ((state & ODS_SELECTED))
			dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);
		else
			dc.DrawEdge(&rect, EDGE_RAISED, BF_RECT);

		dc.SetBkColor(RGB(255,255,255));   //Setting the Text Background color
		dc.SetTextColor(RGB(255, 255, 255));     //Setting the Text Color
		dc.SetBkMode(TRANSPARENT);

		TCHAR buffer[MAX_PATH];           //To store the Caption of the button.
		ZeroMemory(buffer, MAX_PATH);     //Intializing the buffer to zero
		::GetWindowText(lpDrawItemStruct->hwndItem, buffer, MAX_PATH); //Get the Caption of Button Window 

		dc.DrawText(buffer, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);//Redraw the  Caption of Button Window 
		dc.Detach();  // Detach the Button DC
	}
	CButton::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
