#include "hwid.h"
#include <windows.h>
#include <wininet.h>
#include <sstream>
#include <iostream> // Optional: for debugging output

#pragma comment(lib, "wininet.lib")

std::string GetHWID() {
    HKEY hKey;
    const char* regPath = R"(SYSTEM\CurrentControlSet\Control\IDConfigDB\Hardware Profiles\0001)";
    const char* valueName = "HwProfileGuid";
    char value[255];
    DWORD value_length = sizeof(value);
    DWORD type = 0;

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, regPath, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueExA(hKey, valueName, nullptr, &type, reinterpret_cast<LPBYTE>(value), &value_length) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            if (type == REG_SZ) {
                return std::string(value);
            }
        } else {
            RegCloseKey(hKey);
        }
    }
    return "";
}

bool CheckCredentialsOnline(const std::string& user, const std::string& pass, const std::string& hwid) {
    HINTERNET hInternet = InternetOpenA("LoginCheck", INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, 0);
    if (!hInternet) return false;

    HINTERNET hFile = InternetOpenUrlA(hInternet, "https://alexander-abot.netlify.app/users.txt", nullptr, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hFile) {
        InternetCloseHandle(hInternet);
        return false;
    }

    char buffer[4097]; // One extra byte for null terminator
    DWORD bytesRead;
    std::string fileContent;

    while (InternetReadFile(hFile, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead != 0) {
        buffer[bytesRead] = '\0'; // Null terminate
        fileContent += buffer;
    }

    InternetCloseHandle(hFile);
    InternetCloseHandle(hInternet);

    std::istringstream stream(fileContent);
    std::string line;
    std::string target = user + " / " + pass + " / " + hwid;

    while (std::getline(stream, line)) {
        if (line == target) {
            return true;
        }
    }

    return false;
}