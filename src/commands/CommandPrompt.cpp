#include "CommandPrompt.h"
#include <iostream>
#include "../Utils.h"

CommandPrompt::CommandPrompt(Application* app, Game* game) {
	this->app = app;
	this->dim = { 50, app->getWindowHeight() - 50, 400, 20 };
	this->isFocused = false;

	m_indicatorTimer = 0;

	handler = std::make_unique<CommandHandler>(app, game);
}

CommandPrompt::~CommandPrompt() {
//	std::cout << "CommandPrompt deleted" << std::endl;
}

void CommandPrompt::update() {

}

void CommandPrompt::render() {
	if (isFocused) {
		// renders background
		SDL_SetRenderDrawColor(app->renderer, 90, 75, 80, 145);
		SDL_RenderFillRect(app->renderer, &dim);

		// renders line
		if (!line.empty()) {
			TextManager::renderString(line.c_str(), Fonts::OPEN_SANS_REGULAR, FontSize::DEFAULT_MID, { 0, 0, 0, 255 }, dim.x, dim.y);
		}

		// renders indicator
		m_indicatorTimer++;
		SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
		if (m_indicatorTimer >= 60 / 2) {
			TextManager::renderString("|", Fonts::OPEN_SANS_REGULAR, FontSize::DEFAULT_MID, { 0, 0, 0, 255 }, getIndicatorX(), dim.y);
			if (m_indicatorTimer == 60) {
				m_indicatorTimer = 0;
			}
		}
	}
	
	
	
}

void CommandPrompt::handleEvents(SDL_Event event) {
	if (isFocused) {
		if (event.type == SDL_KEYDOWN) {
			SDL_Keycode keyCode = event.key.keysym.sym;

			// handle letters and symbols
			if (keyCode >= SDLK_a && keyCode <= SDLK_z && keyCode != SDLK_BACKSPACE || keyCode >= SDLK_0 && keyCode <= SDLK_9 || keyCode == SDLK_SPACE || keyCode == SDLK_PERIOD) {
				char letter = static_cast<char>(keyCode);

				// handle shift
				if (event.key.keysym.mod & KMOD_SHIFT) {
					letter = std::toupper(letter);
				}

				line += letter;
				//std::cout << line << std::endl;
				
			}

			// handle backspace
			if (keyCode == SDLK_BACKSPACE && !line.empty()) {
				line.pop_back();
			}

			// handle enter
			if (keyCode == SDLK_RETURN) {
				execute();
			}

		}
	}
}

void CommandPrompt::windowResized() {
	this->dim = { 50, app->getWindowHeight() - 50, 400, 20 };
}

void CommandPrompt::open() {
	isFocused = true;
}

void CommandPrompt::close() {
	isFocused = false;
	line.clear();
}

bool CommandPrompt::isOpen() {
	return isFocused;
}

void CommandPrompt::execute() {
	handler->executeCommand(line);
	line.clear();
	isFocused = false;
}

int CommandPrompt::getIndicatorX() {

	TTF_Font* font = TTF_OpenFont(Fonts::OPEN_SANS_REGULAR, FontSize::DEFAULT_MID);
	if (!font) { std::cerr << "Failed to load font: " << TTF_GetError() << std::endl; }

	int size = dim.x + TextManager::getTextWidth(font, line) - 2;

	TTF_CloseFont(font);

	return size;
}
