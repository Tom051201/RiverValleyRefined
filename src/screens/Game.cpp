#include "Game.h"
#include "../player/Player.h"
#include <iostream>
#include "MainMenu.h"
#include "SettingsMenu.h"

Game::Game(Application* app, const std::string& folderPath) {
	this->app = app;
	setState(State::PLAYING);

	m_saveFilePath = folderPath + "/save.txt";
	m_worldFilePath = folderPath + "/world.tmap";
	m_entitiesFilePath = folderPath + "/entities.json";
	m_dataFilePath = folderPath + "/data.json";

	initHandlers();

	initUI();

	initECS();

	setupHandlers();


	this->loadData();
}

Game::~Game() {
	this->saveData();
	world->saveMapFile();
	ecs->saveData(m_entitiesFilePath);

	// delete last!
	dataHandler->saveGameData(m_saveFilePath);
	dataHandler->saveJsonData();
}

void Game::initHandlers() {
	dataHandler = std::make_unique<GameDataHandler>(m_dataFilePath);
	dataHandler->loadJsonData();
	dataHandler->loadGameData(m_saveFilePath);

	guiHandler = std::make_unique<GuiHandler>();

	tileHandler = std::make_unique<TileHandler>("res/world/tilesSheet.png");

	world = std::make_shared<Realm>(m_worldFilePath.c_str(), this, app);
	world->setBgColor(tileHandler->getTile(TileHandler::WATER)->getColor());

	itemDatabase = std::make_unique<ItemDatabase>(this);

	player = std::make_shared<Player>(app, this, "res/entities/playerSheet.png");

	camera = std::make_unique<Camera>(player->getWorldX(), player->getWorldY(), app->getWindowWidth(), app->getWindowHeight());

	commandPrompt = std::make_unique<CommandPrompt>(app, this);

	dayNightHandler = std::make_unique<DayNightHandler>(app);

	weatherHandler = std::make_unique<WeatherHandler>(app, this);

	ecs = std::make_unique<ECS>();
}

void Game::setupHandlers() {
	dayNightHandler->setup(this);
	weatherHandler->setup(this);
}

void Game::initUI() {
	miniMapUI = std::make_shared<MiniMapUI>(this, 40, 40, 150, 150);
	miniMapUI->setFunctionForPosX([]() { return 40; });
	miniMapUI->setFunctionForPosY([]() { return 40; });
	guiHandler->addElement(miniMapUI);

	saveGameButton = std::make_shared<Button>(app, app->getWindowWidth() / 2 - 150, app->getWindowHeight() / 2 + 100, 300, 70, [this]() { app->setActiveScreen(std::make_unique<MainMenu>(app)); });
	saveGameButton->setText("Save Game");
	saveGameButton->setTextSize(24);
	saveGameButton->disable();
	saveGameButton->setFunctionForPosX([this]() { return app->getWindowWidth() / 2 - 150; });
	saveGameButton->setFunctionForPosY([this]() { return app->getWindowHeight() / 2 + 100; });
	guiHandler->addElement(saveGameButton);

	settingsButton = std::make_shared<Button>(app, app->getWindowWidth() / 2 - 150, app->getWindowHeight() / 2 + 200, 300, 70, [this]() { app->setActiveScreen(std::make_unique<SettingsMenu>(app)); });
	settingsButton->setText("Settings");
	settingsButton->setTextSize(24);
	settingsButton->disable();
	settingsButton->setFunctionForPosX([this]() { return app->getWindowWidth() / 2 - 150; });
	settingsButton->setFunctionForPosY([this]() { return app->getWindowHeight() / 2 + 200; });
	guiHandler->addElement(settingsButton);
}

