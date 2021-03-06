///////////////////////////////////////////////////////////////////////////////
// SmartControllerModuleTaskServices.h

#pragma once

#include "TcServices.h"

const ULONG DrvID_SmartControllerModuleTask = 0x3F000000;
#define SRVNAME_SMARTCONTROLLERMODULETASK "SmartControllerModuleTask"

///<AutoGeneratedContent id="ClassIDs">
const CTCID CID_SmartControllerModuleTaskCTaskModule = {0xe0279160,0xa268,0x4d13,{0x9c,0x0a,0x29,0x7e,0xeb,0xcf,0xf5,0x88}};
///</AutoGeneratedContent>

///<AutoGeneratedContent id="ParameterIDs">
const PTCID PID_TaskModuleDefaultAdsPort = 0x00000001;
const PTCID PID_TaskModuleCounter = 0x00000003;
///</AutoGeneratedContent>

///<AutoGeneratedContent id="DataTypes">
#if !defined(_TC_TYPE_C5C5B0E8_872E_4CFF_AD85_54B3C603F64D_INCLUDED_)
#define _TC_TYPE_C5C5B0E8_872E_4CFF_AD85_54B3C603F64D_INCLUDED_
#pragma pack(push,1)
typedef struct _Output_Aixs_Data
{
	LONG m_target_position;
	LONG m_target_velocity;
	SHORT m_target_current;
	unsigned char m_mode_of_operation;
	SHORT m_control_word;
	ULONG m_digital_output;
	ULONG m_profile_velocity;
	ULONG m_profile_acceleration;
	ULONG m_profile_deceleration;
	USHORT m_Max_Current;
} Output_Aixs_Data, *POutput_Aixs_Data;
#pragma pack(pop)
#endif // !defined(_TC_TYPE_C5C5B0E8_872E_4CFF_AD85_54B3C603F64D_INCLUDED_)

#if !defined(_TC_TYPE_9F95CDE0_62DD_4CDF_9BA3_95E449321733_INCLUDED_)
#define _TC_TYPE_9F95CDE0_62DD_4CDF_9BA3_95E449321733_INCLUDED_
#pragma pack(push,1)
typedef struct _Input_Axis_Data
{
	LONG m_actual_position;
	LONG m_actual_velocity;
	SHORT m_actual_current;
	char m_mode_of_operation_display;
	USHORT m_status_word;
	LONG m_absolute_actual_position;
	LONG m_digital_inputs;
} Input_Axis_Data, *PInput_Axis_Data;
#pragma pack(pop)
#endif // !defined(_TC_TYPE_9F95CDE0_62DD_4CDF_9BA3_95E449321733_INCLUDED_)

typedef struct _TaskModuleInputs
{
	ULONG Value;
	ULONG Status;
	ULONG Data;
} TaskModuleInputs, *PTaskModuleInputs;

typedef struct _TaskModuleOutputs
{
	ULONG Value;
	ULONG Control;
	ULONG Data;
} TaskModuleOutputs, *PTaskModuleOutputs;

#pragma pack(push,1)
typedef struct _TaskModuleInputEtherCAT
{
	Input_Axis_Data Axis[6];
} TaskModuleInputEtherCAT, *PTaskModuleInputEtherCAT;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct _TaskModuleOutputEtherCAT
{
	Output_Aixs_Data Axis[6];
} TaskModuleOutputEtherCAT, *PTaskModuleOutputEtherCAT;
#pragma pack(pop)

///</AutoGeneratedContent>



///<AutoGeneratedContent id="DataAreaIDs">
#define ADI_TaskModuleInputs 0
#define ADI_TaskModuleOutputs 1
#define ADI_TaskModuleInputEtherCAT 3
#define ADI_TaskModuleOutputEtherCAT 4
///</AutoGeneratedContent>

///<AutoGeneratedContent id="InterfaceIDs">
///</AutoGeneratedContent>

///<AutoGeneratedContent id="EventClasses">
///</AutoGeneratedContent>
