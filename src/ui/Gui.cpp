#include "Gui.h"
#include "../Application.h"
#include "../Utils.h"
#include <iostream>
#include "GuiHandler.h"
#include "../screens/Game.h"


/*-------- GuiElement -----------------------------------------------------------------------------------------------------------------------------*/


void GuiElement::setVisible(bool visible) { m_visible = visible; }
bool GuiElement::isVisible() const { return m_visible; }
void GuiElement::setActive(bool active) { m_active = active; }
bool GuiElement::isActive() const { return m_active; }
void GuiElement::enable() {
	m_visible = true;
	m_active = true;
}
void GuiElement::disable() {
	m_visible = false;
	m_active = false;
}
void GuiElement::setPosX(int x) { m_dim.x = x; }
void GuiElement::setPosY(int y) { m_dim.y = y; }
void GuiElement::setWidth(int w) { m_dim.w = w; }
void GuiElement::setHeight(int h) { m_dim.h = h; }
const SDL_Rect& GuiElement::getDim() const { return m_dim; }
void GuiElement::setFunctionForPosX(std::function<int()> calcPosX) { m_calcPosX = calcPosX; }
void GuiElement::setFunctionForPosY(std::function<int()> calcPosY) { m_calcPosY = calcPosY; }


/*-------- Bar -----------------------------------------------------------------------------------------------------------------------------*/


Bar::Bar(int x, int y, int w, int h, int max) {
	m_dim = { x, y, w, h };
	m_visible = true;
	m_active = true;
	this->max = max;
	this->filledTo = max;
	this->offsetIndicator = h * 0.15;
	this->fill = { x, y + offsetIndicator, w, h - offsetIndicator * 2 };
	this->baseColor = { 60, 50, 55, 255 };
	this->indicatorColor = { 185, 175, 175, 255 };
}

Bar::~Bar() {
//	std::cout << "Bar deleted" << std::endl;
}

void Bar::render() {
	if (m_visible) {
		// render base rect and color
		SDL_SetRenderDrawColor(Application::renderer, baseColor.r, baseColor.g, baseColor.b, baseColor.a);
		SDL_RenderFillRect(Application::renderer, &m_dim);

		// render indicator rect and color
		SDL_SetRenderDrawColor(Application::renderer, indicatorColor.r, indicatorColor.g, indicatorColor.b, indicatorColor.a);
		SDL_RenderFillRect(Application::renderer, &fill);

		// render text under bar
		std::string text = std::to_string(filledTo) + " / " + std::to_string(max);
		TextManager::renderStringCentered(text.c_str(), Fonts::DEFAULT, FontSize::DEFAULT, { 0, 0, 0, 0 }, m_dim.x + m_dim.w / 2, m_dim.y + m_dim.h);
	}
}

void Bar::windowResized() {
	m_dim.x = m_calcPosX();
	m_dim.y = m_calcPosY();

	fill = { m_dim.x, m_dim.y + offsetIndicator, fill.w, fill.h };
}

void Bar::setPosX(int x) {
	m_dim.x = x;
	fill.x = x;
}

void Bar::setPosY(int y) {
	m_dim.y = y;
	fill.y = y;
}

void Bar::add(int a) {
	this->filledTo += a;

	float multiplier = (float)filledTo / max;
	fill.w = (int)(m_dim.w * multiplier);
}

void Bar::reduce(int a) {
	this->filledTo -= a;

	float multiplier = (float)filledTo / max;
	fill.w = (int)(m_dim.w * multiplier);
}

void Bar::fillEntirely() {
	filledTo = max;
	fill.w = m_dim.w;
}

void Bar::emptyEntirely() {
	filledTo = 0;
	fill.w = 0;
}

void Bar::setBaseColor(SDL_Color c) {
	baseColor = c;
}

void Bar::setIndicatorColor(SDL_Color c) {
	indicatorColor = c;
}

void Bar::setFill(int a) {
	filledTo = a;
	
	float multiplier = (float)filledTo / max;
	fill.w = (int)(m_dim.w * multiplier);
}


/*-------- Button -----------------------------------------------------------------------------------------------------------------------------*/

