// CTextbox.cpp: 구현 파일
//

#include "stdafx.h"
#include "TeamProject.h"
#include "CTextbox.h"


// CTextbox

IMPLEMENT_DYNAMIC(CTextbox, CEdit)

CTextbox::CTextbox()
{

}

CTextbox::~CTextbox()
{

}


BEGIN_MESSAGE_MAP(CTextbox, CEdit)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()



// CTextbox 메시지 처리기




HBRUSH CTextbox::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO:  여기서 DC의 특성을 변경합니다.
	switch (nCtlColor)
	{
	case CTLCOLOR_EDIT:

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(color1);

		pDC->SelectStockObject(NULL_BRUSH);
		//AfxMessageBox(_T("aaa"));
		return (HBRUSH)GetStockObject(NULL_BRUSH);

	case CTLCOLOR_STATIC:

		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(color1);
		pDC->SelectStockObject(NULL_BRUSH);
		//AfxMessageBox(_T("bbb"));
		return (HBRUSH)GetStockObject(NULL_BRUSH);
		break;
	}
	// TODO:  부모 처리기가 호출되지 않을 경우 Null이 아닌 브러시를 반환합니다.
	return NULL;
}
