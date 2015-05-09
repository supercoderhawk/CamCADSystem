#pragma once

class CData
{
public:
	CData(double m_dt[8],double m_dDelta,double m_dh);
	~CData(void);
	double t[8];
	double f[7];
	double a[2];
	double b[7];
	double c[7];
	double delta;
	double h;
	double *s;
	double *v;
	// 计算参数
	void CalcParameter(void);
	// 计算行程与速度
	void CalcShifting(void);
};
