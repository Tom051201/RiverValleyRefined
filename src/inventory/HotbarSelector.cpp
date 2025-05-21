#include "HotbarSelector.h"
#include <iostream>

HotbarSelector::HotbarSelector(PlayerInventory* linkedInv, Application* app) {
	this->linkedInv = linkedInv;
	this->app = app;
	m_dim = { linkedInv->getHotbarX(), linkedInv->getHotbarY(), PlayerInventory::SLOT_SIZE, PlayerInventory::SLOT_SIZE };
	m_selectedSlot = linkedInv->getEquipmentSlot(ItemType::RESSOURCE_ITEM);
	m_currentSlotIndex = 0;
}

HotbarSelector::~HotbarSelector() {}

void HotbarSelector::render() {
	SDL_SetRenderDrawColor(Application::renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(Application::renderer, &m_dim);

	if (!m_selectedSlot->isEmpty()) {
		TextManager::renderStringCentered(
			m_selectedSlot->getItem()->getName().c_str(),
			Fonts::DEFAULT,
			FontSize::DEFAULT_MID,
			{ 0, 0, 0, 255 },
			linkedInv->getHotbarX() + linkedInv->getHotbarWidth() / 2,
			linkedInv->getHotbarY() - 20
		);
	}
	
}

void HotbarSelector::handleEvents() {
	
	if (app->getAppEvent().type == SDL_MOUSEWHEEL) {
		if (app->getAppEvent().wheel.y > 0) {
			// up
			moveRight();
		}
		else if (app->getAppEvent().wheel.y < 0) {
			// down
			moveLeft();
		}
	}

	if (app->getAppEvent().type == SDL_MOUSEBUTTONUP) {
		switch (app->getAppEvent().button.button) {
			case SDL_BUTTON_LEFT:
				if (!m_selectedSlot->isEmpty()) {
					m_selectedSlot->getItem()->use();
				}
				break;

			default: break;
		}
	}
}

void HotbarSelector::windowResized() {
	m_dim = { linkedInv->getHotbarX(), linkedInv->getHotbarY(), PlayerInventory::SLOT_SIZE, PlayerInventory::SLOT_SIZE };
	updatePositionFromIndex();
}

void HotbarSelector::moveRight() {
	if (m_currentSlotIndex < 3) {
		m_currentSlotIndex++;
		updatePositionFromIndex();
	}
}

void HotbarSelector::moveLeft() {
	if (m_currentSlotIndex > 0) {
		m_currentSlotIndex--;
		updatePositionFromIndex();
	}
}

void HotbarSelector::updatePositionFromIndex() {
	m_dim.x = linkedInv->getHotbarX() + m_currentSlotIndex * PlayerInventory::SLOT_SIZE;

	int index = (m_dim.x - linkedInv->getHotbarX()) / PlayerInventory::SLOT_SIZE;
	switch (index) {
		case 0: m_selectedSlot = linkedInv->getEquipmentSlot(ItemType::RESSOURCE_ITEM); break;
		case 1: m_selectedSlot = linkedInv->getEquipmentSlot(ItemType::WEAPON_ITEM); break;
		case 2: m_selectedSlot = linkedInv->getEquipmentSlot(ItemType::TOOL_ITEM); break;
		case 3: m_selectedSlot = linkedInv->getEquipmentSlot(ItemType::FOOD_ITEM); break;
		default: break;
	}
}
