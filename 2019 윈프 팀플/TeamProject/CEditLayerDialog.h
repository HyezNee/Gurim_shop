#include "LayerManager.h"
#include "Layer.h"

#pragma once

// CEditLayerDialog 대화 상자

class CEditLayerDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CEditLayerDialog)

public:
	//CEditLayerDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CEditLayerDialog(LayerManager* manager,int num,CWnd* pParent = nullptr);
	virtual ~CEditLayerDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EDITLAYER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editName;
	CEdit m_editOpacity;
	CButton m_checkShow;
	CButton m_checkUseTColor;
	CButton m_buttonChooseTColor;
	CStatic m_picTColor;
	virtual BOOL OnInitDialog();
private :
	LayerManager* manager;
	int num;
	Layer* layer;

	COLORREF color;
	virtual void OnOK();
public:
	afx_msg void OnBnClickedButtonChoosecolor();
//	afx_msg void OnPaint();
	
	void updatePicureTColor();
};
