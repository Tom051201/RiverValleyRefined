#include "Tile.h"
#include "../Utils.h"
#include <iostream>

Tile::Tile(SDL_Texture* sheet, int row, int col, int texDim, bool solid, SDL_Color c) {
	m_textureSheet = sheet;
	m_texDim = texDim;
	m_src = { col * texDim, row * texDim, texDim, texDim };
	m_solid = solid;
	this->setColor(c);
}

Tile::Tile(SDL_Texture* sheet, int row, int col, bool solid, SDL_Color c) {
	m_textureSheet = sheet;
	m_texDim = 16;
	m_src = { col * m_texDim, row * m_texDim, m_texDim, m_texDim };
	m_solid = solid;
	this->setColor(c);
}

Tile::Tile() {
	m_textureSheet = nullptr;
	m_texDim = 16;
	m_src = { 0, 0, 0, 0 };
	m_solid = false;
	this->setColor({ 0, 0, 0, 255 });
}

Tile::~Tile() {
	
}
