#include "StdAfx.h"
#include "GCode.h"

CGCode::CGCode(double m_dDelta0, double m_dDelta01,
			   double m_dDelta1, double m_dDelta02,
			   double mpx[COUNT+3], double mpy[COUNT+3],
			   double m_dr0, double m_dh,
			   CString m_sKnifeID, CString m_sKnifeFace,
			   double m_dFeed, int m_nSpeed
			   )
{

	delta0=m_dDelta0;
	delta01=m_dDelta01;
	delta1=m_dDelta1;
	delta02=m_dDelta02;
	mpx2=mpx;
	mpy2=mpy;
	r0=m_dr0;
	h=m_dh;
	KnifeID=m_sKnifeID;
	KnifeFace=m_sKnifeFace;
	feed=m_dFeed;
	speed=m_nSpeed;

	CalGCode();

}

CGCode::~CGCode(void)
{
	
	delete[] pstartx;
	delete[] pstarty;
	delete[] pendx;
	delete[] pendy;
	delete[] tx;
	delete[] ty;
	delete[] r1;
	delete[] r2;
	delete[] o1x;
	delete[] o1y;
	delete[] o2x;
	delete[] o2y;
	delete[] RiseCode;
	
	delete[] pstartx2;
	delete[] pstarty2;
	delete[] pendx2;
	delete[] pendy2;
	delete[] tx2;
	delete[] ty2;
	delete[] r12;
	delete[] r22;
	delete[] o1x2;
	delete[] o1y2;
	delete[] o2x2;
	delete[] o2y2;
	delete[] ReturnCode;
	
	delete[] GCode;
}

