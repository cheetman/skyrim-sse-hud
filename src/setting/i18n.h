#pragma once
#include <filesystem>

#define I18N(STR) i18n::Translate(STR)
#define I18Nc(STR) i18n::Translate(STR).c_str()
#define I18Ni(c, STR) i18n::Translate(c, STR).c_str()
#define I18Nc2(STR, STR2) i18n::Translate2(STR, STR2).c_str()

namespace i18n
{
	extern std::unordered_map<std::string, std::unordered_map<std::string, std::string>> maps;
	bool load();
	extern std::string languageName;
	extern std::filesystem::path i18nPath;
	std::string Translate(const std::string& msg);
	std::string Translate(const char* icon, const std::string& msg);
	std::string Translate2(const std::string& msg, const char* msg2);
}
