
#include "Kinematics.h"
#include "Fpu87.h"

Kinematics::Kinematics()
{
	m_Link[0] = 0.0f;
	m_Link[1] = 0.35f;
	m_Link[2] = 0.34f;
	m_Gravity = 9.8f;
	m_PI = 3.14159265358979323846f;
	m_Rad2Deg = 180.0f / m_PI;
	m_Deg2Rad = m_PI / 180.0f;
	m_Mass[0] = 0.0f;
	m_Mass[1] = 2.32f;
	m_Mass[2] = 2.76f;
	m_CoM[0] = 0.0;
	m_CoM[1] = 0.21f;
	m_CoM[2] = 0.17f;
	r11 = 0.0;
	r21 = 0.0;
	r31 = 0.0;
	r32 = 0.0;
	r33 = 0.0;
}

Kinematics::~Kinematics()
{

}

float* Kinematics::ForwardKinPos(float q1, float q2, float q3)
{
	float pos[3] = { 0.0f, 0.0f, 0.0f };

	pos[0] = cos_(q1)*((m_Link[1] * cos_(q2)) + (m_Link[2] * cos_(q2 + q3)));
	pos[1] = sin_(q1)*((m_Link[1] * cos_(q2)) + (m_Link[2] * cos_(q2 + q3)));
	pos[2] = (m_Link[1] * sin_(q2)) + (m_Link[2] * sin_(q2 + q3));

	return pos;
}


float* Kinematics::Orientation(float q1, float q2, float q3, float q4)
{
	float _Orientation[3] = { 0.0f, 0.0f, 0.0f };


	r11 = -sin_(q4)*(cos_(-m_PI / 2)*sin_(q3)*(cos_(q1)*cos_(q2) - cos_(m_PI / 2)*sin_(q1)*sin_(q2)) - sin_(-m_PI / 2)*(cos_(0.0)*sin_(m_PI / 2)*sin_(q1) + sin_(0.0)*cos_(q1)*sin_(q2) + cos_(m_PI / 2)*sin_(0.0)*cos_(q2)*sin_(q1)) + cos_(-m_PI / 2)*cos_(q3)*(cos_(0.0)*cos_(q1)*sin_(q2) - sin_(m_PI / 2)*sin_(0.0)*sin_(q1) + cos_(m_PI / 2)*cos_(0.0)*cos_(q2)*sin_(q1))) - cos_(q4)*(sin_(q3)*(cos_(0.0)*cos_(q1)*sin_(q2) - sin_(m_PI / 2)*sin_(0.0)*sin_(q1) + cos_(m_PI / 2)*cos_(0.0)*cos_(q2)*sin_(q1)) - cos_(q3)*(cos_(q1)*cos_(q2) - cos_(m_PI / 2)*sin_(q1)*sin_(q2)));
	r21 = -cos_(q4)*(sin_(q3)*(sin_(m_PI / 2)*sin_(0.0)*cos_(q1) + cos_(0.0)*sin_(q1)*sin_(q2) - cos_(m_PI / 2)*cos_(0.0)*cos_(q1)*cos_(q2)) - cos_(q3)*(cos_(q2)*sin_(q1) + cos_(m_PI / 2)*cos_(q1)*sin_(q2))) - sin_(q4)*(sin_(-m_PI / 2)*(cos_(0.0)*sin_(m_PI / 2)*cos_(q1) - sin_(0.0)*sin_(q1)*sin_(q2) + cos_(m_PI / 2)*sin_(0.0)*cos_(q1)*cos_(q2)) + cos_(-m_PI / 2)*sin_(q3)*(cos_(q2)*sin_(q1) + cos_(m_PI / 2)*cos_(q1)*sin_(q2)) + cos_(-m_PI / 2)*cos_(q3)*(sin_(m_PI / 2)*sin_(0.0)*cos_(q1) + cos_(0.0)*sin_(q1)*sin_(q2) - cos_(m_PI / 2)*cos_(0.0)*cos_(q1)*cos_(q2)));

	_Orientation[1] = 0.0;
	_Orientation[2] = atan_(r21/r11);


	return _Orientation;
}

