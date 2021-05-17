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

class AlpopcLib
{
public:
	AlpopcLib();
	~AlpopcLib();

	void POPC_Intialize(float _MIN_FORCE_ERROR, float _MIN_VEL_ERROR, float _M_vms, float _K_vms, float _B_vms);
	/*
		m*x_ddot + b * x_dot + k * x
		_M_vms : virtual mass
		_K_vms : virtual spring
		_B_vms : virtual damping  
	*/

	
	
	int ImpedancePOPC_NonVMS(float& E_s_in, float& E_m_in, float& E_m_out_origin, float& E_m_out, 
		float& Fm, float& F_modified, float Vm, float Dt);
	/*
		Not Using Virtual Mass spring 
		Original Impedance POPC
	*/
	
	int AdmittancePOPC(float& E_m_in, float& E_s_in, float& E_s_out_origin, float& E_s_out, float& F, float& V, float Dt);
	/*
		Basic Slave Controller - Admittance Passvity Controller
	*/
	
	float InputEnergyUpdate(float previous_energy, float F, float V, float Dt);
	float OutputEnergyUpdate(float previous_energy, float F, float V, float Dt);
	inline float Modified_Fm(float Fm, float Vm, float alpha);
	inline float Modified_Vs(float Vs, float Fs, float beta);
	float Damping_Alpha(float E_out, float E_in, float Vm, float Dt);
	float Damping_Beta(float E_out, float E_in, float Fs, float Dt);

public:
	int c;
private:
	float MIN_FORCE_ERROR;
	float MIN_VEL_ERROR;

	float M_vms;
	float K_vms;
	float B_vms;

	//OBG controller Parameter
	float O_Fs_d;
	float O_prev_Fs_d;
	float O_Fm_proxy;
	float O_prev_Fm_proxy;
	float O_prev_Fm;
	float O_prev_error;
	float O_prev_Fs;

	// MOVING EVERAGE FILTER
	bool MOVING_EVER_INIT;
	int moving_aver_num;
	int moving_counter;
	bool moving_counter_checker;
	float *moving_aver;

	int Intialize_fail;
};