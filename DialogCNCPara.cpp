// DialogCNCPara.cpp : 实现文件
//

#include "stdafx.h"
#include "CamCADSystem.h"
#include "DialogCNCPara.h"

#include "MainFrm.h"
#include "CamCADSystemDoc.h"
#include "CamCADSystemView.h"


// CDialogCNCPara 对话框

IMPLEMENT_DYNAMIC(CDialogCNCPara, CDialog)

CDialogCNCPara::CDialogCNCPara(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogCNCPara::IDD, pParent)
	, KnifeID(_T("T1"))
	, KnifeFace(_T("D1"))
	, Feed(0.3)
	, Speed(1000)
{

}

CDialogCNCPara::~CDialogCNCPara()
{
}

void CDialogCNCPara::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITKNIFE, KnifeID);
	DDX_Text(pDX, IDC_EDIT_KNIFEID, KnifeFace);
	DDX_Text(pDX, IDC_EDIT_FEED, Feed);
	DDX_Text(pDX, IDC_EDIT_SPEED, Speed);
}


BEGIN_MESSAGE_MAP(CDialogCNCPara, CDialog)
	ON_BN_CLICKED(IDOK, &CDialogCNCPara::OnBnClickedEnterCNCPara)
END_MESSAGE_MAP()


// CDialogCNCPara 消息处理程序

void CDialogCNCPara::OnBnClickedEnterCNCPara()
{
	// TODO: 在此添加控件通知处理程序代码
	 CMainFrame*   pFrame=(CMainFrame*)AfxGetMainWnd();   
    CCamCADSystemView* pView = (CCamCADSystemView*)pFrame->GetActiveView();
	UpdateData(TRUE);
	pView->cam.SetParameter2(KnifeID,KnifeFace,Feed,Speed);
	OnOK();
	
}
