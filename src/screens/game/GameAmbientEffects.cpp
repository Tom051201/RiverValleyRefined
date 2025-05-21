#include "GameAmbientEffects.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <iomanip>


/*------- Day Night Ambient ---------------------------------------------------------------------------------------*/


DayNightHandler::DayNightHandler(Application* app) {
	this->app = app;
	this->m_time = 0.0f;
	this->m_timeSpeed = 0.1f;
	this->m_dayDurationSeconds = 600.0f; // 600.0f = 10m
	this->m_startTicks = SDL_GetTicks();
	this->m_currentTimeState = TimeState::Night;
	this->m_previousTimeState = TimeState::Night;
	this->m_paused = false;
	this->m_pauseTicks = 0;
	if (app->settingsHandler->getData("renderNightShading") == "true") { m_allowNightShading = true; }
	else { m_allowNightShading = false; }

	//loadData(game->dataHandler);
}

DayNightHandler::~DayNightHandler() {
//	std::cout << "DayNightHandler deleted" << std::endl;
}

void DayNightHandler::setup(Game* game) {
	m_timeIcon = std::make_shared<InfoIcon>(*game->guiHandler, UiSheetType::AmbientSheet, 0, 0,
		(game->miniMapUI->getDim().x * 2) + game->miniMapUI->getDim().w,
		game->miniMapUI->getDim().y, 64, 64);
	m_timeIcon->setFunctionForPosX([game]() { return (game->miniMapUI->getDim().x * 2) + game->miniMapUI->getDim().w; });
	m_timeIcon->setFunctionForPosY([game]() { return game->miniMapUI->getDim().y; });
	game->guiHandler->addElement(m_timeIcon);
}

void DayNightHandler::update() {

	Uint32 currentTicks = SDL_GetTicks();
	float elapsedSeconds = (currentTicks - m_startTicks) / 1000.0f;
	m_time = fmodf(elapsedSeconds / m_dayDurationSeconds, 1.0f);

	TimeState newState = getTimeState(m_time);
	if (newState != m_currentTimeState) {
		m_previousTimeState = m_currentTimeState;
		m_currentTimeState = newState;

		onTimeStateChange(m_currentTimeState);
	}

}

void DayNightHandler::render() {

	if (m_allowNightShading) {
		// renders darkness effect
		float darkness = 1.0f - getBrightness();
		Uint8 alpha = static_cast<Uint8>(darkness * 180);

		SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, alpha);

		SDL_Rect screenDim = { 0, 0, app->getWindowWidth(), app->getWindowHeight() };
		SDL_RenderFillRect(app->renderer, &screenDim);
	}

	m_timeIcon->setDescription(getFormattedTime(m_time));
}

void DayNightHandler::windowResized() {}

void DayNightHandler::enableGUI() {
	m_timeIcon->enable();
}

void DayNightHandler::disableGUI() {
	m_timeIcon->disable();
}

void DayNightHandler::setTime(float time) {
	time = fmodf(time, 1.0f);

	Uint32 currentTicks = SDL_GetTicks();
	float targetElapsed = time * m_dayDurationSeconds;
	m_startTicks = currentTicks - static_cast<Uint32>(targetElapsed * 1000.0f);

	this->update();
}

float DayNightHandler::getTime() { return this->m_time; }

float DayNightHandler::getBrightness() const { return 0.5f * std::sinf(m_time * 2.0f * M_PI - M_PI / 2.0f) + 0.5f; }

void DayNightHandler::loadData(const json& data) {
	if (data.contains("Time")) {
		float loadedTime = data["Time"];
		setStartFromFile(loadedTime);
	}
}

void DayNightHandler::saveData(json& data) const {
	data["Time"] = m_time;
}

void DayNightHandler::setStartFromFile(float loadedTime) {
	Uint32 currentTicks = SDL_GetTicks();
	float elapsedSecs = loadedTime * m_dayDurationSeconds;
	m_startTicks = currentTicks - static_cast<Uint32>(elapsedSecs * 1000.0f);
}

