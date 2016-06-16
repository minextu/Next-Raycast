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

void Map::render2d(NextEngine engine, int worldMap[], Player player, NextImage images[])
{
	int y = -1;
	for (int x = 0; x < this->w*this->h; x++)
	{
		if (x%this->w == 0)
			y++;
		
		int drawX = (x%this->w) * 5;
		int drawY = (this->h - y)*5;
		
		int textureNum = worldMap[x];
		engine.drawImage(images[textureNum], 0, 0, 64, 64, drawX, drawY, 5, 5);
	}
	
	engine.setColor("red");
	engine.fillRect(player.posX*5, (this->h - player.posY)*5, 2, 2);
	engine.drawLine(player.posX*5,(this->h - player.posY)*5, player.posX*5 + player.dirX*10, (this->h - player.posY)*5 - player.dirY*10);
	
	double rotateX = player.dirX;
	double rotateY = player.dirY;
	rotateDir(rotateX, rotateY, 90);
}
