#include "Camera.h"
#include <iostream>

Camera::Camera(int worldX, int worldY, int screenWidth, int screenHeight) {
	m_posX = worldX;
	m_posY = worldY;
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
}

Camera::~Camera() {
//	std::cout << "Camera deleted" << std::endl;
}

void Camera::update(int worldX, int worldY) {
	m_posX = worldX;
	m_posY = worldY;
}

void Camera::windowResized(int windowWidth, int windowHeight) {
	m_screenWidth = windowWidth;
	m_screenHeight = windowHeight;
}

SDL_Point Camera::getScreenCenter() const {
	return { (m_screenWidth / 2) - 32, (m_screenHeight / 2) - 32 };
}

SDL_Point Camera::getCameraPosition() const {
	return { m_posX, m_posY };
}
