// hwid.h
#pragma once
#include <string>

std::string GetHWID();
bool CheckCredentialsOnline(const std::string& user, const std::string& pass, const std::string& hwid);