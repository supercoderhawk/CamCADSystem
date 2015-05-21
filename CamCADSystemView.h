// CamCADSystemView.h : CCamCADSystemView 类的接口
//
#include"Cam.h"

#pragma once


class CCamCADSystemView : public CView
{
protected: // 仅从序列化创建
	CCamCADSystemView();
	DECLARE_DYNCREATE(CCamCADSystemView)

// 属性
public:
	CCamCADSystemDoc* GetDocument() const;

// 操作
public:
	CCam cam;	//凸轮类

private:
	bool m_bIsEnterModify;
	bool m_bIsDrawCam;
	bool m_bIsDrawMotion;
//	bool m_bIsDraw
	CArray<double> t;
	CArray<double> h;
	CToolTipCtrl m_tt;
	
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CCamCADSystemView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnterBasicPara();
	afx_msg void OnDrawCam();
	afx_msg void OnPartModify();
	afx_msg void OnCreateGCode();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	bool IsPointSelected(CPoint point);
	afx_msg void OnDrawMotionLaw();


	afx_msg void OnCheckPreAngle();
	// 计算运动规律的T值
	void CalMotionLaw(int nIndexMotion,
					int nIndexMotionReturn,
					int CheckStyle);
	// 绘制运动规律的位移
	void DrawMotionLaw(CDC* pDC);
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	void Save3BSpline(DL_Dxf* dxf,DL_WriterA* dw);
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// 绘制A4图纸
	void DrawA4(DL_Dxf* dxf, DL_WriterA* dw);
	// 绘制直线
	void DrawDxfLine(DL_Dxf* dxf, DL_WriterA* dw, char* LayerName, CPoint pstart, CPoint pend,int width=-1);
	// 绘制文字
	void DrawDxfText(DL_Dxf* dxf, DL_WriterA* dw, char* str, CRect r1, int h,int mode=5,int width=200);
	// 绘制图纸信息
	void DrawDxfInfo(DL_Dxf* dxf, DL_WriterA* dw);
};

#ifndef _DEBUG  // CamCADSystemView.cpp 中的调试版本
inline CCamCADSystemDoc* CCamCADSystemView::GetDocument() const
   { return reinterpret_cast<CCamCADSystemDoc*>(m_pDocument); }
#endif

