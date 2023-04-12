#pragma once
#include <afxwin.h>
class CMyColorButton :
	public CButton
{
public:
	CMyColorButton();
	~CMyColorButton();
	COLORREF m_color;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};