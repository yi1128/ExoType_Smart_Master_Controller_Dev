///////////////////////////////////////////////////////////////////////////////
// SmartControllerModuleTaskDriver.h

#ifndef __SMARTCONTROLLERMODULETASKDRIVER_H__
#define __SMARTCONTROLLERMODULETASKDRIVER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TcBase.h"

#define SMARTCONTROLLERMODULETASKDRV_NAME        "SMARTCONTROLLERMODULETASK"
#define SMARTCONTROLLERMODULETASKDRV_Major       1
#define SMARTCONTROLLERMODULETASKDRV_Minor       0

#define DEVICE_CLASS CSmartControllerModuleTaskDriver

#include "ObjDriver.h"

class CSmartControllerModuleTaskDriver : public CObjDriver
{
public:
	virtual IOSTATUS	OnLoad();
	virtual VOID		OnUnLoad();

	//////////////////////////////////////////////////////
	// VxD-Services exported by this driver
	static unsigned long	_cdecl SMARTCONTROLLERMODULETASKDRV_GetVersion();
	//////////////////////////////////////////////////////
	
};

Begin_VxD_Service_Table(SMARTCONTROLLERMODULETASKDRV)
	VxD_Service( SMARTCONTROLLERMODULETASKDRV_GetVersion )
End_VxD_Service_Table


#endif // ifndef __SMARTCONTROLLERMODULETASKDRIVER_H__