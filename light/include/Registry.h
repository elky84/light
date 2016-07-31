#pragma once

#include "Position.h"

namespace light
{

class Registry
{


/*********************************************************************************************
/* Parameters 
/*  bAutoExec[in]     : TRUE이면 시작프로그램 레지스트리에 등록, FALSE면 해제
/*  lpValueName[in]   : 설정할 값의 이름
/*  lpExeFileName[in] : 실행시킬 프로그램 Full 경로 (NULL 일수 있음, 단, bAutoExec값이 FALSE이여야 함)
/*
/* Return Values
/*  시작프로그램 레지스트리에 등록/헤제 성공이면 TRUE, 실패면 FALSE
*********************************************************************************************/
BOOL StartProgram(BOOL bAutoExec, LPCSTR lpValueName, LPCSTR lpExeFileName)
{
	BOOL _result;
	HKEY hKey;
    LONG lRes;

    if(bAutoExec)
    {
        if(lpValueName == NULL || lpExeFileName == NULL)
            return FALSE;

        lRes = ::RegOpekeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\on_update", 0L, KEY_WRITE, &hKey);
        if( lRes == ERROR_SUCCESS )
        {
            lRes = ::RegSetValueEx(hKey, lpValueName,
                                   0, REG_SZ, (BYTE*)lpExeFileName, lstrlen(lpExeFileName)); 
            ::RegCloseKey(hKey);
            if(lRes == ERROR_SUCCESS) 
                _result = TRUE;
            else
            {
                AfxMessageBox("Error");
                _result = FALSE;
            }
        }
        else if(lRes == ERROR_ACCESS_DENIED)
        {
            AfxMessageBox("권한이 없습니다!!");
            _result = FALSE;
        }
        else
        {
            AfxMessageBox("Error");
            _result = FALSE;
        }
    }
    else                 // on_update 에서 제거
    {
        lRes = ::RegOpekeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\on_update", 0 , KEY_ALL_ACCESS, &hKey);
        if( lRes != ERROR_SUCCESS )
            _result = FALSE;

        lRes = RegDeleteValue(hKey, lpValueName);      
        RegCloseKey(hKey);
        if(lRes == ERROR_SUCCESS)
            _result = TRUE;
        else if(lRes == ERROR_FILE_NOT_FOUND)
            _result = FALSE;
        else
        {
            AfxMessageBox("시작 프로그램에서 삭제하지 못했습니다.");
            _result = FALSE;
        }
    }
    return _result;
}

};

} //namespace light
