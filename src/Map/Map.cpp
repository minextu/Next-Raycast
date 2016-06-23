#include "Block.cpp"
#include "Map.h"

void Map::setSize(int width, int height)
{
	this->width = width;
	this->height = height;
}

void Map::set(std::vector<Block> map)
{
	this->map = map;
}

int Map::getWidth()
{
	return this->width;
}

int Map::getHeight()
{
	return this->height;
}

const std::vector<Block> Map::get() const
{
	return this->map;
}

const Block Map::getBlock(int x, int y) const
{
	// convert x, y to 1D Array index
	int mapIndex = y * this->width + x;
	return this->map[mapIndex];
}
