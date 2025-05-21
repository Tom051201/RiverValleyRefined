#include "Item.h"
#include <iostream>


/*--------- Item ----------------------------------------------------------------------------------------------------------------------------*/


Item::Item(int id, const std::string& name, SDL_Texture* sheet, SDL_Rect& src, ItemType type)
	: m_id(id), m_name(name), m_textureSheet(sheet), m_src(src), m_type(type) {

	//std::cout << "NEW " << this << std::endl;
}

Item::~Item() {
	//std::cout << "DEL " << this << std::endl;
}

std::shared_ptr<Item> Item::clone() const {
	auto cloned = std::make_shared<Item>(*this);
	//std::cout << "CLO " << cloned << std::endl;
	return cloned;
}

void Item::use() const {}

int Item::getID() const { return m_id; }

SDL_Texture* Item::getTextureSheet() const { return m_textureSheet; }

const SDL_Rect& Item::getSrc() const { return m_src; }

ItemType Item::getType() const { return m_type; }

const std::string& Item::getName() const { return m_name; }

/*--------- ToolItem ------------------------------------------------------------------------------------------------------------------------*/


ToolItem::ToolItem(int id, const std::string& name, SDL_Texture* sheet, SDL_Rect& src, std::function<void()> onUse)
	: Item(id, name, sheet, src, ItemType::TOOL_ITEM), onUse(onUse) {
}

std::shared_ptr<Item> ToolItem::clone() const {
	auto cloned = std::make_shared<ToolItem>(*this);
	//std::cout << "CLO " << cloned << std::endl;
	return cloned;
}

void ToolItem::use() const {
	onUse();
}


/*--------- WeaponItem ------------------------------------------------------------------------------------------------------------------------*/


WeaponItem::WeaponItem(int id, const std::string& name, SDL_Texture* sheet, SDL_Rect& src, std::function<void()> onUse, int damage)
	: Item(id, name, sheet, src, ItemType::WEAPON_ITEM), onUse(onUse), damage(damage) {
}

std::shared_ptr<Item> WeaponItem::clone() const {
	auto cloned = std::make_shared<WeaponItem>(*this);
	//std::cout << "CLO " << cloned << std::endl;
	return cloned;
}

void WeaponItem::use() const {
	onUse();
}


/*--------- HelmetItem ------------------------------------------------------------------------------------------------------------------------*/


HelmetItem::HelmetItem(int id, const std::string& name, SDL_Texture* sheet, SDL_Rect& src, int armor)
	: Item(id, name, sheet, src, ItemType::HELMET_ITEM), armor(armor) {

}

std::shared_ptr<Item> HelmetItem::clone() const {
	auto cloned = std::make_shared<HelmetItem>(*this);
	return cloned;
}

int HelmetItem::getArmor() { return armor; }


/*--------- ChestplateItem ------------------------------------------------------------------------------------------------------------------------*/


ChestplateItem::ChestplateItem(int id, const std::string& name, SDL_Texture* sheet, SDL_Rect& src, int armor)
	: Item(id, name, sheet, src, ItemType::CHESTPLATE_ITEM), armor(armor) {

}

std::shared_ptr<Item> ChestplateItem::clone() const {
	auto cloned = std::make_shared<ChestplateItem>(*this);
	return cloned;
}

int ChestplateItem::getArmor() { return armor; }


/*--------- LeggingsItem ------------------------------------------------------------------------------------------------------------------------*/


LeggingsItem::LeggingsItem(int id, const std::string& name, SDL_Texture* sheet, SDL_Rect& src, int armor)
	: Item(id, name, sheet, src, ItemType::LEGGINGS_ITEM), armor(armor) {

}

std::shared_ptr<Item> LeggingsItem::clone() const {
	auto cloned = std::make_shared<LeggingsItem>(*this);
	return cloned;
}

int LeggingsItem::getArmor() { return armor; }


/*--------- BootsItem ------------------------------------------------------------------------------------------------------------------------*/


BootsItem::BootsItem(int id, const std::string& name, SDL_Texture* sheet, SDL_Rect& src, int armor)
	: Item(id, name, sheet, src, ItemType::BOOTS_ITEM), armor(armor) {

}

std::shared_ptr<Item> BootsItem::clone() const {
	auto cloned = std::make_shared<BootsItem>(*this);
	return cloned;
}

int BootsItem::getArmor() { return armor; }


/*--------- ´FoodItem ------------------------------------------------------------------------------------------------------------------------*/


FoodItem::FoodItem(int id, const std::string& name, SDL_Texture* sheet, SDL_Rect& src, std::function<void()> onUse)
	: Item(id, name, sheet, src, ItemType::FOOD_ITEM), onUse(onUse) {

}

std::shared_ptr<Item> FoodItem::clone() const {
	auto cloned = std::make_shared<FoodItem>(*this);
	return cloned;
}

void FoodItem::use() const {
	onUse();
}
