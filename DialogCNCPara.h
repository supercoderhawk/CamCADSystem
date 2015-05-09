#pragma once


// CDialogCNCPara 对话框

class CDialogCNCPara : public CDialog
{
	DECLARE_DYNAMIC(CDialogCNCPara)

public:
	CDialogCNCPara(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogCNCPara();

// 对话框数据
	enum { IDD = IDD_DIALOG_CNCPARA };
private:
	CString KnifeID;
	CString KnifeFace;
	double Feed;
	int Speed;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedEnterCNCPara();
};
