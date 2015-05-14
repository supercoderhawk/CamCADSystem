#include "StdAfx.h"
#include "Cam.h"
#include "GCode.h"
#include "math.h"
#pragma comment(lib, "v4501v.lib")

CCam::CCam(void)
: m_dDelta0(0)
, m_dDelta01(0)
, m_dDelta1(0)
, m_dDelta02(0)
, m_dh(0)
, m_de(0)
, m_dr0(0)
, m_drr0(0)

, m_nIndexMotion(0)
, m_nIndexFollower(0)
{
	//输入运动规律
	//推程
//	tt[0]=0;tt[1]=0;tt[2]=0;tt[3]=0.5;
//	tt[4]=0.5;tt[5]=1;tt[6]=1;tt[7]=1;
	//回程
//	th[0]=0;th[1]=0;th[2]=0.5;th[3]=0.5;
//	th[4]=0.5;th[5]=0.5;th[6]=1;th[7]=1;
	
//	Motion[0]="等速";
//	Motion[1]="等加速";
//	Motion[2]="余弦";


	m_bDataRise=false;
	m_bDataReturn=false;
	m_bData2Rise=false;
	m_bData2Return=false;
	m_bGCode=false;

	
}

CCam::~CCam(void)
{
//	if (m_bDataRise)
//	{
//		delete t;
//	if(m_bDataReturn)
//		delete h;
//	}

//	if (m_bData2Rise)
//	{
//		delete t2;
//	}
//	if(m_bData2Return)
//	{
//		delete h2;
//	}
	if(m_bGCode)
		delete gcode;
	
}

// 获得推程运动角
double CCam::GetDelta0(void)
{
	return m_dDelta0;
}

// 获得远休止角
double CCam::GetDelta01(void)
{
	return m_dDelta01;
}

// 获得回程运动角
double CCam::GetDelta1(void)
{
	return m_dDelta1;
}

// 获得近休止角
double CCam::GetDelta02(void)
{
	return m_dDelta02;
}

// 获得偏心距
double CCam::Gete(void)
{
	return m_de;
}

// 获得基圆半径
double CCam::Getr0(void)
{
	return m_dr0;
}

// 获得工作行程
double CCam::Geth(void)
{
	return m_dh;
}

// 获得滚子半径
double CCam::Getrr0(void)
{
	return m_drr0;
}

// 输入参数
void CCam::SetParameter(double Delta0, double Delta1, 
						double Delta01, double Delta02, 
						double h, double r0, 
						double e, double rr0,
						int nIndexMotion, int nIndexMotionReturn,
						CArray<double>* t,CArray<double>* hh,
						int nIndexFollower)
{
	//输入基本参数
	m_dDelta0=Delta0;
	m_dDelta1=Delta1;
	m_dDelta01=Delta01;
	m_dDelta02=Delta02;
	m_dh=h;
	m_de=e;
	m_dr0=r0;
	m_drr0=rr0;
	//输入运动规律的t值
	tt[0]=0;
	th[0]=0;
	for (int i=0;i<7;i++)
		tt[i+1]=t->GetAt(i);
	for (int i=0;i<7;i++)
		th[i+1]=hh->GetAt(i);
	//相关的索引值
	m_nIndexMotion=nIndexMotion;
	m_nIndexMotionReturn=nIndexMotionReturn;
	m_nIndexFollower=nIndexFollower;
	

}

