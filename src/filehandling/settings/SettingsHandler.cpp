#include "SettingsHandler.h"
#include <vector>
#include "../../Utils.h"
#include <sstream>
#include <iostream>
#include <string>
#include <map>

SettingsHandler::SettingsHandler() {
	fileHandler = std::make_unique<FileHandler>();
}

SettingsHandler::~SettingsHandler() {
}

void SettingsHandler::loadSettings(const std::string& path) {
	std::vector<std::string> content;
	content = fileHandler->readFile(path.c_str());

	std::pair<std::string, std::string> idAndData;

	for (auto& e : content) {
		idAndData = Tools::splitStr(e, '=');

		for (auto& e : content) {
			idAndData = Tools::splitStr(e, '=');

			dataTable[idAndData.first] = idAndData.second;
		}
	}

	std::cout << "Settings loaded!" << std::endl;

}

void SettingsHandler::saveSettings(const std::string& path) {

	std::vector<std::string> add;
	for (const auto& e : saveQueue) {
		std::ostringstream oss;
		oss << e.first << "=" << e.second;
		std::string push = oss.str();
		add.push_back(push);
	}

	fileHandler->writeFile(path.c_str(), add);

	std::cout << "Settings saved!" << std::endl;
}

void SettingsHandler::addToSaveQueue(std::string identifier, std::string info) {
	saveQueue[identifier] = info;
}

void SettingsHandler::addDataTableToSaveQueue() {
	for (const auto& e : dataTable) {
		addToSaveQueue(e.first, e.second);
	}
}

std::string& SettingsHandler::getData(const std::string& id) {
	return dataTable[id];
}

void SettingsHandler::setData(const std::string& id, const std::string& data) {
	this->dataTable[id] = data;
}