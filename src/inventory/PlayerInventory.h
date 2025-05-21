#pragma once
#include <memory>
#include <vector>
#include "../items/Item.h"
#include "InventorySlot.h"
#include "../Application.h"
#include "../filehandling/gamedata/GameDataHandler.h"
#include "../screens/Game.h"
#include "InventoryPicker.h"
#include "HotbarSelector.h"

class InventoryPicker;
class HotbarSelector;

class SlotData {
public:
	std::shared_ptr<TypeBoundSlot> slot;
	SDL_Rect dim;
};

class PlayerInventory {
public:

	const static int SLOT_SIZE = 16 * 4;
	const static int ITEM_OFFSET = 4;
	const static int ITEM_SIZE = SLOT_SIZE - (ITEM_OFFSET * 2);

	PlayerInventory(Application* app, Game* game, int rows, int cols, int x, int y);
	~PlayerInventory();

	void update();
	void render();
	void handleEvents();
	void windowResized();
	void print();

	void addItem(std::shared_ptr<Item> item, int amount);
	void setSlot(std::shared_ptr<Item> item, int amount, int row, int col);

	void openInventory();
	void closeInventory();
	const bool isOpen() const;

	// util
	void loadData(const json& data);
	void saveData(json& data);
	int getItemSize();

	// inventory
	std::shared_ptr<InventorySlot> getInventorySlot(int row, int col);
	const SDL_Rect& getInventoryDim() const;
	int getInvWidth() const;
	int getInvHeight() const;
	int getInvX() const;
	int getInvY() const;
	int getInvSlotSize() const;

	//hotbar
	const SDL_Rect& getHotbarDim() const;
	int getHotbarWidth() const;
	int getHotbarHeight() const;
	int getHotbarX() const;
	int getHotbarY() const;

	// armor bar
	const SDL_Rect& getArmorBarDim() const;
	int getArmorBarWidth() const;
	int getArmorBarHeight() const;
	int getArmorBarX() const;
	int getArmorBarY() const;

	std::shared_ptr<TypeBoundSlot> getEquipmentSlot(ItemType type);
	const SDL_Rect& getEquipmentDim(ItemType type);
	void setEquipmentPos(ItemType type, int x, int y);

private:

	Application* app;
	Game* game;

	int m_invRows, m_invCols;
	bool m_open;
	std::unique_ptr<InventoryPicker> m_picker;
	std::unique_ptr<HotbarSelector> m_hotbarSelector;
	SDL_Rect m_invDim;
	SDL_Rect m_hotbarDim;
	SDL_Rect m_armorBarDim;

	std::vector<std::vector<std::shared_ptr<InventorySlot>>> m_inventorySlots;
	std::unordered_map<ItemType, SlotData> m_equipmentSlots;

	bool isHotbarType(ItemType type);
	std::string getKeyFromType(ItemType type);
	ItemType getTypeFromKey(const std::string& key);
	void renderSlot(std::shared_ptr<InventorySlot> slot, const SDL_Rect& dim);

};
