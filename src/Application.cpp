#include "Application.h"
#include <iostream>
#include "sounds/SoundHandler.h"

SDL_Renderer* Application::renderer = nullptr;

Application::Application(MemoryMetrics& memoryMetrics) {
	settingsHandler = std::make_unique<SettingsHandler>();
	settingsHandler->loadSettings("res/data/settings.txt");

	m_memoryMetrics = &memoryMetrics;
}

Application::~Application() {
	settingsHandler->addToSaveQueue("music", Tools::boolToStr(globalSounds->isMusicAllowed()));
	settingsHandler->addToSaveQueue("SFX", Tools::boolToStr(globalSounds->isSoundsAllowed()));
	settingsHandler->addToSaveQueue("renderNightShading", settingsHandler->getData("renderNightShading"));
	settingsHandler->saveSettings("res/data/settings.txt");
}

void Application::setupApp() {
	SDL_SetWindowResizable(window, SDL_FALSE);

	SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
	SDL_RenderClear(renderer);
	SDL_Rect screenDim = { 0, 0, getWindowWidth(), getWindowHeight() };
	TextManager::renderStringInRect(
		"Loading...",
		Fonts::DEFAULT,
		40,
		{ 255, 255, 255, 255 },
		screenDim
	);
	SDL_RenderPresent(renderer);
	SDL_Delay(10);

	notificationHandler = std::make_unique<NotificationHandler>(this);

	globalSounds = std::make_unique<SoundHandler>();
	if (settingsHandler->getData("music") == "false") { globalSounds->forbidMusic(); }
	if (settingsHandler->getData("SFX") == "false") { globalSounds->forbidSounds(); }

	globalSounds->loadMusic(GlobalMusicList::POSSESSED_MIND, "res/sounds/Possessed_Mind.mp3");
	globalSounds->loadMusic(GlobalMusicList::YOU_KNOW_THE_VIBE, "res/sounds/youknowthevibe.mp3");

	globalSounds->loadEffect(GlobalSoundsList::ERROR_SOUND, "res/sounds/error.mp3");
	globalSounds->loadEffect(GlobalSoundsList::NOTIFICATION, "res/sounds/notification.mp3");
	globalSounds->setEffectVolume(GlobalSoundsList::NOTIFICATION, 40);
	globalSounds->loadEffect(GlobalSoundsList::UI_CLICK, "res/sounds/uiclick.mp3");
	globalSounds->setEffectVolume(GlobalSoundsList::UI_CLICK, 5);
	globalSounds->loadEffect(GlobalSoundsList::BREAK_ROCK, "res/sounds/break_rock.mp3");
	globalSounds->setEffectVolume(GlobalSoundsList::BREAK_ROCK, 5);
	globalSounds->loadEffect(GlobalSoundsList::CHOPPING_TREE, "res/sounds/chopping_tree.mp3");
	globalSounds->setEffectVolume(GlobalSoundsList::CHOPPING_TREE, 10);
	globalSounds->loadEffect(GlobalSoundsList::EQUIP_ITEM, "res/sounds/equip_leather.mp3");
	globalSounds->loadEffect(GlobalSoundsList::THUNDER, "res/sounds/thunder.mp3");
	globalSounds->setEffectVolume(GlobalSoundsList::THUNDER, 10);
	globalSounds->loadEffect(GlobalSoundsList::AMBIENT_RAIN, "res/sounds/rain.mp3");
	globalSounds->setEffectVolume(GlobalSoundsList::AMBIENT_RAIN, 10);
	globalSounds->loadEffect(GlobalSoundsList::AMBIENT_CLEAR, "res/sounds/ambient_clear.mp3");
	globalSounds->setEffectVolume(GlobalSoundsList::AMBIENT_CLEAR, 50);

	globalSounds->loopMusic(GlobalMusicList::POSSESSED_MIND);
	globalSounds->setMusicVolume(2);



	SDL_SetWindowResizable(window, SDL_TRUE);
}

void Application::init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "SDL initialized!" << std::endl;
		running = true;
	}
	else {
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		running = false;
	}

	if (IMG_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "IMG initialized!" << std::endl;
	}
	else {
		std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
		SDL_Quit();
		IMG_Quit();
		running = false;
	}

	if (TTF_Init() == 0) {
		std::cout << "TTF initialized!" << std::endl;
	}
	else {
		std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
		SDL_Quit();
		IMG_Quit();
		TTF_Quit();
		running = false;
	}

	if (Mix_Init(MIX_INIT_MP3) == 0) {
		std::cerr << "Mix_Init failed! " << Mix_GetError() << std::endl;
		SDL_Quit();
		running = false;
	}
	else {
		std::cout << "Mix initialized!" << std::endl;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
		std::cerr << "Mix_OpenAudio failed! " << Mix_GetError() << std::endl;
		Mix_Quit();
		SDL_Quit();
		running = false;
	}
	else {
		std::cout << "Mix_OpenAudio successful!" << std::endl;
	}
	Mix_AllocateChannels(Channels::MAX_CHANNELS);
}

