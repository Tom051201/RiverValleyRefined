#pragma once
#include <map>
#include <memory>
#include <string>
#include "Item.h"
#include "../screens/Game.h"

static const int WOOD = 0;
static const int ROCK = 1;
static const int AXE = 2;
static const int PICKAXE = 3;
static const int SWORD = 4;
static const int IRON_HELMET = 5;
static const int IRON_CHESTPLATE = 6;
static const int IRON_LEGGINGS = 7;
static const int IRON_BOOTS = 8;
static const int MEAT = 9;
static const int MILK = 10;
static const int FISHING_ROD = 11;

enum class SheetType {
	RESSOURCE,
	EQUIPMENT,
	CONSUMABLE
};

class ItemDatabase {
public:

	ItemDatabase(Game* game);
	~ItemDatabase();

	// Item templates
	std::shared_ptr<Item> createItem(int id);
	const std::map<int, std::shared_ptr<Item>>& getItemTemplatesList() const;

	// Texture sheets
	SDL_Texture* getTextureSheet(SheetType type);
	
private:

	// Item templates
	void addTemplate(int id, std::shared_ptr<Item> item);
	std::map<int, std::shared_ptr<Item>> m_itemTemplates;

	// Texture sheets
	void addTextureSheet(SheetType type, const std::string& path);
	std::map<SheetType, SDL_Texture*> m_textureSheets;

};