float* Kinematics::Orientation_wrist(float t_90, float q4, float q5, float q6)
{
	float _Orientation[2] = { 0.0f, 0.0f };

	r31 = sin_(0.0)*cos_(q6)*(sin_(m_PI / 2)*(cos_(-m_PI / 2)*cos_(0.0) - sin_(-m_PI / 2)*sin_(0.0)*cos_(t_90)) + cos_(m_PI / 2)*cos_(q5)*(cos_(-m_PI / 2)*sin_(0.0) + cos_(0.0)*sin_(-m_PI / 2)*cos_(t_90)) - cos_(m_PI / 2)*sin_(-m_PI / 2)*sin_(q5)*sin_(t_90)) - cos_(0.0)*(cos_(m_PI / 2)*(cos_(-m_PI / 2)*cos_(0.0) - sin_(-m_PI / 2)*sin_(0.0)*cos_(t_90)) - sin_(m_PI / 2)*cos_(q5)*(cos_(-m_PI / 2)*sin_(0.0) + cos_(0.0)*sin_(-m_PI / 2)*cos_(t_90)) + sin_(-m_PI / 2)*sin_(m_PI / 2)*sin_(q5)*sin_(t_90)) - sin_(0.0)*sin_(q6)*(sin_(q5)*(cos_(-m_PI / 2)*sin_(0.0) + cos_(0.0)*sin_(-m_PI / 2)*cos_(t_90)) + sin_(-m_PI / 2)*cos_(q5)*sin_(t_90));
	r32 = sin_(0.0)*(cos_(m_PI / 2)*(cos_(-m_PI / 2)*cos_(0.0) - sin_(-m_PI / 2)*sin_(0.0)*cos_(t_90)) - sin_(m_PI / 2)*cos_(q5)*(cos_(-m_PI / 2)*sin_(0.0) + cos_(0.0)*sin_(-m_PI / 2)*cos_(t_90)) + sin_(-m_PI / 2)*sin_(m_PI / 2)*sin_(q5)*sin_(t_90)) + cos_(0.0)*cos_(q6)*(sin_(m_PI / 2)*(cos_(-m_PI / 2)*cos_(0.0) - sin_(-m_PI / 2)*sin_(0.0)*cos_(t_90)) + cos_(m_PI / 2)*cos_(q5)*(cos_(-m_PI / 2)*sin_(0.0) + cos_(0.0)*sin_(-m_PI / 2)*cos_(t_90)) - cos_(m_PI / 2)*sin_(-m_PI / 2)*sin_(q5)*sin_(t_90)) - cos_(0.0)*sin_(q6)*(sin_(q5)*(cos_(-m_PI / 2)*sin_(0.0) + cos_(0.0)*sin_(-m_PI / 2)*cos_(t_90)) + sin_(-m_PI / 2)*cos_(q5)*sin_(t_90));
	r33 = sin_(q6)*(sin_(m_PI / 2)*(cos_(-m_PI / 2)*cos_(0.0) - sin_(-m_PI / 2)*sin_(0.0)*cos_(t_90)) + cos_(m_PI / 2)*cos_(q5)*(cos_(-m_PI / 2)*sin_(0.0) + cos_(0.0)*sin_(-m_PI / 2)*cos_(t_90)) - cos_(m_PI / 2)*sin_(-m_PI / 2)*sin_(q5)*sin_(t_90)) + cos_(q6)*(sin_(q5)*(cos_(-m_PI / 2)*sin_(0.0) + cos_(0.0)*sin_(-m_PI / 2)*cos_(t_90)) + sin_(-m_PI / 2)*cos_(q5)*sin_(t_90));


	// Roll => pitch
	_Orientation[1] = atan_(r32/r33);
	// Pitch = roll
	_Orientation[0] = atan2_(-r31, sqrt_(pow_(r32, 2) + pow_(r33, 2)));


	return _Orientation;
}

float(*Kinematics::Jacobian(float q1, float q2, float q3))[3]
{

	float ja[3][3] = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };

	ja[0][0] = -sin_(q1)*((m_Link[1] * cos_(q2)) + (m_Link[2] * cos_(q2 + q3)));
	ja[0][1] = -cos_(q1)*((m_Link[1] * sin_(q2)) + (m_Link[2] * sin_(q2 + q3)));;
	ja[0][2] = -cos_(q1)*m_Link[2] * sin_(q2 + q3);

	ja[1][0] = cos_(q1)*((m_Link[1] * cos_(q2)) + (m_Link[2] * cos_(q2 + q3)));
	ja[1][1] = -sin_(q1)*((m_Link[1] * sin_(q2)) + (m_Link[2] * sin_(q2 + q3)));;
	ja[1][2] = -sin_(q1)*m_Link[2] * sin_(q2 + q3);

	ja[2][0] = 0;
	ja[2][1] = (m_Link[1] * cos_(q2)) + (m_Link[2] * cos_(q2 + q3));
	ja[2][2] = m_Link[2] * cos_(q2 + q3);


	return ja;
}

float(*Kinematics::JacobianTransPose(float q1, float q2, float q3))[3]
{
	float(*jacob)[3] = Jacobian(q1, q2, q3);
	float jacob_trans[3][3] = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			jacob_trans[i][j] = jacob[j][i];
		}
	}

	return jacob_trans;
}

