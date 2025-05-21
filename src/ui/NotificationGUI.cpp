#include "NotificationGUI.h"
#include "../Utils.h"

NotificationHandler::NotificationHandler(Application* app) {
	this->app = app;
	dim = { app->getWindowWidth() / 2 - 150, 50, 300, 40 };
	msg = " ";
	isVisible = false;
	counter = upTime;
}

NotificationHandler::~NotificationHandler() {

}

void NotificationHandler::render() {
	if (isVisible) {
		SDL_SetRenderDrawColor(app->renderer, 200, 200, 200, counter);
		SDL_RenderFillRect(app->renderer, &dim);

		TextManager::renderStringInRect(msg.c_str(), Fonts::DEFAULT, FontSize::DEFAULT, { 0, 0, 0, (Uint8)counter }, dim);
	}
}

void NotificationHandler::update() {
	if (isVisible) {
		if (counter > 1) {
			counter--;
		}
		else {
			isVisible = false;
			counter = upTime;
		}
	}
}

void NotificationHandler::handleEvents() {

}

void NotificationHandler::windowResized() {
	dim = { app->getWindowWidth() / 2 - 150, 50, 300, 40 };
}

void NotificationHandler::launch(const std::string& msg) {
	this->msg = msg;
	this->isVisible = true;
	app->globalSounds->playEffect(GlobalSoundsList::NOTIFICATION, Channels::GUI_INTERACTION);
}
