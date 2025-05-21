#pragma once
#include "EntityComponentSystem.h"
#include "Components.h"

class MovementSystem : public System {
public:
	void update(ECS& coordinator) override;
};

class RenderSystem : public System {
public:
	void render(ECS& coordinator, SDL_Renderer* renderer, const Camera& cam) override;
};

class WanderSystem : public System {
public:
	void update(ECS& coordinator) override;

};
