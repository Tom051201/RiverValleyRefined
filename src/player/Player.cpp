#include "Player.h";
#include "../Utils.h"
#include <iostream>
#include <memory>
#include <iostream>

Player::Player(Application* app, Game* game, const char* textureSheetPath) {
	this->app = app;
	this->game = game;

	inv = std::make_unique<PlayerInventory>(app, game, 6, 8, 100, 100);
	//inv->loadData(*game->dataHandler, "pi");
	inv->loadData(game->dataHandler->getSection("PlayerInventory"));
	
	m_worldX = game->world->getSpawnX();
	m_worldY = game->world->getSpawnY();

	m_screenX = app->getWindowWidth() / 2 - 64 / 2;
	m_screenY = app->getWindowHeight() / 2 - 64 / 2;

	m_defaultSpeed = 5;
	m_speed = m_defaultSpeed;

	m_velocity = { 0, 0 };
	
	m_textureSheet = TextureManager::loadTexture(textureSheetPath);
	m_src = { 0, 0, 16, 16 };
	m_dst = { m_screenX, m_screenY, 64, 64 };
	m_textureCount = 0;
	m_textureState = 0;

	m_WKeyDown = m_AKeyDown = m_SKeyDown = m_DKeyDown = m_shiftKeyDown = false;
	m_isMoving = false;

	m_collisionBox = { 0, 0, 32, 32 };

	m_maxHealth = 100;
	m_health = m_maxHealth;

	m_maxEndurance = 100;
	m_endurance = m_maxEndurance;
	m_enduranceEfficiency = 30; // represents the ticks it takes to reduce endurance (60=1sec, 30=0.5sec
	m_enduranceCounter = 0;

	m_healthBar = std::make_shared<Bar>(app->getWindowWidth() - 190, 40, 150, 30, m_maxHealth);
	m_healthBar->setIndicatorColor({ 0, 255, 0, 255 });
	m_healthBar->setFill(m_health);
	m_healthBar->setFunctionForPosX([app]() { return app->getWindowWidth() - 190; });
	m_healthBar->setFunctionForPosY([]() { return 40; });
	game->guiHandler->addElement(m_healthBar);

	m_enduranceBar = std::make_shared<Bar>(app->getWindowWidth() - 190, 100, 150, 30, m_maxEndurance);
	m_enduranceBar->setIndicatorColor({ 0, 0, 255, 255 });
	m_enduranceBar->setFill(m_endurance);
	m_enduranceBar->setFunctionForPosX([app]() { return app->getWindowWidth() - 190; });
	m_enduranceBar->setFunctionForPosY([]() { return 100; });
	game->guiHandler->addElement(m_enduranceBar);

	m_interactionArea = { m_worldX, m_worldY, 64, 30 };

	m_hitboxOn = false;
	m_createMode = false;
	m_selectedCreateTileIndex = 0;

	windowResized();
}

Player::~Player() {

//	std::cout << "Player deleted" << std::endl;

	SDL_DestroyTexture(m_textureSheet);
}

void Player::render() {
	// render texture
	TextureManager::render(m_textureSheet, m_src, m_dst);

	// render hitbox
	if (m_hitboxOn) {
		SDL_SetRenderDrawColor(app->renderer, 255, 0, 0, 255);
		SDL_Rect hitbox = { m_screenX + 16, m_screenY + 16, m_collisionBox.w, m_collisionBox.h };
		SDL_RenderDrawRect(app->renderer, &hitbox);

		// render interactionArea
		SDL_SetRenderDrawColor(app->renderer, 255, 0, 0, 255);
		SDL_Rect interArea;
		if (m_facing == Facing::UP) {
			interArea = { m_screenX, m_screenY - 20, m_interactionArea.w, m_interactionArea.h };
		}
		else if (m_facing == Facing::LEFT) {
			interArea = { m_screenX - 20, m_screenY, m_interactionArea.w, m_interactionArea.h };
		}
		else if (m_facing == Facing::DOWN) {
			interArea = { m_screenX, m_screenY + 64 - 10, m_interactionArea.w, m_interactionArea.h };
		}
		else if (m_facing == Facing::RIGHT) {
			interArea = { m_screenX + 64 - 10, m_screenY, m_interactionArea.w, m_interactionArea.h };
		}
		SDL_RenderDrawRect(app->renderer, &interArea);
	}

	inv->render();

	// render selected createTile
	if (m_createMode) {
		TextureManager::render(game->tileHandler->getTile(m_selectedCreateTileIndex)->getTextureSheet(), game->tileHandler->getTile(m_selectedCreateTileIndex)->getSrcRect(), { 40, 300, 64, 64 });
		SDL_Rect c = { 40, 300, 64, 64 };
		SDL_RenderDrawRect(app->renderer, &c);
	}
}

