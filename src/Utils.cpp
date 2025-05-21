#include "Utils.h"
#include <random>
#include <sstream>
#include <iostream>
#include <fstream>
#include "SDL_image.h"
#include "Application.h"
#include "screens/Game.h"

#include <vector>
#include <Windows.h>

/*-Tools------------------------------------------------------------------------------------------------------*/

int Tools::generateRandomInt(int from, int to) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(from, to);
	return distr(gen);
}

float Tools::generateRandomFloat(float from, float to) {
	std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distr(from, to);
	return distr(gen);
}

bool Tools::strStartsWith(const char* str, const char* prefix) {
	size_t prefixLength = std::strlen(prefix);
	return std::strncmp(str, prefix, prefixLength) == 0;
}

bool Tools::strStartsWith(const std::string& str, const std::string& prefix) {
	return str.find(prefix) == 0;
}

std::pair<std::string, std::string> Tools::splitStr(const std::string& str, char s) {
	std::pair<std::string, std::string> result;

	size_t sPos = str.find(s);

	if (sPos != std::string::npos) {
		result.first = str.substr(0, sPos);
		result.second = str.substr(sPos + 1);
	}
	else {
		result.first = str;
		result.second = "";
	}

	return result;
}

std::string Tools::boolToStr(bool in) {
	if (in) { return "true"; }
	else { return "false"; }
}

int Tools::stringToInt(const std::string& str) {
	try {
		int num = std::stoi(str);
		return num;
	}
	catch (const std::invalid_argument& e) {
		std::cerr << "Invalid argument: " << e.what() << std::endl;
		return -1;
	}
	catch (const std::out_of_range& e) {
		std::cerr << "Out of range: " << e.what() << std::endl;
		return -1;
	}
}

/*-TextureManager------------------------------------------------------------------------------------------------------*/

SDL_Texture* TextureManager::loadTexture(const char* filePath) {

	SDL_Surface* tmpSurface = IMG_Load(filePath);
	if (!tmpSurface) {
		std::cerr << "Unable to load image " << filePath << "! IMG_Error: " << IMG_GetError() << std::endl;
		return nullptr;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(Application::renderer, tmpSurface);
	if (!texture) {
		std::cerr << "Unable to create texture from surface! SDL_Error: " << SDL_GetError() << std::endl;
	}

	SDL_FreeSurface(tmpSurface);

	return texture;
}

void TextureManager::render(SDL_Texture* texture, SDL_Rect src, SDL_Rect dst) {

	SDL_RenderCopy(Application::renderer, texture, &src, &dst);

}

int TextureManager::getTextureWidth(SDL_Texture* tex) {
	int width;
	if (SDL_QueryTexture(tex, nullptr, nullptr, &width, nullptr) != 0) {
		std::cerr << "Failed to query texture width: " << SDL_GetError() << std::endl;
		return -1;
	}
	return width;
}

int TextureManager::getTextureHeight(SDL_Texture* tex) {
	int height;
	if (SDL_QueryTexture(tex, nullptr, nullptr, nullptr, &height) != 0) {
		std::cerr << "Failed to query texture width: " << SDL_GetError() << std::endl;
		return -1;
	}
	return height;
}

SDL_Color TextureManager::getPixelColor(SDL_Texture* tex, int x, int y) {
	int texWidth, texHeight;
	SDL_QueryTexture(tex, nullptr, nullptr, &texWidth, &texHeight);

	if (x < 0 || x >= texWidth || y < 0 || y >= texHeight) {
		std::cerr << "Coordinates out of bounds!" << std::endl;
		return { 0, 0, 0, 0 };
	}

	SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, texWidth, texHeight, 32, SDL_PIXELFORMAT_RGBA32);
	if (!surface) {
		std::cerr << "Failed to create surface: " << SDL_GetError() << std::endl;
		return { 0, 0, 0, 0 };
	}

	if (SDL_SetRenderTarget(Application::renderer, tex) != 0) {
		std::cerr << "Failed to set render target: " << SDL_GetError() << std::endl;
		SDL_FreeSurface(surface);
		return { 0, 0, 0, 0 };
	}

	if (SDL_RenderReadPixels(Application::renderer, nullptr, surface->format->format, surface->pixels, surface->pitch) != 0) {
		std::cerr << "Failed to read pixels: " << SDL_GetError() << std::endl;
		SDL_FreeSurface(surface);
		return { 0, 0, 0, 0 };
	}

	Uint32* pixels = (Uint32*)surface->pixels;
	Uint32 pixel = pixels[(y * texWidth) + x];

	Uint8 r, g, b, a;
	SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);

	SDL_FreeSurface(surface);

	return { r, g, b, a };
}

