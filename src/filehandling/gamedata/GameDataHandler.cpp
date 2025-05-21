#include "GameDataHandler.h"
#include <vector>
#include <iostream>
#include <sstream>

GameDataHandler::GameDataHandler(const std::string& jsonpath) {
	fileHandler = std::make_unique<FileHandler>();
	m_jsonPath = jsonpath;
}

GameDataHandler::~GameDataHandler() {
//	std::cout << "DataHandler deleted" << std::endl;
}

void GameDataHandler::loadJsonData() {
	std::ifstream file(m_jsonPath);
	if (file.is_open()) {
		try {
			file >> m_loadedData;
		}
		catch (...) {
			std::cerr << "Invalid JSON file!" << std::endl;
			m_loadedData = json::object(); // Start fresh
		}
	}
	else {
		std::cerr << "Couldn't open file, starting fresh." << std::endl;
		m_loadedData = json::object();
	}
}

void GameDataHandler::saveJsonData() const {
	std::ofstream file(m_jsonPath);
	if (file.is_open()) {
		file << m_loadedData.dump(4);
	}
}

json& GameDataHandler::getSection(const std::string& key) { return m_loadedData[key]; }

const json& GameDataHandler::getSection(const std::string& key) const { return m_loadedData.at(key); }


/*--- legacy -----------------------------*/


void GameDataHandler::loadGameData(const std::string& filePath) {
	std::vector<std::string> content;
	content = fileHandler->readFile(filePath.c_str());

	std::pair<std::string, std::string> idAndData;

	for (auto& e : content) {
		idAndData = Tools::splitStr(e, '=');

		dataTable[idAndData.first] = idAndData.second;
	}

	std::cout << "Game Data loaded!" << std::endl;
}

void GameDataHandler::saveGameData(const std::string& filePath) {
	std::vector<std::string> data;

	for (auto& e : saveQueue) {
		std::ostringstream oss;
		oss << e.first << "=" << e.second;
		std::string push = oss.str();
		data.push_back(push);
	}

	fileHandler->writeFile(filePath.c_str(), data);

	std::cout << "Game Data saved!" << std::endl;
}

void GameDataHandler::addToSaveQueue(const std::string& id, const std::string& data) {
	saveQueue[id] = data;
}

std::string& GameDataHandler::getData(const std::string& id) {
	return dataTable[id];
}