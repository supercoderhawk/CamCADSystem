// ShowGCodeView.cpp : 实现文件
//

#include "stdafx.h"
#include "CamCADSystem.h"
#include "ShowGCodeView.h"

#include "MainFrm.h"
#include "CamCADSystemDoc.h"
#include "CamCADSystemView.h"


// CShowGCodeView

IMPLEMENT_DYNCREATE(CShowGCodeView, CScrollView)

CShowGCodeView::CShowGCodeView()
{

}

CShowGCodeView::~CShowGCodeView()
{
}


BEGIN_MESSAGE_MAP(CShowGCodeView, CScrollView)
	ON_WM_DESTROY()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()


// CShowGCodeView 绘图

//void CShowGCodeView::OnInitialUpdate()
//{
//	CScrollView::OnInitialUpdate();
//
//	CSize sizeTotal;
//	// TODO: 计算此视图的合计大小
//	sizeTotal.cx = sizeTotal.cy = 100;
//	SetScrollSizes(MM_TEXT, sizeTotal);
//}

void CShowGCodeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
	CMainFrame*   pFrame=(CMainFrame*)AfxGetMainWnd();   
    CCamCADSystemView* pView = (CCamCADSystemView*)pFrame->GetActiveView();
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	CString *GCode=new CString[pView->cam.gcode->GetGCodeLength()];
		pView->cam.gcode->GetGCode(GCode);
	for(int i=0;i<2*(pView->cam.GetDelta0()+pView->cam.GetDelta1())+8;i++)
		pDC->TextOutW(0,(tm.tmHeight+tm.tmExternalLeading)*i,GCode[i]);
	delete[] GCode;
}


// CShowGCodeView 诊断

#ifdef _DEBUG
void CShowGCodeView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CShowGCodeView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CShowGCodeView 消息处理程序

void CShowGCodeView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: 在此添加专用代码和/或调用基类
	CDC *pDC=GetDC();
	CMainFrame*   pFrame=(CMainFrame*)AfxGetMainWnd();   
    CCamCADSystemView* pView = (CCamCADSystemView*)pFrame->GetActiveView();
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	CSize sz;
	sz.cx=100;
	sz.cy=(int)(2*(pView->cam.GetDelta0()+pView->cam.GetDelta1())+11)*(tm.tmHeight+tm.tmExternalLeading);
	SetScrollSizes(MM_TEXT, sz);
//	ScrollToPosition(CPoint(0,0));
	ReleaseDC(pDC);
}

void CShowGCodeView::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

int CShowGCodeView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CShowGCodeView::OnActivateFrame(UINT nState, CFrameWnd* pDeactivateFrame)
{
	// TODO: 在此添加专用代码和/或调用基类

//	CWnd::OnActivateFrame(nState, pDeactivateFrame);
}

void CShowGCodeView::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类

	CWnd::PostNcDestroy();
}