Button::Button() {
	this->app = nullptr;
	m_dim = { 0, 0, 0, 0 };
	this->onClick = nullptr;
	this->text = "";
	this->baseColor = { 255, 255, 255, 255 };
	this->textColor = { 0, 0, 0, 255 };
	this->fontSize = FontSize::DEFAULT;
}

Button::Button(Application* app, int x, int y, int w, int h, std::function<void()> onClick) {
	this->app = app;
	m_dim = { x, y, w, h };
	this->onClick = onClick;
	this->text = ""; 
	this->baseColor = { 255, 255, 255, 255 };
	this->textColor = { 0, 0, 0, 255 };
	this->fontSize = FontSize::DEFAULT;
}

Button::~Button() {}

void Button::render() {
	if (m_visible) {
		SDL_SetRenderDrawColor(Application::renderer, baseColor.r, baseColor.g, baseColor.b, baseColor.a);
		SDL_RenderFillRect(Application::renderer, &m_dim);

		if (text != "") TextManager::renderStringInRect(text.c_str(), Fonts::DEFAULT, fontSize, textColor, m_dim);
	}
}

bool Button::handleEvents() {
	if (app->getAppEvent().type == SDL_MOUSEBUTTONDOWN && app->getAppEvent().button.button == SDL_BUTTON_LEFT) {
		if (checkClick()) return true;
	}
	return false;
}

void Button::windowResized() {
	m_dim.x = m_calcPosX();
	m_dim.y = m_calcPosY();
}

bool Button::checkClick() {
	if (m_active) {
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		if (mouseX >= m_dim.x && mouseX <= m_dim.x + m_dim.w &&
			mouseY >= m_dim.y && mouseY <= m_dim.y + m_dim.h) {
			app->globalSounds->playEffect(GlobalSoundsList::UI_CLICK, Channels::GUI_INTERACTION);
			onClick();
			return true;
		}
	}
	return false;
}

void Button::setBaseColor(const SDL_Color& c) { this->baseColor = c; }

void Button::setTextColor(const SDL_Color& c) { this->textColor = textColor; }

void Button::setText(const std::string& text) { this->text = text; }

void Button::setTextSize(int size) { this->fontSize = size; }


/*---------- Toggle ------------------------------------------------------------------------------------------------------------------------------*/


Toggle::Toggle(Application* app, int x, int y, int w, int h, std::function<void()> onTurnOn, std::function<void()> onTurnOff) {
	this->app = app;
	m_dim = { x, y, w, h };
	this->onTurnOn = onTurnOn;
	this->onTurnOff = onTurnOff;
	this->on = true;
}

Toggle::~Toggle() {}

