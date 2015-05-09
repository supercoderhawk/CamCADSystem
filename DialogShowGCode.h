#pragma once
#include "ShowGCodeView.h"

// CDialogShowGCode 对话框

class CDialogShowGCode : public CDialog
{
	DECLARE_DYNAMIC(CDialogShowGCode)

public:
	CDialogShowGCode(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogShowGCode();
private:
	CShowGCodeView *m_pSWGView;
// 对话框数据
	enum { IDD = IDD_DIALOG_SWGCODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
