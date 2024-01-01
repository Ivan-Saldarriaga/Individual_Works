#pragma once
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
using namespace std;
class Tiles
{
	string textureName;
	int tileNum;
	int xPosition;
	int yPosition;
	string tileStatus;
	bool flagged;
	sf::Texture tileTexture;
	sf::Sprite tileSprite;
	vector<Tiles*> adjancentTiles;
	bool ismine;
	bool hasAdjacentMines;
	int numberOfAdjacentMines;
	//other variables such as the number, if its a mine etc
public:
	Tiles() {
		tileTexture = TextureManager::GetTexture("tile_hidden");
		tileSprite.setTexture(tileTexture);
		flagged = false;
		ismine = false;
		numberOfAdjacentMines = 0;
	}
	Tiles(string typeOfTile) {
		textureName = typeOfTile;
		flagged = false;
		ismine = false;
		numberOfAdjacentMines = 0;
	}
	int GetTileNum() {
		return tileNum;
	}
	string GetTileName() {
		return textureName;
	}
	Tiles* GetAdjTile(int tile) {
		return adjancentTiles[tile];
	}
	int AdjTileSize() {
		return adjancentTiles.size();
	}
	void SetPosition(int x, int y) {
		tileSprite.setPosition(x, y);
		xPosition = x;
		yPosition = y;
	}
	void SetTileTexture(string newTexture) {
		textureName = newTexture;
	}
	sf::Sprite GetSprite() {
		tileTexture = TextureManager::GetTexture(textureName);
		tileSprite.setTexture(tileTexture);
		return tileSprite;
	}
	void AddAdjacent(Tiles* adjacentTile) {
		adjancentTiles.push_back(adjacentTile);
	}
	void SetNumAdjMines() {
		for (int i = 0; i < adjancentTiles.size(); i++) {
			if (adjancentTiles[i]->GetMineStatus() == true) {
				numberOfAdjacentMines++;
			}
		}
	}
	void ResetNumAdjMines(int reset) {
		numberOfAdjacentMines = reset;
	}
	void ClearAdjacent() {
		//adjancentTiles.clear();
		numberOfAdjacentMines = 0;
	}
	void ToggleFlag() {
		flagged = !flagged;
	}
	void ToggleMine() {
		ismine = !ismine;
	}
	bool GetFlagStatus() {
		return flagged;
	}
	bool GetMineStatus() {
		return ismine;
	}
	int GetNumAdjacentMines() {
		return numberOfAdjacentMines;
	}
	int GetxPosition() {
		return xPosition;
	}
	int GetyPosition() {
		return yPosition;
	}
};

