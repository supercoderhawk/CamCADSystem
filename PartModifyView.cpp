// PartModifyView.cpp : 实现文件
//

#include "stdafx.h"
#include "MainFrm.h"
#include "CamCADSystem.h"
#include "CamCADSystemDoc.h"
#include "CamCADSystemView.h"
#include "PartModifyView.h"


// CPartModifyView

IMPLEMENT_DYNCREATE(CPartModifyView, CScrollView)

CPartModifyView::CPartModifyView()
{
	m_bEnterRubber=false;
	m_bIsSelected=false;
	m_bIsSetCross=false;
	m_nCurPtNum=0;
	
	m_nPtNum=-1;
	m_hCursor=LoadCursor(NULL,IDC_ARROW);
	for(int i=0;i<256;i++)
	{
		nBuffer[i]=0;
		nPreBuffer[i]=0;
	}
	

	FOLD=3;


}

CPartModifyView::~CPartModifyView()
{
}


BEGIN_MESSAGE_MAP(CPartModifyView, CScrollView)
//	ON_WM_NCDESTROY()
	ON_WM_DESTROY()
	ON_WM_MOUSEACTIVATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
//	ON_WM_SETCURSOR()
ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// CPartModifyView 绘图

void CPartModifyView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CPartModifyView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CPartModifyView 诊断

#ifdef _DEBUG
void CPartModifyView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CPartModifyView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPartModifyView 消息处理程序

//void CPartModifyView::OnNcDestroy()
//{
//	CScrollView::OnNcDestroy();
//
//	// TODO: 在此处添加消息处理程序代码
//}

void CPartModifyView::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类

	CWnd::PostNcDestroy();
}

void CPartModifyView::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

void CPartModifyView::OnActivateFrame(UINT nState, CFrameWnd* pDeactivateFrame)
{
	// TODO: 在此添加专用代码和/或调用基类

//	CWnd::OnActivateFrame(nState, pDeactivateFrame);
}

int CPartModifyView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CPartModifyView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	GetClientRect(&r);
		CDC *pDC=GetDC();
	CRect r1;
	GetClientRect(&r1);
	CString str;
	//检查点是否选中，若被选中，更新
	if(m_bIsSelected)
	{

		if(m_nPtNum==0)
		{
			pDC->MoveTo(point);
			pDC->LineTo(nBuffer[2],nBuffer[3]);
		}
		else if (m_nPtNum==(SPNUM+2))
		{
			pDC->MoveTo(nBuffer[2*SPNUM+2],nBuffer[2*SPNUM+3]);
			pDC->LineTo(point);
		}
		else
		{
			pDC->MoveTo(nBuffer[2*m_nPtNum-2],nBuffer[2*m_nPtNum-1]);
			pDC->LineTo(point);
			pDC->LineTo(nBuffer[2*m_nPtNum+2],nBuffer[2*m_nPtNum+3]);
		}
		//更新节点
		nBuffer[2*m_nPtNum]=point.x;
		nBuffer[2*m_nPtNum+1]=point.y;
		DrawBSpline(pDC,nBuffer,SPNUM+3);
		m_bEnterRubber=false;
		m_bIsSelected=false;

	
	}
	//若未被选中，检查状态
	else
	{
		m_nPtNum=PointIsSelected(point);
		if(m_nPtNum!=-1)
		{
			m_bIsSelected=true;
			m_pPre=CPoint(nBuffer[2*m_nPtNum],nBuffer[2*m_nPtNum+1]);
//			str.Format(_T("选中了%d"),m_nPtNum);
//			MessageBox(str);
			m_bEnterRubber=true;
		}
	}
	ReleaseDC(pDC);
	CScrollView::OnLButtonDown(nFlags, point);
}

void CPartModifyView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(PointIsSelected(point)!=-1)
	{
		m_bIsSetCross=true;
			
			m_hCursor=LoadCursor(NULL,IDC_CROSS);
	}
	else
	{
		m_hCursor=LoadCursor(NULL,IDC_ARROW);
		m_bIsSetCross=false;
	}

	//若需要进入橡皮条
	if(m_bEnterRubber)
	{
		
		m_hCursor=LoadCursor(NULL,IDC_CROSS);
		DrawRubber(point);
		
		
	}
	CScrollView::OnMouseMove(nFlags, point);
}

//BOOL CPartModifyView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
//}

