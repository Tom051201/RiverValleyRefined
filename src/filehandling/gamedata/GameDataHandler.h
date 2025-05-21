#pragma once
#include <string>
#include <map>
#include "../../Utils.h"

#include "../../../includes/json.hpp"
#include <fstream>
using json = nlohmann::json;

class GameDataHandler {
public:

	GameDataHandler(const std::string& jsonpath);
	~GameDataHandler();

	void loadJsonData();
	void saveJsonData() const;
	json& getSection(const std::string& key);
	const json& getSection(const std::string& key) const;

	void loadGameData(const std::string& filePath);
	void saveGameData(const std::string& filePath);
	void addToSaveQueue(const std::string& id, const std::string& data);
	std::string& getData(const std::string& id);

private:

	std::string m_jsonPath;
	json m_loadedData;

	std::unique_ptr<FileHandler> fileHandler;
	std::map<std::string, std::string> dataTable;
	std::map<std::string, std::string> saveQueue;
};
