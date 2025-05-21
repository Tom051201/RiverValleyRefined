#include "TileHandler.h"
#include "../Utils.h"
#include <iostream>

TileHandler::TileHandler(const char* textureSheet) {
	m_textureSheet = TextureManager::loadTexture(textureSheet);

	add(GRASS, new Tile(this->m_textureSheet, 0, 0, false, { 80, 130, 52, 255 })); // grass
	add(TREE, new Tile(this->m_textureSheet, 0, 3, true, { 99, 74, 33, 255 })); // tree
	add(ROCK, new Tile(this->m_textureSheet, 0, 5, true, { 74, 74, 74, 255 })); // rock
	add(TREE_STUMP, new Tile(this->m_textureSheet, 0, 4, false, { 80, 130, 52, 255 })); // tree stump
	add(WATER, new Tile(this->m_textureSheet, 1, 0, false, { 34, 118, 188, 255 }));
	add(WATER_REFLECTION, new Tile(this->m_textureSheet, 1, 1, false, { 34, 118, 188, 255 }));
	add(WATER_DEEP, new Tile(this->m_textureSheet, 1, 2, true, { 34, 105, 164, 255 }));
	add(WATER_LINE_UP, new Tile(this->m_textureSheet, 1, 3, false, { 34, 118, 188, 255 }));
	add(WATER_LINE_LEFT, new Tile(this->m_textureSheet, 1, 4, false, { 34, 118, 188, 255 }));
	add(WATER_LINE_DOWN, new Tile(this->m_textureSheet, 1, 5, false, { 34, 118, 188, 255 }));
	add(WATER_LINE_RIGHT, new Tile(this->m_textureSheet, 1, 6, false, { 34, 118, 188, 255 }));
	add(WATER_BIG_CORNER_DOWN_LEFT, new Tile(this->m_textureSheet, 1, 7, false, { 34, 118, 188, 255 }));
	add(WATER_BIG_CORNER_DOWN_RIGHT, new Tile(this->m_textureSheet, 1, 8, false, { 34, 118, 188, 255 }));
	add(WATER_BIG_CORNER_UP_LEFT, new Tile(this->m_textureSheet, 1, 9, false, { 34, 118, 188, 255 }));
	add(WATER_BIG_CORNER_UP_RIGHT, new Tile(this->m_textureSheet, 2, 0, false, { 34, 118, 188, 255 }));
	add(WATER_SMALL_CORNER_DOWN_LEFT, new Tile(this->m_textureSheet, 2, 1, false, { 34, 118, 188, 255 }));
	add(WATER_SMALL_CORNER_DOWN_RIGHT, new Tile(this->m_textureSheet, 2, 2, false, { 34, 118, 188, 255 }));
	add(WATER_SMALL_CORNER_UP_LEFT, new Tile(this->m_textureSheet, 2, 3, false, { 34, 118, 188, 255 }));
	add(WATER_SMALL_CORNER_UP_RIGHT, new Tile(this->m_textureSheet, 2, 4, false, { 34, 118, 188, 255 }));
	add(GRASS_FLOWERS, new Tile(this->m_textureSheet, 0, 6, false, { 80, 130, 52, 255 }));
	add(WATER_FISHING, new Tile(this->m_textureSheet, 2, 5, false, { 34, 118, 188, 255 }));
}

TileHandler::~TileHandler() {
	for (auto& e : m_tiles) {
		delete e.second;
	}
	m_tiles.clear();

	SDL_DestroyTexture(m_textureSheet);

//	std::cout << "TileHandler deleted" << std::endl;
}

void TileHandler::add(int index, Tile* tile) {
	m_tiles[index] = tile;
}

int TileHandler::getIndex(Tile* tile) const {
	for (auto& e : m_tiles) {
		if (tile == e.second) {
			return e.first;
		}
	}
}
