#pragma once
#include "screens/Screen.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "filehandling/settings/SettingsHandler.h"
#include "sounds/SoundHandler.h"
#include "ui/NotificationGUI.h"
#include <memory>

#include "MemoryTracker.h"

class NotificationHandler;

class Application {
public:

	static SDL_Renderer* renderer;
	std::unique_ptr<SettingsHandler> settingsHandler;
	std::unique_ptr<SoundHandler> globalSounds;
	std::unique_ptr<NotificationHandler> notificationHandler;

	Application(MemoryMetrics& memoryMetrics);
	~Application();

	void init();
	void setupApp();
	void setupWindow();
	void startLoop();
	void close();

	void setActiveScreen(std::unique_ptr<Screen> screen);

	void render();
	void update();
	void handleEvents();
	void windowResized();

	bool isRunning();
	void setRunning(bool run);
	SDL_Event& getAppEvent();

	int getWindowWidth();
	int getWindowHeight();

	void hideCursor();
	void showCursor();
	bool isCursorHidden();
	void setCursorPosition(int x, int y);
	void centerCursorInWindow();

private:

	std::unique_ptr<Screen> m_activeScreen;
	SDL_Window* window;
	SDL_Event appEvent;
	bool running;

	const int FPS = 60;
	const int FRAME_DELAY = 1000 / FPS;

	int TPS = 60;
	int tickInverval = 1000 / TPS;

	MemoryMetrics* m_memoryMetrics;
	void renderMemoryUsage();
};