// 计算采样点坐标
void CCam::CalcPoint(void)
{
	//初始化matcom4.5
	initM(MATCOM_VERSION);
	Mm delta;
	Mm ss=zeros(NUM,1);
	Mm sinTheta=zeros(NUM,1);
	Mm cosTheta=zeros(NUM,1);
	Mm DsDelta=zeros(NUM,1);
	for (int i=0;i<NUM;i++)
	{
		ss.r(i+1)=m_ds[i];
		sinTheta.r(i+1)=sintheta[i];
		cosTheta.r(i+1)=costheta[i];
		DsDelta.r(i+1)=dsdelta[i];
	}
	
	
		

	//生成向量
	delta=colon(1*pi/180,2*pi/NUM,2*pi);
	//转置
	delta=ctranspose(delta);
	Mm x;
	Mm y;
	//计算坐标
	if(m_nIndexFollower==0)
	{
		x=times(sqrt(m_dr0*m_dr0-m_de*m_de)+ss,sin(delta))+times(m_de,cos(delta))-times(m_drr0,cosTheta);
		y=times(sqrt(m_dr0*m_dr0-m_de*m_de)+ss,cos(delta))-times(m_de,sin(delta))-times(m_drr0,sinTheta);
	}
	else if(m_nIndexFollower==1)
	{
		x=times(sqrt(m_dr0*m_dr0-m_de*m_de)+ss,sin(delta))+times(m_de,cos(delta));
		y=times(sqrt(m_dr0*m_dr0-m_de*m_de)+ss,cos(delta))-times(m_de,sin(delta));
	}
	else if (m_nIndexFollower==2)
	{
		x=times(m_dr0+ss,sin(delta))+times(DsDelta,cos(delta));
		y=times(m_dr0+ss,cos(delta))-times(DsDelta,sin(delta));
	}
	
	for (int i=0;i<NUM;i++)
	{
		px[i]=x.r(i+1);
		py[i]=y.r(i+1);
	}
	exitM();
}

// 计算运动规律参数
void CCam::CalcParameter(void)
{
	double dxdelta;
	double dydelta;
	double Delta;	
	//远休止位移
	for(int i=(int)m_dDelta0/TIMES;i<(int)(m_dDelta0+m_dDelta01)/TIMES;i++)
	{
		Delta=TIMES*i/360.0*2*Pi;
		m_ds[i]=m_dh;
		dsdelta[i]=0;
		dxdelta=-m_de*sin(Delta)+
				(sqrt(m_dr0*m_dr0-m_de*m_de)+m_dh)*cos(Delta);
		dydelta=-m_de*cos(Delta)-
				(sqrt(m_dr0*m_dr0-m_de*m_de)+m_dh)*sin(Delta);
		sintheta[i]=dxdelta/(sqrt(dxdelta*dxdelta+dydelta*dydelta));
		costheta[i]=-dydelta/(sqrt(dxdelta*dxdelta+dydelta*dydelta));
	}

	//近休止位移
	for(int i=(int)(m_dDelta0+m_dDelta01+m_dDelta1)/TIMES;i<NUM;i++)
	{
		Delta=TIMES*i/360.0*2*Pi;
		m_ds[i]=0;
		dsdelta[i]=0;
		dxdelta=-m_de*sin(Delta)+
				sqrt(m_dr0*m_dr0-m_de*m_de)*cos(Delta);
		dydelta=-m_de*cos(Delta)-
				sqrt(m_dr0*m_dr0-m_de*m_de)*sin(Delta);
		sintheta[i]=dxdelta/(sqrt(dxdelta*dxdelta+dydelta*dydelta));
		costheta[i]=-dydelta/(sqrt(dxdelta*dxdelta+dydelta*dydelta));

	}

	//推程位移
	if(m_nIndexMotion==2)
	{
		for(int i=0;i<(int)m_dDelta0/TIMES;i++)
		{
			Delta=TIMES*i/360.0*2*Pi;
			m_ds[i]=m_dh*i*TIMES/m_dDelta0;
			dsdelta[i]=m_dh/m_dDelta0;
			dxdelta=(m_dh/m_dDelta0-m_de)*sin(Delta)+
				(sqrt(m_dr0*m_dr0-m_de*m_de)+m_ds[i])*cos(Delta);
			dydelta=(m_dh/m_dDelta0-m_de)*cos(Delta)-
				(sqrt(m_dr0*m_dr0-m_de*m_de)+m_ds[i])*sin(Delta);
			sintheta[i]=dxdelta/(sqrt(dxdelta*dxdelta+dydelta*dydelta));
			costheta[i]=-dydelta/(sqrt(dxdelta*dxdelta+dydelta*dydelta));
		}
	}
	else
	{
		t=new CData(tt,m_dDelta0/TIMES,m_dh);
		m_bDataRise=true;
		for(int i=0; i<(int)m_dDelta0/TIMES;i++)
		{
			Delta=TIMES*i/360.0*2*Pi;
			m_ds[i]=t->s[i];
			dsdelta[i]=t->v[i]*m_dh/m_dDelta0;
			dxdelta=(t->v[i]*m_dh/m_dDelta0-m_de)*sin(Delta)+
				(sqrt(m_dr0*m_dr0-m_de*m_de)+m_ds[i])*cos(Delta);
			dydelta=(t->v[i]*m_dh/m_dDelta0-m_de)*cos(Delta)-
				(sqrt(m_dr0*m_dr0-m_de*m_de)+m_ds[i])*sin(Delta);
			sintheta[i]=dxdelta/(sqrt(dxdelta*dxdelta+dydelta*dydelta));
			costheta[i]=-dydelta/(sqrt(dxdelta*dxdelta+dydelta*dydelta));
		}
		delete t;

	}

	//回程位移
	if (m_nIndexMotionReturn==2)
	{
		for(int i=(int)(m_dDelta0+m_dDelta01)/TIMES;
			i<(int)(m_dDelta0+m_dDelta01+m_dDelta1)/TIMES;i++)
		{
			Delta=TIMES*i/360.0*2*Pi;
			m_ds[i]=m_dh*(1-(i-(m_dDelta0+m_dDelta01)/TIMES)/(m_dDelta1/TIMES));
			dsdelta[i]=-m_dh/m_dDelta0;
			dxdelta=(-m_dh/m_dDelta0-m_de)*sin(Delta)+
				(sqrt(m_dr0*m_dr0-m_de*m_de)+m_ds[i])*cos(Delta);
			dydelta=(-m_dh/m_dDelta0-m_de)*cos(Delta)-
				(sqrt(m_dr0*m_dr0-m_de*m_de)+m_ds[i])*sin(Delta);
			sintheta[i]=dxdelta/(sqrt(dxdelta*dxdelta+dydelta*dydelta));
			costheta[i]=-dydelta/(sqrt(dxdelta*dxdelta+dydelta*dydelta));
		}
	}
	else
	{
	
		h=new CData(th,m_dDelta1/TIMES,m_dh);
		m_bDataReturn=true;
		for(int i=(int)(m_dDelta0+m_dDelta01)/TIMES;
			i<(int)(m_dDelta0+m_dDelta01+m_dDelta1)/TIMES;i++)
		{
			Delta=TIMES*i/360.0*2*Pi;
			m_ds[i]=m_dh-h->s[i-(int)(m_dDelta0+m_dDelta01)/TIMES];
			dsdelta[i]=-h->v[i-(int)(m_dDelta0+m_dDelta01)/TIMES]*m_dh/m_dDelta0;
			dxdelta=(-h->v[i-(int)(m_dDelta0+m_dDelta01)/TIMES]*m_dh/m_dDelta0-m_de)*sin(Delta)+
				(sqrt(m_dr0*m_dr0-m_de*m_de)+m_ds[i])*cos(Delta);
			dydelta=(-h->v[i-(int)(m_dDelta0+m_dDelta01)/TIMES]*m_dh/m_dDelta0-m_de)*cos(Delta)-
				(sqrt(m_dr0*m_dr0-m_de*m_de)+m_ds[i])*sin(Delta);
			sintheta[i]=dxdelta/(sqrt(dxdelta*dxdelta+dydelta*dydelta));
			costheta[i]=-dydelta/(sqrt(dxdelta*dxdelta+dydelta*dydelta));
		}
		delete h;
	}

}

