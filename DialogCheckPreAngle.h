#pragma once


// CDialogCheckPreAngle 对话框

class CDialogCheckPreAngle : public CDialog
{
	DECLARE_DYNAMIC(CDialogCheckPreAngle)

public:
	CDialogCheckPreAngle(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogCheckPreAngle();

// 对话框数据
	enum { IDD = IDD_DIALOG_CHECK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 许用压力角
	double AllowAlpha;
	// 输出结果
	CString CheckResult;
	afx_msg void OnBnClickedButtonCheck();
};
