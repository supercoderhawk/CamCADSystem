// DialogPartModify.cpp : 实现文件
//

#include "stdafx.h"
#include "CamCADSystem.h"
#include "DialogPartModify.h"
#include "PartModifyView.h"


// CDialogPartModify 对话框

IMPLEMENT_DYNAMIC(CDialogPartModify, CDialog)

CDialogPartModify::CDialogPartModify(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPartModify::IDD, pParent)
	, fold(3)
{

}

CDialogPartModify::~CDialogPartModify()
{
}

void CDialogPartModify::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, fold);
	DDV_MinMaxDouble(pDX, fold, 0, 100);
}


BEGIN_MESSAGE_MAP(CDialogPartModify, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CDialogPartModify::OnBnClickedButtonPartModify)
	ON_BN_CLICKED(1002, &CDialogPartModify::OnBnClickedSweep)
END_MESSAGE_MAP()


// CDialogPartModify 消息处理程序

void CDialogPartModify::OnBnClickedButtonPartModify()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	Invalidate();
	UpdateWindow();
	m_pPtModView->FOLD=fold;
	m_pPtModView->ConductPartModify();
}

void CDialogPartModify::OnBnClickedSweep()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pPtModView->Invalidate();
	m_pPtModView->UpdateWindow();
}

BOOL CDialogPartModify::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UINT TargetCtrlID = IDC_STATIC;    
    CWnd *pWnd = this->GetDlgItem(IDC_STATIC);    
    CRect RectTargetCtrl;    
    pWnd->GetWindowRect(RectTargetCtrl);    
    pWnd->DestroyWindow();    
    this->ScreenToClient(RectTargetCtrl);    
  
    //在目标位置动态创建CScrollView    
    if (NULL==m_pPtModView)    
    {    
        return FALSE;    
    }   
    m_pPtModView = (CPartModifyView*)RUNTIME_CLASS(CPartModifyView)->CreateObject(); //这条语句不能少，有的教程中少了，会出错。  
    m_pPtModView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW|WS_VSCROLL|WS_HSCROLL, RectTargetCtrl, this, TargetCtrlID);    
    //使用CreateView创建的视图 不能自动调用OnInitialUpdate函数，需要人工调用OnInitialUpdate函数或者发送 WM_INITIALUPDATE消息  
    m_pPtModView->OnInitialUpdate();  
    // 使用CreateView创建的视图不会自动显示并且激活，需要人工操作   
    m_pPtModView->ShowWindow(SW_SHOW);    
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
