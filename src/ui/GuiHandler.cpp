#include "GuiHandler.h"
#include <iostream>

GuiHandler::GuiHandler() {
	m_uiSheets[UiSheetType::AmbientSheet] = TextureManager::loadTexture("res/ui/ambientUiSheet.png");
	m_uiSheets[UiSheetType::InventorySheet] = TextureManager::loadTexture("res/ui/inventoryUiSheet.png");
}

GuiHandler::~GuiHandler() {
	for (auto& e : m_uiSheets) {
		SDL_DestroyTexture(e.second);
	}
//	std::cout << "GuiHandler deleted" << std::endl;
}

void GuiHandler::update() {
	for (auto& e : m_elements) {
		e->update();
	}
}

void GuiHandler::render() {
	for (auto& e : m_elements) {
		e->render();
	}
}

void GuiHandler::handleEvents() {
	for (auto& e : m_elements) {
		if (e->handleEvents()) return;
	}
}

void GuiHandler::windowResized() {
	for (auto& e : m_elements) {
		e->windowResized();
	}
}

void GuiHandler::addElement(std::shared_ptr<GuiElement> element) {
	m_elements.push_back(element);
}

SDL_Texture* GuiHandler::getUiSheet(UiSheetType type) { return m_uiSheets[type]; }
