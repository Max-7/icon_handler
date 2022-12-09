// IconHandler.cpp : Implementation of CIconHandler

#include "pch.h"
#include "IconHandler.h"

static const char* DEBUG_PATH = "C:\\debug.txt";

// CIconHandler

const wchar_t* __stdcall _debug_icon_handler(LPCWSTR path) {
    const wchar_t DRIVES[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
    int drive_number = PathGetDriveNumberW(path);

    // Parsec supports drive from H to Z (included). If the drive letter is not
    // one of those, we can return FALSE immediately
    // A is 0, Z is 25, our drive should be between 7 and 25 included

    if (drive_number == -1 || drive_number < 7 || PathIsRootW(path)) {
        return NULL;
    }

    // Parsec write a registry key to set the icon for its workspaces.
    // We check if the key is present for the current drive.
    wchar_t reg_key[MAX_PATH * 32] = { 0 };
    swprintf(reg_key, MAX_PATH * 32, L"Software\\Classes\\Applications\\Explorer.exe\\Drives\\%c\\DefaultIcon", DRIVES[drive_number]);

    return reg_key;
}


static void debug_msg(const wchar_t* msg) {
    FILE* fd = fopen(DEBUG_PATH, "a");

    fwprintf(fd, L"%s\n", msg);
    fclose(fd);
}

HRESULT __stdcall CIconHandler::IsMemberOf(LPCWSTR pwszPath, DWORD dwAttrib)
{
    const wchar_t DRIVES[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
    int drive_number = PathGetDriveNumberW(pwszPath);

    // Parsec supports drive from H to Z (included). If the drive letter is not
    // one of those, we can return FALSE immediately
    // A is 0, Z is 25, our drive should be between 7 and 25 included

    if (drive_number == -1 || drive_number < 7 || PathIsRootW(pwszPath)) {
        return S_FALSE;
    }

    debug_msg(pwszPath);

    // Parsec write a registry key to set the icon for its workspaces.
    // We check if the key is present for the current drive.
    wchar_t reg_key[MAX_PATH * 32] = { 0 };
    swprintf(reg_key, MAX_PATH * 32, L"Software\\Classes\\Applications\\Explorer.exe\\Drives\\%c\\DefaultIcon", DRIVES[drive_number]);
    HKEY key;

    if (RegOpenKeyEx(HKEY_CURRENT_USER, reg_key, 0, KEY_READ, &key) != ERROR_SUCCESS) {
        return S_FALSE;
    }

    return S_OK;
}

HRESULT __stdcall CIconHandler::GetOverlayInfo(LPWSTR pwszIconFile, int cchMax, int* pIndex, DWORD* pdwFlags)
{
    // const LPWSTR path = L"C:\\Users\\Corentin\\Documents\\icon_handler\\vs\\check.ico";
    const LPWSTR path = L"E:\\projects\\icon_handler\\check.ico";

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