#pragma once
#include "../screens/Game.h"
#include "../ui/Gui.h"
#include "../inventory/PlayerInventory.h"

class Game;
class PlayerInventory;

class Player {
public:

	std::unique_ptr<PlayerInventory> inv;

	Player(Application* app, Game* game, const char* textureSheetPath);
	~Player();

	void render();
	void update();
	void handleEvents();
	void windowResized();

	void enableGUI();
	void disableGUI();

	inline void setWorldX(int x) { m_worldX = x; }
	inline void setWorldY(int y) { m_worldY = y; }
	inline void changeWorldX(int x) { m_worldX += x; }
	inline void changeWorldY(int y) { m_worldY += y; }
	inline int getWorldX() const { return m_worldX; }
	inline int getWorldY() const { return m_worldY; }

	inline int getScreenX() const { return m_screenX; }
	inline int getScreenY() const { return m_screenY; }

	inline Facing getFacing() const { return m_facing; }
	inline const SDL_Rect& getHitBox() const { return m_collisionBox; }

	void addHealth(int a);
	void reduceHealth(int a);
	void setHealth(int a);
	inline int getHealth() const { return m_health; }

	void addEndurance(int a);
	void reduceEndurance(int a);
	void setEndurance(int a);
	inline int getEndurance() const { return m_endurance; }

	inline void toggleHitBox() { m_hitboxOn = !m_hitboxOn; }
	inline void toggleCreateMode() { m_createMode = !m_createMode; }

	void destroyRock(int x, int y);
	void destroyTree(int x, int y);

	void loadData(const json& data);
	void saveData(json& data) const;

private:
	
	Application* app;
	Game* game;

	int m_worldX, m_worldY;
	int m_screenX, m_screenY;

	int m_speed, m_defaultSpeed;

	int m_maxHealth, m_health;

	int m_maxEndurance, m_endurance, m_enduranceEfficiency, m_enduranceCounter;

	bool m_WKeyDown, m_AKeyDown, m_SKeyDown, m_DKeyDown, m_shiftKeyDown;
	bool m_isMoving;

	Facing m_facing;

	SDL_Point m_interactionPoint;

	std::shared_ptr<Bar> m_healthBar;
	std::shared_ptr<Bar> m_enduranceBar;

	SDL_Rect m_collisionBox;
	bool m_colliding;

	bool m_hitboxOn;

	SDL_Texture* m_textureSheet;
	SDL_Rect m_src, m_dst;
	int m_textureCount, m_textureState;

	bool m_createMode;
	int m_selectedCreateTileIndex;

	SDL_Rect m_interactionArea;

	Vector2 m_velocity;


	void checkCollision();
	bool checkOutOfBounds();
	void flipTexture();
	void nextTile();
	void checkMapInteraction();
	
};
