#pragma once
#include "PlayerInventory.h"

class HotbarSelector {
public:

	HotbarSelector(PlayerInventory* linkedInv, Application* app);
	~HotbarSelector();

	void render();
	void handleEvents();
	void windowResized();

	void moveRight();
	void moveLeft();

private:

	PlayerInventory* linkedInv;
	Application* app;
	SDL_Rect m_dim;

	int m_currentSlotIndex;
	std::shared_ptr<InventorySlot> m_selectedSlot;

	void updatePositionFromIndex();

};
