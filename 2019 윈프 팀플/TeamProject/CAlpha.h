#pragma once

#include "CAdvImg.h"

// CAlpha 대화 상자

class CAlpha : public CDialogEx
{
	DECLARE_DYNAMIC(CAlpha)

public:
	//CAlpha(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CAlpha(CAdvImg* img, CRgn* rgn, CWnd* pParent = nullptr);
	virtual ~CAlpha();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CUTPREV };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CStatic _prevPic;

	CAdvImg* targetImg;
	CDC* targetDC;

	CAdvImg tempOrgImg;
	CDC* tempOrgDC;

	CAdvImg tempAppliedImg;
	CDC* tempAppliedDC;

	CDC maskDC;
	CBitmap maskBmp;
	CDC invMaskDC;
	CBitmap invMaskBmp;

	CRgn* rgnToApply;
	CRgn rgnNew;
	CRect box;

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//CSliderCtrl m_sliderHue;
	afx_msg void OnNMCustomdrawSliderAlpha(NMHDR* pNMHDR, LRESULT* pResult);
	CSliderCtrl m_sliderAlpha;
	CSliderCtrl m_sliderBeta;

	void setPreviewImg();
	//afx_msg void OnBnClickedCheckBlur();
	//CButton m_checkBlur;
	afx_msg void OnBnClickedApplybt();
	//CStatic m_textHue;
	//CStatic m_textAlpha;
	//CStatic m_testBeta;
	//afx_msg void OnNMCustomdrawSliderHue(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSliderBeta(NMHDR* pNMHDR, LRESULT* pResult);
	//CSliderCtrl m_sliderSaturation;
	//CButton m_checkGrey;
};