// 计算控制点坐标
void CCam::CalcCtrlPoint(void)
{
	initM(MATCOM_VERSION);
	Mm node;
	//采样点，最后一点即为第一个点
	node=zeros(NUM+1,2);
	Mm m_node;
	for (int i=0;i<NUM;i++)
	{
		node.r(i+1,1)=px[i];
		node.r(i+1,2)=py[i];
	}
	node.r(NUM+1,1)=node.r(1,1);
	node.r(NUM+1,2)=node.r(1,2);

	//控制点
	m_node=zeros(NUM+1,2);
	Mm B;
	B=zeros(NUM+1,NUM+3);
	for (int i=1;i<=NUM+1;i++)
	{
		B.r(i,i)=mrdivide(1.0,6.0);
		B.r(i,i+1)=mrdivide(4.0,6.0);
		B.r(i,i+2)=mrdivide(1.0,6.0);

	}

	Mm C;
	C=zeros(NUM+3,NUM+1);
	for (int i2=1;i2<=NUM+1;i2++)
		C.r(i2,i2)=1;

	C.r(NUM+2,1)=-1;
	C.r(NUM+2,2)=1;
	C.r(NUM+2,NUM+1)=1;
	C.r(NUM+3,1)=-1;
	C.r(NUM+3,3)=1;
	C.r(NUM+3,NUM+1)=1;

	Mm D;
	D=zeros(NUM+1);
	D=B*C;
	m_node=mldivide(D,node);
	
	Mm a=zeros(2,2);

	a.r(1,1)=m_node.r(NUM+1,1)-m_node.r(1,1)+m_node.r(2,1);
	a.r(1,2)=m_node.r(NUM+1,2)-m_node.r(1,2)+m_node.r(2,2);

	a.r(2,1)=m_node.r(NUM+1,1)+m_node.r(3,1)-m_node.r(1,1);
	a.r(2,2)=m_node.r(NUM+1,2)+m_node.r(3,2)-m_node.r(1,2);
	m_node=vertcat(m_node,a);

		
	for (int i3=0;i3<NUM+3;i3++)
	{
		mpx[i3]=m_node.r(i3+1,1);
		mpy[i3]=m_node.r(i3+1,2);
	}
	exitM();
}

