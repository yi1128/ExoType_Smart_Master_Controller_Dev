///////////////////////////////////////////////////////////////////////////////
// SmartControllerModuleTaskCtrl.h

#ifndef __SMARTCONTROLLERMODULETASKCTRL_H__
#define __SMARTCONTROLLERMODULETASKCTRL_H__

#include <atlbase.h>
#include <atlcom.h>

#define SMARTCONTROLLERMODULETASKDRV_NAME "SMARTCONTROLLERMODULETASK"

#include "resource.h"       // main symbols
#include "SmartControllerModuleTaskW32.h"
#include "TcBase.h"
#include "SmartControllerModuleTaskClassFactory.h"
#include "TcOCFCtrlImpl.h"

class CSmartControllerModuleTaskCtrl 
	: public CComObjectRootEx<CComMultiThreadModel>
	, public CComCoClass<CSmartControllerModuleTaskCtrl, &CLSID_SmartControllerModuleTaskCtrl>
	, public ISmartControllerModuleTaskCtrl
	, public ITcOCFCtrlImpl<CSmartControllerModuleTaskCtrl, CSmartControllerModuleTaskClassFactory>
{
public:
	CSmartControllerModuleTaskCtrl();
	virtual ~CSmartControllerModuleTaskCtrl();

DECLARE_REGISTRY_RESOURCEID(IDR_SMARTCONTROLLERMODULETASKCTRL)
DECLARE_NOT_AGGREGATABLE(CSmartControllerModuleTaskCtrl)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CSmartControllerModuleTaskCtrl)
	COM_INTERFACE_ENTRY(ISmartControllerModuleTaskCtrl)
	COM_INTERFACE_ENTRY(ITcCtrl)
	COM_INTERFACE_ENTRY(ITcCtrl2)
END_COM_MAP()

};

#endif // #ifndef __SMARTCONTROLLERMODULETASKCTRL_H__
