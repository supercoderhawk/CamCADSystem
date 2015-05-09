#pragma once

class CGCode
{
public:
	CGCode(double m_dDelta0, double m_dDelta01,
			   double m_dDelta1, double m_dDelta02,
			   double mpx[COUNT+3], double mpy[COUNT+3],
			   double m_dr0, double m_dh,
			   CString m_sKnifeID, CString m_sKnifeFace,
			   double m_dFeed, int m_nSpeed
			   );
	~CGCode(void);
private:
	//凸轮参数
	double delta0;
	double delta01;
	double delta1;
	double delta02;
	double r0;
	double h;
	double *mpx2;
	double *mpy2;
	//数控加工参数
	CString KnifeID;
	CString KnifeFace;
	double feed;
	int speed;
	
	//双圆弧插补参数
	double *pstartx;
	double *pstarty;
	double *pendx;
	double *pendy;
	double *pstartx2;
	double *pstarty2;
	double *pendx2;
	double *pendy2;

	double *tx;
	double *ty;

	double *tx2;
	double *ty2;
	
	double *r1;
	double *r2;

	double *r12;
	double *r22;

	double *o1x;
	double *o1y;

	double *o1x2;
	double *o1y2;

	double *o2x;
	double *o2y;

	double *o2x2;
	double *o2y2;
	
	CString *RiseCode;
	CString *ReturnCode;
	CString *GCode;

public:
	// 计算推程运动段G代码
	void CalRiseGCode(void);
	// 计算回程运动段G代码
	void CalReturnGCode(void);
	// 生成G代码
	void CalGCode(void);
	// 获得G代码长度
	int GetGCodeLength(void);
	// 获得G代码
	void GetGCode(CString* m_sGCode);
};