// 使用三次B样条曲线绘制凸轮
void CCam::DrawCam3BSpline(CDC* pDC, double * mpx,
						   double * mpy, int nCount,CRect r1)
{
	double x = ( mpx[0] + 4.0 * mpx[1] + mpx[2] ) / 6.0+(r1.right-r1.left)/2;
	double y = -( mpy[0] + 4.0 * mpy[1] + mpy[2] ) / 6.0+(r1.bottom-r1.top)/2;
	
	pDC->MoveTo((int)x,(int)y);
	HPEN hPen;
	HPEN hPenOld;
	hPen=CreatePen( PS_SOLID, 0, RGB(0,0,0));
	hPenOld = ( HPEN )SelectObject(pDC->m_hDC,hPen);

		for ( int i = 0; i < nCount; i++ )
	{
		// 找到最大和最小的坐标，求得最大的范围，从而确定u的最小步长
		int xMax, xMin, yMax, yMin;
		xMax = xMin = mpx[i];
		yMax = yMin = mpy[i];
		if ( xMax < mpx[i+1] ) xMax = (int)mpx[i+1];
		if ( xMin > mpx[i+1] ) xMin = (int)mpx[i+1];
		if ( xMax < mpx[i+2] ) xMax = (int)mpx[i+2];
		if ( xMin > mpx[i+2] ) xMin = (int)mpx[i+2];
		if ( xMax < mpx[i+3] ) xMax = (int)mpx[i+3];
		if ( xMin > mpx[i+3] ) xMin = (int)mpx[i+3];

		if ( yMax < mpy[i+1] ) yMax =(int)mpy[i+1];
		if ( yMin > mpy[i+1] ) yMin = (int)mpy[i+1];
		if ( yMax < mpy[i+2] ) yMax = (int)mpy[i+2];
		if ( yMin > mpy[i+2] ) yMin = (int)mpy[i+2];
		if ( yMax < mpy[i+3] ) yMax = (int)mpy[i+3];
		if ( yMin > mpy[i+3] ) yMin = (int)mpy[i+3];
		double fMax = (double)xMax - xMin;
		if ( fMax < (double)(yMax - yMin) ) fMax = yMax - yMin;
		double	u, uStep = 1.0 / fMax ;		// u值步长
				
		for ( u = uStep; u<=1.0; u+= uStep )
		{
			
			x = (1.0-u)*(1.0-u)*(1.0-u)* mpx[i]
				+ ( 3.0*u*u*u - 6.0*u*u + 4.0 )*mpx[i+1]
				+ ( -3.0*u*u*u + 3.0*u*u + 3.0*u + 1.0 )*mpx[i+2]
				+ u*u*u* mpx[i+3];
			y = (1.0-u)*(1.0-u)*(1.0-u)* mpy[i]
				+ ( 3.0*u*u*u - 6.0*u*u + 4.0 )* mpy[i+1]
				+ ( -3.0*u*u*u + 3.0*u*u + 3.0*u + 1.0 )*mpy[i+2]
				+ u*u*u*mpy[i+3];
			x = x / 6.0;	y = y / 6.0;
			if (pDC)
			{
				
				pDC->LineTo((int)(x+r1.CenterPoint().x), (int)(-y+r1.CenterPoint().y));
			}
			
		}
	}

	//如果从动件是滚子，显示滚子图像
	if(m_nIndexFollower==0)
	{
		double times=0.80;
		x=m_de;
		y=sqrt(m_dr0*m_dr0-m_de*m_de);
		pDC->MoveTo((int)(x+r1.CenterPoint().x),
			(int)(-y+r1.CenterPoint().y));
		pDC->LineTo((int)(x+r1.CenterPoint().x),
			(int)(-y+r1.CenterPoint().y-100));
		CPoint pt[2]={CPoint((int)x-(int)(m_drr0*times)+r1.CenterPoint().x,
					-(int)y-(int)(m_drr0*times)+r1.CenterPoint().y),
					CPoint((int)x+(int)(m_drr0*times)+r1.CenterPoint().x,
					-(int)y+(int)(m_drr0*times)+r1.CenterPoint().y)};
		pDC->Ellipse(pt[0].x, pt[0].y, pt[1].x, pt[1].y);
	}
	else if(m_nIndexFollower==1)
	{

	}
	else if(m_nIndexFollower==2)
	{
		pDC->MoveTo(r1.CenterPoint().x-50,r1.CenterPoint().y-(int)m_dr0);
		pDC->LineTo(r1.CenterPoint().x-50,r1.CenterPoint().y-(int)m_dr0);
		pDC->MoveTo(r1.CenterPoint().x,r1.CenterPoint().y-(int)m_dr0);
		pDC->LineTo(r1.CenterPoint().x,r1.CenterPoint().y-(int)m_dr0-50);
	}
//		for(int i=0;i<100;i++)
//		pDC->LineTo((long)(px[0]+(r1.right-r1.left)/2), (long)(-py[0]-i+(r1.bottom-r1.top)/2));
	SelectObject(pDC->m_hDC,hPenOld);
	DeleteObject( hPen );
	


}

