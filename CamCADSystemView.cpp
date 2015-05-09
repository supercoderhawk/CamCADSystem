// CamCADSystemView.cpp : CCamCADSystemView 类的实现
//

#include "stdafx.h"
#include "CamCADSystem.h"

#include "CamCADSystemDoc.h"
#include "CamCADSystemView.h"
#include "DialogBasicPara.h"
#include "DialogCNCPara.h"
#include "DialogShowGCode.h"
#include "DialogPartModify.h"
#include "DialogCheckPreAngle.h"
#include "DialogMotionLaw.h"
#include "GCode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCamCADSystemView

IMPLEMENT_DYNCREATE(CCamCADSystemView, CView)

BEGIN_MESSAGE_MAP(CCamCADSystemView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_32771, &CCamCADSystemView::OnEnterBasicPara)
	ON_COMMAND(ID_32773, &CCamCADSystemView::OnDrawCam)
	ON_COMMAND(ID_32774, &CCamCADSystemView::OnPartModify)
	ON_COMMAND(ID_32776, &CCamCADSystemView::OnCreateGCode)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_32772, &CCamCADSystemView::OnDrawMotionLaw)
//	ON_COMMAND(ID_CheckPreAngle, &CCamCADSystemView::OnCheckpreangle)
//	ON_COMMAND(ID_Check, &CCamCADSystemView::OnCheck)
//	ON_COMMAND(ID_32777, &CCamCADSystemView::OnCheckAngle)
	ON_COMMAND(ID_32775, &CCamCADSystemView::OnCheckPreAngle)
END_MESSAGE_MAP()

// CCamCADSystemView 构造/析构

CCamCADSystemView::CCamCADSystemView()
{
	// TODO: 在此处添加构造代码
	m_bIsEnterModify=false;
	m_bIsDraw=false;
	t.SetSize(7);
	h.SetSize(7);

}

CCamCADSystemView::~CCamCADSystemView()
{
}

BOOL CCamCADSystemView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
//	cs.cx=100;
//	cs.cy=100;
//	cs.cy = ::GetSystemMetrics(SM_CYSCREEN) / 3;
//	cs.cx = ::GetSystemMetrics(SM_CXSCREEN) / 3;
//	cs.y = 50;
//	cs.x = 50;
//	return TRUE;

	return CView::PreCreateWindow(cs);
}

// CCamCADSystemView 绘制

void CCamCADSystemView::OnDraw(CDC* /*pDC*/)
{
	CCamCADSystemDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CCamCADSystemView 打印

BOOL CCamCADSystemView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CCamCADSystemView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CCamCADSystemView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// CCamCADSystemView 诊断

#ifdef _DEBUG
void CCamCADSystemView::AssertValid() const
{
	CView::AssertValid();
}

void CCamCADSystemView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCamCADSystemDoc* CCamCADSystemView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCamCADSystemDoc)));
	return (CCamCADSystemDoc*)m_pDocument;
}
#endif //_DEBUG


// CCamCADSystemView 消息处理程序

void CCamCADSystemView::OnEnterBasicPara()
{
	// TODO: 在此添加命令处理程序代码
	CDialogBasicPara dlgbasic;	//创建对话框类
	if(dlgbasic.DoModal()==IDOK)
	{
		//成功创建模态对话框后
		
		if (dlgbasic.nIndexMotion==6)
		{
			
			t.Copy(dlgbasic.t);
			h.Copy(dlgbasic.hh);

		}
		else
			CalMotionLaw(dlgbasic.nIndexMotion,
						dlgbasic.nIndexMotionReturn,
						dlgbasic.nCheckStyle);
		//进行数据交换
		cam.SetParameter(dlgbasic.Delta0,dlgbasic.Delta1,
		dlgbasic.Delta01,dlgbasic.Delta02,
		dlgbasic.h,dlgbasic.r0, dlgbasic.e,dlgbasic.rr0,
		dlgbasic.nIndexMotion, dlgbasic.nIndexMotionReturn,
		&t, &h, dlgbasic.nIndexFollower);
		//计算坐标NUM
		cam.CalcParameter();
		cam.CalcPoint();
		cam.CalcCtrlPoint();
		//计算坐标COUNT
		cam.CalcParameter2();
		cam.CalcPoint2();
		cam.CalcCtrlPoint2();
		//计算压力角
		cam.CalcPreAngle();
	}
	
}

