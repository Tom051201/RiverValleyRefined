#pragma once
#include "SDL.h"
#include <string>
#include "../Application.h"
#include "CommandHandler.h"
#include "../screens/Game.h"

class CommandHandler;

class CommandPrompt {
public:

	CommandPrompt(Application* app, Game* game);
	~CommandPrompt();

	void update();
	void render();
	void handleEvents(SDL_Event event);
	void windowResized();

	void open();
	void close();
	bool isOpen();

private:

	Application* app;
	SDL_Rect dim;
	bool isFocused;
	std::string line;
	std::unique_ptr<CommandHandler> handler;

	int m_indicatorTimer;

	void execute();

	int getIndicatorX();

};
