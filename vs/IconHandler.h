// IconHandler.h : Declaration of the CIconHandler

#pragma once
#include "resource.h"       // main symbols

#include <ShlObj_core.h>

#include "ParsecShell_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;

extern "C" {
	__declspec(dllexport) const wchar_t* __stdcall _debug_icon_handler(LPCWSTR);
}

// CIconHandler

class ATL_NO_VTABLE CIconHandler :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CIconHandler, &CLSID_IconHandler>,
	public IShellIconOverlayIdentifier
{
public:
	CIconHandler()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(106)


	BEGIN_COM_MAP(CIconHandler)
		COM_INTERFACE_ENTRY(IShellIconOverlayIdentifier)
	END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

		HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

private:
	// Inherited via IShellIconOverlayIdentifier
	virtual HRESULT __stdcall IsMemberOf(LPCWSTR pwszPath, DWORD dwAttrib) override;
	virtual HRESULT __stdcall GetOverlayInfo(LPWSTR pwszIconFile, int cchMax, int* pIndex, DWORD* pdwFlags) override;
	virtual HRESULT __stdcall GetPriority(int* pPriority) override;
};

OBJECT_ENTRY_AUTO(__uuidof(IconHandler), CIconHandler)