// 使用直线绘制凸轮轮廓
void CCam::DrawCamLine(CDC* pDC, double* px, double* py, int nCount, CRect r1, int mode)
{
	pDC->MoveTo((int)px[0]+r1.CenterPoint().x,-(int)py[0]+r1.CenterPoint().y);
	for (int i=1;i<nCount;i++)
		pDC->LineTo((int)px[i]+r1.CenterPoint().x,-(int)py[i]+r1.CenterPoint().y);
	if (mode==1)
		pDC->LineTo((int)px[0]+r1.CenterPoint().x,-(int)py[0]+r1.CenterPoint().y);

}

void CCam::CalcParameter2(void)
{
	double dxdelta;
	double dydelta;
	double Delta;	
	for(int i=(int)m_dDelta0;i<(int)(m_dDelta0+m_dDelta01);i++)
	{
		Delta=i/360.0*2*Pi;
		m_ds2[i]=m_dh;
		dsdelta2[i]=0;
		dxdelta=-m_de*sin(Delta)+
				(sqrt(m_dr0*m_dr0-m_de*m_de)+m_dh)*cos(Delta);
		dydelta=-m_de*cos(Delta)-
				(sqrt(m_dr0*m_dr0-m_de*m_de)+m_dh)*sin(Delta);
		sintheta2[i]=dxdelta/(sqrt(dxdelta*dxdelta+dydelta*dydelta));
		costheta2[i]=-dydelta/(sqrt(dxdelta*dxdelta+dydelta*dydelta));
	}

	for(int i=(int)(m_dDelta0+m_dDelta01+m_dDelta1);i<COUNT;i++)
	{
		Delta=i/360.0*2*Pi;
		m_ds2[i]=0;
		dsdelta2[i]=0;
		dxdelta=-m_de*sin(Delta)+
				sqrt(m_dr0*m_dr0-m_de*m_de)*cos(Delta);
		dydelta=-m_de*cos(Delta)-
				sqrt(m_dr0*m_dr0-m_de*m_de)*sin(Delta);
		sintheta2[i]=dxdelta/(sqrt(dxdelta*dxdelta+dydelta*dydelta));
		costheta2[i]=-dydelta/(sqrt(dxdelta*dxdelta+dydelta*dydelta));
	}

	//计算推程位移
	if(m_nIndexMotion==2)
	{
		//计算行程
		for(int i=0;i<(int)m_dDelta0;i++)
		{
			Delta=i/360.0*2*Pi;
			m_ds2[i]=m_dh*i/m_dDelta0;	
			dsdelta2[i]=m_dh/m_dDelta0;
			dxdelta=(m_dh/m_dDelta0-m_de)*sin(Delta)+
				(sqrt(m_dr0*m_dr0-m_de*m_de)+m_ds2[i])*cos(Delta);
			dydelta=(m_dh/m_dDelta0-m_de)*cos(Delta)-
				(sqrt(m_dr0*m_dr0-m_de*m_de)+m_ds2[i])*sin(Delta);
			sintheta2[i]=dxdelta/(sqrt(dxdelta*dxdelta+dydelta*dydelta));
			costheta2[i]=-dydelta/(sqrt(dxdelta*dxdelta+dydelta*dydelta));
		}
	}
	else
	{
		t2=new CData(tt,m_dDelta0,m_dh);
		m_bData2Rise=true;
		for(int i=0;i<(int)m_dDelta0;i++)
		{
			Delta=i/360.0*2*Pi;
			m_ds2[i]=t2->s[i];
			dsdelta2[i]=t2->v[i]*m_dh/m_dDelta0;
			dxdelta=(t2->v[i]*m_dh/m_dDelta0-m_de)*sin(Delta)+
				(sqrt(m_dr0*m_dr0-m_de*m_de)+m_ds2[i])*cos(Delta);
			dydelta=(t2->v[i]*m_dh/m_dDelta0-m_de)*cos(Delta)-
				(sqrt(m_dr0*m_dr0-m_de*m_de)+m_ds2[i])*sin(Delta);
			sintheta2[i]=dxdelta/(sqrt(dxdelta*dxdelta+dydelta*dydelta));
			costheta2[i]=-dydelta/(sqrt(dxdelta*dxdelta+dydelta*dydelta));

		}
		delete t2;
	}

	//计算回程位移
	if (m_nIndexMotionReturn==2)
	{
		for(int i=(int)(m_dDelta0+m_dDelta01);i<(int)(m_dDelta0+m_dDelta01+m_dDelta1);i++)
		{
			Delta=i/360.0*2*Pi;
			m_ds2[i]=m_dh*(1-(i-(m_dDelta0+m_dDelta01))/(m_dDelta1));
			dsdelta[i]=-m_dh/m_dDelta1;
			dxdelta=(-m_dh/m_dDelta1-m_de)*sin(Delta)+
				(sqrt(m_dr0*m_dr0-m_de*m_de)+m_ds2[i])*cos(Delta);
			dydelta=(-m_dh/m_dDelta1-m_de)*cos(Delta)-
				(sqrt(m_dr0*m_dr0-m_de*m_de)+m_ds2[i])*sin(Delta);
			sintheta2[i]=dxdelta/(sqrt(dxdelta*dxdelta+dydelta*dydelta));
			costheta2[i]=-dydelta/(sqrt(dxdelta*dxdelta+dydelta*dydelta));
		}
	}
	else
	{
		m_bData2Return=true;	
		h2=new CData(th,m_dDelta1,m_dh);
		for(int i=(int)(m_dDelta0+m_dDelta01);i<(int)(m_dDelta0+m_dDelta01+m_dDelta1);i++)
		{
			Delta=i/360.0*2*Pi;
			m_ds2[i]=m_dh-h2->s[i-(int)(m_dDelta0+m_dDelta01)];
			dsdelta2[i]=-h2->v[i-(int)(m_dDelta0+m_dDelta01)]*m_dh/m_dDelta1;
			dxdelta=(-h2->v[i-(int)(m_dDelta0+m_dDelta01)]*m_dh/m_dDelta1-m_de)*sin(Delta)+
				(sqrt(m_dr0*m_dr0-m_de*m_de)+m_ds2[i])*cos(Delta);
			dydelta=(-h2->v[i-(int)(m_dDelta0+m_dDelta01)]*m_dh/m_dDelta1-m_de)*cos(Delta)-
				(sqrt(m_dr0*m_dr0-m_de*m_de)+m_ds2[i])*sin(Delta);
			sintheta2[i]=dxdelta/(sqrt(dxdelta*dxdelta+dydelta*dydelta));
			costheta2[i]=-dydelta/(sqrt(dxdelta*dxdelta+dydelta*dydelta));
		}
		delete h2;

	}
}

