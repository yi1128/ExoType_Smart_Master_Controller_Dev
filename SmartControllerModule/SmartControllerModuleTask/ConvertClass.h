#include <Fpu87.h>
class ConvertClass
{
public:
	ConvertClass()
	{	
		_InitABS[0] = 9916;
		_InitABS[1] = 6093;
		_InitABS[2] = 11080;
		_InitABS[3] = 2193664;
		_InitABS[4] = 7160;
		_InitABS[5] = 1036032;

		_InitDEG[0] = 0.0;
		_InitDEG[1] = 90.0f;
		_InitDEG[2] = -90.0f;
		_InitDEG[3] = 0.0;
		_InitDEG[4] = 90.0f;
		_InitDEG[5] = -90.0f;

		_ABS_dir[0] = -1.0;
		_ABS_dir[1] = -1.0;
		_ABS_dir[2] = -1.0;
		_ABS_dir[3] = -1.0;
		_ABS_dir[4] = -1.0;
		_ABS_dir[5] = -1.0;

		_InitENC[0] = 0.0;
		_InitENC[1] = 0.0;
		_InitENC[2] = 0.0;
		_InitENC[3] = 0.0;
		_InitENC[4] = 0.0;
		_InitENC[5] = 0.0;

		_GEARRATIO[0] = 50;
		_GEARRATIO[1] = 50;
		_GEARRATIO[2] = 50;
		_GEARRATIO[3] = 50;
		_GEARRATIO[4] = 50;
		_GEARRATIO[5] = 50;

		_PI = 3.14159265359;
		_RAD2DEG = 180.0 / _PI;
		_DEG2RAD = _PI / 180.0;

		_ABS_CNT[0] = 16384;
		_ENC_CNT[0] = 8000 * _GEARRATIO[0];
		_ABS_CNT[1] = 16384;
		_ENC_CNT[1] = 8000 * _GEARRATIO[1];
		_ABS_CNT[2] = 16384;
		_ENC_CNT[2] = 8000 * _GEARRATIO[2];
		_ABS_CNT[3] = 16384;
		_ENC_CNT[3] = 8000 * _GEARRATIO[3];
		_ABS_CNT[4] = 16384;
		_ENC_CNT[4] = 8000 * _GEARRATIO[4];
		_ABS_CNT[5] = 16384;
		_ENC_CNT[5] = 8000 * _GEARRATIO[5];

		_ENC2ABS[0] = _ABS_CNT[0] / _ENC_CNT[0];
		_ENC2ABS[1] = _ABS_CNT[1] / _ENC_CNT[1];
		_ENC2ABS[2] = _ABS_CNT[2] / _ENC_CNT[2];
		_ENC2ABS[3] = _ABS_CNT[3] / _ENC_CNT[3];
		_ENC2ABS[4] = _ABS_CNT[4] / _ENC_CNT[4];
		_ENC2ABS[5] = _ABS_CNT[5] / _ENC_CNT[5];

		_ABS2ENC[0] = _ENC_CNT[0] / _ABS_CNT[0];
		_ABS2ENC[1] = _ENC_CNT[1] / _ABS_CNT[1];
		_ABS2ENC[2] = _ENC_CNT[2] / _ABS_CNT[2];
		_ABS2ENC[3] = _ENC_CNT[3] / _ABS_CNT[3];
		_ABS2ENC[4] = _ENC_CNT[4] / _ABS_CNT[4];
		_ABS2ENC[5] = _ENC_CNT[5] / _ABS_CNT[5];

		_RAD2ENC[0] = _ENC_CNT[0] / (_PI * 2.0);
		_RAD2ENC[1] = _ENC_CNT[1] / (_PI * 2.0);
		_RAD2ENC[2] = _ENC_CNT[2] / (_PI * 2.0);
		_RAD2ENC[3] = _ENC_CNT[3] / (_PI * 2.0);
		_RAD2ENC[4] = _ENC_CNT[4] / (_PI * 2.0);
		_RAD2ENC[5] = _ENC_CNT[5] / (_PI * 2.0);

		_DEG2ENC[0] = _ENC_CNT[0] / 360.0;
		_DEG2ENC[1] = _ENC_CNT[1] / 360.0;
		_DEG2ENC[2] = _ENC_CNT[2] / 360.0;
		_DEG2ENC[3] = _ENC_CNT[3] / 360.0;
		_DEG2ENC[4] = _ENC_CNT[4] / 360.0;
		_DEG2ENC[5] = _ENC_CNT[5] / 360.0;

		_ENC2RAD[0] = (_PI * 2.0) / _ENC_CNT[0];
		_ENC2RAD[1] = (_PI * 2.0) / _ENC_CNT[1];
		_ENC2RAD[2] = (_PI * 2.0) / _ENC_CNT[2];
		_ENC2RAD[3] = (_PI * 2.0) / _ENC_CNT[3];
		_ENC2RAD[4] = (_PI * 2.0) / _ENC_CNT[4];
		_ENC2RAD[5] = (_PI * 2.0) / _ENC_CNT[5];

		_ABS2RAD[0] = (1 / _ABS_CNT[0])*(2 * _PI);
		_ABS2RAD[1] = (1 / _ABS_CNT[1])*(2 * _PI);
		_ABS2RAD[2] = (1 / _ABS_CNT[2])*(2 * _PI);
		_ABS2RAD[3] = (1 / _ABS_CNT[3])*(2 * _PI);
		_ABS2RAD[4] = (1 / _ABS_CNT[4])*(2 * _PI);
		_ABS2RAD[5] = (1 / _ABS_CNT[5])*(2 * _PI);

	}

