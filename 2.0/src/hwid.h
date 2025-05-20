#pragma once
#include <string>

std::string GetHWID();
std::string GetUserStatusOnline(const std::string& user, const std::string& pass, const std::string& hwid);
