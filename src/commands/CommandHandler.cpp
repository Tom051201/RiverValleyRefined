#include "CommandHandler.h"
#include <iostream>

CommandHandler::CommandHandler(Application* app, Game* game) {
	this->app = app;
	this->game = game;

	registerCommand("tph", [game, app]() {
		game->player->toggleHitBox();
		app->notificationHandler->launch("Player Hitbox Toggled");
	});

	registerCommand("tpf", [game]() {
		switch (game->player->getFacing()) {
		case Facing::UP:
			game->player->changeWorldY(-64);
			break;

		case Facing::DOWN:
			game->player->changeWorldY(64);
			break;

		case Facing::LEFT:
			game->player->changeWorldX(-64);
			break;

		case Facing::RIGHT:
			game->player->changeWorldX(64);
			break;

		default:
			break;
		}
	});

	registerCommand("create", [game]() {
		game->player->toggleCreateMode();
	});

	registerCommandWithParameter("time", [game](std::string& in) {
		float time = std::stof(in);
		game->dayNightHandler->setTime(time);
	});

	registerCommandWithParameter("weather", [game](std::string& in) {
		if (in == "clear") { game->weatherHandler->setWeather(WeatherType::Clear); }
		else if (in == "rain") { game->weatherHandler->setWeather(WeatherType::Rain); }
	});

}

CommandHandler::~CommandHandler() {}

void CommandHandler::registerCommand(const std::string& key, std::function<void()> func) { commands[key] = func; }

void CommandHandler::registerCommandWithParameter(const std::string& key, std::function<void(std::string&)> func) { commandsWithParameter[key] = func; }

void CommandHandler::executeCommand(const std::string& key) {
	if (commands.find(key) != commands.end()) {
		commands[key]();
		return;
	}
	
	if (key.find(' ') != std::string::npos) {
		std::pair<std::string, std::string> keyAndParameter = Tools::splitStr(key, ' ');

		if (commandsWithParameter.find(keyAndParameter.first) != commandsWithParameter.end()) {
			commandsWithParameter[keyAndParameter.first](keyAndParameter.second);
			return;
		}
	}
}
