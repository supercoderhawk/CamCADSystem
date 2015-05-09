#pragma once


// CDialogMotionLaw 对话框

class CDialogMotionLaw : public CDialog
{
	DECLARE_DYNAMIC(CDialogMotionLaw)

public:
	CDialogMotionLaw(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogMotionLaw();

// 对话框数据
	enum { IDD = IDD_DIALOG_MOTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double t0;
	double t1;
	double t2;
	double t3;
	double t4;
	double t5;
	double t6;
	double t7;
};