float* Kinematics::GravityMat(float q2, float q3)
{
	float g[3] = { 0.0f, 0.0f, 0.0f };

	g[0] = 0;
	g[1] = (cos_(q2)*((m_Mass[1] * m_Gravity*m_CoM[1]) + (m_Mass[2] * m_Gravity*m_Link[1]))) + (cos_(q2 + q3)*(m_Mass[2] * m_Gravity * m_CoM[2]));
	g[2] = m_Mass[2] * m_Gravity * m_CoM[2] * cos_(q2 + q3);

	return g;
}

float(*Kinematics::CoriolisMat(float q1, float q2, float q3, float dq1, float dq2, float dq3))[3]
{
	//float c[3] = {0.0f, 0.0f, 0.0f};
	float cm[3][3] = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
	float C_A;
	float C_B;
	float C_C;

	C_A = m_Mass[1] * pow_(m_CoM[1], 2) * cos_(q2) * sin_(q2) + m_Mass[1] * (m_Link[1] * cos_(q2) + m_CoM[2] * cos_(q2 + q3)) * (m_Link[1] * sin_(q2) + m_CoM[2] * sin_(q2 + q3));
	C_B = m_Mass[2] * m_CoM[2] * sin_(q2 + q3) * (m_Link[1] * cos_(q2) + m_CoM[2] * cos_(q2 + q3));
	C_C = m_Link[1] * m_Mass[2] * m_CoM[2] * sin_(q3);

	cm[0][0] = -(C_A * dq2 + C_B * dq3);
	cm[0][1] = -C_A * dq1;
	cm[0][2] = -C_B * dq1;

	cm[1][0] = C_A * dq1;
	cm[1][1] = -C_C * dq3;
	cm[1][2] = -(C_C * dq2 + C_C * dq3);

	cm[2][0] = C_B * dq1;
	cm[2][1] = C_C * dq2;
	cm[2][2] = 0.0f;
	/*
	for (int i = 0 ; i < 3; i ++)
	{
	c[i] = (cm[i][0]*dq1) + (cm[i][1]*dq2) + (cm[i][2]*dq3);
	}
	*/

	return cm;
}

float(*Kinematics::MassMat(float q2, float q3))[3]
{
	float m[3][3] = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };

	m[0][0] = (m_Mass[1] * pow_(m_CoM[1], 2)*pow_(cos_(q2), 2)) + (m_Mass[2] * pow_((m_Link[0] * cos_(q2)) + (m_CoM[2] * cos_(q2 + q3)), 2));
	m[0][1] = 0;
	m[0][2] = 0;

	m[1][0] = 0;
	m[1][1] = (m_Mass[2] * pow_(m_Link[1], 2)) + (m_Mass[2] * pow_(m_CoM[1], 2)) + (m_Mass[2] * pow_(m_CoM[2], 2)) + (2.0f*m_Mass[2] * m_Link[1] * m_CoM[1] * cos_(q3));
	m[1][2] = (m_Mass[2] * pow_(m_CoM[2], 2)) + (m_Mass[2] * m_Link[1] * m_CoM[2] * cos_(q3));

	m[2][0] = 0;
	m[2][1] = (m_Mass[2] * pow_(m_CoM[2], 2)) + (m_Mass[2] * m_Link[1] * m_CoM[2] * cos_(q3));
	m[2][2] = m_Mass[2] * pow_(m_CoM[2], 2);

	return m;
}



float* Kinematics::GetLinkLength()
{
	float link[3] = { 0.0f, 0.0f, 0.0f };

	link[0] = m_Link[0];
	link[1] = m_Link[1];
	link[2] = m_Link[2];

	return link;
}

float* Kinematics::GetLinkMass()
{
	float m[3] = { 0.0f, 0.0f, 0.0f };

	m[0] = m_Mass[0];
	m[1] = m_Mass[1];
	m[2] = m_Mass[2];

	return m;
}

float* Kinematics::GetLinkRadius()
{
	float r[3] = { 0.0f, 0.0f, 0.0f };

	r[0] = m_CoM[0];
	r[1] = m_CoM[1];
	r[2] = m_CoM[2];

	return r;
}

float Kinematics::GetGravity()
{
	return m_Gravity;
}

void Kinematics::SetLinkLength(float l1, float l2, float l3)
{
	m_Link[0] = l1;
	m_Link[1] = l2;
	m_Link[2] = l3;
}

void Kinematics::SetLinkMass(float m1, float m2, float m3)
{
	m_Mass[0] = m1;
	m_Mass[1] = m2;
	m_Mass[2] = m3;
}

void Kinematics::SetLinkRadius(float r1, float r2, float r3)
{
	m_CoM[0] = r1;
	m_CoM[1] = r2;
	m_CoM[2] = r3;
}

void Kinematics::SetGravity(float g)
{
	m_Gravity = g;
}