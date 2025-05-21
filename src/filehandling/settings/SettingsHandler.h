#pragma once
#include "../../Utils.h"
#include <map>
#include <memory>

class SettingsHandler {
public:

	SettingsHandler();
	~SettingsHandler();

	void loadSettings(const std::string& path);
	void saveSettings(const std::string& path);

	void addToSaveQueue(std::string identifier, std::string info);
	void addDataTableToSaveQueue();
	std::string& getData(const std::string& id);
	void setData(const std::string& id, const std::string& data);

private:

	std::unique_ptr<FileHandler> fileHandler;
	std::map<std::string, std::string> dataTable;
	std::map<std::string, std::string> saveQueue;

};
