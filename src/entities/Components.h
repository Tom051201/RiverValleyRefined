#pragma once
#include "SDL.h"
#include <string>

struct PositionComponent {
	int x;
	int y;
};

struct VelocityComponent {
	float dx;
	float dy;
};

struct HealthComponent {
	int current;
	int max;
};

struct SpriteComponent {
	SpriteComponent() : path(""), texture(nullptr), src{}, dst{} {}
	SpriteComponent(const std::string& _path, SDL_Rect _src, SDL_Rect _dst) : path(_path), texture(nullptr), src(_src), dst(_dst) {}
	std::string path;
	SDL_Texture* texture;
	SDL_Rect src, dst;
};

struct ColliderComponent {
	int width;
	int height;
};

struct AIComponent {
	float timer;
};
