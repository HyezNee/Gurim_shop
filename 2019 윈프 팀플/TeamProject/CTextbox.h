#pragma once


// CTextbox

class CTextbox : public CEdit
{
	DECLARE_DYNAMIC(CTextbox)

public:
	CTextbox();
	virtual ~CTextbox();

protected:
	DECLARE_MESSAGE_MAP()
public:
	COLORREF color1;
	COLORREF color2;
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
};


