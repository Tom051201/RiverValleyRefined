#pragma once
#include <vector>
#include <string>
#include "../Tile.h"
#include "../../screens/Game.h"
#include "../../Application.h"

using TileMap2D = std::vector<std::vector<Tile*>>;
using IntMap2D = std::vector<std::vector<int>>;

class Player;
class Game;

class Realm {
public:

	Realm(const char* mapFile, Game* game, Application* app);
	~Realm();

	void render();

	inline Tile* getTileAt(int row, int col) const { return tileMap[row][col]; }
	inline int getMaxRows() const { return maxRows; }
	inline int getMaxCols() const { return maxCols; }
	inline void setBgColor(SDL_Color c) { bgColor = c; }

	void overwriteTile(int x, int y, int tileIndex);

	bool loadMapFile();
	bool saveMapFile();

	int getTileRow(Tile* tile) const;
	int getTileCol(Tile* tile) const;

	void setSpawn(int x, int y);
	inline int getSpawnX() const { return spawnX; }
	inline int getSpawnY() const { return spawnY; }

private:

	TileMap2D tileMap;
	int maxRows, maxCols;
	Game* game;
	Application* app;
	SDL_Color bgColor;
	const char* mapFile;
	int spawnX, spawnY;

};