	ConvertClass(double link1, double link2, double link3)
	{

		_InitABS[0] = 9916;
		_InitABS[1] = 6093;
		_InitABS[2] = 8983;
		_InitABS[3] = 2193664;
		_InitABS[4] = 7160;
		_InitABS[5] = 1036032;

		_InitDEG[0] = 0.0;
		_InitDEG[1] = 90.0f;
		_InitDEG[2] = -90.0f;
		_InitDEG[3] = 0.0;
		_InitDEG[4] = 90.0f;
		_InitDEG[5] = -90.0f;

		_ABS_dir[0] = -1.0;
		_ABS_dir[1] = -1.0;
		_ABS_dir[2] = -1.0;
		_ABS_dir[3] = -1.0;
		_ABS_dir[4] = -1.0;
		_ABS_dir[5] = -1.0;

		_InitENC[0] = 0.0;
		_InitENC[1] = 0.0;
		_InitENC[2] = 0.0;
		_InitENC[3] = 0.0;
		_InitENC[4] = 0.0;
		_InitENC[5] = 0.0;

		_GEARRATIO[0] = 50;
		_GEARRATIO[1] = 50;
		_GEARRATIO[2] = 50;
		_GEARRATIO[3] = 50;
		_GEARRATIO[4] = 50;
		_GEARRATIO[5] = 50;

		_PI = 3.14159265359;
		_RAD2DEG = 180.0 / _PI;
		_DEG2RAD = _PI / 180.0;

		_ABS_CNT[0] = 16384;
		_ENC_CNT[0] = 8000 * _GEARRATIO[0];
		_ABS_CNT[1] = 16384;
		_ENC_CNT[1] = 8000 * _GEARRATIO[1];
		_ABS_CNT[2] = 16384;
		_ENC_CNT[2] = 8000 * _GEARRATIO[2];
		_ABS_CNT[3] = 16384;
		_ENC_CNT[3] = 8000 * _GEARRATIO[3];
		_ABS_CNT[4] = 16384;
		_ENC_CNT[4] = 8000 * _GEARRATIO[4];
		_ABS_CNT[5] = 16384;
		_ENC_CNT[5] = 8000 * _GEARRATIO[5];

		_ENC2ABS[0] = _ABS_CNT[0] / _ENC_CNT[0];
		_ENC2ABS[1] = _ABS_CNT[1] / _ENC_CNT[1];
		_ENC2ABS[2] = _ABS_CNT[2] / _ENC_CNT[2];
		_ENC2ABS[3] = _ABS_CNT[3] / _ENC_CNT[3];
		_ENC2ABS[4] = _ABS_CNT[4] / _ENC_CNT[4];
		_ENC2ABS[5] = _ABS_CNT[5] / _ENC_CNT[5];

		_ABS2ENC[0] = _ENC_CNT[0] / _ABS_CNT[0];
		_ABS2ENC[1] = _ENC_CNT[1] / _ABS_CNT[1];
		_ABS2ENC[2] = _ENC_CNT[2] / _ABS_CNT[2];
		_ABS2ENC[3] = _ENC_CNT[3] / _ABS_CNT[3];
		_ABS2ENC[4] = _ENC_CNT[4] / _ABS_CNT[4];
		_ABS2ENC[5] = _ENC_CNT[5] / _ABS_CNT[5];

		_RAD2ENC[0] = _ENC_CNT[0] / (_PI * 2.0);
		_RAD2ENC[1] = _ENC_CNT[1] / (_PI * 2.0);
		_RAD2ENC[2] = _ENC_CNT[2] / (_PI * 2.0);
		_RAD2ENC[3] = _ENC_CNT[3] / (_PI * 2.0);
		_RAD2ENC[4] = _ENC_CNT[4] / (_PI * 2.0);
		_RAD2ENC[5] = _ENC_CNT[5] / (_PI * 2.0);

		_DEG2ENC[0] = _ENC_CNT[0] / 360.0;
		_DEG2ENC[1] = _ENC_CNT[1] / 360.0;
		_DEG2ENC[2] = _ENC_CNT[2] / 360.0;
		_DEG2ENC[3] = _ENC_CNT[3] / 360.0;
		_DEG2ENC[4] = _ENC_CNT[4] / 360.0;
		_DEG2ENC[5] = _ENC_CNT[5] / 360.0;

		_ENC2RAD[0] = (_PI * 2.0) / _ENC_CNT[0];
		_ENC2RAD[1] = (_PI * 2.0) / _ENC_CNT[1];
		_ENC2RAD[2] = (_PI * 2.0) / _ENC_CNT[2];
		_ENC2RAD[3] = (_PI * 2.0) / _ENC_CNT[3];
		_ENC2RAD[4] = (_PI * 2.0) / _ENC_CNT[4];
		_ENC2RAD[5] = (_PI * 2.0) / _ENC_CNT[5];

		_ABS2RAD[0] = (1 / _ABS_CNT[0])*(2 * _PI);
		_ABS2RAD[1] = (1 / _ABS_CNT[1])*(2 * _PI);
		_ABS2RAD[2] = (1 / _ABS_CNT[2])*(2 * _PI);
		_ABS2RAD[3] = (1 / _ABS_CNT[3])*(2 * _PI);
		_ABS2RAD[4] = (1 / _ABS_CNT[4])*(2 * _PI);
		_ABS2RAD[5] = (1 / _ABS_CNT[5])*(2 * _PI);
	}

	~ConvertClass()
	{

	}

	float _InitDEG[6];
	float _RAD2DEG;
	float _DEG2RAD;
	float _DEG2ENC[6];
	float _PI;
	float _RAD2ENC[6];
	float _ENC2RAD[6];
	float _InitABS[6];
	float _InitENC[6];
	float _ABS2ENC[6];
	float _ENC2ABS[6];
	float _ABS2RAD[6];
	float _ABS_dir[6];

private:
	float _GEARRATIO[6];
	float _ABS_CNT[6];
	float _ENC_CNT[6];
	
public:
	int* DECtoHEX(int value)
	{
		int hex[8] = { 0, };
		int dec = value;
		int i = 7;
	
		while (true)
		{
			int mod = dec % 16;
			hex[i] = mod;
			dec = dec / 16;
			i--;
			if (dec == 0)
				break;
			if (i == -1)
				break;

		}
		return hex;
	}
	int* HEXtoBIN(int value)
	{
		int bin[4] = { 0, };
		int hex = value;
		int i = 3;
		while (true)
		{
			int mod = hex % 2;
			bin[i] = mod;
			hex = hex / 2;
			i--;
			if (hex == 0)
				break;
			if (i == -1)
				break;
		}
		return bin;
	}
	

};
