#include "hwid.h"
#include <windows.h>
#include <wininet.h>
#include <sstream>
#include <iostream>

#pragma comment(lib, "wininet.lib")

void Trim(std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    if (start == std::string::npos || end == std::string::npos) {
        s.clear();
    } else {
        s = s.substr(start, end - start + 1);
    }
}

// Helper: strip leading and trailing braces '{' and '}' from string
std::string StripBraces(const std::string& s) {
    if (s.size() >= 2 && s.front() == '{' && s.back() == '}') {
        return s.substr(1, s.size() - 2);
    }
    return s;
}

std::string GetHWID() {
    HKEY hKey;
    const char* regPath = R"(SYSTEM\CurrentControlSet\Control\IDConfigDB\Hardware Profiles\0001)";
    const char* valueName = "HwProfileGuid";
    char value[255] = {0};
    DWORD value_length = sizeof(value);
    DWORD type = 0;

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, regPath, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueExA(hKey, valueName, nullptr, &type, reinterpret_cast<LPBYTE>(value), &value_length) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            if (type == REG_SZ) {
                value[value_length - 1] = '\0';  // Ensure null-termination
                std::string hwidStr(value);
                Trim(hwidStr);
                return hwidStr;
            }
        } else {
            RegCloseKey(hKey);
        }
    }
    return "";
}

std::string GetUserStatusOnline(const std::string& user, const std::string& pass, const std::string& hwid) {
    HINTERNET hInternet = InternetOpenA("LoginCheck", INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, 0);
    if (!hInternet) return "";

    HINTERNET hFile = InternetOpenUrlA(hInternet, "https://alexander-abot.netlify.app/users.txt", nullptr, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hFile) {
        InternetCloseHandle(hInternet);
        return "";
    }

    char buffer[4096];
    DWORD bytesRead;
    std::string fileContent;

    while (true) {
        BOOL result = InternetReadFile(hFile, buffer, sizeof(buffer) - 1, &bytesRead);
        if (!result || bytesRead == 0) break;
        buffer[bytesRead] = '\0';
        fileContent += buffer;
    }

    InternetCloseHandle(hFile);
    InternetCloseHandle(hInternet);

    std::istringstream stream(fileContent);
    std::string line;

    while (std::getline(stream, line)) {
        std::istringstream linestream(line);
        std::string fileUser, filePass, fileHWID, fileStatus;

        if (std::getline(linestream, fileUser, '/') &&
            std::getline(linestream, filePass, '/') &&
            std::getline(linestream, fileHWID, '/') &&
            std::getline(linestream, fileStatus)) {

            Trim(fileUser);
            Trim(filePass);
            Trim(fileHWID);
            Trim(fileStatus);

            // Compare stripped fileHWID with given hwid (no braces)
            if (fileUser == user && filePass == pass && StripBraces(fileHWID) == hwid) {
                return fileStatus;
            }
        }
    }

    return "";
}
