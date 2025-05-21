#pragma once
#include "../../Application.h"
#include "SDL.h"
#include "../../filehandling/gamedata/GameDataHandler.h"
#include "../Game.h"
#include <iostream>
#include <memory>

class InfoIcon;


/*------- Day Night Ambient ---------------------------------------------------------------------------------------*/


enum class TimeState {
	Sunrise,
	Day,
	Sunset,
	Night
};

class DayNightHandler {
public:

	const float c_nightBegin = 0.80f;
	const float c_sunriseBegin = 0.20f;
	const float c_dayBegin = 0.30f;
	const float c_sunsetBegin = 0.70f;

	DayNightHandler(Application* app);
	~DayNightHandler();
	void setup(Game* game);

	void update();
	void render();
	void windowResized();

	void setTime(float time);
	float getTime();

	void loadData(const json& data);
	void saveData(json& data) const;

	void pauseTime();
	void resumeTime();

	void enableGUI();
	void disableGUI();

private:

	Application* app;

	float m_time;
	float m_timeSpeed;
	float m_dayDurationSeconds;
	Uint32 m_startTicks;

	TimeState m_currentTimeState;
	TimeState m_previousTimeState;

	bool m_paused;
	Uint32 m_pauseTicks;

	std::shared_ptr<InfoIcon> m_timeIcon;

	bool m_allowNightShading;

	float getBrightness() const;
	void setStartFromFile(float loadedTime);
	TimeState getTimeState(float time);
	void onTimeStateChange(TimeState newState);
	std::string getFormattedTime(float time);

};


/*----------- Weather Ambient ----------------------------------------------------------------------------------*/


enum class WeatherType {
	Clear,
	Rain
};

class WeatherSystem {
public:

	virtual ~WeatherSystem() = default;

	virtual void update() = 0;
	virtual void render() = 0;
	virtual void windowResized() = 0;

};

class WeatherHandler {
public:

	WeatherHandler(Application* app, Game* game);
	~WeatherHandler();
	void setup(Game* game);

	void update();
	void render();
	void windowResized();

	void setWeather(WeatherType weather);
	WeatherType getWeather() const;

	void loadData(const json& data);
	void saveData(json& data) const;

	void enableGUI();
	void disableGUI();

private:

	Application* app;
	Game* game;

	WeatherType m_currentWeatherType;
	std::unique_ptr<WeatherSystem> m_currentWeatherSystem;

	std::shared_ptr<InfoIcon> m_weatherIcon;

	bool m_paused;

};

class ClearWeatherSystem : public WeatherSystem {
public:

	ClearWeatherSystem();
	~ClearWeatherSystem() override;

	void update() override;
	void render() override;
	void windowResized() override;

};

class Raindrop {
public:
	float x, y;
	float speed;
	int length;
};

class RainSystem : public WeatherSystem {
public:

	RainSystem(Application* app, Game* game, int maxDrops);
	~RainSystem() override;

	void update() override;
	void render() override;
	void windowResized() override;

private:

	Application* app;
	Game* game;

	std::vector<Raindrop> m_drops;
	SDL_Rect m_overlay;

	Raindrop createRandomDrop();

};