void CPartModifyView::DrawBSpline(CDC* pDC, int nBuffer[], int n)
{
	double x = ( nBuffer[0] + 4.0 * nBuffer[2] + nBuffer[4] ) / 6.0;
	double y = ( nBuffer[1] + 4.0 * nBuffer[3] + nBuffer[5] ) / 6.0;

	CPen newPen(PS_SOLID, 0, RGB(0,0,0)), *oldPen;
	if (pDC)
	{
		oldPen = pDC->SelectObject(&newPen);
		pDC->MoveTo( (long)x, (long)y );
	}

	for ( int nSeg = 0; nSeg < n-3; nSeg++ )
	{
		// 找到最大和最小的坐标，求得最大的范围，从而确定u的最小步长
		int xMax, xMin, yMax, yMin;
		xMax = xMin = nBuffer[2*nSeg + 2];
		yMax = yMin = nBuffer[2*nSeg + 3];
		if ( xMax < nBuffer[2*nSeg + 4] ) xMax = nBuffer[2*nSeg + 4];
		if ( xMin > nBuffer[2*nSeg + 4] ) xMin = nBuffer[2*nSeg + 4];
		if ( yMax < nBuffer[2*nSeg + 5] ) yMax = nBuffer[2*nSeg + 5];
		if ( yMin > nBuffer[2*nSeg + 5] ) yMin = nBuffer[2*nSeg + 5];
		double fMax = (double)(xMax - xMin);
		if ( fMax < (double)(yMax - yMin) ) fMax = (double)(yMax - yMin);
		double	u, uStep = 3.0 / fMax ;		// u值步长

		for ( u = uStep; u<=1.0; u+= uStep )
		{
			x = (1.0-u)*(1.0-u)*(1.0-u)
					* (double)(nBuffer[2*nSeg])
				+ ( 3.0*u*u*u - 6.0*u*u + 4.0 )
					* (double)(nBuffer[2*nSeg + 2])
				+ ( -3.0*u*u*u + 3.0*u*u + 3.0*u + 1.0 )
					* (double)(nBuffer[2*nSeg + 4])
				+ u*u*u
					* (double)(nBuffer[2*nSeg + 6]);
			y = (1.0-u)*(1.0-u)*(1.0-u)
					* (double)(nBuffer[2*nSeg + 1])
				+ ( 3.0*u*u*u - 6.0*u*u + 4.0 )
					* (double)(nBuffer[2*nSeg + 3])
				+ ( -3.0*u*u*u + 3.0*u*u + 3.0*u + 1.0 )
					* (double)(nBuffer[2*nSeg + 5])
				+ u*u*u
					* (double)(nBuffer[2*nSeg + 7]);
			x = x / 6.0;	y = y / 6.0;
			if (pDC)
				pDC->LineTo((long)x, (long)y );
	
		}
	}

	if (pDC)
		pDC->SelectObject( oldPen );
	
	for (int i=0;i<2*n;i++)
		nPreBuffer[i]=nBuffer[i];
}

// 橡皮条技术函数
void CPartModifyView::DrawRubber(CPoint point)
{
	CDC *pDC = GetDC();
	CRect r1;
	GetClientRect(&r1);
	
		//删去先前的线条
		pDC->SetROP2( R2_NOTXORPEN );
		if(m_nPtNum==0)
		{
			pDC->MoveTo(m_pPre);
			pDC->LineTo(nBuffer[2],nBuffer[3]);
			nPreBuffer[0]=m_pPre.x;
			nPreBuffer[1]=m_pPre.y;
		}
		else if (m_nPtNum==SPNUM+2)
		{
			pDC->MoveTo(nBuffer[2*SPNUM+2],nBuffer[2*SPNUM+3]);
			pDC->LineTo(m_pPre);
			nPreBuffer[2*SPNUM+4]=m_pPre.x;
			nPreBuffer[2*SPNUM+5]=m_pPre.y;
		}
		else
		{
			pDC->MoveTo(nBuffer[2*m_nPtNum-2],nBuffer[2*m_nPtNum-1]);
			pDC->LineTo(m_pPre);
			pDC->LineTo(nBuffer[2*m_nPtNum+2],nBuffer[2*m_nPtNum+3]);
			nPreBuffer[2*m_nPtNum]=m_pPre.x;
			nPreBuffer[2*m_nPtNum+1]=m_pPre.y;
		}
		DrawBSpline(pDC,nPreBuffer,SPNUM+3);

		//绘制现在的线条
		pDC->SetROP2( R2_NOTXORPEN );		
		if(m_nPtNum==0)
		{
			pDC->MoveTo(point);
			pDC->LineTo(nBuffer[2],nBuffer[3]);
			nPreBuffer[0]=point.x;
			nPreBuffer[1]=point.y;
		}
		else if (m_nPtNum==SPNUM+2)
		{
			pDC->MoveTo(nBuffer[2*SPNUM+2],nBuffer[2*SPNUM+3]);
			pDC->LineTo(point);
			nPreBuffer[2*SPNUM+4]=point.x;
			nPreBuffer[2*SPNUM+5]=point.y;
		}
		else
		{
			pDC->MoveTo(nBuffer[2*m_nPtNum-2],nBuffer[2*m_nPtNum-1]);
			pDC->LineTo(point);
			pDC->LineTo(nBuffer[2*m_nPtNum+2],nBuffer[2*m_nPtNum+3]);
			nPreBuffer[2*m_nPtNum]=point.x;
			nPreBuffer[2*m_nPtNum+1]=point.y;
		}
		DrawBSpline(pDC,nPreBuffer,SPNUM+3);
		m_pPre=point;
		ReleaseDC(pDC);
}

