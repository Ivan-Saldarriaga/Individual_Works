#pragma once
#include "Tiles.h"
#include <vector>
class Board
{
	int windowWidth;
	int windowHeight;
	int numberMines;
	int origNumMines;
	int numUnflaggedMines;
	int numTiles;
	string gameStatus;
	vector<Tiles> allTiles;
	bool debugStatus;
public: 
	Board() {
		windowWidth = 0;
		windowHeight = 0;
		numberMines = 0;
		origNumMines = 0;
		numTiles = 0;
		numUnflaggedMines = 0;
		debugStatus = false;
	}
	Board(int width, int height, int mines) {
		windowWidth = width;
		windowHeight = height;
		numberMines = mines;
		origNumMines = numberMines;
		numTiles = width * height;
		numUnflaggedMines = mines;
		debugStatus = false;
	}
	void EditGameStatus(string status) {
		gameStatus = status;
	}
	string GetGameStatus() {
		return gameStatus;
	}
	void SetNumMinesToOrig() {
		numberMines = origNumMines;
	}
	void SetNumMines(int num) {
		numberMines = num;
	}
	int GetNumOrigMines() {
		return origNumMines;
	}
	int GetWindowWidth() {
		return windowWidth;
	}
	int GetWindowHeight() {
		return windowHeight;
	}
	int GetNumTiles() {
		return numTiles;
	}
	int GetNumMines() {
		return numberMines;
	}
	void SetBoardTiles(vector<Tiles>& tiles) {
		allTiles = tiles;
	}
	Tiles GetTiles(int tileNumber) {
		return allTiles[tileNumber];
	}
	Tiles* GetTilePointer(int tileNumber) {
		return &allTiles[tileNumber];
	}
	void ToggleDebugStatus() {
		debugStatus = !debugStatus;
	}
	bool GetDebugStatus() {
		return debugStatus;
	}
	void AddTile(Tiles& newTile) {
		allTiles.push_back(newTile);
	}
	void EditTileName(int tileNum, string newTexture) {
		allTiles[tileNum].SetTileTexture(newTexture);
	}
	void EditFlagStatus(int tileNum) {
		allTiles[tileNum].ToggleFlag();
	}
	void EditMineStatus(int tileNum) {
		allTiles[tileNum].ToggleMine();
	}
	void AddAdjacentPointer(int tileNum, int adjacentTileNum) {
		allTiles[tileNum].AddAdjacent(&allTiles[adjacentTileNum]);
	}
	void EditNumFlaggedMines(int num) {
		numUnflaggedMines = num;
	}
	int GetNumUnflaggedMines() {
		return numUnflaggedMines;
	}

};

