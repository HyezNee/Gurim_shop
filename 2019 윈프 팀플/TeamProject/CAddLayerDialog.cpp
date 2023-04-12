// CAddLayerDialog.cpp: 구현 파일
//

#include "stdafx.h"
#include "TeamProject.h"
#include "CAddLayerDialog.h"
#include "afxdialogex.h"


// CAddLayerDialog 대화 상자

IMPLEMENT_DYNAMIC(CAddLayerDialog, CDialogEx)


CAddLayerDialog::CAddLayerDialog(LayerManager* manager, CWnd* pParent)
	: CDialogEx(IDD_DIALOG_ADDLAYER, pParent)
{
	this->manager = manager;
}

CAddLayerDialog::~CAddLayerDialog()
{
}

void CAddLayerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_pathctl);
	DDX_Control(pDX, IDC_RADIO1, m_checkFromFle);
	DDX_Control(pDX, IDC_RADIO2, m_checkMakeNew);
	DDX_Control(pDX, IDC_EDIT1, m_editW);
	DDX_Control(pDX, IDC_EDIT2, m_editH);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_STATIC_PATH, m_staticPath);
	DDX_Control(pDX, IDC_BUTTON_CHOOSEFILE, m_buttonCF);
}


BEGIN_MESSAGE_MAP(CAddLayerDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CHOOSEFILE, &CAddLayerDialog::OnBnClickedButtonChoosefile)
	ON_BN_CLICKED(IDC_RADIO1, &CAddLayerDialog::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CAddLayerDialog::OnBnClickedRadio2)
END_MESSAGE_MAP()


// CAddLayerDialog 메시지 처리기


void CAddLayerDialog::OnBnClickedButtonChoosefile()
{

	CFileDialog fileDlg(TRUE, _T("jpg file(*.jpg)"),_T("*.jpg"),6UL,_T("jpg file(*.jpg)|*.jpg|png file(*.png)|*.png|bmp file(*.bmp)|*.bmp|"));
	if (fileDlg.DoModal() == true) {
		path = fileDlg.GetPathName();
		m_staticPath.SetWindowText(path);
	}
}


void CAddLayerDialog::OnOK()
{
	CString _name;
	m_editName.GetWindowText(_name);

	if (m_checkFromFle.GetCheck()) {
		manager->addLayer(path,_name);
	}
	else if (m_checkMakeNew.GetCheck()) {
		CString sw, sh;
		m_editW.GetWindowText(sw);
		m_editH.GetWindowText(sh);
		int _w = _ttoi(sw);
		int _h = _ttoi(sh);
		manager->addLayer(_w, _h, 32, _name);

	}
	else {
		AfxMessageBox(_T("Cannot create Layer."));
	}

	CDialogEx::OnOK();
}


BOOL CAddLayerDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// set name with "Layer(NUMBER)"
	CString name;
	name.Format(_T("Layer%d"), manager->getLayerCounts());
	m_editName.SetWindowText(name);

	m_checkMakeNew.SetCheck(TRUE);
	setDisabled();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CAddLayerDialog::setDisabled()
{
	if (m_checkFromFle.GetCheck()) {
		// disable all components about radio2
		m_editW.EnableWindow(FALSE);
		m_editH.EnableWindow(FALSE);

		// enable all components about radio1
		m_buttonCF.EnableWindow(TRUE);
	}
	else if (m_checkMakeNew.GetCheck()) {
		// enable all components about radio2
		m_editW.EnableWindow(TRUE);
		m_editH.EnableWindow(TRUE);

		// disable all components about radio1
		m_buttonCF.EnableWindow(FALSE);
	}
}


void CAddLayerDialog::OnBnClickedRadio1()
{
	setDisabled();
}


void CAddLayerDialog::OnBnClickedRadio2()
{

	setDisabled();
}