void CCam::CalcPoint2(void)
{initM(MATCOM_VERSION);
	Mm delta;
	Mm sinTheta=zeros(COUNT,1);
	Mm cosTheta=zeros(COUNT,1);
	Mm ss=zeros(COUNT,1);
	Mm DsDelta2=zeros(COUNT,1);
	for (int i=0;i<COUNT;i++)
	{
		ss.r(i+1)=m_ds2[i];
		sinTheta.r(i+1)=sintheta2[i];
		cosTheta.r(i+1)=costheta2[i];
		DsDelta2.r(i+1)=dsdelta2[i];
	}
	//生成向量
	delta=colon(1*Pi/180,2*Pi/COUNT,2*Pi);
	//转置
	delta=ctranspose(delta);
	Mm x;
	Mm y;
	//计算坐标
	if(m_nIndexFollower==0)
	{
//		x=times(sqrt(m_dr0*m_dr0-m_de*m_de)+ss,sin(delta))+times(m_de,cos(delta));
//		y=times(sqrt(m_dr0*m_dr0-m_de*m_de)+ss,cos(delta))-times(m_de,sin(delta));

		x=times(sqrt(m_dr0*m_dr0-m_de*m_de)+ss,sin(delta))+times(m_de,cos(delta))-times(m_drr0,cosTheta);
		y=times(sqrt(m_dr0*m_dr0-m_de*m_de)+ss,cos(delta))-times(m_de,sin(delta))-times(m_drr0,sinTheta);
	}
	else if(m_nIndexFollower==1)
	{
		x=times(sqrt(m_dr0*m_dr0-m_de*m_de)+ss,sin(delta))+times(m_de,cos(delta));
		y=times(sqrt(m_dr0*m_dr0-m_de*m_de)+ss,cos(delta))-times(m_de,sin(delta));
	}
	else if (m_nIndexFollower==2)
	{
		x=times(m_dr0+ss,sin(delta))+times(DsDelta2,cos(delta));
		y=times(m_dr0+ss,cos(delta))-times(DsDelta2,sin(delta));
	}
//	x=times(sqrt(m_dr0*m_dr0-m_de*m_de)+ss,sin(delta))+times(m_de,cos(delta));
//	y=times(sqrt(m_dr0*m_dr0-m_de*m_de)+ss,cos(delta))-times(m_de,sin(delta));
	
	for (int i=0;i<COUNT;i++)
	{
		px2[i]=x.r(i+1);
		py2[i]=y.r(i+1);
	}
	exitM();

}

