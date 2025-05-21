#include "Realm.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "../../screens/Game.h"
#include "../../Utils.h"
#include <iomanip>

Realm::Realm(const char* mapFile, Game* game, Application* app) {
	this->game = game;
	this->app = app;
	this->mapFile = mapFile;
	if (!loadMapFile()) { std::cerr << "Failed to load map from file: " << mapFile << std::endl; }
	this->bgColor = { 0, 0, 0, 255 };
	this->spawnX = 0;
	this->spawnY = 0;

	//std::cout << "Realm has " << maxRows << " rows and " << maxCols << " cols!" << std::endl;
}

Realm::~Realm() {
//	std::cout << "Realm deleted" << std::endl;
}

bool Realm::loadMapFile() {
	std::ifstream file(mapFile);
	if (!file.is_open()) {
		std::cerr << "Failed to open map file: " << mapFile << std::endl;
		return false;
	}

	IntMap2D tmpMap;
	std::string line;

	while (std::getline(file, line)) {
		std::vector<int> row;
		std::istringstream ss(line);
		int tileID;
		while (ss >> tileID) {
			row.push_back(tileID);
		}
		tmpMap.push_back(row);
	}

	maxRows = tmpMap.size();
	maxCols = maxRows > 0 ? tmpMap[0].size() : 0;

	tileMap.resize(maxRows, std::vector<Tile*>(maxCols));

	for (int r = 0; r < maxRows; ++r) {
		for (int c = 0; c < maxCols; ++c) {
			int tileID = tmpMap[r][c];
			tileMap[r][c] = game->tileHandler->getTile(tileID);
		}
	}

	std::cout << "loaded map: " << mapFile << std::endl;

	file.close();
	return true;
}

bool Realm::saveMapFile() {
	std::ofstream file(mapFile);
	
	if (!file.is_open()) {
		std::cerr << "Failed to open map file for saving: " << mapFile << std::endl;
		return false;
	}

	for (int r = 0; r < maxRows; ++r) {
		for (int c = 0; c < maxCols; c++) {
			int tileID = game->tileHandler->getIndex(tileMap[r][c]);

			file << std::setw(3) << std::setfill('0') << tileID;

			if (c < maxCols - 1) {
				file << " ";
			}
		}
		file << "\n";
	}

	std::cout << "Map saved: " << mapFile << std::endl;
	file.close();
	return true;
}

void Realm::render() {
	// render the backgound color
	SDL_SetRenderDrawColor(app->renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	SDL_RenderClear(app->renderer);

	// render the tiles
	int wc = 0;
	int wr = 0;

	while (wc < maxCols && wr < maxRows) {
		Tile* t = getTileAt(wr, wc);

		int wx = wc * 64;
		int wy = wr * 64;
		int sx = wx - game->player->getWorldX() + game->player->getScreenX();
		int sy = wy - game->player->getWorldY() + game->player->getScreenY();

		if (wx + 64 > game->player->getWorldX() - game->player->getScreenX() &&
			wx - 64 < game->player->getWorldX() + game->player->getScreenX() &&
			wy + 64 > game->player->getWorldY() - game->player->getScreenY() &&
			wy - 64 < game->player->getWorldY() + game->player->getScreenY())
		{
			SDL_Rect d = { sx, sy, 64, 64 };
			TextureManager::render(t->getTextureSheet(), t->getSrcRect(), d);
		}
		wc++;

		if (wc == maxCols) {
			wc = 0;
			wr++;
		}
	}
}

void Realm::overwriteTile(int x, int y, int tileIndex) {
	if (tileIndex >= 0 && tileIndex <= game->tileHandler->getTilesAmount()) {
		if (tileMap[y][x] != game->tileHandler->getTile(tileIndex)) {
			tileMap[y][x] = game->tileHandler->getTile(tileIndex);
		}
		
	}
}

int Realm::getTileRow(Tile* tile) const {
	for (int r = 0; r < maxRows; ++r) {
		for (int c = 0; c < maxCols; ++c) {
			if (tileMap[r][c] == tile) {
				return r;
			}
		}
	}
	return -1;
}

int Realm::getTileCol(Tile* tile) const {
	for (int r = 0; r < maxRows; r++) {
		for (int c = 0; c < maxCols; c++) {
			std::cout << c << std::endl;
			if (getTileAt(r, c) == tile) {
				return c;
			}
		}
	}
	return -1;
}

void Realm::setSpawn(int x, int y) {
	this->spawnX = x;
	this->spawnY = y;
}
