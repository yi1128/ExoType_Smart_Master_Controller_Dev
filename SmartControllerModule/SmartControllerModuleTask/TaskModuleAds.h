#pragma once

#include "TcDef.h"

enum TaskModuleIndexGroups : ULONG
{
	TaskModuleIndexGroup1 = 0x00000001,
	TaskModuleIndexGroup2 = 0x00000002,
	IG_OVERWRITE = 0x00000003,  // and new command (sample code)
	GUIButton = 0x00000004

};

enum TaskModuleIndexOffsets : ULONG
{
	TaskModuleIndexOffset1 = 0x00000001,
	TaskModuleIndexOffset2 = 0x00000002,
	TaskModuleIndexOffset3 = 0x00000003,
	TaskModuleIndexOffset4 = 0x00000004,
	TaskModuleIndexOffset5 = 0x00000005,
	TaskModuleIndexOffset6 = 0x00000006,
	TaskModuleIndexOffset7 = 0x00000007,
	TaskModuleIndexOffset8 = 0x00000008,
	TaskModuleIndexOffset9 = 0x00000009,
	TaskModuleIndexOffset10 = 0x0000000a,
	TaskModuleIndexOffset11 = 0x0000000b,
	TaskModuleIndexOffset12 = 0x0000000c,
	TaskModuleIndexOffset13 = 0x0000000d,
	TaskModuleIndexOffset14 = 0x0000000e,
	TaskModuleIndexOffset15 = 0x0000000f,
	TaskModuleIndexOffset16 = 0x00000010,
	TaskModuleIndexOffset17 = 0x00000011,
	TaskModuleIndexOffset18 = 0x00000012,
	TaskModuleIndexOffset19 = 0x00000013,
	TaskModuleIndexOffset20 = 0x00000014,
	TaskModuleIndexOffset21 = 0x00000015,
	TaskModuleIndexOffset22 = 0x00000016,
	TaskModuleIndexOffset23 = 0x00000017,
	TaskModuleIndexOffset24 = 0x00000018,
	TaskModuleIndexOffset25 = 0x00000019,
	TaskModuleIndexOffset26 = 0x00000020,
	TaskModuleIndexOffset27 = 0x00000021,
	TaskModuleIndexOffset28 = 0x00000022,
	TaskModuleIndexOffset29 = 0x00000023,
	TaskModuleIndexOffset30 = 0x00000024,
	TaskModuleIndexOffset31 = 0x00000025,
	TaskModuleIndexOffset32 = 0x00000026,
	TaskModuleIndexOffset33 = 0x00000027,
	TaskModuleIndexOffset34 = 0x00000028,
	TaskModuleIndexOffset35 = 0x00000029,
	TaskModuleIndexOffset36 = 0x00000030, // Normal Controller
	TaskModuleIndexOffset37 = 0x00000031, // TDPA Controller
	TaskModuleIndexOffset38 = 0x00000032, // SSI Controller
	TaskModuleIndexOffset39 = 0x00000033, // Receieve Force & Energy
};

#define	ButtonServoOn			0x01
#define	Button_Max_Current		0x02
#define Button_Mode_Of_Operation_Display 0x03
#define	ButtonStop				0x04
#define ButtonMove				0x05
#define ButtonInitPos			0x06
#define ButtonCartesianMove		0x07
#define InitCartesianPoseComplete	0x08
#define ButtonEmergencyStop		0x09
#define InitPoseComplete		0x10
#define Watingstate				0x11
#define ButtonHomeingPos		0x12
#define ButtonHomeComplete		0x13
#define ButtonDemoRun			0x14
#define ButtonNormalController	0x15
#define ButtonTDPAController	0x16
#define ButtonSSIController		0x17

// Digital Input
#define LEFT_BUTTON_0 0x003E0008
#define LEFT_BUTTON_1 0x00370008
#define LEFT_BUTTON_2 0x00370008
#define LEFT_BUTTON_3 0x003D0008
#define LEFT_BUTTON_4 0x003B0008
#define LEFT_BUTTON_5 0x003D0008
