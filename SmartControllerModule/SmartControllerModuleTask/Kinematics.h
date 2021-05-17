class Kinematics
{
public:
	Kinematics();
	~Kinematics();


	float* ForwardKinPos(float q1, float q2, float q3 );
	float* Orientation(float q1, float q2, float q3, float q4);
	float* Orientation_wrist(float t_90, float q4, float q5, float q6);
	float (*Jacobian(float q1, float q2, float q3))[3];
	float (*JacobianTransPose(float q1, float q2, float q3))[3];
	float* GravityMat(float q2, float q3);
	float (*CoriolisMat(float q1, float q2, float q3, float dq1, float dq2, float dq3))[3];
	float (*MassMat(float q2, float q3))[3];


	float* GetLinkLength();
	float* GetLinkMass();
	float* GetLinkRadius();
	float GetGravity();

	void SetLinkLength(float l1, float l2, float l3);
	void SetLinkMass(float m1, float m2, float m3);
	void SetLinkRadius(float r1, float r2, float r3);
	void SetGravity(float g);

	float m_PI;
	float m_Rad2Deg;
	float m_Deg2Rad;

private:
	float r11;
	float r21;
	float r31;
	float r32;
	float r33;
	float m_Mass[3];
	float m_Gravity;
	float m_CoM[3];
	float m_Link[3];
	
};

