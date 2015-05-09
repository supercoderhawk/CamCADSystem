#pragma once



// CShowGCodeView 视图

class CShowGCodeView : public CScrollView
{
	DECLARE_DYNCREATE(CShowGCodeView)

protected:
	CShowGCodeView();           // 动态创建所使用的受保护的构造函数
	virtual ~CShowGCodeView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
//	virtual void OnInitialUpdate();     // 构造后的第一次

	DECLARE_MESSAGE_MAP()
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
	afx_msg void OnDestroy();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
protected:
	virtual void OnActivateFrame(UINT nState, CFrameWnd* pDeactivateFrame);
	virtual void PostNcDestroy();
};