// 计算推程运动段G代码
void CGCode::CalRiseGCode(void)
{
	pstartx=new double[(int)delta0];
	pstarty=new double[(int)delta0];
	pendx=new double[(int)delta0];
	pendy=new double[(int)delta0];
	tx=new double[(int)delta0];
	ty=new double[(int)delta0];
	r1=new double[(int)delta0];
	r2=new double[(int)delta0];
	o1x=new double[(int)delta0];
	o1y=new double[(int)delta0];
	o2x=new double[(int)delta0];
	o2y=new double[(int)delta0];
	RiseCode=new CString[2*(int)delta0];
	

	double *L=new double[(int)delta0];
	double *sinPhi=new double[(int)delta0];
	double *cosPhi=new double[(int)delta0];
	double *k1=new double[(int)delta0];
	double *k2=new double[(int)delta0];
	double *gx=new double[(int)delta0];
	double *gy=new double[(int)delta0];
	double *gu=new double[(int)delta0];
	double *gv=new double[(int)delta0];
	double *pendu=new double[(int)delta0];
	double *pendv=new double[(int)delta0];
	double *alpha1=new double[(int)delta0];
	double *alpha2=new double[(int)delta0];
	double *tu=new double[(int)delta0];
	double *tv=new double[(int)delta0];
	double *o1u=new double[(int)delta0];
	double *o1v=new double[(int)delta0];
	double *o2u=new double[(int)delta0];
	double *o2v=new double[(int)delta0];

	
	

	for (int i=0;i<delta0;i++)
	{
		//每段三次B样条曲线起点和终点坐标
		pstartx[i]=(mpx2[i]+mpx2[i+2])/6.0+2.0/3.0*mpx2[i+1];
		pstarty[i]=(mpy2[i]+mpy2[i+2])/6.0+2.0/3.0*mpy2[i+1];
		pendx[i]=(mpx2[i+1]+mpx2[i+3])/6.0+2.0/3.0*mpx2[i+2];
		pendy[i]=(mpy2[i+1]+mpy2[i+3])/6.0+2.0/3.0*mpy2[i+2];
		//每段样条曲线的弦长
		L[i]=sqrt((pendy[i]-pstarty[i])*(pendy[i]-pstarty[i])+(pendx[i]-pstartx[i])*(pendx[i]-pstartx[i]));
		sinPhi[i]=(pendy[i]-pstarty[i])/L[i];
		cosPhi[i]=(pendx[i]-pstartx[i])/L[i];
		k1[i]=(mpy2[i+2]-mpy2[i])/(mpx2[i+2]-mpx2[i]);
		k2[i]=(mpy2[i+3]-mpy2[i+1])/(mpx2[i+3]-mpx2[i+1]);
		//切线交点
		gx[i]=(k1[i]*pstartx[i]-k2[i]*pendx[i]+pendy[i]-pstarty[i])/(k1[i]-k2[i]);
		gy[i]=(k1[i]*k2[i]*(pstartx[i]-pendx[i])+k1[i]*pendy[i]-k2[i]*pstarty[i])/(k1[i]-k2[i]);
		//局部坐标系中切线交点的坐标
		gu[i]=(gx[i]-pstartx[i])*cosPhi[i]+(gy[i]-pstarty[i])*sinPhi[i];
		gv[i]=(gy[i]-pstarty[i])*cosPhi[i]-(gx[i]-pstartx[i])*sinPhi[i];
		//局部坐标系中P(1)点坐标
		pendu[i]=(pendx[i]-pstartx[i])*cosPhi[i]+(pendy[i]-pstarty[i])*sinPhi[i];
		pendv[i]=(pendy[i]-pstarty[i])*cosPhi[i]-(pendx[i]-pstartx[i])*sinPhi[i];
		//计算双圆弧圆心角
		alpha1[i]=atan(gv[i]/gu[i]);
		alpha2[i]=atan(gv[i]/(pendu[i]-gu[i]));
		//T点局部坐标
		tu[i]=L[i]*sin(alpha2[i]/2)*cos(alpha1[i]/2)/sin((alpha1[i]+alpha2[i])/2);  
		tv[i]=(1-cos(alpha1[i]))/sin(alpha1[i])*tu[i];
		//双圆弧半径
		r1[i]=tu[i]/sin(alpha1[i]);
		r2[i]=(L[i]-tu[i])/sin(alpha2[i]);
		//双圆弧圆心
		o1u[i]=tu[i];
		o1v[i]=-r1[i]+tv[i];
		o2u[i]=tu[i];
		o2v[i]=-r2[i]+tv[i];
		//局部坐标系转整体坐标系
		tx[i]=tu[i]*cosPhi[i]-tv[i]*sinPhi[i]+pstartx[i];
		ty[i]=tv[i]*sinPhi[i]+tv[i]*cosPhi[i]+pstarty[i];
		o1x[i]=o1u[i]*cosPhi[i]-o1v[i]*sinPhi[i]+pstartx[i];
		o1y[i]=o1v[i]*sinPhi[i]+o1v[i]*cosPhi[i]+pstarty[i];
		o2x[i]=o2u[i]*cosPhi[i]-o2v[i]*sinPhi[i]+pstartx[i];
		o2y[i]=o2v[i]*sinPhi[i]+o2v[i]*cosPhi[i]+pstarty[i];
	}

	RiseCode[0].Format(_T("N%04d G2 X %.2f Z %.2f CR=%.2f"),50,tx[0],ty[0],r1[0]);
	for (int i=1;i<(int)delta0;i++)
	{
		RiseCode[2*i-1].Format(_T("N%04d    X %.2f Z %.2f CR=%.2f"),(2*i+4)*10,pendx[i-1],pendy[i-1],r2[i-1]);
		RiseCode[2*i].Format(_T("N%04d    X %.2f Z %.2f CR=%.2f"),(2*i+5)*10,tx[i],ty[i],r1[i]);
	}
	RiseCode[2*(int)delta0-1].Format(_T("N%04d    X %.2f Z %.2f CR=%.2f"),10*(2*(int)delta0+4),pendx[(int)delta0-1],pendy[(int)delta0-1],r2[(int)delta0-1]);

	delete[] L;
	delete[] sinPhi;
	delete[] cosPhi;
	delete[] k1;
	delete[] k2;
	delete[] gx;
	delete[] gy;
	delete[] gu;
	delete[] gv;
	delete[] pendu;
	delete[] pendv;
	delete[] alpha1;
	delete[] alpha2;
	delete[] tu;
	delete[] tv;
	delete[] o1u;
	delete[] o1v;
	delete[] o2u;
	delete[] o2v;
}

