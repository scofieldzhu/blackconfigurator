#ifndef __REGISTRY_H__
#define __REGISTRY_H__

#include "common.h"

__BCONF_BEGIN__

// This function will register a component in the Registry.
// DllRegisterServer function should call this function.
HRESULT RegisterServer(const CLSID& clsid, 
                                                          LPCTSTR filename, 
                                                          LPCTSTR prog_id,
                                                          LPCTSTR description, 
                                                          LPCTSTR ver_prog_id);

// This function will unregister a component.  Components
// DllUnregisterServer function should call this function.
HRESULT UnregisterServer(const CLSID& clsid, LPCTSTR prog_id, LPCTSTR ver_prog_id);

__BCONF_END__

#endif