void CCam::CalcCtrlPoint2(void)
{
initM(MATCOM_VERSION);
	Mm node2;
	//采样点，最后一点即为第一个点
	node2=zeros(COUNT+1,2);
	Mm m_node2;
	for (int i=0;i<COUNT;i++)
	{
		node2.r(i+1,1)=px2[i];
		node2.r(i+1,2)=py2[i];
	}
	node2.r(COUNT+1,1)=node2.r(1,1);
	node2.r(COUNT+1,2)=node2.r(1,2);

	//控制点
	m_node2=zeros(COUNT+1,2);
	Mm B2;
	B2=zeros(COUNT+1,COUNT+3);
	for (int i=1;i<=COUNT+1;i++)
	{
		B2.r(i,i)=mrdivide(1.0,6.0);
		B2.r(i,i+1)=mrdivide(4.0,6.0);
		B2.r(i,i+2)=mrdivide(1.0,6.0);

	}

	Mm C2;
	C2=zeros(COUNT+3,COUNT+1);
	for (int i2=1;i2<=COUNT+1;i2++)
		C2.r(i2,i2)=1;

	C2.r(COUNT+2,1)=-1;
	C2.r(COUNT+2,2)=1;
	C2.r(COUNT+2,COUNT+1)=1;
	C2.r(COUNT+3,1)=-1;
	C2.r(COUNT+3,3)=1;
	C2.r(COUNT+3,COUNT+1)=1;

	Mm D2;
	D2=zeros(COUNT+1);
	D2=B2*C2;
	m_node2=mldivide(D2,node2);
	
	Mm a2=zeros(2,2);

	a2.r(1,1)=m_node2.r(COUNT+1,1)-m_node2.r(1,1)+m_node2.r(2,1);
	a2.r(1,2)=m_node2.r(COUNT+1,2)-m_node2.r(1,2)+m_node2.r(2,2);

	a2.r(2,1)=m_node2.r(COUNT+1,1)+m_node2.r(3,1)-m_node2.r(1,1);
	a2.r(2,2)=m_node2.r(COUNT+1,2)+m_node2.r(3,2)-m_node2.r(1,2);
	m_node2=vertcat(m_node2,a2);

		
	for (int i3=0;i3<COUNT+3;i3++)
	{
		mpx2[i3]=m_node2.r(i3+1,1);
		mpy2[i3]=m_node2.r(i3+1,2);
	}
	exitM();
}