void Game::initECS() {
	ecs->registerComponent<PositionComponent>();
	ecs->registerComponent<VelocityComponent>();
	ecs->registerComponent<SpriteComponent>();
	ecs->registerComponent<HealthComponent>();
	ecs->registerComponent<ColliderComponent>();
	ecs->registerComponent<AIComponent>();

	movementSystem = ecs->registerSystem<MovementSystem>();
	Signature movementSignature;
	movementSignature.set(ecs->getComponentTypeID<PositionComponent>());
	movementSignature.set(ecs->getComponentTypeID<VelocityComponent>());
	ecs->setSystemSignature<MovementSystem>(movementSignature);

	renderSystem = ecs->registerSystem<RenderSystem>();
	Signature renderSignature;
	renderSignature.set(ecs->getComponentTypeID<SpriteComponent>());
	renderSignature.set(ecs->getComponentTypeID<PositionComponent>());
	ecs->setSystemSignature<RenderSystem>(renderSignature);

	wanderSystem = ecs->registerSystem<WanderSystem>();
	Signature wanderSignature;
	wanderSignature.set(ecs->getComponentTypeID<AIComponent>());
	wanderSignature.set(ecs->getComponentTypeID<VelocityComponent>());
	ecs->setSystemSignature<WanderSystem>(wanderSignature);

	ecs->loadData(m_entitiesFilePath);

	/*
	cow = ecs->createEntity();
	ecs->addComponent<PositionComponent>(cow, { 0, 0 });
	ecs->addComponent<VelocityComponent>(cow, { 1, 1 });
	SpriteComponent cowSprite("res/entities/cowSheet.png", { 0, 0, 16, 16 }, { ecs->getComponent<PositionComponent>(cow).x, ecs->getComponent<PositionComponent>(cow).y, 64, 64 });
	cowSprite.texture = TextureManager::loadTexture(cowSprite.path.c_str());
	ecs->addComponent<SpriteComponent>(cow, cowSprite);
	ecs->addComponent<HealthComponent>(cow, { 100, 100 });
	ecs->addComponent<AIComponent>(cow, { 10 });
	*/
}

void Game::loadData() {
	player->loadData(dataHandler->getSection("Player"));
	weatherHandler->loadData(dataHandler->getSection("Ambient"));
	dayNightHandler->loadData(dataHandler->getSection("Ambient"));
}

void Game::saveData() {
	player->saveData(dataHandler->getSection("Player"));
	weatherHandler->saveData(dataHandler->getSection("Ambient"));
	dayNightHandler->saveData(dataHandler->getSection("Ambient"));
}

void Game::render() {
	world->render();
	dayNightHandler->render();
	weatherHandler->render();
	commandPrompt->render();
	renderSystem->render(*ecs, app->renderer, *camera);
	player->render();

	if (m_state == State::PAUSED) {
		SDL_SetRenderDrawColor(app->renderer, 25, 25, 25, 205);
		SDL_Rect blend = { 0, 0, app->getWindowWidth(), app->getWindowHeight() };
		SDL_RenderFillRect(app->renderer, &blend);

		TextManager::renderStringInRect("Paused", Fonts::DEFAULT, 80, { 255, 255, 255, 255 }, blend);
	}

	guiHandler->render();
}

void Game::update() {
	if (m_state == State::PLAYING) {
		dayNightHandler->update();
		weatherHandler->update();
		player->update();
		camera->update(player->getWorldX(), player->getWorldY());

		movementSystem->update(*ecs);
		wanderSystem->update(*ecs);
	}
}

void Game::handleEvents() {
	player->handleEvents();
	commandPrompt->handleEvents(app->getAppEvent());

	if (app->getAppEvent().type == SDL_KEYUP) {
		switch (app->getAppEvent().key.keysym.sym) {
		case SDLK_ESCAPE:
			if (m_state == State::PLAYING) {
				if (commandPrompt->isOpen()) {
					commandPrompt->close();
				}
				else if (player->inv->isOpen()) {
					player->inv->closeInventory();
				}
				else {
					setState(State::PAUSED);
				}
			}
			else {
				setState(State::PLAYING);
			}
			break;

		case SDLK_t:
			commandPrompt->open();
			break;

		case SDLK_i:
			if (player->inv->isOpen() && !commandPrompt->isOpen()) {
				player->inv->closeInventory();
			}
			else if (!commandPrompt->isOpen()) {
				player->inv->openInventory();
			}
			break;

		default:
			break;
		}

	}

	// do as last
	guiHandler->handleEvents();

}

void Game::windowResized() {
	camera->windowResized(app->getWindowWidth(), app->getWindowHeight());
	player->windowResized();
	commandPrompt->windowResized();
	weatherHandler->windowResized();
	guiHandler->windowResized();
}

void Game::setState(State state) {
	m_state = state;

	if (state == State::PLAYING) {
		app->hideCursor();
		if (saveGameButton) saveGameButton->disable();
		if (settingsButton) settingsButton->disable();
		if (dayNightHandler) { dayNightHandler->resumeTime(); dayNightHandler->enableGUI(); }
		if (weatherHandler) weatherHandler->enableGUI();
		if (player) player->enableGUI();
		if (miniMapUI) miniMapUI->enable();
	}
	else {
		app->showCursor();
		app->centerCursorInWindow();
		if (saveGameButton) saveGameButton->enable();
		if (settingsButton) settingsButton->enable();
		if (dayNightHandler) { dayNightHandler->pauseTime(); dayNightHandler->disableGUI(); }
		if (weatherHandler) weatherHandler->disableGUI();
		if (player) player->disableGUI();
		if (miniMapUI) miniMapUI->disable();
	}
}
