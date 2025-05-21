#pragma once
#include "SDL.h"
#include "../Application.h"
#include <string>

class Application;

class NotificationHandler {
public:

	NotificationHandler(Application* app);
	~NotificationHandler();

	void render();
	void update();
	void handleEvents();
	void windowResized();

	void launch(const std::string& msg);

private:

	Application* app;
	SDL_Rect dim;
	std::string msg;
	bool isVisible;
	const int upTime = 255;
	int counter;

};
