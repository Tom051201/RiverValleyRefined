#pragma once
#include <string>
#include <map>
#include <vector>
#include "SDL.h"
#include <SDL_ttf.h>

class Tools {
public:
	static int generateRandomInt(int from, int to);
	static float generateRandomFloat(float from, float to);
	static bool strStartsWith(const char* str, const char* prefix);
	static bool strStartsWith(const std::string& str, const std::string& prefix);
	static std::pair<std::string, std::string> splitStr(const std::string& str, char s);
	static std::string boolToStr(bool in);
	static int stringToInt(const std::string& str);
};

class TextureManager {
public:
	static SDL_Texture* loadTexture(const char* filePath);
	static void render(SDL_Texture* texture, SDL_Rect src, SDL_Rect dst);
	static int getTextureWidth(SDL_Texture* tex);
	static int getTextureHeight(SDL_Texture* tex);
	static SDL_Color getPixelColor(SDL_Texture* tex, int x, int y);
};

class TextManager {
public:
	static void renderString(const char* txt, const char* fontFile, int fontSize, SDL_Color color, int x, int y);
	static void renderStringCentered(const char* txt, const char* fontFile, int fontSize, SDL_Color color, int aroundX, int y);
	static void renderStringInRect(const char* txt, const char* fontFile, int fontSize, SDL_Color color, SDL_Rect& dim);
	static int getTextWidth(TTF_Font* font, const std::string& text);
};

class Fonts {
public:
	static const char* DEFAULT;
	static const char* OPEN_SANS_REGULAR;
};

class FontSize {
public:
	static const int DEFAULT;
	static const int DEFAULT_MID;
	static const int DEFAULT_SMALL;
};

class FileHandler {
public:

	FileHandler();
	~FileHandler();

	std::vector<std::string> readFile(const char* path);
	void writeFile(const char* path, std::vector<const char*> input);
	void writeFile(const char* path, std::vector<std::string> input);
	void writeFile(const std::string& path, std::vector<std::string> input);
	static bool doesFileExist(const std::string& path);

};

enum class Facing {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Vector2 {
public:

	float x;
	float y;

	Vector2() = default;
	Vector2(float _x, float _y);

	Vector2 operator+(const Vector2& other) const;
	Vector2 operator-(const Vector2& other) const;
	Vector2 operator*(float scalar) const;
	Vector2 operator/(float scalar) const;
	Vector2& operator+=(const Vector2& other);
	Vector2& operator-=(const Vector2& other);
	Vector2& operator*=(float scalar);
	Vector2& operator/=(float scalar);
	bool operator==(const Vector2& other) const;
	bool operator!=(const Vector2& other) const;
	float magnitude() const;
	void normalize();
	void zero();
	void one();

};