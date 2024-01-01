#include "TextureManager.h"

unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::LoadTexture(string textureName)
{
	string path = "images/";
	path += textureName + ".png";
	textures[textureName].loadFromFile(path);
}

sf::Texture& TextureManager::GetTexture(string textureName)
{
	// if it doesnt exist....
	if (textures.find(textureName) == textures.end()) {
		LoadTexture(textureName);
	}
	return textures[textureName];
	// TODO: insert return statement here
}

void TextureManager::Clear()
{
	textures.clear(); //delete all stored abjects in the map!
}
