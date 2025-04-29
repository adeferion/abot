// hwid.cpp
#include "hwid.h"
#include <windows.h>
#include <wininet.h>
#include <sstream>

#pragma comment(lib, "wininet.lib")

std::string GetHWID() {
    HKEY hKey;
    const char* regPath = R"(Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Cryptography)";
    const char* valueName = "MachineGuid";
    char value[255];
    DWORD value_length = sizeof(value);
    DWORD type = 0;

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, regPath, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueExA(hKey, valueName, nullptr, &type, (LPBYTE)&value, &value_length) == ERROR_SUCCESS && type == REG_SZ) {
            RegCloseKey(hKey);
            return std::string(value);
        }
        RegCloseKey(hKey);
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

    char buffer[4096];
    DWORD bytesRead;
    std::string fileContent;

    while (InternetReadFile(hFile, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead != 0) {
        buffer[bytesRead] = '\0';
        fileContent += buffer;
    }

    InternetCloseHandle(hFile);
    InternetCloseHandle(hInternet);

    std::istringstream stream(fileContent);
    std::string line;
    std::string target = user + " / " + pass + " / " + hwid;

    while (std::getline(stream, line)) {
        if (line == target)
            return true;
    }

    return false;
}
