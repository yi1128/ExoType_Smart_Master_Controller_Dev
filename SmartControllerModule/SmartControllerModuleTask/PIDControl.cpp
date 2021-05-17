
#include "PIDControl.h"

PIDControlClass::PIDControlClass()
{
	e1 = 0;
	u = 0;
	up = 0;
	ui = 0;
	uil = 0;
	ud = 0;
	e = 0;
	m_DGain = 0;
	m_IGain = 0;
	m_PGain = 0;
	m_ILimit[0] = 0;
	m_ILimit[1] = 0;
}

PIDControlClass::~PIDControlClass()
{
	
}

void PIDControlClass::InitGain()
{
	e1 = 0;
	u = 0;
	up = 0;
	ui = 0;
	uil = 0;
	ud = 0;
	e = 0;
	m_DGain = 0;
	m_IGain = 0;
	m_PGain = 0;
	m_ILimit[0] = 0;
	m_ILimit[1] = 0;
	
}

float PIDControlClass::UpdatePID(float curValue, float refValue)
{
	e = refValue - curValue;

	up = m_PGain * e;
	ui = uil + m_IGain * e;
	ud = m_DGain * (e - e1);

	if( ui > m_ILimit[1] ) ui = m_ILimit[1] ;
	else if( ui < m_ILimit[0] ) ui = m_ILimit[0] ;

	uil=ui ;
	e1=e ;

	u = up + ui + ud;

	if (u > m_OutLimit[1]) u = m_OutLimit[1];
	else if (u < m_OutLimit[0]) u = m_OutLimit[0];

	return u;
}

float PIDControlClass::UpdatePID(float curValue, float refValue, bool bioLimit, bool bOutLimit)
{
	e = refValue - curValue;

	up = m_PGain * e;
	ui = uil + m_IGain * e;
	ud = m_DGain * (e - e1);

	if((bioLimit) &&( ui > m_ILimit[1] )) ui = m_ILimit[1] ;
	else if((bioLimit) &&( ui < m_ILimit[0] )) ui = m_ILimit[0] ;

	uil = ui;
	e1 = e;

	u = up + ui + ud;

	if ((bOutLimit) &&(u > m_OutLimit[1])) u = m_OutLimit[1];
	else if ((bOutLimit) &&(u < m_OutLimit[0])) u = m_OutLimit[0];

	return u;
}

float PIDControlClass::UpdatePID(float curValue, float refValue, float dt, bool bioLimit, bool bOutLimit)
{
	e = refValue - curValue;

	up = m_PGain * e;
	ui = uil + m_IGain * e * dt;
	ud = m_DGain * (e - e1) / dt;

	if((bioLimit) &&( ui > m_ILimit[1] )) ui = m_ILimit[1] ;
	else if((bioLimit) &&( ui < m_ILimit[0] )) ui = m_ILimit[0] ;

	uil = ui;
	e1 = e;

	u = up + ui + ud;

	if ((bOutLimit) &&(u > m_OutLimit[1])) u = m_OutLimit[1];
	else if ((bOutLimit) &&(u < m_OutLimit[0])) u = m_OutLimit[0];

	return u;
}

float* PIDControlClass::GetGain()
{
	float k[3] = {0.0f, 0.0f, 0.0f};

	return k;
}

float* PIDControlClass::GetILimit()
{
	return m_ILimit;
}

void PIDControlClass::SetGain(float kp, float ki, float kd)
{
	m_PGain = kp;
	m_IGain = ki;
	m_DGain = kd;
}

void PIDControlClass::SetKILimit(float imin, float imax)
{
	m_ILimit[0] = imin;
	m_ILimit[1] = imax;
}

void PIDControlClass::SetOutLimit(float imin, float imax)
{
	m_OutLimit[0] = imin;
	m_OutLimit[1] = imax;

}