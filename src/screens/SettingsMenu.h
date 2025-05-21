#pragma once
#include "Screen.h"
#include "../Application.h"
#include "../ui/GuiHandler.h"

class SettingsMenu : public Screen {
public:

	std::unique_ptr<GuiHandler> guiHandler;

	SettingsMenu(Application* app);
	~SettingsMenu();

	void render() override;
	void update() override;
	void handleEvents() override;
	void windowResized() override;

private:

	Application* app;

	std::shared_ptr<Button> m_backMainButton;
	std::shared_ptr<Button> m_backGameButton;
	std::shared_ptr<Toggle> m_musicToggle;
	std::shared_ptr<Toggle> m_SFXToggle;
	std::shared_ptr<Toggle> m_nightShadingToggle;

	SDL_Texture* m_wallpaper;
	SDL_Rect m_wallpaperSrc, m_wallpaperDst;

};
