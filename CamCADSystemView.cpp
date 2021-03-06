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
#include <string>
#include "math.h"

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
	ON_COMMAND(ID_FILE_SAVE, &CCamCADSystemView::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CCamCADSystemView::OnFileSaveAs)
//	ON_WM_TIMER()
//ON_WM_TIMER()
ON_WM_TIMER()
END_MESSAGE_MAP()

// CCamCADSystemView 构造/析构

CCamCADSystemView::CCamCADSystemView()
{
	// TODO: 在此处添加构造代码
	m_bIsEnterModify=false;
	m_bIsDrawCam=false;
	m_bIsDrawMotion=false;
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

void CCamCADSystemView::OnDraw(CDC* pDC)
{
	CCamCADSystemDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	CRect r1;
	GetClientRect(&r1);	//获得客户区大小
	if (m_bIsDrawMotion)
		DrawMotionLaw(pDC);
	else if(m_bIsDrawCam)
	{
		
		//绘制凸轮
		cam.DrawCam3BSpline(pDC,cam.mpx2,cam.mpy2,COUNT,r1);
//		cam.DrawCam3BSpline(pDC,cam.mpx,cam.mpy,NUM,r1);
//		cam.DrawCamLine(pDC,cam.px2,cam.py2,COUNT,r1,1);
		//显示压力角
		cam.ShowPresAngle(pDC,r1);
//		SetTimer(1,1000,NULL);
	}
	else if(m_bIsEnterModify)
	{
		int r=5;
		CRect r2=CRect((int)(r1.CenterPoint().x+cam.px2[0]-r),
				(int)(r1.CenterPoint().y-cam.py2[0]-r),
				(int)(r1.CenterPoint().x+cam.px2[0]+r),
				(int)(r1.CenterPoint().y-cam.py2[0]+r));
		BOOL BSus=pDC->Ellipse(&r2);//绘制圆
		cam.DrawCam3BSpline(pDC,cam.mpx2,cam.mpy2,COUNT,r1);

	}


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
//	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
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
//		if(cam.m_bDataRise)
//			delete cam.t;
//		if(cam.m_bDataReturn)
//			delete cam.h;

		cam.CalcParameter();
		cam.CalcPoint();
		cam.CalcCtrlPoint();

//		if(cam.m_bData2Rise)
//			delete cam.t2;
//		if(cam.m_bData2Return)
//			delete cam.h2;
		//计算坐标COUNT
		cam.CalcParameter2();
		cam.CalcPoint2();
		cam.CalcCtrlPoint2();
		//计算压力角
		cam.CalcPreAngle();
	}
//	_CrtDumpMemoryLeaks();

}

void CCamCADSystemView::OnDrawCam()
{
	// TODO: 在此添加命令处理程序代码
	m_bIsDrawCam=true;	//标记绘制
	m_bIsDrawMotion=false;
	CDC *pDC=GetDC();
	CRect r1;
	GetClientRect(&r1);	//获得客户区大小
	Invalidate();	//将客户区设置为无效
	UpdateWindow();	//更新窗口
	//绘制凸轮
//	cam.DrawCam3BSpline(pDC,cam.mpx2,cam.mpy2,COUNT,r1);
//	cam.DrawCamLine(pDC,cam.px2,cam.py2,COUNT,r1,1);
	//显示压力角
//	cam.ShowPresAngle(pDC,r1);
	ReleaseDC(pDC);
}

void CCamCADSystemView::OnPartModify()
{
	// TODO: 在此添加命令处理程序代码
	m_bIsEnterModify=true;	//标记进入局部修改
	m_bIsDrawCam=false;
	m_bIsDrawMotion=false;
	
//	CDC *pDC=GetDC();
//	CRect r1;
//	GetClientRect(&r1);
//	int r=10;
	//圆的外接矩形
	

	Invalidate();
	UpdateWindow();

//	cam.DrawCam3BSpline(pDC,cam.mpx,cam.mpy,NUM,r1);
//	cam.DrawCamLine(pDC,cam.px2,cam.py2,COUNT,r1,1);
//	ReleaseDC(pDC);
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
	int n1=(int)(cam.px2[0]+r1.CenterPoint().x);
	int n2=(int)(-cam.py2[0]+r1.CenterPoint().y);
	for(int i=-3;i<4;i++)
		for (int j=-3;j<4;j++)
			if(n1==(point.x+i) && n2==(point.y+j))
				return true;
		
	return false;
}

