///////////////////////////////////////////////////////////////////////////////
// SmartControllerModuleTaskDriver.cpp
#include "TcPch.h"
#pragma hdrstop

#include "SmartControllerModuleTaskDriver.h"
#include "SmartControllerModuleTaskClassFactory.h"

DECLARE_GENERIC_DEVICE(SMARTCONTROLLERMODULETASKDRV)

IOSTATUS CSmartControllerModuleTaskDriver::OnLoad( )
{
	TRACE(_T("CObjClassFactory::OnLoad()\n") );
	m_pObjClassFactory = new CSmartControllerModuleTaskClassFactory();

	return IOSTATUS_SUCCESS;
}

VOID CSmartControllerModuleTaskDriver::OnUnLoad( )
{
	delete m_pObjClassFactory;
}

unsigned long _cdecl CSmartControllerModuleTaskDriver::SMARTCONTROLLERMODULETASKDRV_GetVersion( )
{
	return( (SMARTCONTROLLERMODULETASKDRV_Major << 8) | SMARTCONTROLLERMODULETASKDRV_Minor );
}

