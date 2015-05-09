// DialogShowGCode.cpp : 实现文件
//

#include "stdafx.h"
#include "CamCADSystem.h"
#include "DialogShowGCode.h"
#include "ShowGCodeView.h"


// CDialogShowGCode 对话框

IMPLEMENT_DYNAMIC(CDialogShowGCode, CDialog)

CDialogShowGCode::CDialogShowGCode(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogShowGCode::IDD, pParent)
{

}

CDialogShowGCode::~CDialogShowGCode()
{
}

void CDialogShowGCode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogShowGCode, CDialog)
END_MESSAGE_MAP()


// CDialogShowGCode 消息处理程序

BOOL CDialogShowGCode::OnInitDialog()
{
	CDialog::OnInitDialog();
	UINT TargetCtrlID = IDC_STATIC;    
    CWnd *pWnd = this->GetDlgItem(IDC_STATIC);    
    CRect RectTargetCtrl;    
    pWnd->GetWindowRect(RectTargetCtrl);    
    pWnd->DestroyWindow();    
    this->ScreenToClient(RectTargetCtrl);    
  
    //在目标位置动态创建CScrollView    
    if (NULL==m_pSWGView)    
    {    
        return FALSE;    
    }   
    m_pSWGView = (CShowGCodeView*)RUNTIME_CLASS(CShowGCodeView)->CreateObject(); //这条语句不能少，有的教程中少了，会出错。  
    m_pSWGView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW|WS_VSCROLL|WS_HSCROLL, RectTargetCtrl, this, TargetCtrlID);    
    //使用CreateView创建的视图 不能自动调用OnInitialUpdate函数，需要人工调用OnInitialUpdate函数或者发送 WM_INITIALUPDATE消息  
    m_pSWGView->OnInitialUpdate();  
    // 使用CreateView创建的视图不会自动显示并且激活，需要人工操作   
    m_pSWGView->ShowWindow(SW_SHOW);    
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
