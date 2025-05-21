#include "MainMenu.h"
#include "Game.h"
#include "SettingsMenu.h"
#include <iostream>

#include <filesystem>
#include <vector>
#include <sstream>

namespace fs = std::filesystem;

MainMenu::MainMenu(Application* app) {
	this->app = app;
	app->showCursor();

	guiHandler = std::make_unique<GuiHandler>();

	m_startGameButton = std::make_shared<Button>(app, app->getWindowWidth() / 2 - 150, app->getWindowHeight() / 2 + 100, 300, 70, [this, app]() { launchGameSave(); });
	m_startGameButton->setText("Start Game");
	m_startGameButton->setTextSize(24);
	m_startGameButton->setFunctionForPosX([app]() { return app->getWindowWidth() / 2 - 150; });
	m_startGameButton->setFunctionForPosY([app]() { return app->getWindowHeight() / 2 + 100; });
	guiHandler->addElement(m_startGameButton);

	m_settingsButton = std::make_shared<Button>(app, app->getWindowWidth() / 2 - 150, app->getWindowHeight() / 2 + 200, 300, 70, [this, app]() { app->setActiveScreen(std::make_unique<SettingsMenu>(app)); });
	m_settingsButton->setText("Settings");
	m_settingsButton->setTextSize(24);
	m_settingsButton->setFunctionForPosX([app]() { return app->getWindowWidth() / 2 - 150; });
	m_settingsButton->setFunctionForPosY([app]() { return app->getWindowHeight() / 2 + 200; });
	guiHandler->addElement(m_settingsButton);

	m_closeGameButton = std::make_shared<Button>(app, app->getWindowWidth() / 2 - 150, app->getWindowHeight() / 2 + 300, 300, 70, [app]() { app->setRunning(false); });
	m_closeGameButton->setText("Close Game");
	m_closeGameButton->setTextSize(24);
	m_closeGameButton->setFunctionForPosX([app]() { return app->getWindowWidth() / 2 - 150; });
	m_closeGameButton->setFunctionForPosY([app]() { return app->getWindowHeight() / 2 + 300; });
	guiHandler->addElement(m_closeGameButton);

	m_gameSaveSelector = std::make_shared<Selector>(app, app->getWindowWidth() / 2 + 170, app->getWindowHeight() / 2 + 100, 200, 170);
	m_gameSaveSelector->setTexts(prepareGameSaves());
	m_gameSaveSelector->setFunctionForPosX([app]() { return app->getWindowWidth() / 2 + 170; });
	m_gameSaveSelector->setFunctionForPosY([app]() { return app->getWindowHeight() / 2 + 100; });
	guiHandler->addElement(m_gameSaveSelector);

	m_wallpaper = TextureManager::loadTexture("res/assets/wallpaper.png");
	m_wallpaperSrc = { 0, 0, 1920, 1080 };
	m_wallpaperDst = { 0, 0, app->getWindowWidth(), app->getWindowHeight() };
	
}

MainMenu::~MainMenu() {
	SDL_DestroyTexture(m_wallpaper);
}

void MainMenu::render() {
	TextureManager::render(m_wallpaper, m_wallpaperSrc, m_wallpaperDst);

	TextManager::renderStringInRect("RiverValley", Fonts::DEFAULT, 80, { 255, 255, 255, 255 }, m_wallpaperDst);

	guiHandler->render();
}

void MainMenu::update() {
	guiHandler->update();
}

void MainMenu::handleEvents() {
	guiHandler->handleEvents();
}

void MainMenu::windowResized() {

	// scale wallpaper
	int texW = TextureManager::getTextureWidth(m_wallpaper);
	int texH = TextureManager::getTextureHeight(m_wallpaper);
	int wW = app->getWindowWidth();
	int wH = app->getWindowHeight();
	float scaleX = (float)wW / texW;
	float scaleY = (float)wH / texH;
	float scale = std::max(scaleX, scaleY);
	int renderW = (int)(texW * scale);
	int renderH = (int)(texH * scale);
	int posX = (wW - renderW) / 2;
	int posY = (wH - renderH) / 2;
	m_wallpaperDst = { posX, posY, renderW, renderH };

	guiHandler->windowResized();
}

std::vector<std::string> MainMenu::prepareGameSaves() {

	std::string path = "res/saves"; // Change this to your folder path
	std::vector<std::string> folders;

	try {
		for (const auto& entry : fs::directory_iterator(path)) {
			if (fs::is_directory(entry.path())) {
				folders.push_back(entry.path().string());
			}
		}
		
		for (auto& folder : folders) {
			std::pair<std::string, std::string> p = Tools::splitStr(folder, '\\');
			folder = p.second;
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
	
	return folders;
}

void MainMenu::launchGameSave() {
	
	std::stringstream ss;

	ss << "res/saves/" << m_gameSaveSelector->getCurrentText();
	std::string folderPath = ss.str();
	ss.str("");
	ss.clear();

	app->setActiveScreen(std::make_unique<Game>(app, folderPath.c_str()));
}
