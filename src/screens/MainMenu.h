#pragma once
#include "Screen.h"
#include "../Application.h"
#include "../ui/Gui.h"
#include "../ui/GuiHandler.h"

class MainMenu : public Screen {
public:

	std::unique_ptr<GuiHandler> guiHandler;

	MainMenu(Application* app);
	~MainMenu() override;

	void render() override;
	void update() override;
	void handleEvents() override;
	void windowResized() override;

private:

	Application* app;

	std::shared_ptr<Button> m_startGameButton;
	std::shared_ptr<Button> m_settingsButton;
	std::shared_ptr<Button> m_closeGameButton;
	std::shared_ptr<Selector> m_gameSaveSelector;

	SDL_Texture* m_wallpaper;
	SDL_Rect m_wallpaperSrc, m_wallpaperDst;

	std::vector<std::string> prepareGameSaves();
	void launchGameSave();

};