void Player::update() {
	if (game->getState() != State::PLAYING || game->commandPrompt->isOpen() || inv->isOpen()) {
		inv->update();
		return;
	}

	m_isMoving = m_WKeyDown || m_AKeyDown || m_SKeyDown || m_DKeyDown;

	// sprinting
	if (m_shiftKeyDown && m_endurance > 0 && m_isMoving) {
		m_speed = m_defaultSpeed * 1.5;
		m_enduranceCounter++;
		if (m_enduranceCounter == m_enduranceEfficiency) {
			reduceEndurance(1);
			m_enduranceCounter = 0;
		}
	}
	else { m_speed = m_defaultSpeed; }

	// set velocity
	m_velocity.zero();
	if (m_isMoving) {
		if (m_WKeyDown) {
			m_velocity.y = -1.0f;
			m_facing = Facing::UP;
			m_interactionArea = { m_worldX, m_worldY - 20, 64, 30 };
		}
		if (m_AKeyDown) {
			m_velocity.x = -1.0f;
			m_facing = Facing::LEFT;
			m_interactionArea = { m_worldX - 20, m_worldY, 30, 64 };
		}
		if (m_SKeyDown) {
			m_velocity.y = 1.0f;
			m_facing = Facing::DOWN;
			m_interactionArea = { m_worldX, m_worldY + 54, 64, 30 };
		}
		if (m_DKeyDown) {
			m_velocity.x = 1.0f;
			m_facing = Facing::RIGHT;
			m_interactionArea = { m_worldX + 54, m_worldY, 30, 64 };
		}
		if (!m_WKeyDown && !m_SKeyDown) {
			m_velocity.y = 0;
		}
		if (!m_AKeyDown && !m_DKeyDown) {
			m_velocity.x = 0;
		}

		// Normalize and apply
		m_velocity.normalize();
		m_colliding = false;
		checkCollision();
		if (!m_colliding && !checkOutOfBounds()) {
			changeWorldX(m_velocity.x * m_speed);
			changeWorldY(m_velocity.y * m_speed);
			m_textureCount++;
		}

		// update animation
		switch (m_facing) {
			case Facing::UP: { m_src.x = 2 * 16; break; }
			case Facing::DOWN: { m_src.x = 0 * 16; break; }
			case Facing::LEFT: { m_src.x = 1 * 16; break; }
			case Facing::RIGHT: { m_src.x = 3 * 16; break; }
			default: break;
		}
		flipTexture();

	}

	m_collisionBox.x = m_worldX + 16;
	m_collisionBox.y = m_worldY + 16;

	inv->update();

}

void Player::handleEvents() {

	inv->handleEvents();

	if (app->getAppEvent().type == SDL_KEYDOWN) {
		switch (app->getAppEvent().key.keysym.sym) {

			case SDLK_w: { m_WKeyDown = true; break; }
			case SDLK_a: { m_AKeyDown = true; break; }
			case SDLK_s: { m_SKeyDown = true; break; }
			case SDLK_d: { m_DKeyDown = true; break; }
			case SDLK_LSHIFT: { m_shiftKeyDown = true; break; }
			default: break;

		}
	}

	if (app->getAppEvent().type == SDL_KEYUP) {
		switch (app->getAppEvent().key.keysym.sym) {

			case SDLK_w: { m_WKeyDown = false; break; }
			case SDLK_a: { m_AKeyDown = false; break; }
			case SDLK_s: { m_SKeyDown = false; break; }
			case SDLK_d: { m_DKeyDown = false; break; }
			case SDLK_LSHIFT: { m_shiftKeyDown = false; break; }
			case SDLK_TAB: { if (m_createMode) nextTile(); break; }
			default: break;

		}
	}
	
	if (app->getAppEvent().type == SDL_MOUSEBUTTONUP) {
		switch (app->getAppEvent().button.button) {
			case SDL_BUTTON_LEFT:
				checkMapInteraction(); // maybe useless, rethink later
				break;
			default: break;
		}
	}
}

