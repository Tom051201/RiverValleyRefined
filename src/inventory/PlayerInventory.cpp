#include "PlayerInventory.h"
#include <iostream>
#include "SDL.h"

PlayerInventory::PlayerInventory(Application* app, Game* game, int rows, int cols, int x, int y) {
	this->app = app;
	this->game = game;

	m_invRows = rows;
	m_invCols = cols;
	m_open = false;

	m_invDim = { x, y, cols * SLOT_SIZE, rows * SLOT_SIZE };
	m_hotbarDim = { 0, 0, 4 * SLOT_SIZE, SLOT_SIZE };
	m_armorBarDim = { 0, 0, SLOT_SIZE, 4 * SLOT_SIZE };
	
	for (int r = 0; r < m_invRows; r++) {
		std::vector<std::shared_ptr<InventorySlot>> row;
		for (int c = 0; c < m_invCols; c++) {
			row.push_back(std::make_shared<InventorySlot>());
		}
		m_inventorySlots.push_back(row);
	}

	m_equipmentSlots[ItemType::RESSOURCE_ITEM] = { std::make_shared<TypeBoundSlot>(ItemType::RESSOURCE_ITEM), {0, 0, SLOT_SIZE, SLOT_SIZE} };
	m_equipmentSlots[ItemType::WEAPON_ITEM] = { std::make_shared<TypeBoundSlot>(ItemType::WEAPON_ITEM), {0, 0, SLOT_SIZE, SLOT_SIZE} };
	m_equipmentSlots[ItemType::TOOL_ITEM] = { std::make_shared<TypeBoundSlot>(ItemType::TOOL_ITEM), {0, 0, SLOT_SIZE, SLOT_SIZE} };
	m_equipmentSlots[ItemType::FOOD_ITEM] = { std::make_shared<TypeBoundSlot>(ItemType::FOOD_ITEM), {0, 0, SLOT_SIZE, SLOT_SIZE} };
	m_equipmentSlots[ItemType::HELMET_ITEM] = { std::make_shared<TypeBoundSlot>(ItemType::HELMET_ITEM), {0, 0, SLOT_SIZE, SLOT_SIZE} };
	m_equipmentSlots[ItemType::CHESTPLATE_ITEM] = { std::make_shared<TypeBoundSlot>(ItemType::CHESTPLATE_ITEM), {0, 0, SLOT_SIZE, SLOT_SIZE} };
	m_equipmentSlots[ItemType::LEGGINGS_ITEM] = { std::make_shared<TypeBoundSlot>(ItemType::LEGGINGS_ITEM), {0, 0, SLOT_SIZE, SLOT_SIZE} };
	m_equipmentSlots[ItemType::BOOTS_ITEM] = { std::make_shared<TypeBoundSlot>(ItemType::BOOTS_ITEM), {0, 0, SLOT_SIZE, SLOT_SIZE} };
	

	m_picker = std::make_unique<InventoryPicker>(app, this, game);
	m_hotbarSelector = std::make_unique<HotbarSelector>(this, app);
}

PlayerInventory::~PlayerInventory() {}

void PlayerInventory::update() {
	if (m_open) m_picker->update();
}

void PlayerInventory::render() {
	if (m_open) {
		
		SDL_Rect bg = m_invDim;
		SDL_SetRenderDrawColor(app->renderer, 70, 70, 70, 220);
		SDL_RenderFillRect(app->renderer, &bg);

		// renders inventory
		for (int i = 0; i < m_invRows; i++) {
			for (int j = 0; j < m_invCols; j++) {
				SDL_Rect outlineR = { m_invDim.x + (j * SLOT_SIZE), m_invDim.y + (i * SLOT_SIZE), SLOT_SIZE, SLOT_SIZE };
				SDL_SetRenderDrawColor(app->renderer, 90, 90, 90, 255);

				renderSlot(m_inventorySlots[i][j], outlineR);

			}
		}

		// renders hotbar
		SDL_SetRenderDrawColor(app->renderer, 70, 70, 70, 220);
		SDL_RenderFillRect(app->renderer, &m_hotbarDim);
		SDL_RenderFillRect(app->renderer, &m_armorBarDim);

		SDL_SetRenderDrawColor(app->renderer, 90, 90, 90, 255);
		for (auto& s : m_equipmentSlots) {
			std::shared_ptr<InventorySlot> slot = s.second.slot;
			SDL_Rect dim = s.second.dim;

			renderSlot(slot, dim);
		}

		m_picker->render();
	}
	else {
		// renders hotbar
		SDL_SetRenderDrawColor(app->renderer, 70, 70, 70, 220);
		SDL_RenderFillRect(app->renderer, &m_hotbarDim);

		SDL_SetRenderDrawColor(app->renderer, 90, 90, 90, 255);
		for (auto& s : m_equipmentSlots) {
			if (isHotbarType(s.first)) {
				std::shared_ptr<InventorySlot> slot = s.second.slot;
				SDL_Rect dim = s.second.dim;

				renderSlot(slot, dim);
			}
		}

		m_hotbarSelector->render();
	}

}

