#include "StdAfx.h"
#include "Data.h"

CData::CData(double m_dt[8],double m_dDelta,double m_dh)
{
	for (int i=0;i<8;i++)
		t[i]=m_dt[i];
	delta=m_dDelta;
	h=m_dh;
	s=new double[(int)delta];
	v=new double[(int)delta];
	CalcParameter();
	CalcShifting();

}

CData::~CData(void)
{
	delete [] s;
	delete [] v;
}

// 计算参数
void CData::CalcParameter(void)
{
	for(int i=0;i<7;i++)
		f[i]=2*(t[i+1]-t[i])/Pi;
	
	double m,m1,m2;
	if((f[4]+f[5]*Pi/2+f[6])==(f[0]+f[1]*Pi/2+f[2]))
		m=1;
	else
		m=(f[0]+f[1]*Pi/2+f[2])/(f[4]+f[5]*Pi/2+f[6]);


	m1=f[2]*f[2]+(t[2]-t[1])*(t[2]-t[1])/2-f[0]*f[0]+f[2]*(1-t[3])+(t[2]-t[1])*(1-t[2])+f[0];
	m2=f[6]*f[6]+(t[6]-t[5])*(t[6]-t[5])/2-f[4]*f[4]+f[4]*(1-t[4])+(t[6]-t[5])*(1-t[6]);

	if ((m1-m*m2)==0)
		a[0]=1;
	else
		a[0]=1.0/(m1-m*m2);
	a[1]=m*a[0];

	c[0]=f[0]*a[0];
	c[1]=-a[0]*t[1]+c[0];
	c[2]=a[0]*t[2]+c[1];
	c[3]=a[0]*f[2]+c[2];
	c[4]=-a[1]*f[4]+c[3];
	c[5]=a[1]*t[5]+c[4];
	c[6]=-a[1]*t[6]+c[5];

	b[0]=0;
	b[1]=-a[1]*f[0]*f[0]+a[0]*t[1]*t[1]/2;
	b[2]=a[0]*f[2]*f[2]-a[0]*t[2]*t[2]/2+b[1];
	b[3]=-a[0]*f[2]*t[3]+b[2];
	b[4]=a[1]*f[4]*t[4]+b[3];
	b[5]=a[1]*f[4]*f[4]-a[1]*t[5]*t[5]/2+b[4];
	b[6]=-a[1]*f[6]*f[6]+a[1]*t[6]*t[6]/2+b[5];

}

