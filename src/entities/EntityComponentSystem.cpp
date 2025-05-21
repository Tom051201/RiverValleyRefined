#include "EntityComponentSystem.h"
#include <fstream>

#include "../../includes/json.hpp"
using json = nlohmann::json;

void ComponentManager::entityDestroyed(Entity e) {
	for (auto const& pair : m_componentArrays) {
		pair.second->entityDestroyed(e);
	}
}



EntityManager::EntityManager() {
	m_nextEntity = 0;
}

EntityManager::~EntityManager() {}

Entity EntityManager::createEntity() { return m_nextEntity++; }

void EntityManager::destroyEntity(Entity e) { m_signatures.erase(e); }

void EntityManager::setSignature(Entity e, Signature signature) { m_signatures[e] = signature; }

Signature EntityManager::getSignature(Entity e) { return m_signatures[e]; }

std::unordered_map<Entity, Signature>& EntityManager::getAllSignatures() { return m_signatures; }



void SystemManager::entityDestroyed(Entity e) {
	for (auto const& pair : m_systems) {
		pair.second->entities.erase(e);
	}
}

void SystemManager::entitySignatureChanged(Entity e, Signature eSignature) {
	for (auto const& pair : m_systems) {
		const char* typeName = pair.first;
		auto const& system = pair.second;
		auto const& systemSignature = m_signatures[typeName];

		if ((eSignature & systemSignature) == systemSignature) {
			system->entities.insert(e);
		}
		else {
			system->entities.erase(e);
		}
	}
}



ECS::ECS() {}

ECS::~ECS() {
//	std::cout << "ECS deleted" << std::endl;
}

Entity ECS::createEntity() { return m_entityManager.createEntity(); }

void ECS::destroyEntity(Entity e) {
	m_entityManager.destroyEntity(e);
	m_componentManager.entityDestroyed(e);
	m_systemManager.entityDestroyed(e);
}

void ECS::loadData(const std::string& path) {
	std::ifstream file(path);
	if (!file.is_open()) return;

	json data;
	file >> data;

	for (const auto& entityData : data["entities"]) {
		Entity entity = createEntity();
		
		if (entityData.contains("Position")) {
			PositionComponent pos{
				entityData["Position"]["x"],
				entityData["Position"]["y"]
			};
			addComponent(entity, pos);
		}

		if (entityData.contains("Velocity")) {
			VelocityComponent vel{
				entityData["Velocity"]["dx"],
				entityData["Velocity"]["dy"]
			};
			addComponent(entity, vel);
		}

		if (entityData.contains("Health")) {
			HealthComponent health{
				entityData["Health"]["current"],
				entityData["Health"]["max"]
			};
			addComponent(entity, health);
		}

		if (entityData.contains("Sprite")) {
			auto& jSprite = entityData["Sprite"];
			SDL_Rect src;
			SDL_Rect dst;

			if (jSprite["src"].is_array()) {
				src = {
					jSprite["src"][0],
					jSprite["src"][1],
					jSprite["src"][2],
					jSprite["src"][3]
				};
			}
			else { src = { 0, 0, 16, 16 }; }

			if (jSprite["dst"].is_array()) {
				dst = {
					jSprite["dst"][0],
					jSprite["dst"][1],
					jSprite["dst"][2],
					jSprite["dst"][3]
				};
			}
			else { dst = { 0, 0, 64, 64 }; }
			
			SpriteComponent sprite(jSprite["texture"], src, dst);
			sprite.texture = TextureManager::loadTexture(sprite.path.c_str());
			addComponent(entity, sprite);
		}

		if (entityData.contains("Collider")) {
			ColliderComponent collider{
				entityData["Collider"]["width"],
				entityData["Collider"]["height"]
			};
			addComponent(entity, collider);
		}

		if (entityData.contains("AI")) {
			AIComponent ai{
				entityData["AI"]["timer"]
			};
			addComponent(entity, ai);
		}

	}
}

void ECS::saveData(const std::string& path) {
	json saveData;

	int index = 0;
	for (const auto& [entity, signature] : m_entityManager.getAllSignatures()) {
		json entityData;

		if (signature.test(getComponentTypeID<PositionComponent>())) {
			auto& pos = getComponent<PositionComponent>(entity);
			entityData["Position"] = { {"x", pos.x}, {"y", pos.y} };
		}

		if (signature.test(getComponentTypeID<VelocityComponent>())) {
			auto& vel = getComponent<VelocityComponent>(entity);
			entityData["Velocity"] = { {"dx", vel.dx}, {"dy", vel.dy} };
		}

		if (signature.test(getComponentTypeID<HealthComponent>())) {
			auto& health = getComponent<HealthComponent>(entity);
			entityData["Health"] = { {"current", health.current}, {"max", health.max} };
		}

		if (signature.test(getComponentTypeID<SpriteComponent>())) {
			auto& sprite = getComponent<SpriteComponent>(entity);
			entityData["Sprite"] = {
				{"texture", sprite.path},
				{ "src", { sprite.src.x, sprite.src.y, sprite.src.w, sprite.src.h } },
				{ "dst", { sprite.dst.x, sprite.dst.y, sprite.dst.w, sprite.dst.h } }
			};
		}

		if (signature.test(getComponentTypeID<ColliderComponent>())) {
			auto& collider = getComponent<ColliderComponent>(entity);
			entityData["Collider"] = { {"width", collider.width}, {"height", collider.height} };
		}

		if (signature.test(getComponentTypeID<AIComponent>())) {
			auto& ai = getComponent<AIComponent>(entity);
			entityData["AI"] = { {"timer", ai.timer} };
		}

		saveData["entities"].push_back(entityData);
	}

	std::ofstream file(path);
	file << saveData.dump(4);
}
