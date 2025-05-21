#include "InventorySlot.h"

/*---------- InventorySlot -------------------------------------------------------------------------*/

InventorySlot::InventorySlot() {
	m_item = nullptr;
}

void InventorySlot::setItem(std::shared_ptr<Item> item, int amount) {
	m_item = item;
	m_amount = amount;
}

void InventorySlot::increaseAmount(int amount) {
	if (m_item != nullptr) {
		m_amount += amount;
	}	
}

void InventorySlot::reduceAmount(int amount) {
	if (m_item != nullptr) {
		if (m_amount - amount > 0) {
			m_amount -= amount;
		}
		else {
			this->clear();
		}
	}
}

std::shared_ptr<Item> InventorySlot::getItem() const {
	return m_item;
}

int InventorySlot::getAmount() const {
	return m_amount;
}

bool InventorySlot::isEmpty() const {
	return m_item == nullptr;
}

void InventorySlot::clear() {
	m_item = nullptr;
	m_amount = 0;
}

/*------- TypeBoundSlot ---------------------------------------------------------------------------------------------------------*/

TypeBoundSlot::TypeBoundSlot(ItemType type) {
	m_type = type;
}

void TypeBoundSlot::setItem(std::shared_ptr<Item> item, int amount) {
	if (item->getType() == m_type) {
		m_item = item;
		m_amount = amount;
	}
}

const ItemType TypeBoundSlot::getType() const { return m_type; }
