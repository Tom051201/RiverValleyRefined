#pragma once
#include <memory>
#include "../items/Item.h"

class InventorySlot {
public:

	InventorySlot();

	virtual void setItem(std::shared_ptr<Item> item, int amount);
	void increaseAmount(int amount);
	void reduceAmount(int amount);

	std::shared_ptr<Item> getItem() const;
	int getAmount() const;
	bool isEmpty() const;

	void clear();

protected:

	std::shared_ptr<Item> m_item;
	int m_amount;

};

class TypeBoundSlot : public InventorySlot {
public:

	TypeBoundSlot(ItemType type);

	void setItem(std::shared_ptr<Item> item, int amount) override;
	const ItemType getType() const;

private:

	ItemType m_type;

};

