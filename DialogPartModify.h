#include "PartModifyView.h"
#pragma once


// CDialogPartModify 对话框

class CDialogPartModify : public CDialog
{
	DECLARE_DYNAMIC(CDialogPartModify)

public:
	CDialogPartModify(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogPartModify();

private:
	CPartModifyView *m_pPtModView;
	double fold;
// 对话框数据
	enum { IDD = IDD_DIALOG_MODIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPartModify();
	afx_msg void OnBnClickedSweep();
	virtual BOOL OnInitDialog();
};
