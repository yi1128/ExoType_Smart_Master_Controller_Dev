
#include "SEATorqueControl.h"
#include "Fpu87.h"

SEATorqueControl::SEATorqueControl()
{
	m_KP_tor = 0;
	m_KD_tor = 0;
    m_kI_tor = 0;
    m_Cur2ECAT = 0;
    m_torque_CMD = 0;
    m_input_torque = 0;
	m_friction_com = 0;

	m_vel_limit_l = -0.02;
	m_vel_limit_h = 0.02;
	m_torque_limit_l = -1.0;
	m_torque_limit_h = 1.0;
}

SEATorqueControl::~SEATorqueControl()
{

}


void SEATorqueControl::SetTorqueControlCoef(float Torque_KP, float Torque_KD, float Torque_KI, float Cur2ECAT, float ECAT2Cur)
{
	m_KP_tor = Torque_KP;
	m_KD_tor = Torque_KD;
	m_kI_tor = Torque_KI;

	m_Cur2ECAT = Cur2ECAT;
	m_ECAT2Cur = ECAT2Cur;
}
void SEATorqueControl::SetTorqueFrictionCoef(float vel_limit_l, float vel_limit_h, float torque_limit_l, float torque_limit_h)
{
	m_vel_limit_l = vel_limit_l;
	m_vel_limit_h = vel_limit_h;
	m_torque_limit_l = torque_limit_l;
	m_torque_limit_h = torque_limit_h;
}

float SEATorqueControl::TorqueFriction(float motor_vel)
{
	if (motor_vel < m_vel_limit_l){
		m_friction_com = m_torque_limit_l;
	}
	else if (motor_vel >= m_vel_limit_l && motor_vel <= m_vel_limit_h){
		m_friction_com = (m_torque_limit_h - m_torque_limit_l) / (m_vel_limit_h - m_vel_limit_l) * motor_vel;
	}
	else{
		m_friction_com = m_torque_limit_h;
	}
	return m_friction_com;
}

float SEATorqueControl::OutputTorqueCalculation(float input_torque)
{
	m_torque_CMD = ( input_torque ) * m_Cur2ECAT;
	return m_torque_CMD;
}

float SEATorqueControl::InputCurrentCalculation(float input_current)
{
	m_input_torque = ( input_current ) * m_ECAT2Cur;
	return m_input_torque;
}