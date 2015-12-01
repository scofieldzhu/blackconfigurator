#include "registry.h"
#include <objbase.h>
#include <assert.h>
#include <stdio.h>

__BCONF_BEGIN__

namespace {    
    const int CLSID_STRING_SIZE = 100;
    const int kStrBufferSize = 256;

    // Convert a CLSID to a char string.
    void CLSIDtoString(const CLSID& clsid, PTSTR clsid_str_buffer, int length){
        assert(length >= CLSID_STRING_SIZE);        
        LPOLESTR ole_clsid_str = NULL;         
        HRESULT hr = StringFromCLSID(clsid, &ole_clsid_str);
        assert(SUCCEEDED(hr));        
        //MultiByteToWideChar(CP_ACP, 0, (char*)ole_clsid_str, -1, clsid_str_buffer, length);
        
        _tcscpy_s(clsid_str_buffer, length, ole_clsid_str);
        CoTaskMemFree(ole_clsid_str);
    }

    //
    // Delete a key and all of its descendents.
    //
    LONG DeleteKey(HKEY root_key, LPCTSTR child_key){                
        HKEY hKeyChild;
        LONG lRes = RegOpenKeyEx(root_key, child_key, 0, KEY_ALL_ACCESS, &hKeyChild);
        if (lRes != ERROR_SUCCESS)        
            return lRes;                
        TCHAR keyname_buffer[kStrBufferSize];
        DWORD buffer_size = kStrBufferSize;
        FILETIME time;
        while(RegEnumKeyEx(hKeyChild, 0, keyname_buffer, &buffer_size, NULL, NULL, NULL, &time) == S_OK){            
            lRes = DeleteKey(hKeyChild, keyname_buffer);
            if (lRes != ERROR_SUCCESS){                
                RegCloseKey(hKeyChild);
                return lRes;
            }
            buffer_size = kStrBufferSize;
        }        
        RegCloseKey(hKeyChild);        
        return RegDeleteKey(root_key, child_key);
    }

    //
    // Create a key and set its value.
    //
    BOOL SetKeyAndValue(LPCTSTR target_key, LPCTSTR subkey, LPCTSTR keyval) {
        TCHAR key_buffer[1024] = { _T('\0') };
        _tcscpy_s(key_buffer, 1024, target_key);
        if (subkey != NULL) {
            _tcscat_s(key_buffer, 1024, _T("\\"));
            _tcscat_s(key_buffer, 1024, subkey);
        }
        HKEY key_handle = NULL;
        long result = RegCreateKeyEx(HKEY_CLASSES_ROOT,
            key_buffer,
            0,
            NULL,
            REG_OPTION_NON_VOLATILE,
            KEY_ALL_ACCESS,
            NULL,
            &key_handle,
            NULL);
        if (result != ERROR_SUCCESS) 
            return FALSE;        
        if (keyval != NULL) {
            LONG err_code = RegSetValueEx(key_handle, NULL, 0, REG_SZ, (BYTE *)keyval, _tcslen(keyval) * sizeof(TCHAR) + 1);
            if (err_code != ERROR_SUCCESS) {
                RegCloseKey(key_handle);                
                return FALSE;
            }
        }
        RegCloseKey(key_handle);        
        return TRUE;
    }
}

HRESULT RegisterServer(const CLSID& clsid, LPCTSTR filename, LPCTSTR prog_id, LPCTSTR description, LPCTSTR ver_prog_id){    
    TCHAR clsid_buffer[CLSID_STRING_SIZE] = { _T('\0') };
    CLSIDtoString(clsid, clsid_buffer, CLSID_STRING_SIZE);
    TCHAR target_key[64] = { _T('\0') };
    _tcscat_s(target_key, 64, _T("CLSID\\"));
    _tcscat_s(target_key, 64, clsid_buffer);    
    
    SetKeyAndValue(target_key, NULL, description);
    SetKeyAndValue(target_key, _T("InprocServer32"), filename);    
    if (prog_id != NULL) {
        SetKeyAndValue(target_key, _T("ProgID"), prog_id);
        SetKeyAndValue(prog_id, _T("CLSID"), clsid_buffer);
    }

    if (ver_prog_id != NULL){        
        SetKeyAndValue(target_key, _T("VersionIndependentProgID"), ver_prog_id);        
        SetKeyAndValue(ver_prog_id, NULL, description);
        SetKeyAndValue(ver_prog_id, _T("CLSID"), clsid_buffer);
        SetKeyAndValue(ver_prog_id, _T("CurVer"), prog_id);        
        SetKeyAndValue(prog_id, NULL, description);
        SetKeyAndValue(prog_id, _T("CLSID"), clsid_buffer);
    }
    return S_OK;
}

HRESULT UnregisterServer(const CLSID& clsid, LPCTSTR prog_id, LPCTSTR ver_prog_id){
    TCHAR clsid_buffer[CLSID_STRING_SIZE];
    CLSIDtoString(clsid, clsid_buffer, sizeof(clsid_buffer));    
    TCHAR szKey[64] = {_T('\0')};
    _tcscat_s(szKey, 64, _T("CLSID\\"));
    _tcscat_s(szKey, 64, clsid_buffer);    

    LONG result = DeleteKey(HKEY_CLASSES_ROOT, szKey);
    if (ver_prog_id != NULL)
        result = DeleteKey(HKEY_CLASSES_ROOT, ver_prog_id);    
    if (prog_id != NULL)
        result = DeleteKey(HKEY_CLASSES_ROOT, prog_id);
    return S_OK;
}

__BCONF_END__