/*-TextManager------------------------------------------------------------------------------------------------------*/

void TextManager::renderString(const char* txt, const char* fontFile, int fontSize, SDL_Color color, int x, int y) {

	// loading font
	TTF_Font* font = TTF_OpenFont(fontFile, fontSize);
	if (!font) { std::cerr << "Failed to load font: " << TTF_GetError() << std::endl; }

	// create surface
	SDL_Surface* surf = TTF_RenderText_Blended(font, txt, color);
	if (!surf) {
		TTF_CloseFont(font);
		std::cerr << "Failed to render Text to surface: " << SDL_GetError() << std::endl;
	}

	// create texture from surface
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Application::renderer, surf);
	if (!texture) { std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl; }

	// clean up
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);

	// rendering
	int tex_w = 0;
	int tex_h = 0;
	SDL_QueryTexture(texture, NULL, NULL, &tex_w, &tex_h);
	SDL_Rect dstRect = { x, y, tex_w, tex_h };
	SDL_RenderCopy(Application::renderer, texture, NULL, &dstRect);

	SDL_DestroyTexture(texture);
}

void TextManager::renderStringCentered(const char* txt, const char* fontFile, int fontSize, SDL_Color color, int aroundX, int y) {
	// loading font
	TTF_Font* font = TTF_OpenFont(fontFile, fontSize);
	if (!font) { std::cerr << "Failed to load font: " << TTF_GetError() << std::endl; }

	// create surface
	SDL_Surface* surf = TTF_RenderText_Blended(font, txt, color);
	if (!surf) {
		TTF_CloseFont(font);
		std::cerr << "Failed to render Text to surface: " << SDL_GetError() << std::endl;
	}

	// create texture from surface
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Application::renderer, surf);
	if (!texture) { std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl; }

	// clean up
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);

	// rendering
	int tex_w = 0;
	int tex_h = 0;
	SDL_QueryTexture(texture, NULL, NULL, &tex_w, &tex_h);
	int dst_x = aroundX - tex_w / 2;
	SDL_Rect dstRect = { dst_x, y, tex_w, tex_h };
	SDL_RenderCopy(Application::renderer, texture, NULL, &dstRect);

	SDL_DestroyTexture(texture);
}

void TextManager::renderStringInRect(const char* txt, const char* fontFile, int fontSize, SDL_Color color, SDL_Rect& dim) {
	// loading font
	TTF_Font* font = TTF_OpenFont(fontFile, fontSize);
	if (!font) { std::cerr << "Failed to load font: " << TTF_GetError() << std::endl; }

	// create surface
	SDL_Surface* surf = TTF_RenderText_Blended(font, txt, color);
	if (!surf) {
		TTF_CloseFont(font);
		std::cerr << "Failed to render Text to surface: " << SDL_GetError() << std::endl;
	}

	// create texture from surface
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Application::renderer, surf);
	if (!texture) { std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl; }

	// clean up
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);

	int tex_w = 0;
	int tex_h = 0;
	SDL_QueryTexture(texture, NULL, NULL, &tex_w, &tex_h);
	int dst_x = dim.x + dim.w / 2 - tex_w / 2;
	int dst_y = dim.y + dim.h / 2 - tex_h / 2;
	SDL_Rect dstRect = { dst_x, dst_y, tex_w, tex_h };
	SDL_RenderCopy(Application::renderer, texture, NULL, &dstRect);

	SDL_DestroyTexture(texture);
}