// 计算回程运动段G代码
void CGCode::CalReturnGCode(void)
{
	pstartx2=new double[(int)delta1];
	pstarty2=new double[(int)delta1];
	pendx2=new double[(int)delta1];
	pendy2=new double[(int)delta1];
	tx2=new double[(int)delta1];
	ty2=new double[(int)delta1];
	r12=new double[(int)delta1];
	r22=new double[(int)delta1];
	o1x2=new double[(int)delta1];
	o1y2=new double[(int)delta1];
	o2x2=new double[(int)delta1];
	o2y2=new double[(int)delta1];
	ReturnCode=new CString[2*(int)delta1];


	double *L=new double[(int)delta1];
	double *sinPhi=new double[(int)delta1];
	double *cosPhi=new double[(int)delta1];
	double *k1=new double[(int)delta1];
	double *k2=new double[(int)delta1];
	double *gx=new double[(int)delta1];
	double *gy=new double[(int)delta1];
	double *gu=new double[(int)delta1];
	double *gv=new double[(int)delta1];
	double *pendu=new double[(int)delta1];
	double *pendv=new double[(int)delta1];
	double *alpha1=new double[(int)delta1];
	double *alpha2=new double[(int)delta1];
	double *tu=new double[(int)delta1];
	double *tv=new double[(int)delta1];
	double *o1u=new double[(int)delta1];
	double *o1v=new double[(int)delta1];
	double *o2u=new double[(int)delta1];
	double *o2v=new double[(int)delta1];
	for (int i=0;i<delta1;i++)
	{
		//每段三次B样条曲线起点和终点坐标
		pstartx2[i]=(mpx2[i+(int)(delta0+delta01)]+mpx2[i+2+(int)(delta0+delta01)])/6.0+2.0/3.0*mpx2[i+1+(int)(delta0+delta01)];
		pstarty2[i]=(mpy2[i+(int)(delta0+delta01)]+mpy2[i+2+(int)(delta0+delta01)])/6.0+2.0/3.0*mpy2[i+1+(int)(delta0+delta01)];
		pendx2[i]=(mpx2[i+1+(int)(delta0+delta01)]+mpx2[i+3+(int)(delta0+delta01)])/6.0+2.0/3.0*mpx2[i+2+(int)(delta0+delta01)];
		pendy2[i]=(mpy2[i+1+(int)(delta0+delta01)]+mpy2[i+3+(int)(delta0+delta01)])/6.0+2.0/3.0*mpy2[i+2+(int)(delta0+delta01)];
		//每段样条曲线的弦长
		L[i]=sqrt((pendy2[i]-pstarty2[i])*(pendy2[i]-pstarty2[i])+(pendx2[i]-pstartx2[i])*(pendx2[i]-pstartx2[i]));
		sinPhi[i]=(pendy2[i]-pstarty2[i])/L[i];
		cosPhi[i]=(pendx2[i]-pstartx2[i])/L[i];
		k1[i]=(mpy2[i+2+(int)(delta0+delta01)]-mpy2[i+(int)(delta0+delta01)])/(mpx2[i+2+(int)(delta0+delta01)]-mpx2[i+(int)(delta0+delta01)]);
		k2[i]=(mpy2[i+3+(int)(delta0+delta01)]-mpy2[i+1+(int)(delta0+delta01)])/(mpx2[i+3+(int)(delta0+delta01)]-mpx2[i+1+(int)(delta0+delta01)]);
		//切线交点
		gx[i]=(k1[i]*pstartx2[i]-k2[i]*pendx2[i]+pendy2[i]-pstarty2[i])/(k1[i]-k2[i]);
		gy[i]=(k1[i]*k2[i]*(pstartx2[i]-pendx2[i])+k1[i]*pendy2[i]-k2[i]*pstarty2[i])/(k1[i]-k2[i]);
		//局部坐标系中切线交点的坐标
		gu[i]=(gx[i]-pstartx2[i])*cosPhi[i]+(gy[i]-pstarty2[i])*sinPhi[i];
		gv[i]=(gy[i]-pstarty2[i])*cosPhi[i]-(gx[i]-pstartx2[i])*sinPhi[i];
		//局部坐标系中P(1)点坐标
		pendu[i]=(pendx2[i]-pstartx2[i])*cosPhi[i]+(pendy2[i]-pstarty2[i])*sinPhi[i];
		pendv[i]=(pendy2[i]-pstarty2[i])*cosPhi[i]-(pendx2[i]-pstartx2[i])*sinPhi[i];
		//计算双圆弧圆心角
		alpha1[i]=atan(gv[i]/gu[i]);
		alpha2[i]=atan(gv[i]/(pendu[i]-gu[i]));
		//T点局部坐标
		tu[i]=L[i]/2;
		tv[i]=L[i]*tan(alpha1[i]/2)/2;
		//双圆弧半径
		r12[i]=L[i]/(2*sin(alpha1[i]));
		r22[i]=L[i]/(2*sin(alpha2[i]));
		//双圆弧圆心
		o1u[i]=tu[i];
		o1v[i]=-r12[i]+tv[i];
		o2u[i]=tu[i];
		o2v[i]=-r22[i]+tv[i];
		//局部坐标系转整体坐标系
		tx2[i]=tu[i]*cosPhi[i]-tv[i]*sinPhi[i]+pstartx2[i];
		ty2[i]=tv[i]*sinPhi[i]+tv[i]*cosPhi[i]+pstarty2[i];
		o1x2[i]=o1u[i]*cosPhi[i]-o1v[i]*sinPhi[i]+pstartx2[i];
		o1y2[i]=o1v[i]*sinPhi[i]+o1v[i]*cosPhi[i]+pstarty2[i];
		o2x2[i]=o2u[i]*cosPhi[i]-o2v[i]*sinPhi[i]+pstartx2[i];
		o2y2[i]=o2v[i]*sinPhi[i]+o2v[i]*cosPhi[i]+pstarty2[i];
	}

	ReturnCode[0].Format(_T("N%04d    X %.2f Z %.2f CR=%.2f"),10*(2*(int)delta0+6),tx2[0],ty2[0],r12[0]);
	for (int i=1;i<(int)delta1;i++)
	{
		ReturnCode[2*i-1].Format(_T("N%04d    X %.2f Z %.2f CR=%.2f"),(2*i-1+2*(int)delta0+6)*10,pendx2[i-1],pendy2[i-1],r22[i-1]);
		ReturnCode[2*i].Format(_T("N%04d    X %.2f Z %.2f CR=%.2f"),(2*i+2*(int)delta0+6)*10,tx2[i],ty2[i],r12[i]);
	}
	ReturnCode[2*(int)delta1-1].Format(_T("N%04d    X %.2f Z %.2f CR=%.2f"),(2*(int)(delta1+delta0)+5)*10,pendx2[(int)delta1-1],pendy2[(int)delta1-1],r22[(int)delta1-1]);

	delete[] L;
	delete[] sinPhi;
	delete[] cosPhi;
	delete[] k1;
	delete[] k2;
	delete[] gx;
	delete[] gy;
	delete[] gu;
	delete[] gv;
	delete[] pendu;
	delete[] pendv;
	delete[] alpha1;
	delete[] alpha2;
	delete[] tu;
	delete[] tv;
	delete[] o1u;
	delete[] o1v;
	delete[] o2u;
	delete[] o2v;
	
}

