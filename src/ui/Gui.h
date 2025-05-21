#pragma once
#include "SDL.h"
#include <functional>
#include <string>
#include "../Application.h"

class GuiHandler;
class Player;
class Realm;
class Game;
enum class UiSheetType;

class GuiElement {
public:

	virtual ~GuiElement() = default;

	virtual void update() {}
	virtual void render() {}
	virtual bool handleEvents() { return false; }
	virtual void windowResized() {}

	virtual void setPosX(int x);
	virtual void setPosY(int y);
	virtual void setWidth(int w);
	virtual void setHeight(int h);
	virtual const SDL_Rect& getDim() const;

	void setVisible(bool visible);
	bool isVisible() const;
	void setActive(bool active);
	bool isActive() const;
	void enable();
	void disable();

	void setFunctionForPosX(std::function<int()> calcPosX);
	void setFunctionForPosY(std::function<int()> calcPosY);

protected:

	SDL_Rect m_dim{};
	bool m_visible = true;
	bool m_active = true;
	std::string m_tag = " ";

	std::function<int()> m_calcPosX;
	std::function<int()> m_calcPosY;

};

class Bar : public GuiElement {
public:

	Bar(int x, int y, int w, int h, int max);
	~Bar() override;

	void render() override;
	void windowResized() override;

	void setPosX(int x) override;
	void setPosY(int y) override;

	void add(int a);
	void reduce(int a);
	void fillEntirely();
	void emptyEntirely();
	void setBaseColor(SDL_Color c);
	void setIndicatorColor(SDL_Color c);
	void setFill(int a);

private:

	SDL_Rect fill;
	int max, filledTo;
	SDL_Color baseColor, indicatorColor;
	int offsetIndicator;

};



class Button : public GuiElement {
public:

	Button();
	Button(Application* app, int x, int y, int w, int h, std::function<void()> onClick);
	~Button() override;

	void render() override;
	bool handleEvents() override;
	void windowResized() override;

	bool checkClick();

	void setBaseColor(const SDL_Color& c);
	void setTextColor(const SDL_Color& c);
	void setText(const std::string& text);
	void setTextSize(int size);

private:

	Application* app;
	SDL_Color baseColor, textColor;
	int fontSize;
	std::string text;
	std::function<void()> onClick;

};



class Toggle : public GuiElement {
public:

	Toggle(Application* app, int x, int y, int w, int h, std::function<void()> onTurnOn, std::function<void()> onTurnOff);
	~Toggle() override;

	void render() override;
	bool handleEvents() override;
	void windowResized() override;

	bool checkClick();
	void turnOn();
	void silentTurnOn();
	void turnOff();
	void silentTurnOff();
	bool isOn();

private:

	Application* app;

	bool on;
	
	std::function<void()> onTurnOn;
	std::function<void()> onTurnOff;

};



class Selector : public GuiElement {
public:

	Selector(Application* app, int x, int y, int w, int h);
	~Selector() override;

	void render() override;
	bool handleEvents() override;
	void windowResized() override;

	bool checkClick();

	void setTexts(std::vector<std::string> texts);
	std::string& getCurrentText();

private:

	Application* app;

	SDL_Rect textField;

	Button goLeftButton;
	Button goRightButton;

	std::vector<std::string> displayTexts;
	std::string* displayedString;
	int displayedStringIndex;

	void goNext();
	void goBack();

};



class InfoIcon : public GuiElement {
public:

	InfoIcon(GuiHandler& handler, UiSheetType type, int row, int col, int x, int y, int width, int height);
	~InfoIcon() override;

	void render() override;
	void windowResized() override;

	void setSrcX(int x);
	void setSrcY(int y);
	void setDescription(const std::string& description);

private:

	SDL_Texture* m_sheet;
	SDL_Rect m_src;
	std::string m_description;

};



class MiniMapUI : public GuiElement {
public:

	MiniMapUI(Game* game, int x, int y, int w, int h);
	~MiniMapUI() override;

	void render() override;
	void windowResized() override;

private:

	std::shared_ptr<Realm> m_realm;
	std::shared_ptr<Player> m_player;

	int m_scalingX;
	int m_scalingY;

};
