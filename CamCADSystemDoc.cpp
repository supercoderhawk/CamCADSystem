// CamCADSystemDoc.cpp : CCamCADSystemDoc 类的实现
//

#include "stdafx.h"
#include "CamCADSystem.h"

#include "CamCADSystemDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCamCADSystemDoc

IMPLEMENT_DYNCREATE(CCamCADSystemDoc, CDocument)

BEGIN_MESSAGE_MAP(CCamCADSystemDoc, CDocument)
END_MESSAGE_MAP()


// CCamCADSystemDoc 构造/析构

CCamCADSystemDoc::CCamCADSystemDoc()
{
	// TODO: 在此添加一次性构造代码

}

CCamCADSystemDoc::~CCamCADSystemDoc()
{
}

BOOL CCamCADSystemDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CCamCADSystemDoc 序列化

void CCamCADSystemDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CCamCADSystemDoc 诊断

#ifdef _DEBUG
void CCamCADSystemDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCamCADSystemDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCamCADSystemDoc 命令
