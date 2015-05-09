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
private:
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

	double px[NUM];		// 采样点横坐标
	double py[NUM];		// 采样点纵坐标

	double mpx[NUM+3];		//控制点横坐标
	double mpy[NUM+3];		//控制点纵坐标

	
	double px2[COUNT];		// 采样点横坐标
	double py2[COUNT];		// 采样点纵坐标

	double mpx2[COUNT+3];		//控制点横坐标
	double mpy2[COUNT+3];		//控制点纵坐标

	
	
	CString m_sKnifeID;
	CString m_sKnifeFace;
	double m_dFeed;
	int m_nSpeed;
	

public:
	// 计算参数
	void CalcParameter(void);
public:
	// 行程
	double m_ds[NUM];
	double m_ds2[COUNT];
	//压力角
	double m_dPreAngle[COUNT];
	CGCode* gcode;
public:
	// 计算控制点坐标
	void CalcCtrlPoint(void);
private:
	int m_nIndexMotion;
	int m_nIndexMotionReturn;
	int m_nIndexFollower;

	
	bool m_bDataRise;
	bool m_bDataReturn;
	bool m_bData2Rise;
	bool m_bData2Return;
	bool m_bGCode;

public:
	// 使用三次B样条曲线绘制凸轮
	void DrawCam3BSpline(CDC* pDC, double * mpx, double * mpy, int nCount,CRect r1);
	// 使用直线绘制凸轮轮廓
	void DrawCamLine(CDC* pDC, double* px, double* py, int nCount, CRect r1, int mode);
	void CalcParameter2(void);
	void CalcPoint2(void);
	void CalcCtrlPoint2(void);
	
	// 设置数控加工参数
	void SetParameter2(CString KnifeID, CString KnifeFace, double Feed, int Speed);
	// 计算压力角
	void CalcPreAngle(void);
	void ShowPresAngle(CDC* pDC,CRect r1);
	// 输入许用压力角
	void SetParameter3(double AllowAlpha);
	void CreateGCode(void);
};
