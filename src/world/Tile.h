#pragma once
#include "SDL.h"

class Tile {
public:

	Tile(SDL_Texture* sheet, int x, int y, int texDim, bool solid, SDL_Color c);
	Tile(SDL_Texture* sheet, int x, int y, bool solid, SDL_Color c);
	Tile();
	~Tile();

	inline SDL_Texture* getTextureSheet() const { return m_textureSheet; }
	inline const SDL_Rect& getSrcRect() const { return m_src; }
	inline bool isSolid() const { return m_solid; }
	inline void setColor(SDL_Color c) { m_color = c; }
	inline const SDL_Color& getColor() const { return m_color; }

private:

	SDL_Texture* m_textureSheet;
	SDL_Rect m_src;
	SDL_Color m_color;
	int m_texDim;
	bool m_solid;

};
