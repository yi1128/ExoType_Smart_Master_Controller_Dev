class SEATorqueControl
{
public:

	SEATorqueControl();
	~SEATorqueControl();

	float OutputTorqueCalculation (float target_torque);
	float InputCurrentCalculation (float input_current);

	void SetTorqueControlCoef(float Torque_KP, float Torque_KD, float Torque_KI, float Cur2ECAT, float ECAT2Cur);
	void SetTorqueFrictionCoef(float vel_limit_l, float vel_limit_h, float torque_limit_l, float torque_limit_h);
	float TorqueFriction(float motor_vel);

	float m_KP_tor;
	float m_KD_tor;
	float m_kI_tor;

	float m_Cur2ECAT;
	float m_ECAT2Cur;

private:
	float m_input_torque;
	float m_torque_CMD;
	float m_friction_com;
	float m_vel_limit_l;
	float m_vel_limit_h;
	float m_torque_limit_l;
	float m_torque_limit_h;
};