void Player::windowResized() {
	m_screenX = app->getWindowWidth() / 2 - 64 / 2;
	m_screenY = app->getWindowHeight() / 2 - 64 / 2;

	m_dst = { m_screenX, m_screenY, 64, 64 };

	inv->windowResized();
}

void Player::enableGUI() {
	m_healthBar->enable();
	m_enduranceBar->enable();
}
void Player::disableGUI() {
	m_healthBar->disable();
	m_enduranceBar->disable();
}

void Player::addHealth(int a) {
	if (m_health + a >= m_maxHealth) {
		m_health = m_maxHealth;
		m_healthBar->fillEntirely();
	}
	else {
		m_health += a;
		m_healthBar->add(a);
	}
}
void Player::reduceHealth(int a) {
	if (m_health - a <= 0) {
		m_health = 0;
		m_healthBar->emptyEntirely();
	}
	else {
		m_health -= a;
		m_healthBar->reduce(a);
	}
}
void Player::setHealth(int a) {
	m_health = a;
	m_healthBar->setFill(a);
}

void Player::addEndurance(int a) {
	if (m_endurance + a >= m_maxEndurance) {
		m_endurance = m_maxEndurance;
		m_enduranceBar->fillEntirely();
	}
	else {
		m_endurance += a;
		m_enduranceBar->add(a);
	}
}
void Player::reduceEndurance(int a) {
	if (m_endurance - a <= 0) {
		m_endurance = 0;
		m_enduranceBar->emptyEntirely();
	}
	else {
		m_endurance -= a;
		m_enduranceBar->reduce(a);
	}
}
void Player::setEndurance(int a) {
	m_endurance = a;
	m_enduranceBar->setFill(a);
}

void Player::checkMapInteraction() {
	m_interactionPoint = { (m_interactionArea.x + (m_interactionArea.w / 2)) / 64, (m_interactionArea.y + (m_interactionArea.h / 2)) / 64 };

	if (m_interactionPoint.x < game->world->getMaxCols() &&
		m_interactionPoint.y < game->world->getMaxRows() &&
		m_interactionPoint.x >= 0 &&
		m_interactionPoint.y >= 0) {

		if (m_createMode) {
			game->world->overwriteTile(m_interactionPoint.x, m_interactionPoint.y, m_selectedCreateTileIndex);
		}

	}

}

void Player::nextTile() {
	if (m_selectedCreateTileIndex + 1 < game->tileHandler->getTilesAmount()) {
		m_selectedCreateTileIndex++;
	}
	else {
		m_selectedCreateTileIndex = 0;
	}
}

void Player::destroyRock(int x, int y) {
	m_interactionPoint = { (m_interactionArea.x + (m_interactionArea.w / 2)) / 64, (m_interactionArea.y + (m_interactionArea.h / 2)) / 64 };

	if (game->tileHandler->getIndex(game->world->getTileAt(m_interactionPoint.y, m_interactionPoint.x)) == TileHandler::ROCK) {

		game->world->overwriteTile(m_interactionPoint.x, m_interactionPoint.y, TileHandler::GRASS);
		if (!app->globalSounds->isChannelPlaying(Channels::GAME_SOUNDS)) {
			app->globalSounds->playEffect(GlobalSoundsList::BREAK_ROCK, Channels::GAME_SOUNDS);
		}

		if (!inv->getEquipmentSlot(ItemType::RESSOURCE_ITEM)->isEmpty() && inv->getEquipmentSlot(ItemType::RESSOURCE_ITEM)->getItem()->getID() == 1) {
			inv->getEquipmentSlot(ItemType::RESSOURCE_ITEM)->increaseAmount(2);
		}
		else {
			inv->addItem(game->itemDatabase->createItem(1), 2);
		}
		
	}
}

void Player::destroyTree(int x, int y) {
	m_interactionPoint = { (m_interactionArea.x + (m_interactionArea.w / 2)) / 64, (m_interactionArea.y + (m_interactionArea.h / 2)) / 64 };

	if (game->tileHandler->getIndex(game->world->getTileAt(m_interactionPoint.y, m_interactionPoint.x)) == TileHandler::TREE) {

		game->world->overwriteTile(m_interactionPoint.x, m_interactionPoint.y, TileHandler::TREE_STUMP);
		if (!app->globalSounds->isChannelPlaying(Channels::GAME_SOUNDS)) {
			app->globalSounds->playEffect(GlobalSoundsList::CHOPPING_TREE, Channels::GAME_SOUNDS);
		}
		
		if (!inv->getEquipmentSlot(ItemType::RESSOURCE_ITEM)->isEmpty() && inv->getEquipmentSlot(ItemType::RESSOURCE_ITEM)->getItem()->getID() == 0) {
			inv->getEquipmentSlot(ItemType::RESSOURCE_ITEM)->increaseAmount(2);
		}
		else {
			inv->addItem(game->itemDatabase->createItem(0), 2);
		}
	}
}

