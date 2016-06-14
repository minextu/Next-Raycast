#include "Map.h"

void Map::setSize(int w, int h)
{
	this->w = w;
	this->h = h;
}

void Map::set(int map[])
{
	this->map = map;
}
