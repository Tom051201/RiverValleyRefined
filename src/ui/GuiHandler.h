#pragma once
#include <vector>
#include <memory>
#include "Gui.h"
#include <map>

enum class UiSheetType {
	AmbientSheet,
	InventorySheet
};

class GuiHandler {
public:

	GuiHandler();
	~GuiHandler();

	void update();
	void render();
	void handleEvents();
	void windowResized();

	void addElement(std::shared_ptr<GuiElement> element);

	SDL_Texture* getUiSheet(UiSheetType type);

private:

	std::vector<std::shared_ptr<GuiElement>> m_elements;
	std::map<UiSheetType, SDL_Texture*> m_uiSheets;

};
