// DialogMotoinLaw.cpp : 实现文件
//

#include "stdafx.h"
#include "CamCADSystem.h"
#include "DialogMotionLaw.h"


// CDialogMotionLaw 对话框

IMPLEMENT_DYNAMIC(CDialogMotionLaw, CDialog)

CDialogMotionLaw::CDialogMotionLaw(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMotionLaw::IDD, pParent)
	, t0(0)
	, t1(0)
	, t2(0.5)
	, t3(0.5)
	, t4(0.5)
	, t5(0.5)
	, t6(1)
	, t7(1)
{

}

CDialogMotionLaw::~CDialogMotionLaw()
{
}

void CDialogMotionLaw::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_T1, t0);
	DDV_MinMaxDouble(pDX, t0, 0, 1);
	DDX_Text(pDX, IDC_EDIT_T2, t1);
	DDV_MinMaxDouble(pDX, t1, 0, 1);
	DDX_Text(pDX, IDC_EDIT_T3, t2);
	DDV_MinMaxDouble(pDX, t2, 0, 1);
	DDX_Text(pDX, IDC_EDIT_T4, t3);
	DDV_MinMaxDouble(pDX, t3, 0, 1);
	DDX_Text(pDX, IDC_EDIT_T5, t4);
	DDV_MinMaxDouble(pDX, t4, 0, 1);
	DDX_Text(pDX, IDC_EDIT_T6, t5);
	DDV_MinMaxDouble(pDX, t5, 0, 1);
	DDX_Text(pDX, IDC_EDIT_T7, t6);
	DDV_MinMaxDouble(pDX, t6, 0, 1);
	DDX_Text(pDX, IDC_EDIT_T8, t7);
	DDV_MinMaxDouble(pDX, t7, 0, 1);
}


BEGIN_MESSAGE_MAP(CDialogMotionLaw, CDialog)
END_MESSAGE_MAP()


// CDialogMotionLaw 消息处理程序
