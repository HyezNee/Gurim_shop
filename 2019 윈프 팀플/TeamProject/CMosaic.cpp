// CMosaic.cpp: 구현 파일
//

#include "stdafx.h"
#include "TeamProject.h"
#include "CMosaic.h"
#include "afxdialogex.h"


// CMosaic 대화 상자

IMPLEMENT_DYNAMIC(CMosaic, CDialogEx)

//CMosaic::CMosaic(CWnd* pParent /*=nullptr*/)
//	: CDialogEx(IDD_DIALOG_CUTPREV, pParent)
//{
//
//}

CMosaic::CMosaic(CAdvImg* img, CRgn* rgn, CWnd* pParent)
	:CDialogEx(IDD_DIALOG_MOSAIC, pParent)
{

	targetImg = img;
	rgnToApply = rgn;

}

CMosaic::~CMosaic()
{
	targetImg->ReleaseDC();
	tempAppliedImg.ReleaseDC();
	tempOrgImg.ReleaseDC();

}

void CMosaic::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREV_PIC, _prevPic);
	/*DDX_Control(pDX, IDC_SLIDER_HUE, m_sliderHue);
	DDX_Control(pDX, IDC_SLIDER_ALPHA, m_sliderAlpha);
	DDX_Control(pDX, IDC_SLIDER_BETA, m_sliderBeta);
	DDX_Control(pDX, IDC_CHECK_BLUR, m_checkBlur);
	DDX_Control(pDX, IDC_TEST_HUE, m_textHue);
	DDX_Control(pDX, IDC_TEXT_ALPHA, m_textAlpha);
	DDX_Control(pDX, IDC_TEXT_BETA, m_testBeta);
	DDX_Control(pDX, IDC_SLIDER_SAT, m_sliderSaturation);
	DDX_Control(pDX, IDC_CHECK_GREY, m_checkGrey);*/
	DDX_Control(pDX, IDC_SLIDER_MOSAIC, m_sliderMosaic);
}


BEGIN_MESSAGE_MAP(CMosaic, CDialogEx)
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ALPHA, &CMosaic::OnNMCustomdrawSliderAlpha)
	//ON_BN_CLICKED(IDC_CHECK_BLUR, &CMosaic::OnBnClickedCheckBlur)
	ON_BN_CLICKED(IDC_APPLYBT, &CMosaic::OnBnClickedApplybt)
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_HUE, &CMosaic::OnNMCustomdrawSliderHue)
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_BETA, &CMosaic::OnNMCustomdrawSliderBeta)
END_MESSAGE_MAP()


// CMosaic 메시지 처리기



BOOL CMosaic::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.



	return CDialogEx::PreCreateWindow(cs);
}


BOOL CMosaic::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// slider
	/*m_sliderHue.SetRange(0, 360, TRUE);
	m_sliderSaturation.SetRange(-100, 100, TRUE);
	m_sliderSaturation.SetPos(0);
	m_sliderAlpha.SetRange(0, 10, TRUE);
	m_sliderAlpha.SetPos(1);
	m_sliderBeta.SetRange(-255, 255, TRUE);
	m_sliderBeta.SetPos(0);*/
	m_sliderMosaic.SetRange(0, 20, TRUE);
	m_sliderMosaic.SetPos(0);


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


void CMosaic::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	setPreviewImg();
	targetDC->BitBlt(box.left, box.top, box.Width(), box.Height(), &maskDC, 0, 0, SRCCOPY);
	CDialogEx::OnOK();
}


/*void CMosaic::OnNMCustomdrawSliderAlpha(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}*/

void CMosaic::setPreviewImg()
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
		tempAppliedImg.addHSV(hue, saturation, 0);

	// apply linear
	int alpha = m_sliderAlpha.GetPos();
	int beta = m_sliderBeta.GetPos();
	if (alpha != 1 || beta != 0)
		tempAppliedImg.applyLinearFunc(alpha, beta);

	//apply blur
	if (m_checkBlur.GetCheck()) {
		tempAppliedImg.applyKernel(ker);
	}

	//apply grey
	if (m_checkGrey.GetCheck()) {
		tempAppliedImg.applyGray();
	}*/

	//apply mosaic
	if (m_sliderMosaic.GetPos() != 0)
		tempAppliedImg.applyPixelization(m_sliderMosaic.GetPos(), m_sliderMosaic.GetPos());

	maskDC.BitBlt(0, 0, box.Width(), box.Height(), tempAppliedDC, 0, 0, SRCAND);
	maskDC.BitBlt(0, 0, box.Width(), box.Height(), &invMaskDC, 0, 0, SRCPAINT);
	//targetDC->BitBlt(box.left, box.top, box.Width(), box.Height(), &maskDC, 0, 0, SRCCOPY);
	dc.BitBlt(0, 0, box.Width(), box.Height(), &maskDC, 0, 0, SRCCOPY);
}


/*void CMosaic::OnBnClickedCheckBlur()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}*/


void CMosaic::OnBnClickedApplybt()
{
	setPreviewImg();
}


/*void CMosaic::OnNMCustomdrawSliderHue(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void CMosaic::OnNMCustomdrawSliderBeta(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}*/
