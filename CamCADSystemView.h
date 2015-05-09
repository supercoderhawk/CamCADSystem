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
	bool m_bIsDraw;
	CArray<double> t;
	CArray<double> h;
	
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
};

#ifndef _DEBUG  // CamCADSystemView.cpp 中的调试版本
inline CCamCADSystemDoc* CCamCADSystemView::GetDocument() const
   { return reinterpret_cast<CCamCADSystemDoc*>(m_pDocument); }
#endif