// 计算行程与速度
void CData::CalcShifting(void)
{
	int n=0,sn[7]={0,0,0,0,0,0,0};
	//得到f[7]中非零的元素索引与个数
	for(int i=0;i<7;i++)
		if(f[i])
			sn[n++]=i;

	int x=(int)(delta/n);	//每段区域的点数
	int x1=(int)(delta-(n-1)*x); //最后一段区域的点数

	int id=0;  //索引号
	
	//计算输入运动规律每度行程对应的位移和速度
	for (int i=0;i<7;i++)
	if(sn[i] || (i==0))
		switch(sn[i])
		{
			case 0:
				if(i<n-1)
				{
					for(int i0=0;i0<x;i0++)
					{
						double t0=t[1]*i0/x;
						s[i0+id]=h*(-a[0]*f[0]*f[0]*sin(t0/f[0])+c[0]*t0+b[0]);
						v[i0+id]=-a[0]*f[0]*cos(t0/f[0])+c[0];
					}
					id+=x;
					break;
				}
				else	
				{
					for(int i0=0;i0<x1;i0++)
					{
						double t0=t[1]*i0/x1;
						s[i0+id]=h*(-a[0]*f[0]*f[0]*sin(t0/f[0])+c[0]*t0+b[0]);
						v[i0+id]=-a[0]*f[0]*cos(t0/f[0])+c[0];

					}
					id+=x1;
					break;

				}
				
			case 1:
				if(i<n-1)
				{
					for (int i1=0;i1<x;i1++)
					{
						double t1=(t[2]-t[1])*i1/x+t[1];
						s[i1+id]=h*(a[0]*t1*t1/2+c[1]*t1+b[1]);
						v[i1+id]=a[0]*t1+c[1];
					}
					id+=x;
					break;
				}
				else
				{
					for (int i1=0;i1<x1;i1++)
					{
						double t1=(t[2]-t[1])*i1/x1+t[1];
						s[i1+id]=h*(a[0]*t1*t1/2+c[1]*t1+b[1]);
						v[i1+id]=a[0]*t1+c[1];
					}
					id+=x1;
					break;
				}
			case 2:
				if(i<n-1)
				{
					for(int i2=0;i2<x;i2++)
						{
							double t2=(t[3]-t[2])*i2/x+t[2];
							s[i2+id]=h*(-a[0]*f[2]*f[2]*sin((t2-t[2])/f[2]+Pi/2)+c[2]*t2+b[2]);
							v[i2+id]=-a[0]*f[2]*cos((t2-t[2])/f[2]+Pi/2)+c[2];
						}
					id+=x;
					break;
				}
				else
				{
					for(int i2=0;i2<x1;i2++)
						{
							double t2=(t[3]-t[2])*i2/x1+t[2];
							s[i2+id]=h*(-a[0]*f[2]*f[2]*sin((t2-t[2])/f[2]+Pi/2)+c[2]*t2+b[2]);
							v[i2+id]=-a[0]*f[2]*cos((t2-t[2])/f[2]+Pi/2)+c[2];
						}
					id+=x1;
					break;
				}
			case 3:
				if(i<n-1)
				{
					for (int i3=0;i3<x;i3++)
					{
						s[i3+id]=h*(c[3]*((t[4]-t[3])*i3/x+t[3])+b[3]);
						v[i3+id]=c[3];
					}
					id+=x;
					break;
				}
				else
				{
					for (int i3=0;i3<x1;i3++)
					{
						s[i3+id]=h*(c[3]*((t[4]-t[3])*i3/x1+t[3])+b[3]);
						v[i3+id]=c[3];
					}
					id+=x1;
					break;
				}
			case 4:
				if (i<n-1)
				{
					for(int i4=0;i4<x;i4++)
					{
						double t4=(t[5]-t[4])*i4/x+t[4];
						s[i4+id]=h*(-a[1]*f[4]*f[4]*sin((t4-t[4])/f[4]+Pi)+c[4]*t4+b[4]);
						v[i4+id]=-a[1]*f[4]*cos((t4-t[4])/f[4]+Pi)+c[4];
					}
					id+=x;
					break;
				}
				else
				{
					for(int i4=0;i4<x1;i4++)
					{
						double t4=(t[5]-t[4])*i4/x1+t[4];
						s[i4+id]=h*(-a[1]*f[4]*f[4]*sin((t4-t[4])/f[4]+Pi)+c[4]*t4+b[4]);
						v[i4+id]=-a[1]*f[4]*cos((t4-t[4])/f[4]+Pi)+c[4];
					}
					id+=x1;
					break;
				}
			case 5:
				if(i<n-1)
				{
					for(int i5=0;i5<x;i5++)
					{
						double t5=(t[6]-t[5])*i5/x+t[5];
						s[i5+id]=h*(-a[1]*t5*t5/2+c[5]*t5+b[5]);
						v[i5+id]=-a[1]*t5+c[5];
					}
					id+=x;
					break;
				}
				else
				{
					for(int i5=0;i5<x1;i5++)
					{
						double t5=(t[6]-t[5])*i5/x1+t[5];
						s[i5+id]=h*(-a[1]*t5*t5/2+c[5]*t5+b[5]);
						v[i5+id]=-a[1]*t5+c[5];
					}
					id+=x1;
					break;}
			case 6:
				if (i<n-1)
				{
					for(int i6=0;i6<x;i6++)
					{
						double t6=(t[7]-t[6])*i6/x+t[6];
						s[i6+id]=h*(-a[1]*f[6]*f[6]*sin((t6-t[6])/f[6]+3*Pi/2.0)+c[6]*t6+b[6]);
						v[i6+id]=-a[1]*f[6]*cos((t6-t[6])/f[6]+3*Pi/2.0)+c[6];
					}
					id+=x;
					break;
				}
				else
				{
					for(int i6=0;i6<x1;i6++)
					{
						double t6=(t[7]-t[6])*i6/x1+t[6];
						s[i6+id]=h*(-a[1]*f[6]*f[6]*sin((t6-t[6])/f[6]+3*Pi/2.0)+c[6]*t6+b[6]);
						v[i6+id]=-a[1]*f[6]*cos((t6-t[6])/f[6]+3*Pi/2.0)+c[6];
					}
					id+=x1;
					break;
				}
		}
}