void CCamCADSystemView::OnDrawCam()
{
	// TODO: 在此添加命令处理程序代码
	m_bIsDraw=true;	//标记绘制
	CDC *pDC=GetDC();
	CRect r1;
	GetClientRect(&r1);	//获得客户区大小
	Invalidate();	//将客户区设置为无效
	UpdateWindow();	//更新窗口
	//绘制凸轮
	cam.DrawCam3BSpline(pDC,cam.mpx,cam.mpy,NUM,r1);
	//显示压力角
	cam.ShowPresAngle(pDC,r1);
	ReleaseDC(pDC);
}

void CCamCADSystemView::OnPartModify()
{
	// TODO: 在此添加命令处理程序代码
	m_bIsEnterModify=true;	//标记进入局部修改
	
	CDC *pDC=GetDC();
	CRect r1;
	GetClientRect(&r1);
	int r=10;
	//圆的外接矩形
	CRect r2=CRect((int)(r1.CenterPoint().x+cam.px[0]-r),
				(int)(r1.CenterPoint().y-cam.py[0]-r),
				(int)(r1.CenterPoint().x+cam.px[0]+r),
				(int)(r1.CenterPoint().y-cam.py[0]+r));


	Invalidate();
	UpdateWindow();
	BOOL BSus=pDC->Ellipse(&r2);//绘制圆
	cam.DrawCam3BSpline(pDC,cam.mpx,cam.mpy,NUM,r1);
//	cam.DrawCamLine(pDC,cam.px2,cam.py2,COUNT,r1,1);
	ReleaseDC(pDC);
}

void CCamCADSystemView::OnCreateGCode()
{
	// TODO: 在此添加命令处理程序代码
	CDC *pDC=GetDC();

	CDialogCNCPara cncdlg;
	if(cncdlg.DoModal()==IDOK)
	{
		UpdateData(TRUE);

	}
	cam.CreateGCode();

	CDialogShowGCode showdlg;
	if(showdlg.DoModal()==IDOK)
	{
		CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("文本文件(*.txt)"));
		CString sfname;
		if(dlg.DoModal()==IDOK)
		{
		sfname = dlg.GetPathName();
		}

		CStdioFile f1(sfname,CFile::modeReadWrite|CFile::modeCreate);

		CString *GCode=new CString[cam.gcode->GetGCodeLength()];
		cam.gcode->GetGCode(GCode);
		for (int i=0;i<2*(cam.GetDelta1()+cam.GetDelta0())+8;i++)
		{
		f1.WriteString(GCode[i]);
		f1.WriteString(_T("\n"));
			
		}
		delete[] GCode;
	}


//	for (int i=0;i<2*cam.GetDelta1();i++)
//		pDC->TextOutW(0,16*i,cam.GCode[i]);
	

	ReleaseDC(pDC);
}

void CCamCADSystemView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	bool bSelected=IsPointSelected(point);
	if(bSelected && m_bIsEnterModify)
	{
		CDialogPartModify dlgmodify;
		dlgmodify.DoModal();
	//	dlgmodify.m_pModifyView->Test();
	//	MessageBox(_T("123"));
	}
	CView::OnLButtonDown(nFlags, point);
}

void CCamCADSystemView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CView::OnMouseMove(nFlags, point);
}

//判断点是否被选中
bool CCamCADSystemView::IsPointSelected(CPoint point)
{
	CRect r1;
	GetClientRect(&r1);
	int n1=(int)(cam.px[0]+r1.CenterPoint().x);
	int n2=(int)(-cam.py[0]+r1.CenterPoint().y);
	for(int i=-3;i<4;i++)
		for (int j=-3;j<4;j++)
			if(n1==(point.x+i) && n2==(point.y+j))
				return true;
		
	return false;
}
void CCamCADSystemView::OnDrawMotionLaw()
{
	// TODO: 在此添加命令处理程序代码
	CDC *pDC=GetDC();
	Invalidate();
	UpdateWindow();
	int n=200;
	pDC->MoveTo(0,n-(int)cam.m_ds2[0]);
	for (int i=1;i<cam.GetDelta0();i++)
		pDC->LineTo(15*i,n-(int)cam.m_ds2[i]);
	pDC->MoveTo(0,n);
	pDC->LineTo((int)cam.GetDelta0()*20,n);

	ReleaseDC(pDC);
}

