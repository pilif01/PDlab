#include <Windows.h>
#include <iostream>
#include <string>
 
 
 
int get_key_subkeys(wchar_t path[10000]) {
    wchar_t* path_to_open = path + 1;
    std::wcout <<"path: " << path_to_open <<std::endl;
    HKEY hKey;
    wchar_t new_path[10000];
    wcscpy_s(new_path, path);
    wcscat_s(new_path, L"\\");
 
    LONG lRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, path_to_open, 0, KEY_READ, &hKey);
    if (lRes == ERROR_SUCCESS) {
        wchar_t szSubkey[256];
        DWORD dwIndex = 0;
        DWORD cbName = sizeof(szSubkey);
        while (RegEnumKeyEx(hKey, dwIndex, szSubkey, &cbName, NULL, NULL, NULL, NULL) != ERROR_NO_MORE_ITEMS) {
 
            HKEY hSubkey;
            if (RegOpenKeyEx(hKey, szSubkey, 0, KEY_READ, &hSubkey) == ERROR_SUCCESS) {
                wchar_t szImagePath[1024];
                DWORD cbData = sizeof(szImagePath);
                DWORD cbType = sizeof(szImagePath);
                int name_len = wcslen(szSubkey);
 
                get_key_subkeys(new_path);
               // remove szSubkey from new_path so that it does not append all subkeys name in a row.
 
                if (RegQueryValueEx(hSubkey, L"ImagePath", NULL, &cbType, (LPBYTE)szImagePath, &cbData) == ERROR_SUCCESS) {
                    if (cbType == 0x01 || cbType == 0x02) {
                        std::wcout << L"Subkey: " << szSubkey << std::endl;
                        std::wcout << L"ImagePath: " << szImagePath << std::endl;
                        std::wcout << L"Type: " << cbType << std::endl << std::endl;
 
                    }
                }
                RegCloseKey(hSubkey);
            }
            dwIndex++;
            cbName = sizeof(szSubkey);
        }
        RegCloseKey(hKey);
    }
    else {
        std::cerr << "Error opening registry key!" << std::endl;
        return 1;
    }
    return 0;
}
 
int main() {
    wchar_t path[100] = L"";
    get_key_subkeys(path);
    return 0;
}