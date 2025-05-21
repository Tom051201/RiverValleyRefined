#pragma once
#include <memory>
#include <string>
#include "SDL.h"
#include <functional>

enum class ItemType {
	RESSOURCE_ITEM ,
	TOOL_ITEM,
	WEAPON_ITEM,
	FOOD_ITEM,
	HELMET_ITEM,
	CHESTPLATE_ITEM,
	LEGGINGS_ITEM,
	BOOTS_ITEM
};

class Item {
public:

	Item(int id, const std::string& name, SDL_Texture* sheet, SDL_Rect& src, ItemType type);
	virtual ~Item();

	virtual std::shared_ptr<Item> clone() const;
	virtual void use() const;

	int getID() const;
	SDL_Texture* getTextureSheet() const;
	const SDL_Rect& getSrc() const;
	ItemType getType() const;
	const std::string& getName() const;

protected:

	int m_id;
	ItemType m_type;
	std::string m_name;
	SDL_Texture* m_textureSheet;
	SDL_Rect m_src;

};

class ToolItem : public Item {
public:

	ToolItem(int id, const std::string& name, SDL_Texture* sheet, SDL_Rect& src, std::function<void()> onUse);

	std::shared_ptr<Item> clone() const override;
	void use() const override;

private:

	std::function<void()> onUse;

};

class WeaponItem : public Item {
public:

	WeaponItem(int id, const std::string& name, SDL_Texture* sheet, SDL_Rect& src, std::function<void()> onUse, int damage);

	std::shared_ptr<Item> clone() const override;
	void use() const override;

private:

	std::function<void()> onUse;
	int damage;

};

class HelmetItem : public Item {
public:

	HelmetItem(int id, const std::string& name, SDL_Texture* sheet, SDL_Rect& src, int armor);

	std::shared_ptr<Item> clone() const override;
	int getArmor();

private:

	int armor;

};

class ChestplateItem : public Item {
public:

	ChestplateItem(int id, const std::string& name, SDL_Texture* sheet, SDL_Rect& src, int armor);

	std::shared_ptr<Item> clone() const override;
	int getArmor();

private:

	int armor;

};

class LeggingsItem : public Item {
public:

	LeggingsItem(int id, const std::string& name, SDL_Texture* sheet, SDL_Rect& src, int armor);

	std::shared_ptr<Item> clone() const override;
	int getArmor();

private:

	int armor;

};

class BootsItem : public Item {
public:

	BootsItem(int id, const std::string& name, SDL_Texture* sheet, SDL_Rect& src, int armor);

	std::shared_ptr<Item> clone() const override;
	int getArmor();

private:

	int armor;

};

class FoodItem : public Item {
public:

	FoodItem(int id, const std::string& name, SDL_Texture* sheet, SDL_Rect& src, std::function<void()> onUse);

	std::shared_ptr<Item> clone() const override;
	void use() const override;

private:

	std::function<void()> onUse;

};
