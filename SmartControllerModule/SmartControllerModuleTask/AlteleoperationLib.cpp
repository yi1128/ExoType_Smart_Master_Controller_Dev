#include "AlteleoperationLib.h"

AlteleoperationLib::AlteleoperationLib()
{

	RMC_Initialize_fail = INITIALIZE_FAIL;
}

AlteleoperationLib::~AlteleoperationLib()
{

}

void AlteleoperationLib::RateMode_Initalize(float A, float B,float kp, float kd, float x0)
{
	Alpha = A;
	Beta = B;
	Kp = kp;
	Kd = kd;
	X0 = x0;

	RMC_Initialize_fail = INITIALIZE_COMPLETE;
}

int AlteleoperationLib::RateModeControl(bool Indexing,float err,float derr, float Pose, 
	float &outputVel, float &OutputForce)
{
	if(RMC_Initialize_fail == INITIALIZE_FAIL)
		return INITIALIZE_FAIL;


	if (Indexing){
		OutputForce = OutputForce + Kp * err + Kd * derr;
		
		if(Pose >= -X0 && Pose <= X0)
		{
			Boundary = Beta * (outputVel);
			outputVel = Boundary;
		}
		else if(Pose < -X0)
		{
			//outputVel = Alpha * (Pose + X0);
			outputVel = Alpha * (Pose);
		}
		else if(Pose > X0)
		{
			//outputVel = Alpha * (Pose - X0);
			outputVel = Alpha * (Pose);
		}
	}
	else{
		OutputForce = Kp * err + Kd * derr;
		outputVel = 0.0;
	}

	return INITIALIZE_COMPLETE;

}