// 输入数控加工参数
void CCam::SetParameter2(CString KnifeID, CString KnifeFace, double Feed, int Speed)
{
	m_sKnifeID=KnifeID;
	m_sKnifeFace=KnifeFace;
	m_dFeed=Feed;
	m_nSpeed=Speed;
}

// 计算压力角
void CCam::CalcPreAngle(void)
{
	//计算推程回程速度
	double *speed=new double[COUNT];
	if(m_nIndexMotion==0)
	{
		//计算速度
		for(int i=0;i<(int)m_dDelta0;i++)
			speed[i]=m_dh/m_dDelta0;
		for(int i=(int)m_dDelta0;i<(int)(m_dDelta0+m_dDelta01);i++)
			speed[i]=0;
		for(int i=(int)(m_dDelta0+m_dDelta01);i<(int)(m_dDelta0+m_dDelta01+m_dDelta1);i++)
			speed[i]=-m_dh/m_dDelta1;
		for(int i=(int)(m_dDelta0+m_dDelta01+m_dDelta1);i<COUNT;i++)
			speed[i]=0;
	}
	else
	{
		t2=new CData(tt,m_dDelta0,m_dh);
		h2=new CData(th,m_dDelta1,m_dh);
		//计算行程
		for(int i=0;i<(int)m_dDelta0;i++)
			speed[i]=t2->v[i]*m_dh/m_dDelta0;
		for(int i=(int)m_dDelta0;i<(int)(m_dDelta0+m_dDelta01);i++)
			speed[i]=0;
		for(int i=(int)(m_dDelta0+m_dDelta01);i<(int)(m_dDelta0+m_dDelta01+m_dDelta1);i++)
			speed[i]=-h2->v[i-(int)(m_dDelta0+m_dDelta01)]*m_dh/m_dDelta1;
		for(int i=(int)(m_dDelta0+m_dDelta01+m_dDelta1);i<COUNT;i++)
			speed[i]=0;

	}
	for (int i=0;i<COUNT;i++)
		m_dPreAngle[i]=abs(atan((speed[i]-m_de)/(sqrt(m_dr0*m_dr0-m_de*m_de)+m_ds2[i]))*180/Pi);
	delete[] speed;
}

void CCam::ShowPresAngle(CDC* pDC,CRect r1)
{

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int wid=r1.Width();
	CString str;
	int height=tm.tmHeight+tm.tmInternalLeading;
	pDC->TextOutW(0,0,_T("压力角显示(角度制)"));
	str.Format(_T("推程起点：%.2f"),m_dPreAngle[0]);
	pDC->TextOutW(0,height,str);
	str.Format(_T("远修止起点：%.2f"),m_dPreAngle[(int)m_dDelta0]);
	pDC->TextOutW(0,2*height,str);
	str.Format(_T("回程起点：%.2f"),m_dPreAngle[(int)(m_dDelta0+m_dDelta01)]);
	pDC->TextOutW(0,3*height,str);
}

// 输入许用压力角
void CCam::SetParameter3(double AllowAlpha)
{
	m_dAllowAlpha=AllowAlpha;
}

void CCam::CreateGCode(void)
{
	gcode=new CGCode(m_dDelta0,m_dDelta01,m_dDelta1,m_dDelta02,
			    mpx2,mpy2,m_dr0,m_dh,m_sKnifeID,m_sKnifeFace,
			    m_dFeed,m_nSpeed);
	m_bGCode=true;
}