int TextManager::getTextWidth(TTF_Font* font, const std::string& text) {
	int w = 0;
	int h = 0;
	if (TTF_SizeText(font, text.c_str(), &w, &h) == 0) {
		return w;
	}
	else {
		SDL_Log("TTF_sizeText failed: %s", TTF_GetError());
		return 0;
	}
}

/*-Fonts------------------------------------------------------------------------------------------------------*/

const char* Fonts::DEFAULT = "res/fonts/Bungee-Regular.ttf";
const char* Fonts::OPEN_SANS_REGULAR = "res/fonts/OpenSans-Regular.ttf";

/*-FontSize------------------------------------------------------------------------------------------------------*/

const int FontSize::DEFAULT = 18;
const int FontSize::DEFAULT_MID = 14;
const int FontSize::DEFAULT_SMALL = 10;

/*-FileHandler----------------------------------------------------------------------------------------------------*/

FileHandler::FileHandler() {}

FileHandler::~FileHandler() {}

std::vector<std::string> FileHandler::readFile(const char* path) {
	std::vector<std::string> v;

	std::ifstream infile(path);
	if (!infile) {
		std::cerr << "Error opening file for reading: " << path << std::endl;
	}

	std::string line;
	while (std::getline(infile, line)) { v.push_back(line); }

	infile.close();

	return v;
}

void FileHandler::writeFile(const char* path, std::vector<const char*> input) {

	std::ofstream outfile(path);
	if (!outfile) {
		std::cerr << "Error opening file for writing: " << path << std::endl;
	}

	for (int i = 0; i < input.size(); i++) {
		outfile << input[i] << std::endl;
	}

	outfile.close();

}

void FileHandler::writeFile(const char* path, std::vector<std::string> input) {

	std::ofstream outfile(path);
	if (!outfile) {
		std::cerr << "Error opening file for writing: " << path << std::endl;
	}

	for (int i = 0; i < input.size(); i++) {
		outfile << input[i] << std::endl;
	}

	outfile.close();

}

void FileHandler::writeFile(const std::string& path, std::vector<std::string> input) {
	std::ofstream outfile(path);
	if (!outfile) {
		std::cerr << "Error opening file for writing: " << path << std::endl;
	}

	for (int i = 0; i < input.size(); i++) {
		outfile << input[i] << std::endl;
	}

	outfile.close();
}

bool FileHandler::doesFileExist(const std::string& path) {
	std::ifstream file(path);
	return file.good();
}


/*-Vector2 ----------------------------------------------------------------------------------------------------*/

Vector2::Vector2(float _x, float _y) : x(_x), y(_y) {}

Vector2 Vector2::operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.x); }

Vector2 Vector2::operator-(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }

Vector2 Vector2::operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }

Vector2 Vector2::operator/(float scalar) const { return Vector2(x / scalar, y / scalar); }

Vector2& Vector2::operator+=(const Vector2& other) {
	x += other.x;
	y += other.y;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& other) {
	x -= other.x;
	y -= other.y;
	return *this;
}

Vector2& Vector2::operator*=(float scalar) {
	x *= scalar;
	y *= scalar;
	return *this;
}

Vector2& Vector2::operator/=(float scalar) {
	x /= scalar;
	y /= scalar;
	return *this;
}

bool Vector2::operator==(const Vector2& other) const { return x == other.x && y == other.y; }

bool Vector2::operator!=(const Vector2& other) const { return !(*this == other); }

float Vector2::magnitude() const { return std::sqrt(x * x + y * y); }

void Vector2::normalize() {
	float mag = magnitude();
	if (mag != 0) {
		x /= mag;
		y /= mag;
	}
}

void Vector2::zero() {
	x = 0;
	x = 0;
}

void Vector2::one() {
	y = 1;
	y = 1;
}