void Player::checkCollision() {
	m_colliding = false;

	if (m_createMode) return;

	// --- Check horizontal movement ---
	if (m_velocity.x != 0) {
		int nextX = m_collisionBox.x + m_velocity.x * m_speed;
		int leftCol = nextX / 64;
		int rightCol = (nextX + m_collisionBox.w) / 64;
		int topRow = m_collisionBox.y / 64;
		int bottomRow = (m_collisionBox.y + m_collisionBox.h) / 64;

		Tile* t1 = game->world->getTileAt(topRow, m_velocity.x < 0 ? leftCol : rightCol);
		Tile* t2 = game->world->getTileAt(bottomRow, m_velocity.x < 0 ? leftCol : rightCol);

		if ((t1 && t1->isSolid()) || (t2 && t2->isSolid())) {
			m_velocity.x = 0;  // Stop horizontal movement if blocked
			m_colliding = true;
		}
	}

	// --- Check vertical movement ---
	if (m_velocity.y != 0) {
		int nextY = m_collisionBox.y + m_velocity.y * m_speed;
		int topRow = nextY / 64;
		int bottomRow = (nextY + m_collisionBox.h) / 64;
		int leftCol = m_collisionBox.x / 64;
		int rightCol = (m_collisionBox.x + m_collisionBox.w) / 64;

		Tile* t1 = game->world->getTileAt(m_velocity.y < 0 ? topRow : bottomRow, leftCol);
		Tile* t2 = game->world->getTileAt(m_velocity.y < 0 ? topRow : bottomRow, rightCol);

		if ((t1 && t1->isSolid()) || (t2 && t2->isSolid())) {
			m_velocity.y = 0;  // Stop vertical movement if blocked
			m_colliding = true;
		}
	}
}

bool Player::checkOutOfBounds() {
	// --- Check if moving out of bounds ---
	int nextX = m_worldX + m_velocity.x * m_speed;
	int nextY = m_worldY + m_velocity.y * m_speed;
	int worldWidth = game->world->getMaxCols() * 64;
	int worldHeight = game->world->getMaxRows() * 64;

	if (nextX < 0 || nextX + 64 > worldWidth ||
		nextY < 0 || nextY + 64 > worldHeight) {
		return true;
	}
	
	return false;
}

void Player::flipTexture() {
	if (m_textureCount == 15) {
		if (m_textureState == 1) {
			m_textureState = 2;
			m_src.y = 16;
		}
		else {
			m_textureState = 1;
			m_src.y = 0;
		}
		m_textureCount = 0;
	}
}

void Player::loadData(const json& data) {
		
	if (data.contains("Position")) {
		m_worldX = data["Position"]["x"];
		m_worldY = data["Position"]["y"];
	}

	if (data.contains("Health")) {
		setHealth(data["Health"]["current"]);
		m_maxHealth = data["Health"]["max"];
	}

	if (data.contains("Endurance")) {
		setEndurance(data["Endurance"]["current"]);
		m_maxEndurance = data["Endurance"]["max"];
		m_enduranceEfficiency = data["Endurance"]["efficiency"];
	}

	if (data.contains("Speed")) {
		m_speed = data["Speed"]["current"];
		m_defaultSpeed = data["Speed"]["default"];
	}
	

	std::cout << "player loaded" << std::endl;
} 

void Player::saveData(json& data) const {
	inv->saveData(game->dataHandler->getSection("PlayerInventory"));

	data["Position"] = { {"x", m_worldX}, {"y", m_worldY} };
	data["Health"] = { {"current", m_health}, {"max", m_maxHealth} };
	data["Endurance"] = { {"current", m_endurance}, {"max", m_maxEndurance}, {"efficiency", m_enduranceEfficiency} };
	data["Speed"] = { {"current", m_speed}, {"default", m_defaultSpeed} };
}
