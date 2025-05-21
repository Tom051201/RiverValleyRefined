#pragma once
#include <map>
#include <functional>
#include <string>
#include "../Application.h"
#include "../screens/Game.h"

class CommandHandler {
public:

	CommandHandler(Application* app, Game* game);
	~CommandHandler();

	void registerCommand(const std::string& key, std::function<void()> func);
	void registerCommandWithParameter(const std::string& key, std::function<void(std::string&)> func);

	void executeCommand(const std::string& key);

private:

	Application* app;
	Game* game;
	std::map<std::string, std::function<void()>> commands;
	std::map<std::string, std::function<void(std::string&)>> commandsWithParameter;

};