//绘制运动规律的位移
void CCamCADSystemView::OnDrawMotionLaw()
{
	// TODO: 在此添加命令处理程序代码
	m_bIsDrawMotion=true;
	m_bIsDrawCam=false;
	CDC *pDC=GetDC();
	DrawMotionLaw(pDC);
	
	
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

// 绘制运动规律的位移
void CCamCADSystemView::DrawMotionLaw(CDC* pDC)
{
	//获取客户区大小
	CRect r1;
	GetClientRect(&r1);

	int left=r1.left+(int)(r1.Width()/15.0);	//原点横坐标
	int n=r1.top+(int)(r1.Height()*3.0/4);		//原点纵坐标
	int timesx=2;	//横轴放大倍数
	int timesy=3;		//纵轴放大倍数
	int arrow=30;	//坐标轴箭头斜线的长度
	
	//设置线宽为4
	CPen pen(PS_SOLID,4,RGB(0,0,0));
	SelectObject(pDC->m_hDC,pen);
	//绘制四段位移曲线
	pDC->MoveTo(left,n-(int)cam.m_ds2[0]);
	for (int i=1;i<cam.GetDelta0();i++)
		pDC->LineTo(left+timesx*i,n-timesy*(int)cam.m_ds2[i]);
	for (int i=(int)cam.GetDelta0();i<(int)(cam.GetDelta0()+cam.GetDelta01());
		i++)
		pDC->LineTo(left+timesx*i,n-timesy*(int)cam.m_ds2[i]);
	for (int i=(int)(cam.GetDelta0()+cam.GetDelta01());
		i<(int)(cam.GetDelta0()+cam.GetDelta01()+cam.GetDelta1());i++)
		pDC->LineTo(left+timesx*i,n-timesy*(int)cam.m_ds2[i]);
	for (int i=(int)(cam.GetDelta0()+cam.GetDelta01()+cam.GetDelta1());
		i<(int)(cam.GetDelta0()+cam.GetDelta01()+cam.GetDelta1()+cam.GetDelta02());
		i++)
		pDC->LineTo(left+timesx*i,n);

	//绘制坐标轴
	//定义画刷
	COLORREF c=RGB(0,0,0);
	CBrush brush(c);
	CPen pen2(PS_SOLID,0,RGB(0,0,0));
	SelectObject(pDC->m_hDC,pen2);
	//绘制横轴
	//绘制横轴箭头
	//定义箭头的三顶点
	CPoint ptx[3]=
	{CPoint(left+400*timesx,n),
	CPoint(left+400*timesx-(int)(arrow*cos(Pi/12)),n-(int)(arrow*sin(Pi/12))),
	CPoint(left+400*timesx-(int)(arrow*cos(Pi/12)),n+(int)(arrow*sin(Pi/12)))};
	//用顶点创建区域
	CRgn regionx;
	regionx.CreatePolygonRgn(ptx,3,WINDING);
	//填充区域
	pDC->FillRgn(&regionx,&brush);
	regionx.DeleteObject();	//删除区域
	//绘制横轴直线
	pDC->MoveTo(left,n);
	pDC->LineTo(left+400*timesx,n);

	//绘制纵轴
	//绘制纵轴箭头
	//定义箭头三个顶点
	CPoint pty[3]=
	{CPoint(left,n-50*timesy),
	CPoint(left-(int)(arrow*sin(Pi/12)),n-50*timesy+(int)(arrow*cos(Pi/12))),
	CPoint(left+(int)(arrow*sin(Pi/12)),n-50*timesy+(int)(arrow*cos(Pi/12)))};
	CRgn regiony;
	regiony.CreatePolygonRgn(pty,3,WINDING);
	pDC->FillRgn(&regiony,&brush);
	regiony.DeleteObject();
	//绘制纵轴直线
	pDC->MoveTo(left,n);
	pDC->LineTo(left,n-50*timesy);
	
	//显示文字
	TEXTMETRIC tm;
	GetTextMetrics(pDC->m_hDC,&tm);	//获得字体结构信息
	CString str;
	//显示原点
	pDC->TextOutW(left,n+tm.tmHeight/3,_T("O"));
	//显示坐标轴信息
	pDC->TextOutW(left+400*timesx,n,_T("凸轮转角δ/（º）"));
	pDC->TextOutW(left-tm.tmAveCharWidth*7,
			n-50*timesy-tm.tmHeight,_T("从动件位移h/(mm)"));
	//显示四个点的角度
	str.Format(_T("%.1f"),cam.GetDelta0());
	pDC->TextOutW(left+(int)cam.GetDelta0()*timesx,
					n+tm.tmAscent/2,str);
	str.Format(_T("%.1f"),cam.GetDelta01()+cam.GetDelta0());
	pDC->TextOutW(left+(int)(cam.GetDelta0()+cam.GetDelta01())*timesx-tm.tmAveCharWidth*3,
					n+tm.tmAscent/2,str);
	str.Format(_T("%.1f"),cam.GetDelta01()+cam.GetDelta0()+cam.GetDelta1());
	pDC->TextOutW(left+(int)(cam.GetDelta0()+
		cam.GetDelta01()+cam.GetDelta1())*timesx-tm.tmAveCharWidth*3,
					n+tm.tmAscent/2,str);
	pDC->TextOutW(left+360*timesx-tm.tmAveCharWidth*2,n+tm.tmHeight/2,_T("360"));
	//显示工作行程
	str.Format(_T("%.2f"),cam.Geth());
	pDC->TextOutW(left-tm.tmAveCharWidth*6,
			n-(int)cam.Geth()*timesy-tm.tmHeight/2,str);

	CPen pen3(PS_DASH,0,RGB(0,0,0));
	SelectObject(pDC->m_hDC,pen3);
	//绘制连接处的虚线
	pDC->MoveTo(left+(int)cam.GetDelta0()*timesx,n);
	pDC->LineTo(left+(int)cam.GetDelta0()*timesx,n-(int)cam.Geth()*timesy);
	pDC->MoveTo(left+(int)(cam.GetDelta0()+cam.GetDelta01())*timesx,n);
	pDC->LineTo(left+(int)(cam.GetDelta0()+cam.GetDelta01())*timesx,
							n-(int)cam.Geth()*timesy);
	pDC->MoveTo(left+(int)cam.GetDelta0()*timesx,n-(int)cam.Geth()*timesy);
	pDC->LineTo(left,n-(int)cam.Geth()*timesy);

	//显示标题
	CFont hFont;
	hFont.CreateFont(40,20,0,0,FW_THIN,false,false,false,
			CHINESEBIG5_CHARSET,OUT_CHARACTER_PRECIS,
			CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,
			FF_MODERN,_T("宋体"));
	SelectObject(pDC->m_hDC,hFont);
	pDC->TextOutW(left+120,20,_T("盘形凸轮机构从动件运动规律曲线"));
	

}

void CCamCADSystemView::OnFileSave()
{
	// TODO: ÔÚ´ËÌí¼ÓÃüÁî´¦Àí³ÌÐò´úÂë
	DL_Dxf* dxf = new DL_Dxf();
    DL_Codes::version exportVersion = DL_Codes::AC1015;

	CFileDialog dlg(FALSE,NULL,NULL,
				OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
				_T("cad file(*.dxf)"));
	CString sfname;
	//CString sfname;
	if(IDOK==dlg.DoModal())
	{
		sfname = dlg.GetFileName();
		

	}
	
//	sfname.
	else return;
	string sname(NULL);
		sname=sfname.GetBuffer(0);
//	char *fname=(char*)sfname.GetBufferSetLength(sfname.GetLength()+1);
//	for(int ii=0;ii<sfname.GetLength();ii++)
//		fname[ii]=(char)sfname.GetAt(ii);
//	fname[sfname.GetLength()]='\0';

	DL_WriterA* dw = dxf->out(sname.c_str(), exportVersion);

	if (dw==NULL) {
		MessageBox(_T("文件保存失败！"));
        return;
        // abort function e.g. with return
    }

	//Header区域定义
	//写入Header信息
    dxf->writeHeader(*dw);
    // 定义图纸的单位，4表示毫米
    dw->dxfString(9, "$INSUNITS");
    dw->dxfInt(70, 4);
    //定义尺寸界线
    dw->dxfString(9, "$DIMEXE");
    dw->dxfReal(40, 3);
    // 设置当前字体样式，标准样式
    dw->dxfString(9, "$TEXTSTYLE");
    dw->dxfString(7, "Standard");
    // XY轴位于左下角，原点位于0,0处:
    dw->dxfString(9, "$LIMMIN");
    dw->dxfReal(10, 0.0);
    dw->dxfReal(20, 0.0);
	//段结束
    dw->sectionEnd();

	//Tablet段定义
    dw->sectionTables();
    dxf->writeVPort(*dw);
	//指定表的最大数量25
    dw->tableLineTypes(25);
	//定义线型
    dxf->writeLineType(*dw, DL_LineTypeData("BYBLOCK", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("BYLAYER", 0));
    dxf->writeLineType(*dw,
                      DL_LineTypeData("CONTINUOUS", 0));
    dxf->writeLineType(*dw,
                      DL_LineTypeData("ACAD_ISO02W100", 0));
    dxf->writeLineType(*dw,
                      DL_LineTypeData("ACAD_ISO03W100", 0));
    dxf->writeLineType(*dw,
                      DL_LineTypeData("ACAD_ISO04W100", 0));
    dxf->writeLineType(*dw,
                      DL_LineTypeData("ACAD_ISO05W100", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("BORDER", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("BORDER2", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("BORDERX2", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("CENTER", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("CENTER2", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("CENTERX2", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("DASHDOT", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("DASHDOT2", 0));
    dxf->writeLineType(*dw,
                      DL_LineTypeData("DASHDOTX2", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("DASHED", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("DASHED2", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("DASHEDX2", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("DIVIDE", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("DIVIDE2", 0));
    dxf->writeLineType(*dw,
                      DL_LineTypeData("DIVIDEX2", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("DOT", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("DOT2", 0));
    dxf->writeLineType(*dw, DL_LineTypeData("DOTX2", 0));
    dw->tableEnd();
	
	//图层数量
    int numberOfLayers = 3;
    dw->tableLayers(numberOfLayers);
    dxf->writeLayer(*dw,
                   DL_LayerData("0", 0),
                   DL_Attributes(
                       std::string(""),      // leave empty
                       DL_Codes::white,        // default color
                       10,                  // default width
                       "CONTINUOUS"));       // default line style

    dxf->writeLayer(*dw,
                   DL_LayerData("mainlayer", 0),
                   DL_Attributes(
                       std::string(""),
                       DL_Codes::white,
                       10,
                       "CONTINUOUS"));

    dxf->writeLayer(*dw,
                   DL_LayerData("anotherlayer", 0),
                   DL_Attributes(
                       std::string(""),
                       DL_Codes::black,
                       10,
                       "CONTINUOUS"));
	dxf->writeLayer(*dw,
                   DL_LayerData("BoldLine", 0),
                   DL_Attributes(
                       std::string(""),
                       DL_Codes::white,
                      30,
                       "CONTINUOUS"));
	dxf->writeLayer(*dw,
                   DL_LayerData("CenterLine", 0),
                   DL_Attributes(
                       std::string(""),
                       DL_Codes::white,
                      10,
                       "CENTER"));
	dxf->writeLayer(*dw,
                   DL_LayerData("DashLine", 0),
                   DL_Attributes(
                       std::string(""),
                       DL_Codes::white,
                      10,
                       "DASHED"));

    dw->tableEnd();


    dxf->writeStyle(*dw);
    dxf->writeView(*dw);
    dxf->writeUcs(*dw);

    dw->tableAppid(1);
    dw->tableAppidEntry(0x12);
    dw->dxfString(2, "ACAD");
    dw->dxfInt(70, 0);
    dw->tableEnd();
    dxf->writeDimStyle(*dw,
                      1,
                      1,
                      1,
                      1,
                      1);
    dxf->writeBlockRecord(*dw);
    dxf->writeBlockRecord(*dw, "myblock1");
    dxf->writeBlockRecord(*dw, "myblock2");
    dw->tableEnd();
    dw->sectionEnd();
    dw->sectionBlocks();

    dxf->writeBlock(*dw,
                   DL_BlockData("*Model_Space", 0, 0.0, 0.0, 0.0));
    dxf->writeEndBlock(*dw, "*Model_Space");

    dxf->writeBlock(*dw,
                   DL_BlockData("*Paper_Space", 0, 0.0, 0.0, 0.0));
    dxf->writeEndBlock(*dw, "*Paper_Space");

    dxf->writeBlock(*dw,
                   DL_BlockData("*Paper_Space0", 0, 0.0, 0.0, 0.0));
    dxf->writeEndBlock(*dw, "*Paper_Space0");

    dxf->writeBlock(*dw,
                   DL_BlockData("myblock1", 0, 0.0, 0.0, 0.0));
    // ...
    // write block entities e.g. with dxf->writeLine(), ..
    // ...
    dxf->writeEndBlock(*dw, "myblock1");

    dxf->writeBlock(*dw,
                   DL_BlockData("myblock2", 0, 0.0, 0.0, 0.0));
    // ...
    // write block entities e.g. with dxf->writeLine(), ..
    // ...
    dxf->writeEndBlock(*dw, "myblock2");

    dw->sectionEnd();
    dw->sectionEntities();

    // write all your entities..
 //   dxf->writePoint(
 //       *dw,
 //       DL_PointData(10.0,
  //                   45.0,
 //                    0.0),
 //       DL_Attributes("mainlayer", 256, -1, "BYLAYER"));
	
//	dxf->writeSpline(*dw,DL_SplineData(3,8,4,8),DL_Attributes("mainlayer", 256, -1, "BYLAYER"));
	//绘制中心线
	dxf->writeLine(
        *dw,
        DL_LineData(100,   // start point
                    50,
                    0.0,
                    100,   // end point
                    250,
                    0.0),
        DL_Attributes("CenterLine", 256, -1, "BYLAYER"));
	dxf->writeLine(
        *dw,
        DL_LineData(25,   // start point
                    170,
                    0.0,
                    175,   // end point
                    170,
                    0.0),
        DL_Attributes("CenterLine", 256, -1, "BYLAYER"));
	dxf->writeLine(
        *dw,
        DL_LineData(225,   // start point
                    170,
                    0.0,
                    285,   // end point
                    170,
                    0.0),
        DL_Attributes("CenterLine", 256, -1, "BYLAYER"));


	Save3BSpline(dxf,dw);
	DrawA4(dxf,dw);
	DrawDxfInfo(dxf,dw);
	

    dw->sectionEnd();
    dxf->writeObjects(*dw);
    dxf->writeObjectsEnd(*dw);
    dw->dxfEOF();
    dw->close();
    delete dw;
    delete dxf;
}

void CCamCADSystemView::OnFileSaveAs()
{
	// TODO: ÔÚ´ËÌí¼ÓÃüÁî´¦Àí³ÌÐò´úÂë
}

void CCamCADSystemView::Save3BSpline(DL_Dxf* dxf,DL_WriterA* dw)
{
	double x1,y1,x,y,max,min;
	x1 = ( cam.mpx2[0] + 4.0 * cam.mpx2[1] + cam.mpx2[2] ) / 6.0;
	y1 = ( cam.mpy2[0] + 4.0 * cam.mpy2[1] + cam.mpy2[2] ) / 6.0;
	min=max=y1;
	for ( int i = 0; i < COUNT; i++ )
	{
		// 找到最大和最小的坐标，求得最大的范围，从而确定u的最小步长
		double xMax, xMin, yMax, yMin;
		xMax = xMin = cam.mpx2[i];
		yMax = yMin = cam.mpy2[i];
		if ( xMax < cam.mpx2[i+1] ) xMax = cam.mpx2[i+1];
		if ( xMin > cam.mpx2[i+1] ) xMin = cam.mpx2[i+1];
		if ( xMax < cam.mpx2[i+2] ) xMax = cam.mpx2[i+2];
		if ( xMin > cam.mpx2[i+2] ) xMin = cam.mpx2[i+2];
		if ( xMax < cam.mpx2[i+3] ) xMax = cam.mpx2[i+3];
		if ( xMin > cam.mpx2[i+3] ) xMin = cam.mpx2[i+3];

		if ( yMax < cam.mpy2[i+1] ) yMax = cam.mpy2[i+1];
		if ( yMin > cam.mpy2[i+1] ) yMin = cam.mpy2[i+1];
		if ( yMax < cam.mpy2[i+2] ) yMax = cam.mpy2[i+2];
		if ( yMin > cam.mpy2[i+2] ) yMin = cam.mpy2[i+2];
		if ( yMax < cam.mpy2[i+3] ) yMax = cam.mpy2[i+3];
		if ( yMin > cam.mpy2[i+3] ) yMin = cam.mpy2[i+3];
		double fMax = (double)(xMax - xMin);
		if ( fMax < (double)(yMax - yMin) ) fMax = (double)(yMax - yMin);
		double	u, uStep = 2.0 / fMax ;		// u值步长
		
		
		
		for ( u = uStep; u<=1.0; u+= uStep )
		{
			
			x = (1.0-u)*(1.0-u)*(1.0-u)
					* (double)(cam.mpx2[i])
				+ ( 3.0*u*u*u - 6.0*u*u + 4.0 )
					* (double)(cam.mpx2[i+1])
				+ ( -3.0*u*u*u + 3.0*u*u + 3.0*u + 1.0 )
					* (double)(cam.mpx2[i+2])
				+ u*u*u
					* (double)(cam.mpx2[i+3]);
			y = (1.0-u)*(1.0-u)*(1.0-u)
					* (double)(cam.mpy2[i])
				+ ( 3.0*u*u*u - 6.0*u*u + 4.0 )
					* (double)(cam.mpy2[i+1])
				+ ( -3.0*u*u*u + 3.0*u*u + 3.0*u + 1.0 )
					* (double)(cam.mpy2[i+2])
				+ u*u*u
					* (double)(cam.mpy2[i+3]);
			x = x / 6.0;	y = y / 6.0;
			if(y>max) max=y;
			if(y<min) min=y;
			dxf->writeLine(
        *dw,
        DL_LineData(x1+100,   // start point
                    y1+170,
                    0.0,
                    x+100,   // end point
                    y+170,
                    0.0),
        DL_Attributes("BoldLine", 256, -1, "BYLAYER"));
			x1=x;y1=y;

			
		}
	}
	dxf->writeLine(
        *dw,
        DL_LineData(240,   // start point
                    min+170,
                    0.0,
                    240,   // end point
                    max+170,
                    0.0),
        DL_Attributes("BoldLine", 256, -1, "BYLAYER"));

	dxf->writeLine(
        *dw,
        DL_LineData(270,   // start point
                    min+170,
                    0.0,
                    270,   // end point
                    max+170,
                    0.0),
        DL_Attributes("BoldLine", 256, -1, "BYLAYER"));

	dxf->writeLine(
        *dw,
        DL_LineData(240,   // start point
                    min+170,
                    0.0,
                    270,   // end point
                    min+170,
                    0.0),
        DL_Attributes("BoldLine", 256, -1, "BYLAYER"));

	dxf->writeLine(
        *dw,
        DL_LineData(240,   // start point
                    max+170,
                    0.0,
                    270,   // end point
                    max+170,
                    0.0),
        DL_Attributes("BoldLine", 256, -1, "BYLAYER"));

	
	

}

//void CCamCADSystemView::OnTimer(UINT_PTR nIDEvent)
//{
//	// TODO: ÔÚ´ËÌí¼ÓÏûÏ¢´¦Àí³ÌÐò´úÂëºÍ/»òµ÷ÓÃÄ¬ÈÏÖµ
//
//	CView::OnTimer(nIDEvent);
//}

//void CCamCADSystemView::OnTimer(UINT_PTR nIDEvent)
//{
//	// TODO: ÔÚ´ËÌí¼ÓÏûÏ¢´¦Àí³ÌÐò´úÂëºÍ/»òµ÷ÓÃÄ¬ÈÏÖµ
//
//	CView::OnTimer(nIDEvent);
//}

void CCamCADSystemView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ÔÚ´ËÌí¼ÓÏûÏ¢´¦Àí³ÌÐò´úÂëºÍ/»òµ÷ÓÃÄ¬ÈÏÖµ

	CView::OnTimer(nIDEvent);
}

// 绘制A4图纸
void CCamCADSystemView::DrawA4(DL_Dxf* dxf, DL_WriterA* dw)
{
	//外框
	DrawDxfLine(dxf,dw,"mainlayer",CPoint(0,0),CPoint(0,297));
	DrawDxfLine(dxf,dw,"mainlayer",CPoint(0,297),CPoint(420,297));
	DrawDxfLine(dxf,dw,"mainlayer",CPoint(420,297),CPoint(420,0));
	DrawDxfLine(dxf,dw,"mainlayer",CPoint(420,0),CPoint(0,0));
	//内框
	DrawDxfLine(dxf,dw,"BoldLine",CPoint(10,10),CPoint(10,287));
	DrawDxfLine(dxf,dw,"BoldLine",CPoint(10,287),CPoint(410,287));
	DrawDxfLine(dxf,dw,"BoldLine",CPoint(410,287),CPoint(410,10));
	DrawDxfLine(dxf,dw,"BoldLine",CPoint(410,10),CPoint(10,10));

	//标题栏
	//外框
	int x=410,y=10;
	DrawDxfLine(dxf,dw,"BoldLine",CPoint(x-140,y),CPoint(x-140,y+40));
	DrawDxfLine(dxf,dw,"BoldLine",CPoint(x-140,y+40),CPoint(x,y+40));
	//内部
	//横线
	DrawDxfLine(dxf,dw,"mainlayer",CPoint(x-65,y+32),CPoint(x,y+32));
	DrawDxfLine(dxf,dw,"mainlayer",CPoint(x-140,y+24),CPoint(x-35,y+24));
	DrawDxfLine(dxf,dw,"mainlayer",CPoint(x-140,y+16),CPoint(x,y+16));
	DrawDxfLine(dxf,dw,"mainlayer",CPoint(x-140,y+8),CPoint(x-65,y+8));
	//竖线
	DrawDxfLine(dxf,dw,"mainlayer",CPoint(x-23,y+40),CPoint(x-23,y+32));
	DrawDxfLine(dxf,dw,"mainlayer",CPoint(x-35,y+40),CPoint(x-35,y+16));
	DrawDxfLine(dxf,dw,"mainlayer",CPoint(x-53,y+40),CPoint(x-53,y+16));
	DrawDxfLine(dxf,dw,"mainlayer",CPoint(x-65,y),CPoint(x-65,y+40));
	DrawDxfLine(dxf,dw,"mainlayer",CPoint(x-100,y),CPoint(x-100,y+24));
	DrawDxfLine(dxf,dw,"mainlayer",CPoint(x-128,y),CPoint(x-128,y+24));
	//绘制文字
	int h=8;
	DrawDxfText(dxf,dw,"凸 轮 机 构",CRect(CPoint(x-140,y+40),CPoint(x-65,y+24)),h);
	DrawDxfText(dxf,dw,"制 图",CRect(CPoint(x-140,y+24),CPoint(x-128,y+16)),h/2);
	DrawDxfText(dxf,dw,"描 图",CRect(CPoint(x-140,y+16),CPoint(x-128,y+8)),h/2);
	DrawDxfText(dxf,dw,"审 核",CRect(CPoint(x-140,y+8),CPoint(x-128,y)),h/2);
	DrawDxfText(dxf,dw,"比 例",CRect(CPoint(x-65,y+40),CPoint(x-53,y+32)),h/2);
	DrawDxfText(dxf,dw,"件 数",CRect(CPoint(x-65,y+32),CPoint(x-53,y+24)),h/2);
	DrawDxfText(dxf,dw,"重 量",CRect(CPoint(x-65,y+24),CPoint(x-53,y+16)),h/2);
	DrawDxfText(dxf,dw,"材 料",CRect(CPoint(x-35,y+40),CPoint(x-23,y+32)),h/2);
	DrawDxfText(dxf,dw,"能源与机械工程学院",CRect(CPoint(x-65,y+16),CPoint(x,y)),h/2);




	
}

// 绘制直线
void CCamCADSystemView::DrawDxfLine(DL_Dxf* dxf, DL_WriterA* dw, char* LayerName, CPoint pstart, CPoint pend,int width)
{
	dxf->writeLine(
        *dw,
        DL_LineData(pstart.x,   // start point
                    pstart.y,
                    0.0,
                    pend.x,   // end point
					pend.y,
                    0.0),
        DL_Attributes(LayerName, 256, width, "BYLAYER"));
}

// 绘制文字
void CCamCADSystemView::DrawDxfText(DL_Dxf* dxf, DL_WriterA* dw, char* str, CRect r1, int h,int mode,int width)
{
	int x,y;
	if(mode==5)
	{
		x=r1.CenterPoint().x;
		y=r1.CenterPoint().y;
	}
	else if(mode==1)
	{
		x=r1.TopLeft().x;
		y=r1.TopLeft().y;

	}
	else if(mode==2)
	{
		x=r1.left+r1.Width()/2;
		y=r1.top;

	}
	dxf->writeMText(*dw,
		DL_MTextData(x,
					y,
					0,
					h,
					width,
					mode,
					1,
					1,
					1,
					str,
					"STANDARD",
						0), 
		DL_Attributes("mainlayer", 256, -1, "BYLAYER"));

}

// 绘制图纸信息
void CCamCADSystemView::DrawDxfInfo(DL_Dxf* dxf, DL_WriterA* dw)
{
	//标题栏
	int x=410,y=10,h=8;
	DrawDxfText(dxf,dw,"夏宇彬",CRect(CPoint(x-128,y+24),CPoint(x-100,y+16)),h/2);
	DrawDxfText(dxf,dw,"夏宇彬",CRect(CPoint(x-128,y+16),CPoint(x-100,y+8)),h/2);
	DrawDxfText(dxf,dw,"夏宇彬",CRect(CPoint(x-128,y+8),CPoint(x-100,y)),h/2);
	DrawDxfText(dxf,dw,"1:1",CRect(CPoint(x-53,y+40),CPoint(x-35,y+32)),h/2);
	DrawDxfText(dxf,dw,"1",CRect(CPoint(x-53,y+32),CPoint(x-35,y+24)),h/2);
//	DrawDxfText(dxf,dw,"未知",CRect(CPoint(x-53,y+24),CPoint(x-35,y+16)),h/2);
	DrawDxfText(dxf,dw,"45",CRect(CPoint(x-23,y+40),CPoint(x,y+32)),h/2);

	//技术要求
	h=6;
	DrawDxfText(dxf,dw,"技术要求",CRect(CPoint(x-240,y+60),CPoint(x-145,y+60-h)),h,2);
	DrawDxfText(dxf,dw,"1,曲线部分以及曲线圆弧部分休整光滑",CRect(CPoint(x-240,y+60-(int)(1.5*h)),CPoint(x-145,y+60-(int)(3.5*h))),h,1,90);
	DrawDxfText(dxf,dw,"2,凸轮轮廓热处理为表面淬火HRC40-45",CRect(CPoint(x-240,y+60-5*h),CPoint(x-145,y+60-7*h)),h,1,90);

	//轮毂与键槽
	dxf->writeArc(*dw,
		DL_ArcData(100,170,0,6,90+asin(1.0/3.0)*180/Pi,360),
		DL_Attributes("BoldLine", 256, -1, "BYLAYER"));
	dxf->writeArc(*dw,
		DL_ArcData(100,170,0,6,0,acos(1.0/3.0)*180/Pi),
		DL_Attributes("BoldLine", 256, -1, "BYLAYER"));
	dxf->writeLine(
        *dw,
        DL_LineData(102,   // start point
                    170+4*sqrt(2.0),
                    0.0,
                    102,   // end point
                    178.8,
                    0.0),
        DL_Attributes("BoldLine", 256, -1, "BYLAYER"));
	dxf->writeLine(
        *dw,
        DL_LineData(98,   // start point
                    170+4*sqrt(2.0),
                    0.0,
                    98,   // end point
                    178.8,
                    0.0),
        DL_Attributes("BoldLine", 256, -1, "BYLAYER"));
	dxf->writeLine(
        *dw,
        DL_LineData(98,   // start point
                    178.8,
                    0.0,
                    102,   // end point
                    178.8,
                    0.0),
        DL_Attributes("BoldLine", 256, -1, "BYLAYER"));
	//左视图
	dxf->writeLine(
        *dw,
        DL_LineData(240,   // start point
                    164,
                    0.0,
                    270,   // end point
                    164,
                    0.0),
        DL_Attributes("DashLine", 256, -1, "BYLAYER"));
	dxf->writeLine(
        *dw,
        DL_LineData(240,   // start point
                    175.6,
                    0.0,
                    270,   // end point
                    175.6,
                    0.0),
        DL_Attributes("DashLine", 256, -1, "BYLAYER"));
	dxf->writeLine(
        *dw,
        DL_LineData(240,   // start point
                    177.8,
                    0.0,
                    270,   // end point
                    177.8,
                    0.0),
        DL_Attributes("DashLine", 256, -1, "BYLAYER"));
	
//	dxf->writeCircle(*dw,
//		DL_CircleData(100,170,0,6),
//		DL_Attributes("BoldLine", 256, -1, "BYLAYER"));
	//尺寸
	dxf->writeDimLinear(*dw,
		DL_DimensionData(x-300,
				y+60,
				0,
				x-270,
				y+60,
				0,
				0,
				5,
				1,
				1,
				"<>",
				"STANDARD",
				0),
				DL_DimLinearData(x-300,y+80,0,x-240,x+80,0,0,0),
				DL_Attributes("mainlayer", 256, -1, "BYLAYER"));
						
}
