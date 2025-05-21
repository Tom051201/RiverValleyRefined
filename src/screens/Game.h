#pragma once
#include <memory>
#include "Screen.h"
#include "game/Camera.h"
#include "../Application.h"
#include "../world/TileHandler.h"
#include "../world/realms/Realm.h"
#include "../player/Player.h"
#include "../filehandling/settings/SettingsHandler.h"
#include "../commands/CommandPrompt.h"
#include "../items/ItemDatabase.h"
#include "../filehandling/gamedata/GameDataHandler.h"
#include "game/GameAmbientEffects.h"
#include "../ui/GuiHandler.h"
#include "../entities/ECS.h"

class Player;
class Realm;
enum class State;
class CommandPrompt;
class Collections;
class ItemDatabase;
class Button;
class DayNightHandler;
class WeatherHandler;

class Game : public Screen {
public:

	std::unique_ptr<Camera> camera;
	std::unique_ptr<GameDataHandler> dataHandler;
	std::unique_ptr<TileHandler> tileHandler;
 	std::shared_ptr<Realm> world;
	std::shared_ptr<Player> player;
	std::unique_ptr<CommandPrompt> commandPrompt;
	std::unique_ptr<ItemDatabase> itemDatabase;
	std::unique_ptr<DayNightHandler> dayNightHandler;
	std::unique_ptr<WeatherHandler> weatherHandler;
	std::unique_ptr<GuiHandler> guiHandler;
	std::unique_ptr<ECS> ecs;

	std::shared_ptr<Button> saveGameButton;
	std::shared_ptr<Button> settingsButton;
	std::shared_ptr<MiniMapUI> miniMapUI;

	Game(Application* app, const std::string& folderPath);
	~Game() override;

	void render() override;
	void update() override;
	void handleEvents() override;
	void windowResized() override;

	void loadData();
	void saveData();

	void setState(State state);
	inline State getState() const { return m_state; }

private:

	Application* app;
	State m_state;

	std::string m_saveFilePath;
	std::string m_worldFilePath;
	std::string m_entitiesFilePath;
	std::string m_dataFilePath;

	/*----- ECS -----*/
	Entity cow;
	std::shared_ptr<System> movementSystem;
	std::shared_ptr<System> renderSystem;
	std::shared_ptr<System> wanderSystem;

	void initHandlers();
	void setupHandlers();
	void initUI();
	void initECS();

 };

enum class State {
	PLAYING,
	PAUSED
};