void PlayerInventory::handleEvents() {
	if (m_open) m_picker->handleEvents();
	m_hotbarSelector->handleEvents();
}

void PlayerInventory::windowResized() {
	// inventory
	m_invDim.x = (app->getWindowWidth() / 2) - (m_invDim.w / 2);
	m_invDim.y = (app->getWindowHeight() / 2) - (m_invDim.h / 2);

	// hotbar slots
	if (m_open) {
		int yPos = m_invDim.y + m_invDim.h + 32;

		setEquipmentPos(ItemType::RESSOURCE_ITEM, m_invDim.x + (m_invDim.w / 2) - 2 * SLOT_SIZE, yPos);
		setEquipmentPos(ItemType::WEAPON_ITEM, m_invDim.x + (m_invDim.w / 2) - SLOT_SIZE, yPos);
		setEquipmentPos(ItemType::TOOL_ITEM, m_invDim.x + (m_invDim.w / 2), yPos);
		setEquipmentPos(ItemType::FOOD_ITEM, m_invDim.x + (m_invDim.w / 2) + SLOT_SIZE, yPos);
		m_hotbarDim.x = getEquipmentDim(ItemType::RESSOURCE_ITEM).x;
		m_hotbarDim.y = getEquipmentDim(ItemType::RESSOURCE_ITEM).y;
	}
	else {
		int yPos = app->getWindowHeight() - 50 - SLOT_SIZE;

		setEquipmentPos(ItemType::RESSOURCE_ITEM, app->getWindowWidth() - 50 - 4 * SLOT_SIZE, yPos);
		setEquipmentPos(ItemType::WEAPON_ITEM, app->getWindowWidth() - 50 - 3 * SLOT_SIZE, yPos);
		setEquipmentPos(ItemType::TOOL_ITEM, app->getWindowWidth() - 50 - 2 * SLOT_SIZE, yPos);
		setEquipmentPos(ItemType::FOOD_ITEM, app->getWindowWidth() - 50 - SLOT_SIZE, yPos);
		m_hotbarDim.x = getEquipmentDim(ItemType::RESSOURCE_ITEM).x;
		m_hotbarDim.y = getEquipmentDim(ItemType::RESSOURCE_ITEM).y;

		m_hotbarSelector->windowResized();
	}

	// armor bar slots
	int xPos = m_invDim.x - 32 - SLOT_SIZE;
	setEquipmentPos(ItemType::HELMET_ITEM, xPos, m_invDim.y + (m_invDim.h / 2) - 2 * SLOT_SIZE);
	setEquipmentPos(ItemType::CHESTPLATE_ITEM, xPos, m_invDim.y + (m_invDim.h / 2) - SLOT_SIZE);
	setEquipmentPos(ItemType::LEGGINGS_ITEM, xPos, m_invDim.y + (m_invDim.h / 2));
	setEquipmentPos(ItemType::BOOTS_ITEM, xPos, m_invDim.y + (m_invDim.h / 2) + SLOT_SIZE);
	m_armorBarDim.x = getEquipmentDim(ItemType::HELMET_ITEM).x;
	m_armorBarDim.y = getEquipmentDim(ItemType::HELMET_ITEM).y;
}

void PlayerInventory::print() {
	for (int i = 0; i < m_invRows; ++i) {
		for (int j = 0; j < m_invCols; ++j) {
			if (m_inventorySlots[i][j]->isEmpty()) { std::cout << "[ Empty ] "; }
			else { std::cout << "[ " << m_inventorySlots[i][j]->getItem()->getID() << " x" << m_inventorySlots[i][j]->getAmount() << " ] "; }
		}
		std::cout << "\n";
	}
}

void PlayerInventory::addItem(std::shared_ptr<Item> item, int amount) {

	// check for slot with already the item
	for (int r = 0; r < m_invRows; r++) {
		for (int c = 0; c < m_invCols; c++) {
			if (!m_inventorySlots[r][c]->isEmpty() && m_inventorySlots[r][c]->getItem()->getID() == item->getID()) {
				m_inventorySlots[r][c]->increaseAmount(amount);
				return;
			}
		}
	}

	for (int r = 0; r < m_invRows; r++) {
		for (int c = 0; c < m_invCols; c++) {
			if (m_inventorySlots[r][c]->isEmpty()) {
				m_inventorySlots[r][c]->setItem(item, amount);
				return;
			}
		}
	}

}

void PlayerInventory::setSlot(std::shared_ptr<Item> item, int amount, int row, int col) { m_inventorySlots[row][col]->setItem(item, amount); }

