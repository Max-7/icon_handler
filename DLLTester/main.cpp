#include <Windows.h>

#include <cstdlib>
#include <iostream>


int main(int ac, char** av) {

	wchar_t dllPath[MAX_PATH] = { 0 };
	std::mbstowcs(dllPath, av[1], strlen(av[1]) + 1);

	std::wcout << dllPath << std::endl;
	HMODULE l = LoadLibrary(dllPath);

	if (!l) {
		return 1;
	}

	FARPROC f = GetProcAddress(
		l,
		"_debug_icon_handler"
	);

	if (!f) {
		FreeLibrary(l);
		return 2;
	}

	typedef const wchar_t* (*DllFunc)(const wchar_t*);

	const wchar_t* key = ((DllFunc)(*f))(L"U:\\524460.jpg");

	std::wcout << L"--" << key << std::endl;

	HKEY opened_key;

	HRESULT result = RegOpenKeyEx(HKEY_CURRENT_USER, key, 0, KEY_READ, &opened_key);

	if (result == ERROR_SUCCESS) {
		std::cout << "OPENED IT!" << std::endl;
	}
	else {
		std::cout << "DID NOT OPEN IT!" << std::endl;
	}

	FreeLibrary(l);
	return 0;
}