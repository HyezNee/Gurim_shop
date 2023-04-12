#pragma once

#include "LayerManager.h"
#include "Layer.h"

// CAddLayerDialog 대화 상자

class CAddLayerDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAddLayerDialog)

public:
	CAddLayerDialog(LayerManager* manager, CWnd* pParent = nullptr);
	virtual ~CAddLayerDialog();

	CString path;
	LayerManager* manager;


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ADDLAYER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonChoosefile();
	CButton m_pathctl;
	virtual void OnOK();
	CButton m_checkFromFle;
	CButton m_checkMakeNew;
	CEdit m_editW;
	CEdit m_editH;
	CEdit m_editName;
	virtual BOOL OnInitDialog();
	CStatic m_staticPath;

private:
	void setDisabled();
public:
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	CButton m_buttonCF;
};
