#include "Ray.cpp"
#include "Camera.h"

// debug
void drawBorderCrossCircle(NextEngine&, std::vector<RayCollision>);


void Camera::setEngine(NextEngine& engine)
{
	this->engine = engine;
}

void Camera::setPlayer(Player* player)
{
	this->player = player;
}

void Camera::setTextures(NextImage images[])
{
	this->textures = images;
}

void Camera::renderMap(Map& map)
{
	double rayStartX = this->player->getX();
	double rayStartY = this->player->getY();
	double startAngle = this->player->getAngle();
	
	// send rays in screenWidth different directions (going left and right from center)
	for (int screenX = 0; screenX <= screenWidth / 2; screenX++)
	{
		// rays should be send with linear angle difference until the fov is reached
		double rayDiff = ((double)this->fov / 2) / screenWidth * screenX;
		rayDiff *= M_PI / 180;
		
		// right from screen center
		this->renderRay(map, rayStartX, rayStartY, screenX, startAngle + rayDiff);
		// left from screen center
		this->renderRay(map, rayStartX, rayStartY, -screenX, startAngle - rayDiff);
	}
}

void Camera::renderRay(Map& map, double rayStartX, double rayStartY, int screenX, double rayAngle)
{
	// Detect every possible border the line crosses
	Ray ray(rayStartX, rayStartY, rayAngle);
	ray.setMapSize(map.getWidth(), map.getHeight());
	std::vector<RayCollision> collisions = ray.send();
	
	// debug
	if (screenX == 0 || screenX == screenWidth / 2 || screenX == -screenWidth / 2)
		drawBorderCrossCircle(this->engine, collisions);
	
	// render blocks further away first, so closer ones can overwrite them (if jumping would not be allowed, only the first block has to be rendered)
	std::reverse(collisions.begin(), collisions.end());
	
	for (int i = 0; i < collisions.size(); ++i)
	{
		double blockX = std::floor(collisions[i].x);
		double blockY = std::floor(collisions[i].y);
		
		
		if (collisions[i].y == blockY)
		{
			// upper block border
			this->renderBlockLine(blockX, blockY, collisions[i], rayStartX, rayStartY, map, screenX, 1);
			// bottom block border
			this->renderBlockLine(blockX, blockY-1, collisions[i], rayStartX, rayStartY, map, screenX, 3);
			
		}
		else if (collisions[i].x == blockX)
		{
			// left block border
			this->renderBlockLine(blockX, blockY, collisions[i], rayStartX, rayStartY, map, screenX, 2);
			// right block border
			this->renderBlockLine(blockX-1, blockY, collisions[i], rayStartX, rayStartY, map, screenX, 4);
		}
	}
}

void Camera::renderBlockLine(double blockX, double blockY, RayCollision collision, double rayStartX, double rayStartY, Map& map, double offsetX, int side)
{
	Block *blockPtr = map.getBlock(blockX, blockY);
	
	if (blockPtr != nullptr && blockPtr->type != 0)
	{
		Block block = *blockPtr;
		
		// distance between ray start point and block
		// Formular: sqrt( (x2 - x1)^2 + (y2 - y1)^2 )
		double fishEyeLength = std::sqrt( std::pow(rayStartX - collision.x, 2) + pow(rayStartY - collision.y, 2) );
		
		// -fov/2 for the most left ray, 0 for the center and fov/2 for the most right ray. (After that convert from degress to radiens)
		double angleOffset = (((double)fov / 2) / (screenWidth) * offsetX) * M_PI / 180;
		// because the screen is flat, every line would appear distorted (fish eye effect)
		// to fix this, the length of each ray has to be corrected, by multiplying it with cos(angleOffset)
		double length = fishEyeLength * std::cos(angleOffset);
		
		double height = block.zHeight;
		height = height / (length);
			
		if (block.type == 1)
		{
			if (side == 1 || side == 3)
				this->engine.setColor("lightblue");
			else
				this->engine.setColor("blue");
		}
		else
		{
			if (side == 1 || side == 3)
				this->engine.setColor("lightgreen");
			else
				this->engine.setColor("green");
		}
		
		double drawX =  screenWidth / 2 + offsetX;
		double drawY = screenHeight - height;
		
		// player Z position (move closer blocks faster than others)
		drawY += this->player->getZ() * (1/length);
		// viewing angle (moving everything down, appears like looking up
		drawY -= this->player->getZAngle();
		// block offset from ground
		drawY -= block.zOffset* (1/length);

		
		//x coordinate on the texture
		int textureX = -1;

		if (side == 3)
			textureX = (collision.x - blockX) * textureWidth;
		else if (side == 1)
			textureX = textureWidth - (collision.x - blockX) * textureWidth;
		else if (side == 2)
			textureX = (collision.y - blockY) * textureWidth;
		else if (side == 4)
			textureX = textureWidth - (collision.y - blockY) * textureWidth;

			
		double textureY = 0;
		double textureW = 1;
		double textureH = textureHeight;
		
		//this->engine.fillRect(screenWidth / 2 + offsetX, drawY, 1, height);
		this->engine.drawImage(this->textures[block.type], textureX, textureY, textureW, textureH, drawX, drawY, 1, height);
	}
}
