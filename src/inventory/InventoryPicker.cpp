#include "InventoryPicker.h"
#include "../Utils.h"
#include "PlayerInventory.h"

InventoryPicker::InventoryPicker(Application* app, PlayerInventory* pi, Game* game) {
	this->app = app;
	m_linkedInv = pi;
	this->game = game;
	m_pickedItem = nullptr;
	m_pickedAmount = 0;
	
	SDL_GetMouseState(&m_mouseX, &m_mouseY);
}

InventoryPicker::~InventoryPicker() {}

void InventoryPicker::update() {
	if (m_pickedItem != nullptr && m_isDragging) {
		SDL_GetMouseState(&m_mouseX, &m_mouseY);
	}
}

void InventoryPicker::render() {
	if (m_pickedItem != nullptr && m_isDragging) {
		int itemRenderSize = m_linkedInv->getItemSize() * 0.9;

		SDL_Rect r = { m_mouseX - (itemRenderSize / 2), m_mouseY - (itemRenderSize / 2), itemRenderSize, itemRenderSize};
		TextureManager::render(m_pickedItem->getTextureSheet(), m_pickedItem->getSrc(), r);
	}
}

void InventoryPicker::handleEvents() {
	if (app->getAppEvent().type == SDL_MOUSEBUTTONDOWN) {
		switch (app->getAppEvent().button.button) {
		case SDL_BUTTON_LEFT:
			m_isDragging = true;
			pick();
			break;

		default: break;
		}
	}

	if (app->getAppEvent().type == SDL_MOUSEBUTTONUP) {
		switch (app->getAppEvent().button.button) {
		case SDL_BUTTON_LEFT:
			m_isDragging = false;
			drop();
			break;

		default: break;
		}
	}
}

void InventoryPicker::pick() {
	SDL_GetMouseState(&m_mouseX, &m_mouseY);

	m_originSlot = getSlotUnderMouse(m_mouseX, m_mouseY);
	if (m_originSlot && !m_originSlot->isEmpty()) {
		m_pickedItem = m_originSlot->getItem();
		m_pickedAmount = m_originSlot->getAmount();
		m_originSlot->clear();
	}

}

void InventoryPicker::drop() {
	SDL_GetMouseState(&m_mouseX, &m_mouseY);
	if (!m_pickedItem) return;

	if (m_mouseX >= m_linkedInv->getInvX() && m_mouseX <= m_linkedInv->getInvX() + m_linkedInv->getInvWidth() &&
		m_mouseY >= m_linkedInv->getInvY() && m_mouseY <= m_linkedInv->getInvY() + m_linkedInv->getInvHeight()) {
		// dropped inside inv

		int dropSlotRow = (m_mouseY - m_linkedInv->getInvY()) / m_linkedInv->getInvSlotSize();
		int dropSlotCol = (m_mouseX - m_linkedInv->getInvX()) / m_linkedInv->getInvSlotSize();

		if (m_linkedInv->getInventorySlot(dropSlotRow, dropSlotCol)->isEmpty()) {
			// dropped on empty slot
			m_linkedInv->setSlot(m_pickedItem, m_pickedAmount, dropSlotRow, dropSlotCol);
			reset();
		}
		else {
			if (m_linkedInv->getInventorySlot(dropSlotRow, dropSlotCol)->getItem()->getID() == m_pickedItem->getID()) {
				// dropped on same item id (adds up)
				m_linkedInv->getInventorySlot(dropSlotRow, dropSlotCol)->increaseAmount(m_pickedAmount);
				reset();
			}
			else {
				// dropped on full slot
				m_originSlot->setItem(m_pickedItem, m_pickedAmount);
				reset();
			}
		}
	}
	else {
		// dropped outside inventory

		// dropped inside hotbar
		if (m_mouseX >= m_linkedInv->getHotbarX() && m_mouseX <= m_linkedInv->getHotbarX() + m_linkedInv->getHotbarWidth() &&
			m_mouseY >= m_linkedInv->getHotbarY() && m_mouseY <= m_linkedInv->getHotbarY() + m_linkedInv->getHotbarHeight()) {

			switch (m_pickedItem->getType()) {
			case ItemType::RESSOURCE_ITEM:
				swapOrPlaceInSlot(m_linkedInv->getEquipmentSlot(ItemType::RESSOURCE_ITEM));
				break;

			case ItemType::WEAPON_ITEM:
				swapOrPlaceInSlot(m_linkedInv->getEquipmentSlot(ItemType::WEAPON_ITEM));
				break;

			case ItemType::TOOL_ITEM:
				swapOrPlaceInSlot(m_linkedInv->getEquipmentSlot(ItemType::TOOL_ITEM));
				break;

			case ItemType::FOOD_ITEM:
				swapOrPlaceInSlot(m_linkedInv->getEquipmentSlot(ItemType::FOOD_ITEM));
				break;

			default: break;
			}
		}

		// dropped inside armor bar
		if (m_mouseX >= m_linkedInv->getArmorBarX() && m_mouseX <= m_linkedInv->getArmorBarX() + m_linkedInv->getArmorBarWidth() &&
			m_mouseY >= m_linkedInv->getArmorBarY() && m_mouseY <= m_linkedInv->getArmorBarY() + m_linkedInv->getArmorBarHeight()) {

			switch (m_pickedItem->getType()) {
			case ItemType::HELMET_ITEM:
				swapOrPlaceInSlot(m_linkedInv->getEquipmentSlot(ItemType::HELMET_ITEM));
				break;

			case ItemType::CHESTPLATE_ITEM:
				swapOrPlaceInSlot(m_linkedInv->getEquipmentSlot(ItemType::CHESTPLATE_ITEM));
				break;

			case ItemType::LEGGINGS_ITEM:
				swapOrPlaceInSlot(m_linkedInv->getEquipmentSlot(ItemType::LEGGINGS_ITEM));
				break;

			case ItemType::BOOTS_ITEM:
				swapOrPlaceInSlot(m_linkedInv->getEquipmentSlot(ItemType::BOOTS_ITEM));
				break;

			default: break;
			}

		}

		if (m_pickedItem != nullptr) {
			m_originSlot->setItem(m_pickedItem, m_pickedAmount);
			reset();
		}

	}
	
}