void Application::setupWindow() {
	// setup window
	window = SDL_CreateWindow("RiverValley", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE);
	if (window) {
		std::cout << "Window created!" << std::endl;
	}
	else {
		std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
		SDL_Quit();
	}

	// setup renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC); // may add ' | SDL_RENDERER_ACCELERATED' as flag
	if (renderer) {
		std::cout << "Renderer created!" << std::endl;
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	}
	else {
		std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	// setup window icon
	SDL_Surface* iconSurface = IMG_Load("res/assets/rivervalley.png");
	if (!iconSurface) {
		std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
	}
	else {
		SDL_SetWindowIcon(window, iconSurface);
		SDL_FreeSurface(iconSurface);
	}

}

void Application::startLoop() {
	Uint32 lastFrameTime = SDL_GetTicks();
	Uint32 lastTickTime = SDL_GetTicks();
	Uint32 frameStart;
	Uint32 frameTime;

	int frameCount = 0;         // Tracks number of frames
	Uint32 fpsTimer = SDL_GetTicks(); // Tracks time for FPS calculation
	int fps = 0;

	while (running) {
		frameStart = SDL_GetTicks();

		while (SDL_PollEvent(&appEvent)) {
			if (appEvent.type == SDL_QUIT) {
				running = false;
			}
			if (appEvent.window.event == SDL_WINDOWEVENT_RESIZED) {
				windowResized();
			}
			handleEvents();
		}

		Uint32 currentTickTime = SDL_GetTicks();
		if (currentTickTime - lastTickTime >= tickInverval) {
			double deltaTime = (currentTickTime - lastTickTime) / 1000.0;
			update();
			lastTickTime = currentTickTime;
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		render();
		renderMemoryUsage();

		// FPS Calculation
		frameCount++;
		if (SDL_GetTicks() - fpsTimer >= 1000) { // One second passed
			fps = frameCount / ((SDL_GetTicks() - fpsTimer) / 1000.0);
			frameCount = 0;
			fpsTimer = SDL_GetTicks();
		}

		const std::string& fpsText = std::to_string(fps) + std::string(" FPS");
		TextManager::renderString(fpsText.c_str(), Fonts::OPEN_SANS_REGULAR, FontSize::DEFAULT_SMALL, { 0, 0, 0, 255 }, 10, 10);

		SDL_RenderPresent(renderer);

		frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < FRAME_DELAY) {
			SDL_Delay(FRAME_DELAY - frameTime);
		}

		lastFrameTime = SDL_GetTicks();
	}
}

void Application::close() {
	settingsHandler->saveSettings("res/data/settings.txt");

	SDL_SetRelativeMouseMode(SDL_FALSE);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	std::cout << "Application cleared!" << std::endl;
}

void Application::setActiveScreen(std::unique_ptr<Screen> screen) {
	m_activeScreen = std::move(screen);
}

void Application::render() {
	if (m_activeScreen != nullptr) { m_activeScreen->render(); }
	notificationHandler->render();
}

void Application::update() {
	if (m_activeScreen != nullptr) { m_activeScreen->update(); }
	notificationHandler->update();
}

void Application::handleEvents() {
	if (m_activeScreen != nullptr) { m_activeScreen->handleEvents(); }
	notificationHandler->handleEvents();
}

void Application::windowResized() {
	if (m_activeScreen != nullptr) { m_activeScreen->windowResized(); }
	notificationHandler->windowResized();
}

bool Application::isRunning() { return running; }

SDL_Event& Application::getAppEvent() { return appEvent; }

int Application::getWindowWidth() {
	int w;
	SDL_GetWindowSize(window, &w, nullptr);
	return w;
}

int Application::getWindowHeight() {
	int h;
	SDL_GetWindowSize(window, nullptr, &h);
	return h;
}

void Application::hideCursor() { SDL_SetRelativeMouseMode(SDL_TRUE); }

void Application::showCursor() { SDL_SetRelativeMouseMode(SDL_FALSE); }

bool Application::isCursorHidden() { return SDL_GetRelativeMouseMode(); }

void Application::setCursorPosition(int x, int y) { SDL_WarpMouseInWindow(window, x, y); }

void Application::centerCursorInWindow() { SDL_WarpMouseInWindow(window, getWindowWidth() / 2, getWindowHeight() / 2); }

void Application::setRunning(bool run) { running = run; }

void Application::renderMemoryUsage() {
	std::string line = "> Heap usage: " + std::to_string(m_memoryMetrics->currentHeapUsage()) + " bytes";
	TextManager::renderString(line.c_str(), Fonts::OPEN_SANS_REGULAR, FontSize::DEFAULT_SMALL, { 0, 0, 0, 255 }, 10, 300);

	line = "> Unfreed allocations: " + std::to_string(m_memoryMetrics->m_countAllocated);
	TextManager::renderString(line.c_str(), Fonts::OPEN_SANS_REGULAR, FontSize::DEFAULT_SMALL, { 0, 0, 0, 255 }, 10, 310);
}
