#pragma once
#include <map>
#include "Tile.h"

class TileHandler {
public:

	static const int GRASS = 0;
	static const int TREE = 1;
	static const int ROCK = 2;
	static const int TREE_STUMP = 3;
	static const int WATER = 4;
	static const int WATER_DEEP = 5;
	static const int WATER_REFLECTION = 6;
	static const int WATER_LINE_UP = 7;
	static const int WATER_LINE_RIGHT = 8;
	static const int WATER_LINE_DOWN = 9;
	static const int WATER_LINE_LEFT = 10;
	static const int WATER_BIG_CORNER_DOWN_LEFT = 11;
	static const int WATER_BIG_CORNER_DOWN_RIGHT = 12;
	static const int WATER_BIG_CORNER_UP_LEFT = 13;
	static const int WATER_BIG_CORNER_UP_RIGHT = 14;
	static const int WATER_SMALL_CORNER_DOWN_LEFT = 15;
	static const int WATER_SMALL_CORNER_DOWN_RIGHT = 16;
	static const int WATER_SMALL_CORNER_UP_LEFT = 17;
	static const int WATER_SMALL_CORNER_UP_RIGHT = 18;
	static const int GRASS_FLOWERS = 19;
	static const int WATER_FISHING = 20;

	TileHandler(const char* textureSheet);
	~TileHandler();

	void add(int index, Tile* tile);

	inline Tile* getTile(int index) { return m_tiles[index]; }
	int getIndex(Tile* tile) const;
	inline int getTilesAmount() const { return m_tiles.size(); }

private:

	std::map<int, Tile*> m_tiles;
	SDL_Texture* m_textureSheet;

};
