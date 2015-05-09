#pragma once
#include "afxwin.h"


// CDialogBasicPara 对话框

class CDialogBasicPara : public CDialog
{
	DECLARE_DYNAMIC(CDialogBasicPara)

public:
	CDialogBasicPara(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogBasicPara();

// 对话框数据
	enum { IDD = IDD_DIALOG_ENTPARA };
	CArray<double> t;
	CArray<double> hh;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	public:
	double Delta0;
	double Delta01;
	// 回程运动角
	double Delta1;
	// 近休止角
	double Delta02;
	// 工作行程
	double h;
	// 偏心距
	double e;
	// 基圆半径
	double r0;
	// 滚子半径
	double rr0;
	afx_msg void OnBnClickedOk();
	CComboBox m_cMotion;
	CComboBox m_cFollower;
	int nIndexMotion;	//选中的运动规律索引
	int nIndexMotionReturn;	//选中的运动规律索引
	int nIndexFollower;	//选中的从动件索引
	int nCheckStyle;
	afx_msg void OnCbnSelchangeComboMotion();
	afx_msg void OnCbnSelchangeComboFollower();
	virtual BOOL OnInitDialog();
	// 滚子半径
	CEdit m_cRr0;
	CStatic m_cR;
	CButton m_bRiseReturn;
	CComboBox m_cReturn;
	CStatic m_sReturn;
	afx_msg void OnCbnSelchangeReturn();
	afx_msg void OnBnClickedCheck1();
};
