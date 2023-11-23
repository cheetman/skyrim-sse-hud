#pragma once
#include <filesystem>

#define I18N(STR) i18n::Translate(STR)
#define I18Nc(STR) i18n::Translate(STR).c_str()
#define I18Ni(c, STR) i18n::Translate(c, STR).c_str()

namespace i18n
{
	extern std::unordered_map<std::string, std::unordered_map<std::string, std::string>> maps;
	bool load();
	extern std::string languageName;
	std::string Translate(const std::string& msg);
	std::string Translate(const char* icon, const std::string& msg);
}