void InventoryPicker::reset() {
	m_originSlot = nullptr;
	m_pickedItem = nullptr;
	m_pickedAmount = 0;

	app->globalSounds->playEffect(GlobalSoundsList::EQUIP_ITEM, Channels::GAME_SOUNDS);
}

void InventoryPicker::swapOrPlaceInSlot(std::shared_ptr<InventorySlot> targetSlot) {
	if (targetSlot->isEmpty()) {
		targetSlot->setItem(m_pickedItem, m_pickedAmount);
	}
	else {
		m_originSlot->setItem(targetSlot->getItem(), targetSlot->getAmount());
		targetSlot->clear();
		targetSlot->setItem(m_pickedItem, m_pickedAmount);
	}
	reset();
}

bool InventoryPicker::isInBounds(int x, int y, const SDL_Rect& b) {
	return x >= b.x &&
		x <= b.x + b.w &&
		y >= b.y &&
		y <= b.y + b.h;
}

std::shared_ptr<InventorySlot> InventoryPicker::getSlotUnderMouse(int x, int y) {
	// inventory
	if (isInBounds(x, y, m_linkedInv->getInventoryDim())) {
		int row = (y - m_linkedInv->getInvY()) / m_linkedInv->getInvSlotSize();
		int col = (x - m_linkedInv->getInvX()) / m_linkedInv->getInvSlotSize();
		return m_linkedInv->getInventorySlot(row, col);
	}

	// hotbar
	if (isInBounds(x, y, m_linkedInv->getHotbarDim())) {
		return getHotbarSlotByMouse(x);
	}

	// armor bar
	if (isInBounds(x, y, m_linkedInv->getArmorBarDim())) {
		return getArmorSlotByMouse(y);
	}

	return nullptr;
}

std::shared_ptr<InventorySlot> InventoryPicker::getHotbarSlotByMouse(int x) {
	int baseX = m_linkedInv->getHotbarX();
	int slotSize = PlayerInventory::SLOT_SIZE;

	if (x < baseX + slotSize) return m_linkedInv->getEquipmentSlot(ItemType::RESSOURCE_ITEM);
	if (x < baseX + slotSize * 2) return m_linkedInv->getEquipmentSlot(ItemType::WEAPON_ITEM);
	if (x < baseX + slotSize * 3) return m_linkedInv->getEquipmentSlot(ItemType::TOOL_ITEM);
	if (x < baseX + slotSize * 4) return m_linkedInv->getEquipmentSlot(ItemType::FOOD_ITEM);

	return nullptr;
}

std::shared_ptr<InventorySlot> InventoryPicker::getArmorSlotByMouse(int y) {
	int baseY = m_linkedInv->getArmorBarY();
	int slotSize = PlayerInventory::SLOT_SIZE;

	if (y < baseY + slotSize) return m_linkedInv->getEquipmentSlot(ItemType::HELMET_ITEM);
	if (y < baseY + slotSize * 2) return m_linkedInv->getEquipmentSlot(ItemType::CHESTPLATE_ITEM);
	if (y < baseY + slotSize * 3) return m_linkedInv->getEquipmentSlot(ItemType::LEGGINGS_ITEM);
	if (y < baseY + slotSize * 4) return m_linkedInv->getEquipmentSlot(ItemType::BOOTS_ITEM);

	return nullptr;
}
