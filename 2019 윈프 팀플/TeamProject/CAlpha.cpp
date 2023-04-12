// CAlpha.cpp: 구현 파일
//

#include "stdafx.h"
#include "TeamProject.h"
#include "CAlpha.h"
#include "afxdialogex.h"


// CAlpha 대화 상자

IMPLEMENT_DYNAMIC(CAlpha, CDialogEx)

//CAlpha::CAlpha(CWnd* pParent /*=nullptr*/)
//	: CDialogEx(IDD_DIALOG_CUTPREV, pParent)
//{
//
//}

CAlpha::CAlpha(CAdvImg* img, CRgn* rgn, CWnd* pParent)
	:CDialogEx(IDD_DIALOG_ALPHA, pParent)
{

	targetImg = img;
	rgnToApply = rgn;

}

CAlpha::~CAlpha()
{
	targetImg->ReleaseDC();
	tempAppliedImg.ReleaseDC();
	tempOrgImg.ReleaseDC();

}

void CAlpha::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREV_PIC, _prevPic);
	//DDX_Control(pDX, IDC_SLIDER_HUE, m_sliderHue);
	DDX_Control(pDX, IDC_SLIDER_ALPHA, m_sliderAlpha);
	DDX_Control(pDX, IDC_SLIDER_BETA, m_sliderBeta);
	//DDX_Control(pDX, IDC_CHECK_BLUR, m_checkBlur);
	//DDX_Control(pDX, IDC_TEST_HUE, m_textHue);
	//DDX_Control(pDX, IDC_TEXT_ALPHA, m_textAlpha);
	//DDX_Control(pDX, IDC_TEXT_BETA, m_testBeta);
	//DDX_Control(pDX, IDC_SLIDER_SAT, m_sliderSaturation);
	//DDX_Control(pDX, IDC_CHECK_GREY, m_checkGrey);
}


BEGIN_MESSAGE_MAP(CAlpha, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ALPHA, &CAlpha::OnNMCustomdrawSliderAlpha)
	//ON_BN_CLICKED(IDC_CHECK_BLUR, &CAlpha::OnBnClickedCheckBlur)
	ON_BN_CLICKED(IDC_APPLYBT, &CAlpha::OnBnClickedApplybt)
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_HUE, &CAlpha::OnNMCustomdrawSliderHue)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_BETA, &CAlpha::OnNMCustomdrawSliderBeta)
END_MESSAGE_MAP()


// CAlpha 메시지 처리기



BOOL CAlpha::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.



	return CDialogEx::PreCreateWindow(cs);
}


BOOL CAlpha::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// slider
	//m_sliderHue.SetRange(0, 360, TRUE);
	//m_sliderSaturation.SetRange(-100, 100, TRUE);
	//m_sliderSaturation.SetPos(0);
	m_sliderAlpha.SetRange(0, 10, TRUE);
	m_sliderAlpha.SetPos(1);
	m_sliderBeta.SetRange(-255, 255, TRUE);
	m_sliderBeta.SetPos(0);


	rgnToApply->GetRgnBox(&box);
	rgnNew.CreateEllipticRgn(0, 0, 0, 0);
	rgnNew.CopyRgn(rgnToApply);
	rgnNew.OffsetRgn(-box.left, -box.top);

	maskDC.CreateCompatibleDC(GetDC());
	maskBmp.CreateCompatibleBitmap(GetDC(), box.Width(), box.Height());
	maskDC.SelectObject(&maskBmp);


	invMaskDC.CreateCompatibleDC(GetDC());
	invMaskBmp.CreateCompatibleBitmap(GetDC(), box.Width(), box.Height());
	invMaskDC.SelectObject(&invMaskBmp);

	targetDC = CDC::FromHandle(targetImg->GetDC());

	tempOrgImg.Create(box.Width(), box.Height(), targetImg->GetBPP());
	tempAppliedImg.Create(box.Width(), box.Height(), targetImg->GetBPP());
	tempOrgDC = CDC::FromHandle(tempOrgImg.GetDC());
	tempAppliedDC = CDC::FromHandle(tempAppliedImg.GetDC());

	tempOrgDC->BitBlt(0, 0, box.Width(), box.Height(), targetDC, box.left, box.top, SRCCOPY);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CAlpha::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	setPreviewImg();
	targetDC->BitBlt(box.left, box.top, box.Width(), box.Height(), &maskDC, 0, 0, SRCCOPY);
	CDialogEx::OnOK();
}


void CAlpha::OnNMCustomdrawSliderAlpha(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CAlpha::setPreviewImg()
{
	CClientDC dc(&_prevPic);
	//dc.FillSolidRect(0, 0, 300, 300, RGB(255, 255, 255));
	ImgKernel ker;
	ker.createGaussian(6);

	CBrush brush;
	brush.CreateSolidBrush(RGB(255, 255, 255));

	maskDC.FillSolidRect(0, 0, box.Width(), box.Height(), RGB(0, 0, 0));
	maskDC.FillRgn(&rgnNew, &brush);
	invMaskDC.BitBlt(0, 0, box.Width(), box.Height(), &maskDC, 0, 0, NOTSRCCOPY);

	tempAppliedDC->BitBlt(0, 0, box.Width(), box.Height(), tempOrgDC, 0, 0, SRCCOPY);
	invMaskDC.BitBlt(0, 0, box.Width(), box.Height(), tempOrgDC, 0, 0, SRCAND);

	/*//rotate hue & saturation
	int hue = m_sliderHue.GetPos();
	int saturation = m_sliderSaturation.GetPos();
	if (hue != 0 && hue != 360 || saturation != 0)
		tempAppliedImg.addHSV(hue, saturation, 0);*/

	// apply linear
	int alpha = m_sliderAlpha.GetPos();
	int beta = m_sliderBeta.GetPos();
	if (alpha != 1 || beta != 0)
		tempAppliedImg.applyLinearFunc(alpha, beta);

	/*//apply blur
	if (m_checkBlur.GetCheck()) {
		tempAppliedImg.applyKernel(ker);
	}

	//apply grey
	if (m_checkGrey.GetCheck()) {
		tempAppliedImg.applyGray();
	}*/

	maskDC.BitBlt(0, 0, box.Width(), box.Height(), tempAppliedDC, 0, 0, SRCAND);
	maskDC.BitBlt(0, 0, box.Width(), box.Height(), &invMaskDC, 0, 0, SRCPAINT);
	//targetDC->BitBlt(box.left, box.top, box.Width(), box.Height(), &maskDC, 0, 0, SRCCOPY);
	dc.BitBlt(0, 0, box.Width(), box.Height(), &maskDC, 0, 0, SRCCOPY);
}


/*void CAlpha::OnBnClickedCheckBlur()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}*/


void CAlpha::OnBnClickedApplybt()
{
	setPreviewImg();
}


/*void CAlpha::OnNMCustomdrawSliderHue(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}*/


void CAlpha::OnNMCustomdrawSliderBeta(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}