void PlayerInventory::openInventory() {
	m_open = true;
	app->showCursor();
	app->centerCursorInWindow();

	windowResized();
}

void PlayerInventory::closeInventory() {
	m_open = false;
	app->hideCursor();

	windowResized();
}

const bool PlayerInventory::isOpen() const { return m_open; }

void PlayerInventory::loadData(const json& data) {
	

	std::cout << "Player Inventory loaded!" << std::endl;
}

void PlayerInventory::saveData(json& data) {

	std::cout << "Player Inventory saved!" << std::endl;
}

int PlayerInventory::getInvWidth() const { return m_invDim.w; }
int PlayerInventory::getInvHeight() const { return m_invDim.h; }
int PlayerInventory::getInvX() const { return m_invDim.x; }
int PlayerInventory::getInvY() const { return m_invDim.y; }
int PlayerInventory::getInvSlotSize() const { return this->SLOT_SIZE; }

const SDL_Rect& PlayerInventory::getHotbarDim() const { return m_hotbarDim; }
int PlayerInventory::getHotbarWidth() const { return m_hotbarDim.w; }
int PlayerInventory::getHotbarHeight() const { return m_hotbarDim.h; }
int PlayerInventory::getHotbarX() const { return m_hotbarDim.x; }
int PlayerInventory::getHotbarY() const { return m_hotbarDim.y; }

const SDL_Rect& PlayerInventory::getArmorBarDim() const { return m_armorBarDim; }
int PlayerInventory::getArmorBarWidth() const { return m_armorBarDim.w; }
int PlayerInventory::getArmorBarHeight() const { return m_armorBarDim.h; }
int PlayerInventory::getArmorBarX() const { return m_armorBarDim.x; }
int PlayerInventory::getArmorBarY() const { return m_armorBarDim.y; }

int PlayerInventory::getItemSize() { return this->ITEM_SIZE; }

std::shared_ptr<InventorySlot> PlayerInventory::getInventorySlot(int row, int col) { return m_inventorySlots[row][col]; }
std::shared_ptr<TypeBoundSlot> PlayerInventory::getEquipmentSlot(ItemType type) { return m_equipmentSlots[type].slot; }
const SDL_Rect& PlayerInventory::getEquipmentDim(ItemType type) { return m_equipmentSlots[type].dim; }
const SDL_Rect& PlayerInventory::getInventoryDim() const { return m_invDim; }
void PlayerInventory::setEquipmentPos(ItemType type, int x, int y) {
	m_equipmentSlots[type].dim.x = x;
	m_equipmentSlots[type].dim.y = y;
}

bool PlayerInventory::isHotbarType(ItemType type) {
	return type == ItemType::RESSOURCE_ITEM ||
		   type == ItemType::WEAPON_ITEM ||
		   type == ItemType::TOOL_ITEM ||
		   type == ItemType::FOOD_ITEM;
}

std::string PlayerInventory::getKeyFromType(ItemType type) {
	switch (type) {
		case ItemType::RESSOURCE_ITEM: return "resource";
		case ItemType::WEAPON_ITEM: return "weapon";
		case ItemType::TOOL_ITEM: return "tool";
		case ItemType::FOOD_ITEM: return "food";
		case ItemType::HELMET_ITEM: return "helmet";
		case ItemType::CHESTPLATE_ITEM: return "chestplate";
		case ItemType::LEGGINGS_ITEM: return "leggings";
		case ItemType::BOOTS_ITEM: return "boots";
		default: return "undef";
	}
}

ItemType PlayerInventory::getTypeFromKey(const std::string& key) {
	if (key == "resource") { return ItemType::RESSOURCE_ITEM; }
	if (key == "weapon") { return ItemType::WEAPON_ITEM; }
	if (key == "tool") { return ItemType::TOOL_ITEM; }
	if (key == "food") { return ItemType::FOOD_ITEM; }
	if (key == "helmet") { return ItemType::HELMET_ITEM; }
	if (key == "chestplate") { return ItemType::CHESTPLATE_ITEM; }
	if (key == "leggings") { return ItemType::LEGGINGS_ITEM; }
	if (key == "boots") { return ItemType::BOOTS_ITEM; }
	//return ItemType::RESSOURCE_ITEM;
}

void PlayerInventory::renderSlot(std::shared_ptr<InventorySlot> slot, const SDL_Rect& dim) {
	SDL_RenderDrawRect(app->renderer, &dim);

	if (!slot->isEmpty()) {

		TextureManager::render(slot->getItem()->getTextureSheet(), slot->getItem()->getSrc(), dim);

		if (slot->getAmount() > 1) {
			TextManager::renderStringCentered(
				std::to_string(slot->getAmount()).c_str(),
				Fonts::OPEN_SANS_REGULAR,
				FontSize::DEFAULT_SMALL,
				{ 0, 0, 0, 255 },
				dim.x + 10,
				dim.y
			);
		}

	}

}
