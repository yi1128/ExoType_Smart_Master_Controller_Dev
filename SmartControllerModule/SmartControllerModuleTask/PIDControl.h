class PIDControlClass
{
public:
	
	PIDControlClass();
	~PIDControlClass();

	void InitGain();

	float UpdatePID(float curValue, float refValue);
	float UpdatePID(float curValue, float refValue, bool bioLimit, bool bOutLimit);
	float UpdatePID(float curValue, float refValue, float dt, bool bioLimit, bool bOutLimit);

	float* GetGain();
	float* GetILimit();
	float* GetOutLimit();
	void SetGain(float kp, float ki, float kd);
	void SetKILimit(float imin, float imax);
	void SetOutLimit(float imin, float imax);

private:
	float e1, u, up, ui, uil, ud, e;
	float m_PGain;
	float m_IGain;
	float m_DGain;
	float m_ILimit[2];
	float m_OutLimit[2];
};