TimeState DayNightHandler::getTimeState(float time) {
	if (time >= c_sunriseBegin && time < c_dayBegin)
		return TimeState::Sunrise;
	else if (time >= c_dayBegin && time < c_sunsetBegin)
		return TimeState::Day;
	else if (time >= c_sunsetBegin && time < c_nightBegin)
		return TimeState::Sunset;
	else
		return TimeState::Night;
}

void DayNightHandler::onTimeStateChange(TimeState newState) {
	switch (newState) {
	case TimeState::Sunrise:
		m_timeIcon->setSrcX(32);
		break;
	case TimeState::Day:
		m_timeIcon->setSrcX(16);
		break;
	case TimeState::Sunset:
		m_timeIcon->setSrcX(48);
		break;
	case TimeState::Night:
		m_timeIcon->setSrcX(0);
		break;
	}
}

std::string DayNightHandler::getFormattedTime(float time) {
	// game time to 24h format
	float hours24 = time * 24.0f;
	int hours = static_cast<int>(hours24);
	int minutes = static_cast<int>((hours24 - hours) * 60);

	// to 12h format
	bool isPM = hours >= 12;
	int hours12 = hours % 12;
	if (hours12 == 0) hours12 = 12;

	// format string
	std::ostringstream oss;
	oss << std::setw(2) << std::setfill('0') << hours12 << ":"
		<< std::setw(2) << std::setfill('0') << minutes << " "
		<< (isPM ? "PM" : "AM");

	return oss.str();
}

void DayNightHandler::pauseTime() {
	if (!m_paused) {
		m_paused = true;
		m_pauseTicks = SDL_GetTicks();
	}
}

void DayNightHandler::resumeTime() {
	if (m_paused) {
		Uint32 pauseDuration = SDL_GetTicks() - m_pauseTicks;
		m_startTicks += pauseDuration;
		m_paused = false;
	}
}


/*----------- Weather Ambient ----------------------------------------------------------------------------------*/


WeatherHandler::WeatherHandler(Application* app, Game* game) {
	this->app = app;
	this->game = game;

	m_currentWeatherType = WeatherType::Clear;
	m_currentWeatherSystem = std::make_unique<ClearWeatherSystem>();
}

WeatherHandler::~WeatherHandler() {
//	std::cout << "WeatherHandler deleted" << std::endl;
}

void WeatherHandler::setup(Game* game) {
	m_weatherIcon = std::make_shared<InfoIcon>(*game->guiHandler, UiSheetType::AmbientSheet, 16, 0,
		(game->miniMapUI->getDim().x * 2) + game->miniMapUI->getDim().w + 64 + 10,
		game->miniMapUI->getDim().y, 64, 64);
	m_weatherIcon->setFunctionForPosX([game]() { return (game->miniMapUI->getDim().x * 2) + game->miniMapUI->getDim().w + 64 + 10; });
	m_weatherIcon->setFunctionForPosY([game]() { return game->miniMapUI->getDim().y; });
	m_weatherIcon->setDescription("Clear");
	game->guiHandler->addElement(m_weatherIcon);

	app->globalSounds->loopEffect(GlobalSoundsList::AMBIENT_CLEAR, Channels::AMBIENT_SOUNDS);
}

void WeatherHandler::update() {
	m_currentWeatherSystem->update();
}

void WeatherHandler::render() {
	m_currentWeatherSystem->render();
}

void WeatherHandler::windowResized() {
	m_currentWeatherSystem->windowResized();
}

void WeatherHandler::enableGUI() {
	m_weatherIcon->enable();
	app->globalSounds->resumeEffect(Channels::AMBIENT_SOUNDS);
}

void WeatherHandler::disableGUI() {
	m_weatherIcon->disable();
	app->globalSounds->pauseEffect(Channels::AMBIENT_SOUNDS);
}

