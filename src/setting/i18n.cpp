
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
namespace i18n
{
	std::filesystem::path i18nPath = "data\\skse\\plugins\\ItemFinderPlus\\i18n";

	//std::unordered_map<std::string, std::string> languageMap;
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> maps;
	std::string languageName;
	int i18nIndex = 0;

	// 递归函数，将 JSON 对象中的多级键插入到 map 中
	void InsertJsonIntoMap(const nlohmann::json& j, std::unordered_map<std::string, std::string>& languageMap, const std::string& parentKey = "")
	{
		for (auto it = j.begin(); it != j.end(); ++it) {
			std::string currentKey = parentKey.empty() ? it.key() : parentKey + "." + it.key();

			if (it->is_object()) {
				// 递归处理子对象
				InsertJsonIntoMap(*it, languageMap, currentKey);
			} else {
				// 将键值对插入到 map 中
				languageMap[currentKey] = it->get<std::string>();
			}
		}
	}

	bool load()
	{
		if (!fs::exists(i18nPath) || !fs::is_directory(i18nPath)) {
			return false;
		}

		for (const auto& entry : std::filesystem::directory_iterator(i18nPath)) {
			if (entry.path().extension() == ".json") {
				try {
					std::ifstream i(entry.path());
					nlohmann::json json;
					i >> json;
					// 处理读取的 JSON 数据

					std::unordered_map<std::string, std::string> tmpLanguageMap;

					InsertJsonIntoMap(json, tmpLanguageMap);
					std::string filename = entry.path().stem().string();
					maps.insert(std::make_pair(filename, tmpLanguageMap));
				} catch (std::exception const& ex) {
					logger::error(ex.what());
				}
			}
		}

		// 默认策略
		if (languageName.empty()) {
			if (!maps.empty()) {
				if (maps.find("Chinese") != maps.end()) {
					languageName = "Chinese";
				} else {
					for (auto& item : maps) {
						languageName = item.first;
						break;
					}
				}
			} else {
				languageName = "无";
			}
		} else {
			if (!maps.empty()) {
				if (maps.find(languageName) == maps.end()) {
					if (maps.find("Chinese") != maps.end()) {
						languageName = "Chinese";
					} else {
						for (auto& item : maps) {
							languageName = item.first;
							break;
						}
					}
				}
			} else {
				languageName = "无";
			}
		}

		return true;
	}

	std::string Translate(const std::string& msg)
	{
		if (maps[languageName][msg].length() == 0) {
			return msg;
		}

		return maps[languageName][msg];
	}

	std::string Translate(const char* icon, const std::string& msg)
	{
		if (maps[languageName][msg].length() == 0) {
			return (icon + msg);
		}

		return (icon + maps[languageName][msg]);
	}

	std::string Translate2(const std::string& msg, const char* msg2)
	{
		if (maps[languageName][msg].length() == 0) {
			return (msg + msg2);
		}

		return (maps[languageName][msg] + msg2);
	}
}
