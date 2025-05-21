#include "ItemDatabase.h"
#include <iostream>
#include "../Utils.h"

// Item templates

ItemDatabase::ItemDatabase(Game* game) {
	addTextureSheet(SheetType::RESSOURCE, "res/items/resourceSheet.png");
	addTextureSheet(SheetType::EQUIPMENT, "res/items/equipmentSheet.png");
	addTextureSheet(SheetType::CONSUMABLE, "res/items/consumableSheet.png");

	SDL_Rect res;
	addTemplate(WOOD, std::make_shared<Item>(WOOD, "Wood", getTextureSheet(SheetType::RESSOURCE), res = { 0, 0, 16, 16 }, ItemType::RESSOURCE_ITEM));
	addTemplate(ROCK, std::make_shared<Item>(ROCK, "Stone", getTextureSheet(SheetType::RESSOURCE), res = { 16, 0, 16, 16 }, ItemType::RESSOURCE_ITEM));
	addTemplate(AXE, std::make_shared<ToolItem>(AXE, "Axe", getTextureSheet(SheetType::EQUIPMENT), res = { 0, 16, 16, 16 },
		[this, game]() {
			game->player->destroyTree(0, 0);
		}
	));
	addTemplate(PICKAXE, std::make_shared<ToolItem>(PICKAXE, "Pickaxe", getTextureSheet(SheetType::EQUIPMENT), res = { 16, 16, 16, 16 },
		[this, game]() {
			game->player->destroyRock(0, 0);
		}
	));
	addTemplate(SWORD, std::make_shared<WeaponItem>(SWORD, "Sword", getTextureSheet(SheetType::EQUIPMENT), res = { 64, 0, 16, 16 },
		[this, game]() {
			//game->player->attack();
		}, 10));
	addTemplate(IRON_HELMET, std::make_shared<HelmetItem>(IRON_HELMET, "Iron Helmet", getTextureSheet(SheetType::EQUIPMENT), res = { 0, 0, 16, 16 }, 5));
	addTemplate(IRON_CHESTPLATE, std::make_shared<ChestplateItem>(IRON_CHESTPLATE, "Iron Chestplate", getTextureSheet(SheetType::EQUIPMENT), res = { 16, 0, 16, 16 }, 5));
	addTemplate(IRON_LEGGINGS, std::make_shared<LeggingsItem>(IRON_LEGGINGS, "Iron Leggings", getTextureSheet(SheetType::EQUIPMENT), res = { 32, 0, 16, 16 }, 5));
	addTemplate(IRON_BOOTS, std::make_shared<BootsItem>(IRON_BOOTS, "Iron Boots", getTextureSheet(SheetType::EQUIPMENT), res = { 48, 0, 16, 16 }, 5));
	addTemplate(MEAT, std::make_shared<FoodItem>(MEAT, "Meat", getTextureSheet(SheetType::CONSUMABLE), res = { 0, 0, 16, 16 },
		[this, game]() {
			game->player->addHealth(10);
			game->player->inv->getEquipmentSlot(ItemType::FOOD_ITEM)->reduceAmount(1);
		}
	));
	addTemplate(MILK, std::make_shared<FoodItem>(MILK, "Milk", getTextureSheet(SheetType::CONSUMABLE), res = { 16, 0, 16, 16 },
		[this, game]() {
			game->player->addEndurance(10);
			game->player->inv->getEquipmentSlot(ItemType::FOOD_ITEM)->reduceAmount(1);
		}
	));
	addTemplate(FISHING_ROD, std::make_shared<ToolItem>(FISHING_ROD, "Fishing Rod", getTextureSheet(SheetType::EQUIPMENT), res = { 64, 0, 16, 16 },
		[this, game]() {
			// TODO add fishing
		}
	));
}

ItemDatabase::~ItemDatabase() {
	for (auto& e : m_textureSheets) {
		SDL_DestroyTexture(e.second);
	}
	//std::cout << "Texture sheets cleared!" << std::endl;
//	std::cout << "ItemDatabase deleted" << std::endl;
}

void ItemDatabase::addTemplate(int id, std::shared_ptr<Item> item) {
	m_itemTemplates[id] = item;
	//std::cout << "New Item added! (" << id << ")" << std::endl;
}

std::shared_ptr<Item> ItemDatabase::createItem(int id) {
	auto it = m_itemTemplates.find(id);
	if (it != m_itemTemplates.end()) {
		//std::cout << "Item created: " << id << " !" << std::endl;
		return it->second->clone();
	}
	throw std::runtime_error("Item not found: " + id);
}

const std::map<int, std::shared_ptr<Item>>& ItemDatabase::getItemTemplatesList() const {
	return m_itemTemplates;
}

// Texture Sheets

void ItemDatabase::addTextureSheet(SheetType type, const std::string& path) {
	m_textureSheets[type] = TextureManager::loadTexture(path.c_str());
}

SDL_Texture* ItemDatabase::getTextureSheet(SheetType type) {
	return m_textureSheets[type];
}