// 生成G代码
void CGCode::CalGCode(void)
{
	CalRiseGCode();
	CalReturnGCode();
	
	GCode=new CString[2*(int)(delta0+delta1)+10];

	GCode[0].Format(_T("N0010 G54 G90 G18"));
	GCode[1].Format(_T("N0020 ")+KnifeID+_T(" ")+KnifeFace);
	GCode[2].Format(_T("N0030 G0 X100 Z100 M%d F%.1f"),speed,feed);
	GCode[3].Format(_T("N0040 G0 X%.2f Z%.2f"),pstartx[0],pstarty[0]);
	for(int i=0;i<2*delta0;i++)
		GCode[i+4]=RiseCode[i];
	GCode[2*(int)delta0+4].Format(_T("N%04d    X %.2f Z %.2f CR=%.2f"),(2*(int)delta0+5)*10,pstartx2[0],pstarty[0],r0+h);
	for(int i=0;i<2*delta1;i++)
		GCode[i+2*(int)delta0+5]=ReturnCode[i];
	GCode[(int)(2*delta0+2*delta1)+5].Format(_T("N%04d    X %.2f Z %.2f CR=%.2f"),((int)(2*delta0+2*delta1)+6)*10,pstartx[0],pstarty[0],r0);
	GCode[(int)(2*delta0+2*delta1)+6].Format(_T("N%04d    M17"),((int)(2*delta0+2*delta1)+7)*10);

}

// 获得G代码长度
int CGCode::GetGCodeLength(void)
{
	return 2*(int)(delta0+delta1)+10;
}

// 获得G代码
void CGCode::GetGCode(CString* m_sGCode)
{
	for (int i=0;i<2*(int)(delta0+delta1)+10;i++)
		m_sGCode[i]=GCode[i];
}
