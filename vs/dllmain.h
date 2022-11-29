// dllmain.h : Declaration of module class.

class CParsecShellModule : public ATL::CAtlDllModuleT< CParsecShellModule >
{
public :
	DECLARE_LIBID(LIBID_ParsecShellLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_PARSECSHELL, "{889a8069-3eb0-4a64-b297-282e95b70bf2}")
};

extern class CParsecShellModule _AtlModule;