void Toggle::render() {
	if (m_visible) {
		SDL_Rect selRec = { 0, m_dim.y, m_dim.w / 2, m_dim.h };

		if (on) {
			SDL_SetRenderDrawColor(Application::renderer, 0, 165, 74, 255);
			SDL_RenderFillRect(Application::renderer, &m_dim);

			selRec.x = m_dim.x + m_dim.w / 2;
			SDL_SetRenderDrawColor(Application::renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(Application::renderer, &selRec);
		}
		else {
			SDL_SetRenderDrawColor(Application::renderer, 64, 64, 64, 255);
			SDL_RenderFillRect(Application::renderer, &m_dim);

			selRec.x = m_dim.x;
			SDL_SetRenderDrawColor(Application::renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(Application::renderer, &selRec);
		}
	}
}

bool Toggle::handleEvents() {
	if (app->getAppEvent().type == SDL_MOUSEBUTTONDOWN && app->getAppEvent().button.button == SDL_BUTTON_LEFT) {
		if (checkClick()) return true;
	}
	return false;
}

void Toggle::windowResized() {
	m_dim.x = m_calcPosX();
	m_dim.y = m_calcPosY();
}

bool Toggle::checkClick() {
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	if (mouseX >= m_dim.x && mouseX <= m_dim.x + m_dim.w &&
		mouseY >= m_dim.y && mouseY <= m_dim.y + m_dim.h) {
		
		if (on) {
			app->globalSounds->playEffect(GlobalSoundsList::UI_CLICK, Channels::GUI_INTERACTION);
			turnOff();
		}
		else {
			app->globalSounds->playEffect(GlobalSoundsList::UI_CLICK, Channels::GUI_INTERACTION);
			turnOn();
		}

		return true;
	}

	return false;
}

void Toggle::turnOn() { 
	this->on = true;
	onTurnOn();
}

void Toggle::silentTurnOn() { this->on = true; }

void Toggle::turnOff() {
	this->on = false;
	onTurnOff();
}

void Toggle::silentTurnOff() { this->on = false; }

bool Toggle::isOn() { return this->on; }


/*---------- Selector ------------------------------------------------------------------------------------------------------------------------------*/


Selector::Selector(Application* app, int x, int y, int w, int h) {
	this->app = app;
	m_dim = { x, y, w, h };

	goLeftButton = Button(app, m_dim.x, m_dim.y + (m_dim.h / 3), m_dim.w / 2, m_dim.h - (m_dim.h / 3 ), [this]() { goBack(); } );
	goLeftButton.setBaseColor({ 220, 220, 220, 255 });
	goLeftButton.setText("<");
	goLeftButton.setTextSize(30);
	goLeftButton.setFunctionForPosX([this]() { return m_dim.x; });
	goLeftButton.setFunctionForPosY([this]() { return m_dim.y + (m_dim.h / 3); });

	goRightButton = Button(app, m_dim.x + m_dim.w / 2, m_dim.y + (m_dim.h / 3), m_dim.w / 2, m_dim.h - (m_dim.h / 3), [this]() { goNext(); } );
	goRightButton.setBaseColor({ 220, 220, 220, 255 });
	goRightButton.setText(">");
	goRightButton.setTextSize(30);
	goRightButton.setFunctionForPosX([this]() { return m_dim.x + m_dim.w / 2; });
	goRightButton.setFunctionForPosY([this]() { return  m_dim.y + (m_dim.h / 3); });

	textField = { m_dim.x, m_dim.y, m_dim.w, m_dim.h - ((m_dim.h / 3) * 2) };
}

Selector::~Selector() {}

void Selector::render() {
	if (m_visible) {
		SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(app->renderer, &m_dim);

		goLeftButton.render();
		goRightButton.render();

		if (displayedString != nullptr) TextManager::renderStringInRect(
			displayedString->c_str(),
			Fonts::DEFAULT,
			FontSize::DEFAULT_MID,
			{ 0, 0, 0, 255 },
			textField
		);
	}
}

bool Selector::handleEvents() {
	if (app->getAppEvent().type == SDL_MOUSEBUTTONDOWN && app->getAppEvent().button.button == SDL_BUTTON_LEFT) {
		if (checkClick()) return true;
	}
	return false;
}

void Selector::windowResized() {
	m_dim.x = m_calcPosX();
	m_dim.y = m_calcPosY();

	textField = { m_dim.x, m_dim.y, m_dim.w, m_dim.h - ((m_dim.h / 3) * 2) };

	goLeftButton.windowResized();
	goRightButton.windowResized();
}

bool Selector::checkClick() {
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	if (mouseX >= m_dim.x && mouseX <= m_dim.x + m_dim.w &&
		mouseY >= m_dim.y && mouseY <= m_dim.y + m_dim.h) {

		if (goLeftButton.checkClick()) return true;
		if (goRightButton.checkClick()) return true;

		return false;
	}

	return false;
}

void Selector::setTexts(std::vector<std::string> texts) {
	this->displayTexts = texts;

	displayedString = &displayTexts[0];
	displayedStringIndex = 0;
}

void Selector::goNext() {
	if ((size_t)displayedStringIndex + 1 < displayTexts.size()) {
		displayedStringIndex++;
	}
	else {
		displayedStringIndex = 0;
	}
	displayedString = &displayTexts[displayedStringIndex];
}

void Selector::goBack() {
	if (displayedStringIndex - 1 >= 0) {
		displayedStringIndex--;
	}
	else {
		displayedStringIndex = displayTexts.size() - 1;
	}
	displayedString = &displayTexts[displayedStringIndex];
}

std::string& Selector::getCurrentText() { return displayTexts[displayedStringIndex]; }


/*-------- InfoIcon -----------------------------------------------------------------------------------------------------------------------------*/


InfoIcon::InfoIcon(GuiHandler& handler, UiSheetType type, int row, int col, int x, int y, int width, int height) {
	m_sheet = handler.getUiSheet(type);
	m_dim = { x, y, width, height };
	m_src = { col, row, 16, 16 };
	m_description = " ";
}

InfoIcon::~InfoIcon() {}

void InfoIcon::render() {
	if (m_visible) {
		// renders timeStateIcon and description
		TextureManager::render(m_sheet, m_src, m_dim);

		TextManager::renderStringCentered(
			m_description.c_str(),
			Fonts::DEFAULT,
			FontSize::DEFAULT_SMALL,
			{ 0, 0, 0, 255 },
			m_dim.x + (m_dim.w / 2),
			m_dim.y + (m_dim.h + 5)
		);
	}
}

void InfoIcon::windowResized() {
	m_dim.x = m_calcPosX();
	m_dim.y = m_calcPosY();
}

void InfoIcon::setSrcX(int x) { m_src.x = x; }

void InfoIcon::setSrcY(int y) { m_src.y = y; }

void InfoIcon::setDescription(const std::string& description) { m_description = description; }


/*-------- MiniMap -----------------------------------------------------------------------------------------------------------------------------*/


MiniMapUI::MiniMapUI(Game* game, int x, int y, int w, int h) {
	m_realm = game->world;
	m_player = game->player;
	m_dim = { x, y, w, h };
	m_scalingX = m_dim.w / m_realm->getMaxCols();
	m_scalingY = m_dim.h / m_realm->getMaxRows();
}

MiniMapUI::~MiniMapUI() {
//	std::cout << "Minimap deleted" << std::endl;
}

void MiniMapUI::render() {
	if (m_visible) {
		// render bg color and border
		SDL_SetRenderDrawColor(Application::renderer, 0, 0, 0, 255);
		SDL_Rect bgR = { m_dim.x - 4, m_dim.y - 4, m_realm->getMaxCols() * m_scalingX + 8, m_realm->getMaxRows() * m_scalingY + 8 };
		SDL_RenderFillRect(Application::renderer, &bgR);

		// render colors of tiles
		for (int i = 0; i < m_realm->getMaxCols(); i++) {
			for (int j = 0; j < m_realm->getMaxRows(); j++) {
				SDL_Color c = m_realm->getTileAt(j, i)->getColor();

				SDL_SetRenderDrawColor(Application::renderer, c.r, c.g, c.b, c.a);
				SDL_Rect r = { m_dim.x + i * m_scalingX, m_dim.y + j * m_scalingY, m_scalingX, m_scalingY };
				SDL_RenderFillRect(Application::renderer, &r);
			}
		}

		// render player
		SDL_SetRenderDrawColor(Application::renderer, 0, 0, 255, 255);
		SDL_Rect playerR = {
			m_dim.x + ((m_player->getWorldX() + 32) / 64 * m_scalingX) + m_scalingX * 0.2,
			m_dim.y + ((m_player->getWorldY() + 32) / 64 * m_scalingY) + m_scalingX * 0.2,
			m_scalingX - (m_scalingX * 0.2),
			m_scalingY - (m_scalingY * 0.2)
		};
		SDL_RenderFillRect(Application::renderer, &playerR);

		// render player position
		std::string playerPos = "X: " + std::to_string((m_player->getWorldX() + 32) / 64) + " Y:  " + std::to_string((m_player->getWorldY() + 32) / 64);
		TextManager::renderStringCentered(playerPos.c_str(), Fonts::DEFAULT, FontSize::DEFAULT, { 0, 0, 0, 255 }, m_dim.x + m_dim.w / 2, m_dim.y + m_dim.h);
	}
}

void MiniMapUI::windowResized() {
	m_dim.x = m_calcPosX();
	m_dim.y = m_calcPosY();
}

