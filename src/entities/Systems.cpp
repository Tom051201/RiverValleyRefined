#include "SDL.h"
#include "Systems.h"
#include "../Utils.h"

void MovementSystem::update(ECS& coordinator) {
	for (auto const& e : entities) {
		auto& pos = coordinator.getComponent<PositionComponent>(e);
		auto& vel = coordinator.getComponent<VelocityComponent>(e);
		pos.x += vel.dx;
		pos.y += vel.dy;
	}
}

void RenderSystem::render(ECS& coordinator, SDL_Renderer* renderer, const Camera& cam) {
	if (!renderer) return;
	int screenX = 0;
	int screenY = 0;

	for (auto const& e : entities) {
		auto& pos = coordinator.getComponent<PositionComponent>(e);
		auto& sprite = coordinator.getComponent<SpriteComponent>(e);
		
		screenX = pos.x - cam.getCameraPosition().x + cam.getScreenCenter().x;
		screenY = pos.y - cam.getCameraPosition().y + cam.getScreenCenter().y;

		sprite.dst.x = screenX;
		sprite.dst.y = screenY;

		TextureManager::render(sprite.texture, sprite.src, sprite.dst);
	}
}

void WanderSystem::update(ECS& coordinator) {
	for (auto const& e : entities) {
		auto& ai = coordinator.getComponent<AIComponent>(e);
		auto& vel = coordinator.getComponent<VelocityComponent>(e);

		ai.timer -= 1.0f;
		if (ai.timer <= 0.0f) {
			ai.timer = rand() % 60 + 30;
			vel.dx = (rand() % 3 - 1) * 1;
			vel.dy = (rand() % 3 - 1) * 1;
		}
	}
}