//void CCamCADSystemView::OnCheckpreangle()
//{//
	// TODO: 在此添加命令处理程序代码
//	CDialogCheckPreAngle dlgcheck;
//	if(dlgcheck.DoModal()==IDOK)
//	{

//	}
//}





void CCamCADSystemView::OnCheckPreAngle()
{
	// TODO: 在此添加命令处理程序代码
	CDialogCheckPreAngle dlgcheck;
	if(dlgcheck.DoModal()==IDOK)
	{

	}
}

// 计算运动规律的T值
void CCamCADSystemView::CalMotionLaw(int nIndexMotion,
									 int nIndexMotionReturn,
									 int CheckStyle)
{
//	int n=t.GetSize();
	
	switch(nIndexMotion)
	{
	case 0:
		{
		t.SetAt(0,0.25);
		t.SetAt(1,0.25);
		t.SetAt(2,0.5);
		t.SetAt(3,0.5);
		t.SetAt(4,0.75);
		t.SetAt(5,0.75);
		t.SetAt(6,1);
		break;
		}
	case 1:
		{
		t.SetAt(0,0);
		t.SetAt(1,0.5);
		t.SetAt(2,0.5);
		t.SetAt(3,0.5);
		t.SetAt(4,0.5);
		t.SetAt(5,1);
		t.SetAt(6,1);
		break;
		}
	case 2:
		{
		t.SetAt(0,0);
		t.SetAt(1,0);
		t.SetAt(2,0);
		t.SetAt(3,0);
		t.SetAt(4,0);
		t.SetAt(5,0);
		t.SetAt(6,0);
		break;
		}
	case 3:
		{
		t.SetAt(0,0.125);
		t.SetAt(1,AA);
		t.SetAt(2,AA+0.125);
		t.SetAt(3,AA+0.125);
		t.SetAt(4,AA+0.25);
		t.SetAt(5,AA+0.25);
		t.SetAt(6,1);
		break;
		}
	case 4:
		{
		t.SetAt(0,0.125);
		t.SetAt(1,0.375);
		t.SetAt(2,0.5);
		t.SetAt(3,0.5);
		t.SetAt(4,0.625);
		t.SetAt(5,0.875);
		t.SetAt(6,1);
		break;
		}
	case 5:
		{
		t.SetAt(0,0);
		t.SetAt(1,0);
		t.SetAt(2,0.5);
		t.SetAt(3,0.5);
		t.SetAt(4,1);
		t.SetAt(5,1);
		t.SetAt(6,1);
		break;
		}
	}
	if(CheckStyle==BST_CHECKED)
		h.Copy(t);
	else
	switch(nIndexMotionReturn)
	{
	case 0:
		{
		h.SetAt(0,0.25);
		h.SetAt(1,0.25);
		h.SetAt(2,0.5);
		h.SetAt(3,0.5);
		h.SetAt(4,0.75);
		h.SetAt(5,0.75);
		h.SetAt(6,1);
		break;
		}
	case 1:
		{
		h.SetAt(0,0);
		h.SetAt(1,0.5);
		h.SetAt(2,0.5);
		h.SetAt(3,0.5);
		h.SetAt(4,0.5);
		h.SetAt(5,1);
		h.SetAt(6,1);
		break;
		}
	case 2:
		{
		h.SetAt(0,0);
		h.SetAt(1,0);
		h.SetAt(2,0);
		h.SetAt(3,0);
		h.SetAt(4,0);
		h.SetAt(5,0);
		h.SetAt(6,0);
		break;
		}
	case 3:
		{
		h.SetAt(0,0.125);
		h.SetAt(1,AA);
		h.SetAt(2,AA+0.125);
		h.SetAt(3,AA+0.125);
		h.SetAt(4,AA+0.25);
		h.SetAt(5,AA+0.25);
		h.SetAt(6,1);
		break;
		}
	case 4:
		{
		h.SetAt(0,0.125);
		h.SetAt(1,0.375);
		h.SetAt(2,0.5);
		h.SetAt(3,0.5);
		h.SetAt(4,0.625);
		h.SetAt(5,0.875);
		h.SetAt(6,1);
		break;
		}
	case 5:
		{
		h.SetAt(0,0);
		h.SetAt(1,0);
		h.SetAt(2,0.5);
		h.SetAt(3,0.5);
		h.SetAt(4,1);
		h.SetAt(5,1);
		h.SetAt(6,1);
		break;
		}
	}


}
