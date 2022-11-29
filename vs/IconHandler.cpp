// IconHandler.cpp : Implementation of CIconHandler

#include "pch.h"
#include "IconHandler.h"


// CIconHandler

CIconHandler::SyncStatus CIconHandler::GetStatus(PCWSTR path)
{
    int hash = int(path[0]);

    return SyncStatus(hash % 3);
}

HRESULT __stdcall CIconHandler::GetClassID(CLSID* pClassID)
{
    return E_NOTIMPL;
}

HRESULT __stdcall CIconHandler::IsDirty(void)
{
    return E_NOTIMPL;
}

HRESULT __stdcall CIconHandler::Load(LPCOLESTR pszFileName, DWORD dwMode)
{
    ATLTRACE(L"CIconHandler::Load %s\n", pszFileName);
    return S_OK;
}

HRESULT __stdcall CIconHandler::Save(LPCOLESTR pszFileName, BOOL fRemember)
{
    return E_NOTIMPL;
}

HRESULT __stdcall CIconHandler::SaveCompleted(LPCOLESTR pszFileName)
{
    return E_NOTIMPL;
}

HRESULT __stdcall CIconHandler::GetCurFile(LPOLESTR* ppszFileName)
{
    return E_NOTIMPL;
}

HRESULT __stdcall CIconHandler::GetIconLocation(UINT uFlags, PWSTR pszIconFile, UINT cchMax, int* piIndex, UINT* pwFlags)
{
    if (m_modulePath[0] == 0) {
        GetModuleFileName(_AtlBaseModule.GetModuleInstance(), m_modulePath, sizeof(m_modulePath));
        ATLTRACE(L"Module path: %s\n", m_modulePath);
    }
    if (m_modulePath[0] == 0) {
        return S_FALSE;
    }

    wcscpy_s(pszIconFile, wcslen(m_modulePath) + 1, m_modulePath);
    ATLTRACE(L"GetIconLocation: %s sync status %d\n", pszIconFile, m_status);
    *piIndex = int(m_status);
    *pwFlags = GIL_PERINSTANCE;

    return S_OK;
}

HRESULT __stdcall CIconHandler::Extract(PCWSTR pszFile, UINT nIconIndex, HICON* phiconLarge, HICON* phiconSmall, UINT nIconSize)
{
    return E_NOTIMPL;
}
