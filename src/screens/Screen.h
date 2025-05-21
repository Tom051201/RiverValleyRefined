#pragma once

class Screen {
public:

	virtual ~Screen() = default;

	virtual void render() = 0;
	virtual void update() = 0;
	virtual void handleEvents() = 0;
	virtual void windowResized() = 0;

};
