#pragma once
#include <memory>
#include "../items/Item.h"
#include "../Application.h"
#include "InventorySlot.h"
#include "PlayerInventory.h"

class PlayerInventory;

class InventoryPicker {
public:

	InventoryPicker(Application* app, PlayerInventory* pi, Game* game);
	~InventoryPicker();
	
	void update();
	void render();
	void handleEvents();

	void pick();
	void drop();

private:

	Application* app;
	PlayerInventory* m_linkedInv;
	Game* game;
	std::shared_ptr<Item> m_pickedItem;
	int m_pickedAmount;

	std::shared_ptr<InventorySlot> m_originSlot;

	int m_mouseX, m_mouseY;
	bool m_isDragging;

	void reset();
	void swapOrPlaceInSlot(std::shared_ptr<InventorySlot> targetSlot);
	bool isInBounds(int x, int y, const SDL_Rect& b);
	std::shared_ptr<InventorySlot> getSlotUnderMouse(int x, int y);
	std::shared_ptr<InventorySlot> getHotbarSlotByMouse(int x);
	std::shared_ptr<InventorySlot> getArmorSlotByMouse(int y);

};
