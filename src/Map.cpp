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

void Map::render2d(NextEngine engine, int worldMap[], Player player)
{
	int y = -1;
	for (int x = 0; x < this->w*this->h; x++)
	{
		if (x%this->w == 0)
			y++;
		
		int drawX = (x%this->w) * 5;
		int drawY = y*5;
		
		std::string color;
		switch(worldMap[x])
		{
			case 0: color = "black"; break;
			case 1:  color = "red";  break;
			case 2:  color = "green";  break;
			case 3:  color = "blue";   break;
			case 4:  color = "gray";  break;
			default: color = "yellow"; break;
		}
		
		engine.setColor(color);
		engine.fillRect(drawX, drawY, 5, 5);
	}
	
	engine.setColor("red");
	engine.fillRect(player.posX*5, player.posY*5, 2, 2);
	engine.drawLine(player.posX*5, player.posY*5, player.posX*5 + player.dirX*10, player.posY*5 + player.dirY*10);
	
	double rotateX = player.dirX;
	double rotateY = player.dirY;
	rotateDir(rotateX, rotateY, 90);
}
