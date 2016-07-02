#include "RayCollision.cpp"
#include "Ray.h"

Ray::Ray(double startX, double startY, double angle)
{
	this->x = startX;
	this->y = startY;
	this->angle = angle;
}

void Ray::setMapSize(int width, int height)
{
	this->mapWidth = width;
	this->mapHeight = height;
}
std::vector<RayCollision> Ray::send()
{
	return this->send(false, false, nullptr);
}
std::vector<RayCollision> Ray::send(bool stopFirst, bool hideAir, Map* map)
{
	std::vector<RayCollision> collisions;
	
	// Currently only a height and width of 1 works correctly
	int blockWidth = 1;
	int blockHeight = 1;
	
	bool end = false;
	double posX = this->x;
	double posY = this->y;
	double rayAngle = this->angle;
	int mapWidth = this->mapWidth;
	int mapHeight = this->mapHeight;
	
	// defines the needed speed to create the ray with the Angle 'rayAngle'
	double speedX = cos(rayAngle);
	double speedY = sin(rayAngle);
	
	for (int i = 0; !end; i++)
	{
		// nextBlock has to be negative if the speed is also negative
		int nextBlockX = blockWidth;
		int nextBlockY = blockHeight;
		if (speedX < 0)
			nextBlockX = -nextBlockX;
		if (speedY < 0)
			nextBlockY = -nextBlockY;
		
		
		/* Calculate the value posX:
		*
		*  Formular: posX + speedX * distanceX = round(posX) + nextBlockX
		*
		*  round(posX) is the next block Border of the current Position
		*  distanceX is the unknown variable.
		*
		*  distanceX = (round(posX) + nextBlockX - posX) / speedX
		*/
		double distanceXCurrent = (round(posX) - posX) / speedX;
		double distanceXNext = (round(posX) + nextBlockX - posX) / speedX;
		double distanceYCurrent = (round(posY) - posY) / speedY;
		double distanceYNext = (round(posY) + nextBlockY - posY) / speedY;
		
		double distanceY;
		double distanceX;
		double distance;
		// do not use the current block border, if we already are on one
		if (distanceXCurrent > 0)
			distanceX = distanceXCurrent;
		else
			distanceX = distanceXNext;
			
		if (distanceYCurrent > 0)
			distanceY = distanceYCurrent;
		else
			distanceY = distanceYNext;
		
		// use the shortest distance
		if (distanceX < distanceY)
			distance = distanceX;
		else
			distance = distanceY;
		
		// stop if first block was hit
		if (stopFirst)
		{
			int blockX = (int)(posX);
			int blockY = (int)(posY);
			Block* block = map->getBlock(blockX,blockY);
			
			if (block != nullptr && block->type != 0)
			{
				end = true;
				break;
			}
		}

		// jump to ne next block border
		posX += speedX * distance;
		posY += speedY * distance;
		
		// save the position
		if (!hideAir)
			collisions.push_back(RayCollision(posX, posY));
		else
		{
			int blockX = (int)floor(posX);
			int blockY = (int)floor(posY);
			Block* block = map->getBlock(blockX,blockY);
			
			if (block != nullptr && block->type != 0)
				collisions.push_back(RayCollision(posX, posY));
		}
		
		// stop the loop, if we are past the map size
		if (speedX <= 0 && posX <= 0 || speedX > 0 && posX >= mapWidth || speedY < 0 && posY <= 0 || speedY >= 0 && posY >= mapHeight)
		{
			end = true;
			break;
		}
	}
		
	return collisions;
}
