// IconHandler.cpp : Implementation of CIconHandler

#include "pch.h"
#include "IconHandler.h"


// CIconHandler

HRESULT __stdcall CIconHandler::IsMemberOf(LPCWSTR pwszPath, DWORD dwAttrib)
{
    // Accept every type of files
    return S_OK;
}

HRESULT __stdcall CIconHandler::GetOverlayInfo(LPWSTR pwszIconFile, int cchMax, int* pIndex, DWORD* pdwFlags)
{
    // const LPWSTR path = L"C:\\Users\\Corentin\\Documents\\icon_handler\\vs\\check.ico";
    const LPWSTR path = L"D:\\projects\\icon_handler\\parsec.ico";

    wcscpy_s(pwszIconFile, wcslen(path) + 1, path);
    *pIndex = 0;
    *pdwFlags = ISIOI_ICONFILE;

    return S_OK;
}

HRESULT __stdcall CIconHandler::GetPriority(int* pPriority)
{
    *pPriority = 50;
    return S_OK;
}