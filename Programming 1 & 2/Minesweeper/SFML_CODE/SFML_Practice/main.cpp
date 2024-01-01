#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <iostream>
#include <vector>
#include "TextureManager.h"
#include "Board.h"
#include "Tiles.h"
#include "Random.h"
using namespace std;

void GetWindowDataCFG(string filepath, Board& board) {
	ifstream inputFile(filepath);
	if (inputFile.is_open()) {
		string currentLine;
		istringstream stream(currentLine);
		int width, height, numberOfMines;
		//istringstream stream(currentLine);
		getline(inputFile, currentLine);
		width = stoi(currentLine);
		currentLine = "";
		//istringstream stream(currentLine);
		getline(inputFile, currentLine);
		height = stoi(currentLine);
		currentLine = "";
		//istringstream stream(currentLine);
		getline(inputFile, currentLine);
		numberOfMines = stoi(currentLine);
		currentLine = "";
		Board newBoard(width, height, numberOfMines);
		board = newBoard;
	}
	else {
		cout << "FILE IS CLOSED SILLY BANILLY" << endl;
	}
}
void RecursiveReveal(Tiles* tile) {
	if (tile->GetTileName() != "tile_revealed" && tile->GetMineStatus() != true && tile->GetFlagStatus() != true) {
		tile->SetTileTexture("tile_revealed");
		if (tile->GetNumAdjacentMines() == 0) {
			tile->SetTileTexture("tile_revealed");
			for (int i = 0; i < tile->AdjTileSize(); i++) {
				RecursiveReveal(tile->GetAdjTile(i));
			}
		}
	}
}
void SetMines(Board& mainboard) {
	for (int i = 0; i < mainboard.GetNumMines(); i++) {
		int randomTileNum = Random::Int(0, mainboard.GetNumTiles() - 1);
		if (mainboard.GetTiles(randomTileNum).GetMineStatus() == true) {
			i--;
		}
		else {
			mainboard.EditMineStatus(randomTileNum);
		}
	}
	mainboard.EditGameStatus("default");
}
void RestartBoard(Board& mainboard) {
	if (mainboard.GetDebugStatus() == true) {
		mainboard.ToggleDebugStatus();
	}
	if (mainboard.GetNumMines() != mainboard.GetNumOrigMines()) {
		mainboard.SetNumMinesToOrig();
	}
	for (int i = 0; i < mainboard.GetNumTiles(); i++) {
		//1st reinitnialze everything
		Tiles* tile = mainboard.GetTilePointer(i);
		tile->SetTileTexture("tile_hidden");
		if (tile->GetFlagStatus() == true) {
			tile->ToggleFlag();
		}
		if (tile->GetMineStatus() == true) {
			tile->ToggleMine();
		}
		tile->ResetNumAdjMines(0);
	}
	SetMines(mainboard);
	for (int i = 0; i < mainboard.GetNumTiles(); i++) {
		Tiles* tileSet = mainboard.GetTilePointer(i);
		tileSet->SetNumAdjMines();
	}
}
void LoadAndSetBoard(Board& mainboard, string boardNum) {
	if (mainboard.GetDebugStatus() == true) {
		mainboard.ToggleDebugStatus();
	}
	for (int i = 0; i < mainboard.GetNumTiles(); i++) {
		//1st reinitnialze everything
		Tiles* tile = mainboard.GetTilePointer(i);
		tile->SetTileTexture("tile_hidden");
		if (tile->GetFlagStatus() == true) {
			tile->ToggleFlag();
		}
		if (tile->GetMineStatus() == true) {
			tile->ToggleMine();
		}
		tile->ResetNumAdjMines(0);
	}
	//set mines is different!!! and number of mines is different- so one must accoutn for it!
	string filepath = "boards/testboard" + boardNum + ".brd";
	ifstream inputFile(filepath);
	if (inputFile.is_open()) {
		int tileCount = 0;
		int numMines = 0;
		for (int i = 0; i < mainboard.GetWindowHeight(); i++) {
			string currentLine;
			istringstream stream(currentLine);
			getline(inputFile, currentLine);
			//cout << currentLine << endl;
			for (int j = 0; j < mainboard.GetWindowWidth(); j++) {
				if (currentLine.at(j) == '1') {
					Tiles* currentTile = mainboard.GetTilePointer(tileCount);
					currentTile->ToggleMine();
					numMines++;
				}
				tileCount++;
			}
			currentLine = "";
		}
		mainboard.SetNumMines(numMines);
	}
	else {
		cout << "FILE IS CLOSED SILLY BANILLY" << endl;
	}

	for (int i = 0; i < mainboard.GetNumTiles(); i++) {
		mainboard.GetTilePointer(i)->SetNumAdjMines();
	}
	mainboard.EditGameStatus("default");
}
int main() {
	
		Board mainboard;
		GetWindowDataCFG("boards/config.cfg", mainboard);

		sf::RenderWindow window(sf::VideoMode(mainboard.GetWindowWidth() * 32, (mainboard.GetWindowHeight() * 32) + 100), "SFML works!");
		//default board can make a function to do it kek!!!

		//EVENTUALLY I WILL HAVE TO MAKE THESE TILE OBJECTS IN THE BOARD CLASS JUST AS I DID WITH THE NORMAL TILES

		Tiles uiSmile("face_happy");
		int xPosition = (mainboard.GetWindowWidth() * 32) / 2;
		uiSmile.SetPosition(xPosition - 32, mainboard.GetWindowHeight() * 32);
		Tiles uiDebug("debug");
		uiDebug.SetPosition(xPosition + 96, mainboard.GetWindowHeight() * 32);
		Tiles uiTest1("test_1");
		uiTest1.SetPosition(xPosition + 160, mainboard.GetWindowHeight() * 32);
		Tiles uiTest2("test_2");
		uiTest2.SetPosition(xPosition + 224, mainboard.GetWindowHeight() * 32);
		Tiles uiTest3("test_3");
		uiTest3.SetPosition(xPosition + 288, mainboard.GetWindowHeight() * 32);
		
		for (int i = 0; i < mainboard.GetWindowHeight(); i++) {
			for (int j = 0; j < mainboard.GetWindowWidth(); j++) {
				Tiles newTile("tile_hidden");
				newTile.SetPosition((32 * j), (32 * i));
				mainboard.AddTile(newTile);
			}
		}
		SetMines(mainboard);
		for (int currentTile = 0; currentTile < mainboard.GetNumTiles(); currentTile++) {
			//		//case1: top row
			if (currentTile - mainboard.GetWindowWidth() >= 0) {
				mainboard.AddAdjacentPointer(currentTile, currentTile - mainboard.GetWindowWidth());
			}
			if ((currentTile - mainboard.GetWindowWidth() - 1 >= 0) && (currentTile % mainboard.GetWindowWidth() != 0)) {
				mainboard.AddAdjacentPointer(currentTile, (currentTile - mainboard.GetWindowWidth() - 1));
			}
			if ((currentTile - mainboard.GetWindowWidth() + 1 >= 0) && (currentTile % mainboard.GetWindowWidth() != 24)) {
				mainboard.AddAdjacentPointer(currentTile, (currentTile - mainboard.GetWindowWidth() + 1));
			}
			//case 2 left and right!
			if ((currentTile - 1 >= 0) && (currentTile % mainboard.GetWindowWidth() != 0)) {
				mainboard.AddAdjacentPointer(currentTile, (currentTile - 1));
			}
			if ((currentTile + 1 < mainboard.GetNumTiles()) && (currentTile % mainboard.GetWindowWidth() != 24)) {
				mainboard.AddAdjacentPointer(currentTile, (currentTile + 1));
			}
			//		//case 3 bottom row
			if ((currentTile + mainboard.GetWindowWidth() < mainboard.GetNumTiles())) {
				mainboard.AddAdjacentPointer(currentTile, (currentTile + mainboard.GetWindowWidth()));
			}
			if ((currentTile + mainboard.GetWindowWidth() - 1 < mainboard.GetNumTiles()) && (currentTile % mainboard.GetWindowWidth() != 0)) {
				mainboard.AddAdjacentPointer(currentTile, (currentTile + mainboard.GetWindowWidth() - 1));
			}
			if ((currentTile + mainboard.GetWindowWidth() + 1 < mainboard.GetNumTiles()) && (currentTile % mainboard.GetWindowWidth() != 24)) {
				mainboard.AddAdjacentPointer(currentTile, (currentTile + mainboard.GetWindowWidth() + 1));
			}
		}
		for (int i = 0; i < mainboard.GetNumTiles(); i++) {
			mainboard.GetTilePointer(i)->SetNumAdjMines();
		}

		while (window.isOpen()) {
			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					window.close();
				}
				sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
				//cout << "Mouse Position: " << mousePosition.x << ", " << mousePosition.y << endl;
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					if (mainboard.GetGameStatus() == "default") {
						for (int i = 0; i < mainboard.GetNumTiles(); i++) {
							sf::Sprite tileSprite = mainboard.GetTiles(i).GetSprite();
							sf::FloatRect rectangle1 = tileSprite.getGlobalBounds();
							if (rectangle1.contains(mousePosition.x, mousePosition.y) && !mainboard.GetTiles(i).GetFlagStatus()) {
								RecursiveReveal(mainboard.GetTilePointer(i));
								if (mainboard.GetTiles(i).GetMineStatus() == true) {
									mainboard.EditGameStatus("lose");
									mainboard.EditTileName(i, "tile_revealed");
									if (mainboard.GetDebugStatus() == true) {
										mainboard.ToggleDebugStatus();
									}
								}
							}
						}
					}
					sf::FloatRect rectangle2 = uiSmile.GetSprite().getGlobalBounds();
					if (rectangle2.contains(mousePosition.x, mousePosition.y)) {
						//restart == true;
						RestartBoard(mainboard);
					}
					sf::FloatRect rectangle3 = uiDebug.GetSprite().getGlobalBounds();
					if (rectangle3.contains(mousePosition.x, mousePosition.y) && mainboard.GetGameStatus() == "default") {
						mainboard.ToggleDebugStatus();
					}
					sf::FloatRect rectangle4 = uiTest1.GetSprite().getGlobalBounds();
					if (rectangle4.contains(mousePosition.x, mousePosition.y)) {
						LoadAndSetBoard(mainboard, "1");
					}
					sf::FloatRect rectangle5 = uiTest2.GetSprite().getGlobalBounds();
					if (rectangle5.contains(mousePosition.x, mousePosition.y)) {
						LoadAndSetBoard(mainboard, "2");
					}
					sf::FloatRect rectangle6 = uiTest3.GetSprite().getGlobalBounds();
					if (rectangle6.contains(mousePosition.x, mousePosition.y)) {
						LoadAndSetBoard(mainboard, "3");
					}
				}
				else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
					if (mainboard.GetGameStatus() == "default") {
						for (int i = 0; i < mainboard.GetNumTiles(); i++) {
							sf::Sprite tileSprite = mainboard.GetTiles(i).GetSprite();
							sf::FloatRect rectangle = tileSprite.getGlobalBounds();
							if (rectangle.contains(mousePosition.x, mousePosition.y) && mainboard.GetTiles(i).GetTileName() != "tile_revealed") {
								mainboard.EditFlagStatus(i);
							}
						}
					}
				}
				//can put this into teh right click above!!!
				if (mainboard.GetGameStatus() != "lose") {
					int remainingTiles = 0;
					for (int i = 0; i < mainboard.GetNumTiles(); i++) {
						if (mainboard.GetTiles(i).GetTileName() == "tile_hidden") {
							remainingTiles++;
						}
					}
					if (remainingTiles == mainboard.GetNumMines()) {
						mainboard.EditGameStatus("win");
						for (int i = 0; i < mainboard.GetNumTiles(); i++) {
							if (mainboard.GetTiles(i).GetMineStatus() == true && mainboard.GetTiles(i).GetFlagStatus() == false) {
								mainboard.EditFlagStatus(i);
							}
							if (mainboard.GetDebugStatus() == true) {
								mainboard.ToggleDebugStatus();
							}
						}
					}
				}
				//Clear anythig already drawn!
				window.clear(sf::Color(255, 255, 255));

					for (int i = 0; i < mainboard.GetNumTiles(); i++) {
							if (mainboard.GetTiles(i).GetFlagStatus()) {
								window.draw(mainboard.GetTiles(i).GetSprite());
								Tiles newTile("flag");
								newTile.SetPosition(mainboard.GetTiles(i).GetxPosition(), mainboard.GetTiles(i).GetyPosition());
								window.draw(newTile.GetSprite());
								if (mainboard.GetGameStatus() == "lose") {
									if (mainboard.GetTiles(i).GetMineStatus() == true) {
										Tiles newTileUnder("tile_revealed");
										newTileUnder.SetPosition(mainboard.GetTiles(i).GetxPosition(), mainboard.GetTiles(i).GetyPosition());
										window.draw(newTileUnder.GetSprite());
										Tiles newTile("mine");
										newTile.SetPosition(mainboard.GetTiles(i).GetxPosition(), mainboard.GetTiles(i).GetyPosition());
										window.draw(newTile.GetSprite());
									}
								}
								else if (mainboard.GetGameStatus() == "win") {
									Tiles newTile("flag");
									newTile.SetPosition(mainboard.GetTiles(i).GetxPosition(), mainboard.GetTiles(i).GetyPosition());
									window.draw(newTile.GetSprite());
								}
								else if (mainboard.GetTiles(i).GetMineStatus() == true && mainboard.GetDebugStatus() == true) {
									Tiles newTile("mine");
									newTile.SetPosition(mainboard.GetTiles(i).GetxPosition(), mainboard.GetTiles(i).GetyPosition());
									window.draw(newTile.GetSprite());
								}
							}
							else if ((mainboard.GetTiles(i).GetNumAdjacentMines() > 0) && mainboard.GetTiles(i).GetTileName() == "tile_revealed") {
								window.draw(mainboard.GetTiles(i).GetSprite());
								string tileName = "number_" + to_string(mainboard.GetTiles(i).GetNumAdjacentMines());
								Tiles newTile(tileName);
								newTile.SetPosition(mainboard.GetTiles(i).GetxPosition(), mainboard.GetTiles(i).GetyPosition());
								window.draw(newTile.GetSprite());
								if (mainboard.GetGameStatus() == "lose") {
									if (mainboard.GetTiles(i).GetMineStatus() == true) {
										Tiles newTileUnder("tile_revealed");
										newTileUnder.SetPosition(mainboard.GetTiles(i).GetxPosition(), mainboard.GetTiles(i).GetyPosition());
										window.draw(newTileUnder.GetSprite());
										Tiles newTile("mine");
										newTile.SetPosition(mainboard.GetTiles(i).GetxPosition(), mainboard.GetTiles(i).GetyPosition());
										window.draw(newTile.GetSprite());
									}
								}
							}
							else {
								window.draw(mainboard.GetTiles(i).GetSprite());
								if (mainboard.GetGameStatus() == "lose") {
									if (mainboard.GetTiles(i).GetMineStatus() == true) {
										Tiles newTileUnder("tile_revealed");
										newTileUnder.SetPosition(mainboard.GetTiles(i).GetxPosition(), mainboard.GetTiles(i).GetyPosition());
										window.draw(newTileUnder.GetSprite());
										Tiles newTile("mine");
										newTile.SetPosition(mainboard.GetTiles(i).GetxPosition(), mainboard.GetTiles(i).GetyPosition());
										window.draw(newTile.GetSprite());
									}
								}
								else if (mainboard.GetTiles(i).GetMineStatus() == true && mainboard.GetDebugStatus() == true) {
									Tiles newTile("mine");
									newTile.SetPosition(mainboard.GetTiles(i).GetxPosition(), mainboard.GetTiles(i).GetyPosition());
									window.draw(newTile.GetSprite());
								}
							}
					}
					


				int numFlaggedMines = mainboard.GetNumMines();
				for (int i = 0; i < mainboard.GetNumTiles(); i++) {
					if (mainboard.GetTiles(i).GetFlagStatus() == true) {
						numFlaggedMines--;
					}
				}
				mainboard.EditNumFlaggedMines(numFlaggedMines);
				//cout << mainboard.GetNumUnflaggedMines() << endl;
				//draw the flags----
				string numUnflaggedMines = to_string(mainboard.GetNumUnflaggedMines());
				bool negativeFlags = false;
				if (numUnflaggedMines.at(0) == '-') {
					negativeFlags = true;
					numUnflaggedMines = numUnflaggedMines.substr(1, numUnflaggedMines.size());
				}
				if (numUnflaggedMines.size() == 2) {
					numUnflaggedMines = "0" + numUnflaggedMines;
				}
				else if (numUnflaggedMines.size() == 1) {
					numUnflaggedMines = "00" + numUnflaggedMines;
				}
				//cout << numUnflaggedMines << endl;
				Tiles digitsTile("digits");
				for (int i = 0; i < numUnflaggedMines.size(); i++) {
					if (negativeFlags) {
						sf::IntRect rectangle(21 * 10 ,0, 21 ,32);
						sf::Sprite negSprite = digitsTile.GetSprite();
						negSprite.setTextureRect(rectangle);
						negSprite.setPosition(0, mainboard.GetWindowHeight() * 32);
						window.draw(negSprite);
					}
					if (numUnflaggedMines.at(i) == '0') {
						sf::IntRect rectangle(21 * 0, 0, 21, 32);
						sf::Sprite negSprite = digitsTile.GetSprite();
						negSprite.setTextureRect(rectangle);
						negSprite.setPosition(21 * (i + 1), mainboard.GetWindowHeight() * 32);
						window.draw(negSprite);
					}
					else if (numUnflaggedMines.at(i) == '1') {
						sf::IntRect rectangle(21 * 1, 0, 21, 32);
						sf::Sprite negSprite = digitsTile.GetSprite();
						negSprite.setTextureRect(rectangle);
						negSprite.setPosition(21 * (i + 1), mainboard.GetWindowHeight() * 32);
						window.draw(negSprite);
					}
					else if (numUnflaggedMines.at(i) == '2') {
						sf::IntRect rectangle(21 * 2, 0, 21, 32);
						sf::Sprite negSprite = digitsTile.GetSprite();
						negSprite.setTextureRect(rectangle);
						negSprite.setPosition(21 * (i + 1), mainboard.GetWindowHeight() * 32);
						window.draw(negSprite);
					}
					else if (numUnflaggedMines.at(i) == '3') {
						sf::IntRect rectangle(21 * 3, 0, 21, 32);
						sf::Sprite negSprite = digitsTile.GetSprite();
						negSprite.setTextureRect(rectangle);
						negSprite.setPosition(21 * (i + 1), mainboard.GetWindowHeight() * 32);
						window.draw(negSprite);
					}
					else if (numUnflaggedMines.at(i) == '4') {
						sf::IntRect rectangle(21 * 4, 0, 21, 32);
						sf::Sprite negSprite = digitsTile.GetSprite();
						negSprite.setTextureRect(rectangle);
						negSprite.setPosition(21 * (i + 1), mainboard.GetWindowHeight() * 32);
						window.draw(negSprite);
					}
					else if (numUnflaggedMines.at(i) == '5') {
						sf::IntRect rectangle(21 * 5, 0, 21, 32);
						sf::Sprite negSprite = digitsTile.GetSprite();
						negSprite.setTextureRect(rectangle);
						negSprite.setPosition(21 * (i + 1), mainboard.GetWindowHeight() * 32);
						window.draw(negSprite);
					}
					else if (numUnflaggedMines.at(i) == '6') {
						sf::IntRect rectangle(21 * 6, 0, 21, 32);
						sf::Sprite negSprite = digitsTile.GetSprite();
						negSprite.setTextureRect(rectangle);
						negSprite.setPosition(21 * (i + 1), mainboard.GetWindowHeight() * 32);
						window.draw(negSprite);
					}
					else if (numUnflaggedMines.at(i) == '7') {
						sf::IntRect rectangle(21 * 7, 0, 21, 32);
						sf::Sprite negSprite = digitsTile.GetSprite();
						negSprite.setTextureRect(rectangle);
						negSprite.setPosition(21 * (i + 1), mainboard.GetWindowHeight() * 32);
						window.draw(negSprite);
					}
					else if (numUnflaggedMines.at(i) == '8') {
						sf::IntRect rectangle(21 * 8, 0, 21, 32);
						sf::Sprite negSprite = digitsTile.GetSprite();
						negSprite.setTextureRect(rectangle);
						negSprite.setPosition(21 * (i + 1), mainboard.GetWindowHeight() * 32);
						window.draw(negSprite);
					}
					else {
						sf::IntRect rectangle(21 * 9, 0, 21, 32);
						sf::Sprite negSprite = digitsTile.GetSprite();
						negSprite.setTextureRect(rectangle);
						negSprite.setPosition(21 * (i + 1), mainboard.GetWindowHeight() * 32);
						window.draw(negSprite);
					}

				}

				
				if (mainboard.GetGameStatus() == "win") {
					uiSmile.SetTileTexture("face_win");
				}
				else if (mainboard.GetGameStatus() == "lose") {
					uiSmile.SetTileTexture("face_lose");
				}
				else if (mainboard.GetGameStatus() == "default") {
					uiSmile.SetTileTexture("face_happy");
				}
				window.draw(uiSmile.GetSprite());
				window.draw(uiDebug.GetSprite());
				window.draw(uiTest1.GetSprite());;
				window.draw(uiTest2.GetSprite());
				window.draw(uiTest3.GetSprite());


				window.display();
			}
		}
	return 0;
}