void WeatherHandler::setWeather(WeatherType weather) {
	if (weather != m_currentWeatherType) {
		m_currentWeatherType = weather;
		switch (weather){
		case WeatherType::Clear:
			m_currentWeatherSystem = std::make_unique<ClearWeatherSystem>();
			m_weatherIcon->setSrcX(0);
			m_weatherIcon->setDescription("Clear");
			app->globalSounds->stopEffect(Channels::AMBIENT_SOUNDS);
			app->globalSounds->loopEffect(GlobalSoundsList::AMBIENT_CLEAR, Channels::AMBIENT_SOUNDS);
			break;
		case WeatherType::Rain:
			m_currentWeatherSystem = std::make_unique<RainSystem>(app, game, 100);
			m_weatherIcon->setSrcX(16);
			m_weatherIcon->setDescription("Rain");
			app->globalSounds->stopEffect(Channels::AMBIENT_SOUNDS);
			app->globalSounds->loopEffect(GlobalSoundsList::AMBIENT_RAIN, Channels::AMBIENT_SOUNDS);
			break;
		default:
			break;
		}
	}
}

WeatherType WeatherHandler::getWeather() const { return m_currentWeatherType; }

void WeatherHandler::loadData(const json& data) {
	if (data.contains("Weather")) {
		if (data["Weather"] == "clear" || data["Weather"] == "undef") { setWeather(WeatherType::Clear); }
		else if (data["Weather"] == "rain") { setWeather(WeatherType::Rain); }
	}
	
}

void WeatherHandler::saveData(json& data) const {
	std::string weather;
	switch (m_currentWeatherType) {
		case WeatherType::Clear: weather = "clear"; break;
		case WeatherType::Rain: weather = "rain"; break;
		default: weather = "undef"; break;
	}
	data["Weather"] = weather;
}

ClearWeatherSystem::ClearWeatherSystem() {}
ClearWeatherSystem::~ClearWeatherSystem() {}
void ClearWeatherSystem::update() {}
void ClearWeatherSystem::render() {}
void ClearWeatherSystem::windowResized() {}

RainSystem::RainSystem(Application* app, Game* game, int maxDrops) {
	this->app = app;
	this->game = game;

	m_overlay = { 0, 0, app->getWindowWidth(), app->getWindowHeight() };

	m_drops.reserve(maxDrops);
	for (int i = 0; i < maxDrops; ++i) {
		m_drops.push_back(createRandomDrop());
	}
}

RainSystem::~RainSystem() {}

void RainSystem::update() {
	for (auto& drop : m_drops) {
		drop.y += drop.speed;
		if (drop.y > m_overlay.h) {
			drop = createRandomDrop();
			drop.y = -drop.length;
		}
	}
}

void RainSystem::render() {

	// renders fog tint
	SDL_SetRenderDrawColor(app->renderer, 100, 100, 100, 70);
	SDL_RenderFillRect(app->renderer, &m_overlay);

	// renders rain drops
	SDL_SetRenderDrawColor(app->renderer, 200, 200, 255, 220);
	for (auto& drop : m_drops) {
		SDL_RenderDrawLine(
			app->renderer,
			static_cast<int>(drop.x),
			static_cast<int>(drop.y),
			static_cast<int>(drop.x),
			static_cast<int>(drop.y + drop.length)
		);
	}

	// renders flashes / lightnings
	if (game->getState() == State::PLAYING) {
		if(rand() % 500 == 0) { // ~once every few seconds
			SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 120); // flash white
			SDL_RenderFillRect(app->renderer, &m_overlay);
			if (!app->globalSounds->isChannelPlaying(Channels::GAME_SOUNDS)) app->globalSounds->playEffect(GlobalSoundsList::THUNDER, Channels::GAME_SOUNDS);
		}
	}
	
}

void RainSystem::windowResized() {
	m_overlay.w = app->getWindowWidth();
	m_overlay.h = app->getWindowHeight();
}

Raindrop RainSystem::createRandomDrop() {
	Raindrop r;
	r.x = static_cast<float>(rand() % m_overlay.w);
	r.y = static_cast<float>(rand() % m_overlay.h);
	r.length = 10 + rand() % 10;
	r.speed = 2.0f + static_cast<float>(rand() % 3); // 2–4 pixels/frame
	return r;
}