int CPartModifyView::PointIsSelected(CPoint point)
{
	int n=3;
	for (int i=0;i<SPNUM+3;i++)
		for (int j=-n;j<=n;j++)
			for (int k=-n;k<=n;k++)
				if(nBuffer[2*i]==(point.x+j) && nBuffer[2*i+1]==(point.y+k))
				{
				
//					m_nPtNum=i;
					return i;
				}
	return -1;
}

// 进行局部修改
void CPartModifyView::ConductPartModify(void)
{
	CMainFrame*   pFrame=(CMainFrame*)AfxGetMainWnd();   
    CCamCADSystemView* pView = (CCamCADSystemView*)pFrame->GetActiveView();
	CDC* pDC=GetDC();
	CRect r1;
	GetClientRect(&r1);

	CalBuffer(pView->cam , r1,6);

	DrawBSpline(pDC,nBuffer,SPNUM+3);
	
	pDC->MoveTo(nBuffer[0],nBuffer[1]);

	for (int i=1;i<SPNUM+3;i++)
		pDC->LineTo(nBuffer[2*i],nBuffer[2*i+1]);
	ReleaseDC(pDC);
}

void CPartModifyView::CalBuffer(CCam& cam, CRect r1, int n)
{
	
	int x,y;

	int xMax, xMin, yMax, yMin;
	
	nBuffer[0]=(int)(cam.mpx[NUM-1]*FOLD);
	nBuffer[2]=(int)(cam.mpx[NUM]*FOLD);
	nBuffer[4]=(int)(cam.mpx[NUM+1]*FOLD);
	nBuffer[6]=(int)(cam.mpx[NUM+2]*FOLD);
	nBuffer[8]=(int)(cam.mpx[3]*FOLD);
	nBuffer[10]=(int)(cam.mpx[4]*FOLD);

	nBuffer[1]=(int)(cam.mpy[NUM-1]*FOLD);
	nBuffer[3]=(int)(cam.mpy[NUM]*FOLD);
	nBuffer[5]=(int)(cam.mpy[NUM+1]*FOLD);
	nBuffer[7]=(int)(cam.mpy[NUM+2]*FOLD);
	nBuffer[9]=(int)(cam.mpy[3]*FOLD);
	nBuffer[11]=(int)(cam.mpy[4]*FOLD);

	xMax = xMin = nBuffer[0];
	yMax = yMin = nBuffer[1];
	for(int i=0;i<n;i++)
		if (nBuffer[2*i]<xMin)
			xMin=nBuffer[2*i];
		else if(nBuffer[2*i]>xMax)
			xMax=nBuffer[2*i];
		else if(nBuffer[2*i+1]>yMax)
			yMax=nBuffer[2*i+1];
		else if (nBuffer[2*i+1]<yMin)
			yMin=nBuffer[2*i+1];
	x=(xMax-xMin)/2+xMin;
	y=(yMax-yMin)/2+yMin;
	x=r1.CenterPoint().x-x;
	y=r1.CenterPoint().y-y;
	nBuffer[0]=(int)(cam.mpx[NUM-1]*FOLD+x);
	nBuffer[2]=(int)(cam.mpx[NUM]*FOLD+x);
	nBuffer[4]=(int)(cam.mpx[NUM+1]*FOLD+x);
	nBuffer[6]=(int)(cam.mpx[NUM+2]*FOLD+x);
	nBuffer[8]=(int)(cam.mpx[3]*FOLD+x);
	nBuffer[10]=(int)(cam.mpx[4]*FOLD+x);

	nBuffer[1]=(int)(cam.mpy[NUM-1]*FOLD+y);
	nBuffer[3]=(int)(cam.mpy[NUM]*FOLD+y);
	nBuffer[5]=(int)(cam.mpy[NUM+1]*FOLD+y);
	nBuffer[7]=(int)(cam.mpy[NUM+2]*FOLD+y);
	nBuffer[9]=(int)(cam.mpy[3]*FOLD+y);
	nBuffer[11]=(int)(cam.mpy[4]*FOLD+y);


}
BOOL CPartModifyView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( m_bIsSelected | m_bIsSetCross)
	{
	//	SetCursor( AfxGetApp()->LoadStandardCursor( IDC_CROSS ) );
		SetCursor(m_hCursor);
		return TRUE;
	}
	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}
