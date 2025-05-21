#pragma once
#include "SDL.h"


class Camera {
public:
	
	Camera(int worldX, int worldY, int screenWidth, int screenHeight);
	~Camera();

	void update(int worldX, int worldY);
	void windowResized(int windowWidth, int windowHeight);

	SDL_Point getScreenCenter() const;
	SDL_Point getCameraPosition() const;

private:

	int m_posX;
	int m_posY;
	int m_screenWidth;
	int m_screenHeight;

};
