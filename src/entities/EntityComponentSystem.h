#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <bitset>
#include <memory>
#include <set>
#include <string>
#include "../screens/game/Camera.h"
#include "../filehandling/gamedata/GameDataHandler.h"

#include "Components.h"

constexpr int MAX_COMPONENTS = 32;
using ComponentTypeID = std::uint8_t;
using Entity = std::uint32_t;
using Signature = std::bitset<MAX_COMPONENTS>;

class ECS;



class Component {
public:
	virtual ~Component() = default;
};



class IComponentArray {
public:
	virtual ~IComponentArray() = default;
	virtual void entityDestroyed(Entity e) = 0;
};



template <typename T>
class ComponentArray : public IComponentArray {
public:

	void insertData(Entity e, T component) { m_componentMap[e] = component; }

	void removeData(Entity e) { m_componentMap.erase(e); }

	T& getComponent(Entity e) { return m_componentMap.at(e); }

	void entityDestroyed(Entity e) override { m_componentMap.erase(e); }

private:

	std::unordered_map<Entity, T> m_componentMap;

};



class ComponentManager {
public:

	template <typename T>
	void registerComponent() {
		const char* typeName = typeid(T).name();
		m_componentTypeIDs[typeName] = nextComponentTypeID++;
		m_componentArrays[typeName] = std::make_shared<ComponentArray<T>>();
	}

	template <typename T>
	ComponentTypeID getComponentTypeID() {
		const char* typeName = typeid(T).name();
		return m_componentTypeIDs[typeName];
	}

	template <typename T>
	void addComponent(Entity e, T component) {
		getComponentArray<T>()->insertData(e, component);
	}

	template <typename T>
	T& getComponent(Entity e) {
		return getComponentArray<T>()->getComponent(e);
	}

	void entityDestroyed(Entity e);

private:

	std::unordered_map<const char*, ComponentTypeID> m_componentTypeIDs;
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_componentArrays;
	ComponentTypeID nextComponentTypeID{};

	template <typename T>
	std::shared_ptr<ComponentArray<T>> getComponentArray() {
		const char* typeName = typeid(T).name();
		return std::static_pointer_cast<ComponentArray<T>>(m_componentArrays[typeName]);
	}

};



class EntityManager {
public:

	EntityManager();
	~EntityManager();

	Entity createEntity();

	void destroyEntity(Entity e);

	void setSignature(Entity e, Signature signature);

	Signature getSignature(Entity e);

	std::unordered_map<Entity, Signature>& getAllSignatures();

private:

	Entity m_nextEntity;
	std::unordered_map<Entity, Signature> m_signatures;

};



class System {
public:
	virtual ~System() = default;
	virtual void update(ECS& coordinator) {};
	virtual void render(ECS& coordinator, SDL_Renderer* renderer, const Camera& cam) {};
	std::set<Entity> entities;
};



class SystemManager {
public:

	template <typename T>
	std::shared_ptr<T> registerSystem() {
		const char* typeName = typeid(T).name();
		auto system = std::make_shared<T>();
		m_systems[typeName] = system;
		return system;
	}

	template <typename T>
	void setSignature(Signature signature) {
		const char* typeName = typeid(T).name();
		m_signatures[typeName] = signature;
	}

	void entityDestroyed(Entity e);

	void entitySignatureChanged(Entity e, Signature eSignature);

private:

	std::unordered_map<const char*, std::shared_ptr<System>> m_systems;
	std::unordered_map<const char*, Signature> m_signatures;

};



class ECS {
public:

	ECS();
	~ECS();

	Entity createEntity();

	void destroyEntity(Entity e);

	template <typename T>
	void registerComponent() {
		m_componentManager.registerComponent<T>();
	}

	template <typename T>
	void addComponent(Entity e, T component) {
		m_componentManager.addComponent<T>(e, component);

		auto signature = m_entityManager.getSignature(e);
		signature.set(m_componentManager.getComponentTypeID<T>(), true);
		m_entityManager.setSignature(e, signature);

		m_systemManager.entitySignatureChanged(e, signature);
	}

	template <typename T>
	T& getComponent(Entity e) {
		return m_componentManager.getComponent<T>(e);
	}

	template <typename T>
	ComponentTypeID getComponentTypeID() {
		return m_componentManager.getComponentTypeID<T>();
	}

	template <typename T>
	std::shared_ptr<System> registerSystem() {
		return m_systemManager.registerSystem<T>();
	}

	template <typename T>
	void setSystemSignature(Signature s) {
		m_systemManager.setSignature<T>(s);
	}

	void loadData(const std::string& path);
	void saveData(const std::string& path);
	

private:

	ComponentManager m_componentManager;
	EntityManager m_entityManager;
	SystemManager m_systemManager;

};

