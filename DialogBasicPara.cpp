// DialogBasicPara.cpp : 实现文件
//

#include "stdafx.h"
#include "CamCADSystem.h"
#include "DialogBasicPara.h"
#include "DialogMotionLaw.h"


// CDialogBasicPara 对话框

IMPLEMENT_DYNAMIC(CDialogBasicPara, CDialog)

CDialogBasicPara::CDialogBasicPara(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogBasicPara::IDD, pParent)
	, Delta0(30)
	, Delta01(150)
	, Delta1(30)
	, Delta02(150)
	, h(17.5)
	, e(10)
	, r0(50)
	, rr0(2)
{
//	m_cMotion.SetCurSel(1);
//	m_cFollower.SetCurSel(0);
	t.SetSize(7);
	hh.SetSize(7);
	nCheckStyle=BST_CHECKED;
	nIndexMotion=1;
	nIndexMotionReturn=1;
}

CDialogBasicPara::~CDialogBasicPara()
{
}

void CDialogBasicPara::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TC, Delta0);
	DDV_MinMaxDouble(pDX, Delta0, 0, 360);
	DDX_Text(pDX, IDC_EDIT_YXZ, Delta01);
	DDV_MinMaxDouble(pDX, Delta01, 0, 360);
	DDX_Text(pDX, IDC_EDIT_HC, Delta1);
	DDV_MinMaxDouble(pDX, Delta1, 0, 360);
	DDX_Text(pDX, IDC_EDIT_JXZ, Delta02);
	DDV_MinMaxDouble(pDX, Delta02, 0, 360);
	DDX_Text(pDX, IDC_EDIT_H, h);
	DDV_MinMaxDouble(pDX, h, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_E, e);
	DDV_MinMaxDouble(pDX, e, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_R0, r0);
	DDV_MinMaxDouble(pDX, r0, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_ALPHA, rr0);
	DDV_MinMaxDouble(pDX, rr0, 0, 1000);
	DDX_Control(pDX, IDC_COMBO2, m_cMotion);
	DDX_Control(pDX, IDC_COMBO1, m_cFollower);
	DDX_Control(pDX, IDC_EDIT_ALPHA, m_cRr0);
	DDX_Control(pDX, IDC_STATIC_R, m_cR);
	DDX_Control(pDX, IDC_CHECK1, m_bRiseReturn);
	DDX_Control(pDX, IDC_COMBO3, m_cReturn);
	DDX_Control(pDX, IDC_STATIC_HC, m_sReturn);
}


BEGIN_MESSAGE_MAP(CDialogBasicPara, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialogBasicPara::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDialogBasicPara::OnCbnSelchangeComboMotion)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDialogBasicPara::OnCbnSelchangeComboFollower)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDialogBasicPara::OnCbnSelchangeReturn)
	ON_BN_CLICKED(IDC_CHECK1, &CDialogBasicPara::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CDialogBasicPara 消息处理程序
void CDialogBasicPara::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnOK();
}

void CDialogBasicPara::OnCbnSelchangeComboMotion()
{
	// TODO: 在此添加控件通知处理程序代码
	nIndexMotion=m_cMotion.GetCurSel();
	if(m_bRiseReturn.GetCheck()==BST_CHECKED)
		nIndexMotionReturn=m_cMotion.GetCurSel();
	if(nIndexMotion==6)
	{
		CDialogMotionLaw dlgmtlaw;
		dlgmtlaw.DoModal();
		t.SetAt(0,dlgmtlaw.t1);
		t.SetAt(1,dlgmtlaw.t2);
		t.SetAt(2,dlgmtlaw.t3);
		t.SetAt(3,dlgmtlaw.t4);
		t.SetAt(4,dlgmtlaw.t5);
		t.SetAt(5,dlgmtlaw.t6);
		t.SetAt(6,dlgmtlaw.t7);

		if(m_bRiseReturn.GetCheck()==BST_CHECKED)
			hh.Copy(t);
	}

	
}

void CDialogBasicPara::OnCbnSelchangeComboFollower()
{
	// TODO: 在此添加控件通知处理程序代码
	nIndexFollower=m_cFollower.GetCurSel();
	BOOL sw;
	if(nIndexFollower)
	{
		sw=m_cRr0.ShowWindow(SW_HIDE);
		sw=m_cR.ShowWindow(SW_HIDE);
	}
	else
	{
		sw=m_cRr0.ShowWindow(SW_SHOW);
		sw=m_cR.ShowWindow(SW_SHOW);
	}
	
}

BOOL CDialogBasicPara::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_cMotion.SetCurSel(1);
	m_cReturn.SetCurSel(1);
	m_cFollower.SetCurSel(0);
	m_bRiseReturn.SetCheck(BST_CHECKED);
//	m_cReturn.SetExtendedUI(CBS_DROPDOWNLIST);
//	m_cReturn.
	m_cReturn.ShowWindow(SW_HIDE);
	m_sReturn.ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDialogBasicPara::OnCbnSelchangeReturn()
{
	// TODO: 在此添加控件通知处理程序代码
	nIndexMotionReturn=m_cReturn.GetCurSel();
	if(nIndexMotionReturn==6)
	{
		CDialogMotionLaw dlgmtlaw;
			dlgmtlaw.DoModal();
			hh.SetAt(0,dlgmtlaw.t1);
			hh.SetAt(1,dlgmtlaw.t2);
			hh.SetAt(2,dlgmtlaw.t3);
			hh.SetAt(3,dlgmtlaw.t4);
			hh.SetAt(4,dlgmtlaw.t5);
			hh.SetAt(5,dlgmtlaw.t6);
			hh.SetAt(6,dlgmtlaw.t7);
	}

}

void CDialogBasicPara::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_bRiseReturn.GetCheck()==BST_CHECKED)
	{
		m_cReturn.ShowWindow(SW_HIDE);
		m_sReturn.ShowWindow(SW_HIDE);
		nIndexMotionReturn=m_cMotion.GetCurSel();
		nCheckStyle=BST_CHECKED;
		if(nIndexMotion==6)
			hh.Copy(t);

	}
	else
	{
		m_cReturn.ShowWindow(SW_SHOW);
		m_sReturn.ShowWindow(SW_SHOW);
		nCheckStyle=BST_UNCHECKED;
	}

}
