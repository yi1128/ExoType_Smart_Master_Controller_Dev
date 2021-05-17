#include "AlpopcLib.h"

AlpopcLib::AlpopcLib()
{
	MIN_FORCE_ERROR = 0.0;
	MIN_VEL_ERROR = 0.0;
		
	M_vms = 0.0;
	K_vms = 0.0;
	B_vms = 0.0;

	O_Fs_d = 0.0;
	O_prev_Fs_d = 0.0;
	O_Fm_proxy = 0.0;
	O_prev_Fm_proxy = 0.0;
	O_prev_Fm = 0.0;
	O_prev_error = 0.0;
	O_prev_Fs = 0.0;

	MOVING_EVER_INIT = INITIALIZE_COMPLETE;
	Intialize_fail = INITIALIZE_COMPLETE;
		
		
}

AlpopcLib::~AlpopcLib()
{

}

void AlpopcLib::POPC_Intialize(float _MIN_FORCE_ERROR, float _MIN_VEL_ERROR, float _M_vms, float _K_vms, float _B_vms)
{
	MIN_FORCE_ERROR = _MIN_FORCE_ERROR;
	MIN_VEL_ERROR = _MIN_VEL_ERROR;

	M_vms = _M_vms;
	K_vms = _K_vms;
	B_vms = _B_vms;

	Intialize_fail = INITIALIZE_COMPLETE;
	//std::cout << "INTIALIZING" << std::endl;
}

int AlpopcLib::ImpedancePOPC_NonVMS(float& E_s_in, float& E_m_in, float& E_m_out_origin, float& E_m_out, 
	float& Fm, float& F_modified, float Vm, float Dt){

	// MinForceFiltering(Fm);
	// MinVelocityFiltering(Vm);

	if(Intialize_fail == INITIALIZE_FAIL)
		return INITIALIZE_FAIL;

	float E_m_out_prev = E_m_out; // E_m_out(k-1)

	//Energy Update
	E_m_in = InputEnergyUpdate(E_m_in,Fm,Vm,Dt); // E_m_in(k)
	E_m_out = OutputEnergyUpdate(E_m_out,Fm,Vm,Dt); // E_m_out(k)

	//Passivity Controller
	float F_origin = Fm;
	F_modified = Fm;
	E_m_out_origin = E_m_out;

	if(E_m_out + E_s_in < 0.0){
		if(Vm*Vm > 0.0){
			E_m_out += Fm*Vm*Dt; // Restoring the Energy
			F_modified = (E_s_in + E_m_out)/(Vm * Dt); // Force modification to make E_m_out(k) same with E_s_in(k)
			E_m_out = -E_s_in; // Energy(E_m_out(k)) Update with modified Force
		}
	}

	return INITIALIZE_COMPLETE;
}

//Admittance POPC
int AlpopcLib::AdmittancePOPC(float& E_m_in, float& E_s_in, float& E_s_out_origin, float& E_s_out, float& F, float& V, float Dt){
	// MinForceFiltering(F);
	// MinVelocityFiltering(V);

	if(Intialize_fail == INITIALIZE_FAIL)
		return INITIALIZE_FAIL;

	float E_s_out_prev = E_s_out;

	//Energy Update
	E_s_in = InputEnergyUpdate(E_s_in,F,V,Dt);
	E_s_out = OutputEnergyUpdate(E_s_out,F,V,Dt);
		
	//Passivity Controller
	float V_origin = V;
	E_s_out_origin = E_s_out;
	float beta = 0.0;
	

	if(E_s_out + E_m_in < 0.0){
		if(F*F > 0.0){
			E_s_out += F*V*Dt;
			V = (E_m_in + E_s_out)/F/Dt;
			E_s_out = -E_m_in;
		}
		//printf("Es_in: %.6f, Es_out: %.6f, Es_out2: %.6f, Em_in: %.6f, Fs: %.6f, Vs: %.6f, Vsm: %.6f\n",E_s_in,E_s_out_origin, E_s_out, E_m_in, F, V_origin, V);
	}

	return INITIALIZE_COMPLETE;
}


float AlpopcLib::InputEnergyUpdate(float previous_energy, float F, float V, float Dt){
	float updated_energy;
	float P; //Power;
	P = F*V;
	if(P < 0){
		updated_energy = previous_energy - P*Dt;
	}
	else{
		updated_energy = previous_energy;
	}
	return updated_energy;
}

float AlpopcLib::OutputEnergyUpdate(float previous_energy, float F, float V, float Dt){
	float updated_energy;
	float P; //Power;
	P = F*V;
	if(P > 0){
		updated_energy = previous_energy - P*Dt;
	}
	else{
		updated_energy = previous_energy;
	}
	return updated_energy;
}


// Impedance Causality
inline float AlpopcLib::Modified_Fm(float Fm, float Vm, float alpha){
	float modified_fm = Fm - alpha*Vm;
	return modified_fm;	
}

// Admittance Causality
inline float AlpopcLib::Modified_Vs(float Vs, float Fs, float beta){
	float modified_vs = Vs - beta*Fs;
	return modified_vs;
}

// Damping Factor for Impedance Causality
float AlpopcLib::Damping_Alpha(float E_out, float E_in, float Vm, float Dt){
	float alpha = 0.0;
	if(Vm != 0.0){
		if(E_out > E_in){
			alpha = (E_out - E_in)/(Dt * Vm*Vm);	
		}
	}
	return alpha;
}

// Damping Factor for Admittance Causality
float AlpopcLib::Damping_Beta(float E_out, float E_in, float Fs, float Dt){
	float beta = 0.0;
	if(Fs != 0.0){
		if(E_out > E_in){
			beta = (E_out - E_in)/(Dt * Fs*Fs);
		}
	}
	return beta;
}

	
