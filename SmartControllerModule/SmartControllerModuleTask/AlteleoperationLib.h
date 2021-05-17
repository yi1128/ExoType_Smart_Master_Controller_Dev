#include <stdio.h>

// Robot Control Basic Parameter
#define LeftPhantomMode 1
#define RightPhantomMode 2

// MODE (Manipulator Robot Mode / Mobile Robot Mode)
#define MANIPULATOR_ROBOT_MODE 1
#define MOBILE_ROBOT_MODE 2
#define STOP_MODE 1000


#define SFAACTIVE 1 
#define RATE_MODE_SFA 11
#define FTSENSOR_FEEDBACK 2 
#define NONFTSENSOR_FEEDBACK 3
#define RATE_MODE 4
#define DIRECT_TELEOPERATION 5

#define SWITCHING_POSE 1
#define SWITCHING_VELO 2

#define POSITION_DRIFT_COMPENSATOR_ON 52345
#define POSITION_DRIFT_COMPENSATOR_OFF 62345
#define VMS_ON 5123
#define VMS_OFF 6132

#define INITIALIZE_FAIL -1
#define INITIALIZE_COMPLETE 1

#define SFA_UP 1
#define SFA_DOWN -1

class AlteleoperationLib
{
public:
	AlteleoperationLib();
	~AlteleoperationLib();

	void RateMode_Initalize(float A, float B,float kp, float kd, float x0);
	/*
		// Master send command to slave
		x : position
		x0 : boundary
		command = A * (x - x0) + B * x_dot
		zero hold force = Kp * err + Kd * derr
	*/
	
	int RateModeControl(bool Indexing,float err,float derr, float Pose, 
		float &outputVel, float &OutputForce);
private:
	// Rate Mode Control Parameter
	float Boundary;
	float Alpha;
	float Beta;
	float Kp;
	float Kd;
	float X0;
	int RMC_Initialize_fail;

};