#pragma once
//#include "MainFrm.h"
//#include "CamCADSystemDoc.h"
//#include "CamCADSystemView.h"
#include "GCode.h"
class CCam
{
public:
	CCam(void);
	~CCam(void);
public:
	// 推程运动角
	double m_dDelta0;
	// 远休止角
	double m_dDelta01;
	// 回程运动角
	double m_dDelta1;
	// 近休止角
	double m_dDelta02;
	// 工作行程
	double m_dh;
	// 偏心距
	double m_de;
	// 基圆半径
	double m_dr0;
	// 滚子半径
	double m_drr0;
	//推程运动段参数
	CData *t;
	//回程运动段参数
	CData *h;
	//推程段t值
	CData *t2;
	//回程运动段参数
	CData *h2;
	//推程段t值
	double tt[8];
	//回城运动段t值
	double th[8];
	//运动规律
	CString m_sMotion;
//	static CString Motion[3]={"等速","等加速","余弦"}; 
	CString Motion[3];
	double m_dAllowAlpha;
public:
	// 获得推程运动角
	double GetDelta0(void);
	// 获得远休止角
	double GetDelta01(void);
	// 获得回程运动角
	double GetDelta1(void);
	// 获得近休止角
	double GetDelta02(void);
	// 获得偏心距
	double Gete(void);
	// 获得基圆半径
	double Getr0(void);
	// 获得工作行程
	double Geth(void);
	// 获得滚子半径
	double Getrr0(void);
	// 输入参数
	void SetParameter(double Delta0, double Delta1,
		double Delta01, double Delta02,
		double h, double r0,
		double e, double rr0,
		int nIndexMotion, int nIndexMotionReturn,
		CArray<double>* t,CArray<double>* hh,int nIndexFollower);
	// 计算采样点坐标
	void CalcPoint(void);
public:
//当采样点个数为为NUM时
	double px[NUM];		// 采样点横坐标
	double py[NUM];		// 采样点纵坐标

	double mpx[NUM+3];		//控制点横坐标
	double mpy[NUM+3];		//控制点纵坐标
	
	//从动件为滚子时的计算参数
	double sintheta[NUM];
	double costheta[NUM];
	double dsdelta[NUM];
//当采样点个数为COUNT个时
	double px2[COUNT];		// 采样点横坐标
	double py2[COUNT];		// 采样点纵坐标

	double mpx2[COUNT+3];		//控制点横坐标
	double mpy2[COUNT+3];		//控制点纵坐标

	//从动件为滚子时的计算参数
	double sintheta2[COUNT];
	double costheta2[COUNT];
	double dsdelta2[COUNT];
//数控加工参数
	CString m_sKnifeID;	//刀具编号
	CString m_sKnifeFace;	//刀面
	double m_dFeed;	//进给率
	int m_nSpeed;	//主轴转速
	

public:
	// 计算运动规律的位移（对应于采样点个数），
	//以及从动件为滚子时的参数
	void CalcParameter(void);
	// 行程
	double m_ds[NUM];
	double m_ds2[COUNT];
	//压力角
	double m_dPreAngle[COUNT];
	//数控加工程序
	CGCode* gcode;
public:
	// 计算控制点坐标，采样点为NUM个
	void CalcCtrlPoint(void);

	//计算采样点为COUNT个时的各数据，意义与采样点为NUM时相同
	void CalcParameter2(void);
	void CalcPoint2(void);
	void CalcCtrlPoint2(void);
public:
	
	int m_nIndexMotion;	//所选推程运动规律，对应于输入对话框组合框中的顺序
	int m_nIndexMotionReturn;	//所选回程运动规律，数值对应于输入对话框组合框中的顺序
	int m_nIndexFollower;	//从动件类型，数值对应于输入对话框组合框中的顺序

	bool m_bDataRise;	//
	bool m_bDataReturn;
	bool m_bData2Rise;
	bool m_bData2Return;
	bool m_bGCode;	//是否已生成数控加工代码

public:
	// 使用三次B样条曲线绘制凸轮
	void DrawCam3BSpline(CDC* pDC, double * mpx, double * mpy, int nCount,CRect r1);
	// 使用直线绘制凸轮轮廓
	void DrawCamLine(CDC* pDC, double* px, double* py, int nCount, CRect r1, int mode);
	
	
	// 设置数控加工参数
	void SetParameter2(CString KnifeID, CString KnifeFace, double Feed, int Speed);
	// 计算压力角
	void CalcPreAngle(void);
	//显示压力角
	void ShowPresAngle(CDC* pDC,CRect r1);
	// 输入许用压力角
	void SetParameter3(double AllowAlpha);
	//生成数控加工程序
	void CreateGCode(void);
};
