// CEditLayerDialog.cpp: 구현 파일
//

#include "stdafx.h"
#include "TeamProject.h"
#include "CEditLayerDialog.h"
#include "afxdialogex.h"


// CEditLayerDialog 대화 상자

IMPLEMENT_DYNAMIC(CEditLayerDialog, CDialogEx)

//CEditLayerDialog::CEditLayerDialog(CWnd* pParent /*=nullptr*/)
//	: CDialogEx(IDD_DIALOG_EDITLAYER, pParent)
//{
//
//}

CEditLayerDialog::CEditLayerDialog(LayerManager* manager, int num, CWnd* pParent)
	: CDialogEx(IDD_DIALOG_EDITLAYER, pParent), color(RGB(0,0,0))
{
	this->manager = manager;
	this->num = num;
	this->layer = manager->getLayer(num);
}

CEditLayerDialog::~CEditLayerDialog()
{
}

void CEditLayerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LAYERNAME, m_editName);
	DDX_Control(pDX, IDC_EDIT_OPACITY, m_editOpacity);
	DDX_Control(pDX, IDC_CHECK_SHOWLAYER, m_checkShow);
	DDX_Control(pDX, IDC_CHECK_TRANSCOLORLAYER, m_checkUseTColor);
	DDX_Control(pDX, IDC_BUTTON_CHOOSECOLOR, m_buttonChooseTColor);
	DDX_Control(pDX, IDC_PIC_TCOLOR, m_picTColor);
}


BEGIN_MESSAGE_MAP(CEditLayerDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CHOOSECOLOR, &CEditLayerDialog::OnBnClickedButtonChoosecolor)
//	ON_WM_PAINT()
END_MESSAGE_MAP()


// CEditLayerDialog 메시지 처리기


BOOL CEditLayerDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// edit control for name 
	
	m_editName.SetWindowText(layer->name);

	// edit control for alpha
	CString strOpacity;
	strOpacity.Format(_T("%d"), (int)(layer->getAlpha() * 1000));
	m_editOpacity.SetWindowTextW(strOpacity);

	// checkbox for show
	m_checkShow.SetCheck(layer->show);

	// check box for Transparent color
	m_checkUseTColor.SetCheck(layer->useTransparentColor);
	color = layer->getTransparentColor();
	updatePicureTColor();

	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CEditLayerDialog::OnOK()
{
	// edit control for name 
	CDialogEx::OnInitDialog();
	CString _name;
	m_editName.GetWindowText(_name);
	layer->name = _name;

	// edit control for alpha
	int opacity;
	CString strOpacity;
	m_editOpacity.GetWindowText(strOpacity);
	opacity = _ttoi(strOpacity);
	// is opacity in 0~1000
	if (opacity < 0 || opacity > 1000) {
		AfxMessageBox(_T("Opacity값은 0이상 1000 이하이어야 합니다."));
		return;
	}
	layer->setAlpha(opacity / 1000.0);

	// checkbox for show
	layer->show = m_checkShow.GetCheck();

	// check box for Transparent color
	layer->useTransparentColor = m_checkUseTColor.GetCheck();
	layer->changeTransparentColor(color);

	CDialogEx::OnOK();
}


void CEditLayerDialog::OnBnClickedButtonChoosecolor()
{
	CColorDialog dlg;
	if (dlg.DoModal() == IDOK) {
		color = dlg.GetColor();
		updatePicureTColor();
	}
}

void CEditLayerDialog::updatePicureTColor()
{
	CClientDC dc(&m_picTColor);
	CRect rect;
	m_picTColor.GetClientRect(&rect);
	dc.FillSolidRect(0, 0, rect.Width(), rect.Height(),color);
}


//void CEditLayerDialog::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
//					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
//}
