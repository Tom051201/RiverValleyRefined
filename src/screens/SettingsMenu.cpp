#include "SettingsMenu.h"
#include "MainMenu.h"
#include <iostream>

SettingsMenu::SettingsMenu(Application* app) {
	this->app = app;
	app->showCursor();

	guiHandler = std::make_unique<GuiHandler>();

	m_backMainButton = std::make_shared<Button>(app, app->getWindowWidth() / 2 - 150, app->getWindowHeight() / 2 + 100, 300, 70, [this, app]() { app->setActiveScreen(std::make_unique<MainMenu>(app)); });
	m_backMainButton->setText("Main Menu");
	m_backMainButton->setTextSize(24);
	m_backMainButton->setFunctionForPosX([app]() { return app->getWindowWidth() / 2 - 150; });
	m_backMainButton->setFunctionForPosY([app]() { return app->getWindowHeight() / 2 + 100; });
	guiHandler->addElement(m_backMainButton);

	m_backGameButton = std::make_shared<Button>(app, app->getWindowWidth() / 2 - 150, app->getWindowHeight() / 2 + 200, 300, 70, []() {});
	m_backGameButton->setText("Resume Game");
	m_backGameButton->setTextSize(24);
	m_backGameButton->setFunctionForPosX([app]() { return app->getWindowWidth() / 2 - 150; });
	m_backGameButton->setFunctionForPosY([app]() { return app->getWindowHeight() / 2 + 200; });
	guiHandler->addElement(m_backGameButton);

	m_musicToggle = std::make_shared<Toggle>(app, app->getWindowWidth() / 2 - 10 - 50, app->getWindowHeight() / 2 - 100, 50, 20,
		[app]() {
			app->globalSounds->allowMusic();
			app->globalSounds->loopMusic(GlobalMusicList::POSSESSED_MIND);
		},
		[app]() {
			app->globalSounds->forbidMusic();
			app->globalSounds->stopMusic();
		}
	);
	if (!app->globalSounds->isMusicAllowed()) { m_musicToggle->silentTurnOff(); }
	m_musicToggle->setFunctionForPosX([app]() { return app->getWindowWidth() / 2 - 10 - 50; });
	m_musicToggle->setFunctionForPosY([app]() { return app->getWindowHeight() / 2 - 100; });
	guiHandler->addElement(m_musicToggle);

	m_SFXToggle = std::make_shared<Toggle>(app, app->getWindowWidth() / 2 - 10 - 50, app->getWindowHeight() / 2 - 75, 50, 20,
		[app]() {
			app->globalSounds->allowSounds();
			app->globalSounds->playEffect(GlobalSoundsList::UI_CLICK, Channels::GUI_INTERACTION); // just so it plays a sound when activating (only here needed!)
		},
		[app]() {
			app->globalSounds->forbidSounds();
		}
	);
	if (!app->globalSounds->isSoundsAllowed()) { m_SFXToggle->silentTurnOff(); }
	m_SFXToggle->setFunctionForPosX([app]() { return app->getWindowWidth() / 2 - 10 - 50; });
	m_SFXToggle->setFunctionForPosY([app]() { return app->getWindowHeight() / 2 - 75; });
	guiHandler->addElement(m_SFXToggle);

	m_nightShadingToggle = std::make_shared<Toggle>(app, app->getWindowWidth() / 2 - 10 - 50, app->getWindowHeight() / 2 - 125, 50, 20,
		[app]() {
			app->settingsHandler->setData("renderNightShading", "true");
		},
		[app]() {
			app->settingsHandler->setData("renderNightShading", "false");
		}
	);
	if (app->settingsHandler->getData("renderNightShading") == "false") { m_nightShadingToggle->silentTurnOff(); }
	m_nightShadingToggle->setFunctionForPosX([app]() { return app->getWindowWidth() / 2 - 10 - 50; });
	m_nightShadingToggle->setFunctionForPosY([app]() { return app->getWindowHeight() / 2 - 125; });
	guiHandler->addElement(m_nightShadingToggle);

	m_wallpaper = TextureManager::loadTexture("res/assets/wallpaper.png");
	m_wallpaperSrc = { 0, 0, 1920, 1080 };
	m_wallpaperDst = { 0, 0, app->getWindowWidth(), app->getWindowHeight() };
}

SettingsMenu::~SettingsMenu() {
	SDL_DestroyTexture(m_wallpaper);
}

void SettingsMenu::render() {
	TextureManager::render(m_wallpaper, m_wallpaperSrc, m_wallpaperDst);
	SDL_SetRenderDrawColor(app->renderer, 60, 60, 60, 180);
	SDL_RenderFillRect(app->renderer, &m_wallpaperDst);

	TextManager::renderStringInRect("Settings", Fonts::DEFAULT, 80, { 255, 255, 255, 255 }, m_wallpaperDst);

	guiHandler->render();

	TextManager::renderString("Music", Fonts::DEFAULT, FontSize::DEFAULT_MID, { 0, 0, 0, 255 }, app->getWindowWidth() / 2 + 10, app->getWindowHeight() / 2 - 100);
	TextManager::renderString("SFX", Fonts::DEFAULT, FontSize::DEFAULT_MID, { 0, 0, 0, 255 }, app->getWindowWidth() / 2 + 10, app->getWindowHeight() / 2 - 75);
	TextManager::renderString("Render Night Shading", Fonts::DEFAULT, FontSize::DEFAULT_MID, { 0, 0, 0, 255 }, app->getWindowWidth() / 2 + 10, app->getWindowHeight() / 2 - 125);
}

void SettingsMenu::update() {}

void SettingsMenu::handleEvents() {
	guiHandler->handleEvents();
}

void SettingsMenu::windowResized() {
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
