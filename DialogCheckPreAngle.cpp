// DialogCheckPreAngle.cpp : 实现文件
//

#include "stdafx.h"
#include "CamCADSystem.h"
#include "DialogCheckPreAngle.h"
#include "MainFrm.h"
#include "CamCADSystemDoc.h"
#include "CamCADSystemView.h"

// CDialogCheckPreAngle 对话框

IMPLEMENT_DYNAMIC(CDialogCheckPreAngle, CDialog)

CDialogCheckPreAngle::CDialogCheckPreAngle(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogCheckPreAngle::IDD, pParent)
	, AllowAlpha(30)
	, CheckResult(_T(""))
{

}

CDialogCheckPreAngle::~CDialogCheckPreAngle()
{
}

void CDialogCheckPreAngle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ENTER, AllowAlpha);
	DDX_Text(pDX, IDC_EDIT_SHOW, CheckResult);
}


BEGIN_MESSAGE_MAP(CDialogCheckPreAngle, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CHECK, &CDialogCheckPreAngle::OnBnClickedButtonCheck)
END_MESSAGE_MAP()


// CDialogCheckPreAngle 消息处理程序

void CDialogCheckPreAngle::OnBnClickedButtonCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CMainFrame*   pFrame=(CMainFrame*)AfxGetMainWnd();   
    CCamCADSystemView* pView = (CCamCADSystemView*)pFrame->GetActiveView();
	pView->cam.SetParameter3(AllowAlpha);
	double max=pView->cam.m_dPreAngle[0];
	for (int i=1;i<COUNT;i++)
		if(pView->cam.m_dPreAngle[i]>max)
			max=pView->cam.m_dPreAngle[i];
	if (max<AllowAlpha)
		CheckResult.Format(_T("压力角最大值为：%.2f，小于%.2f，校核通过"),max,AllowAlpha);
	UpdateData(FALSE);


}
