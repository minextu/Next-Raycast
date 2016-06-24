#include "Ray.cpp"
#include "Line.cpp"
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
	// adapt fov to current screen ratio
	this->currentFov = (double)this->fov / (4.0/3) * ((double)screenWidth/screenHeight);
	
	double rayStartX = this->player->getX();
	double rayStartY = this->player->getY();
	double startAngle = this->player->getAngle();
	
	// send rays in screenWidth different directions (going left and right from center)
	for (int screenX = 0; screenX <= screenWidth / 2; screenX++)
	{
		// rays should be send with linear angle difference until the fov is reached
		double rayDiff = (this->currentFov / 2) / screenWidth * screenX;
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
	
	std::vector<Line> blockLines;
	for (int i = 0; i < collisions.size(); ++i)
	{
		double blockX = std::floor(collisions[i].x);
		double blockY = std::floor(collisions[i].y);
		
		
		if (collisions[i].y == blockY)
		{
			// upper block border
			this->generateBlockLine(blockLines, blockX, blockY, collisions[i], rayStartX, rayStartY, map, screenX, 1);
			// bottom block border
			this->generateBlockLine(blockLines, blockX, blockY-1, collisions[i], rayStartX, rayStartY, map, screenX, 3);
			
		}
		else if (collisions[i].x == blockX)
		{
			// left block border
			this->generateBlockLine(blockLines, blockX, blockY, collisions[i], rayStartX, rayStartY, map, screenX, 2);
			// right block border
			this->generateBlockLine(blockLines, blockX-1, blockY, collisions[i], rayStartX, rayStartY, map, screenX, 4);
		}
	}
	// render blocks further away first, so closer ones can overwrite them (if jumping would not be allowed, only the first block has to be rendered)
	std::reverse(blockLines.begin(), blockLines.end());
	this->renderBlockLines(blockLines, screenX);
}
int lastX = 0;
int lastY = 0;
int lastHeight = 0;
int lastWidth = 0;

void Camera::generateBlockLine(std::vector<Line> &blockLines, double blockX, double blockY, RayCollision collision, double rayStartX, double rayStartY, Map& map, double offsetX, int side)
{
	Block *blockPtr = map.getBlock(blockX, blockY);
	
	if (blockPtr != nullptr && blockPtr->type != 0)
	{
		Block block = *blockPtr;
		
		// distance between ray start point and block
		// Formular: sqrt( (x2 - x1)^2 + (y2 - y1)^2 )
		double fishEyeLength = std::sqrt( std::pow(rayStartX - collision.x, 2) + pow(rayStartY - collision.y, 2) );
		
		// -fov/2 for the most left ray, 0 for the center and fov/2 for the most right ray. (After that convert from degress to radiens)
		double angleOffset = ((this->currentFov / 2) / (screenWidth) * offsetX) * M_PI / 180;
		// because the screen is flat, every line would appear distorted (fish eye effect)
		// to fix this, the length of each ray has to be corrected, by multiplying it with cos(angleOffset)
		double length = fishEyeLength * std::cos(angleOffset);
		
		double height = block.zHeight / 768.0 * screenHeight;
		height = height / (length);
		double width = 1;
		
		double drawX =  screenWidth / 2 + offsetX;
		double drawY = screenHeight - height;
		
		// player Z position (move closer blocks faster than others)
		drawY += this->player->getZ() / 768.0 * screenHeight * (1/length);
		// viewing angle (moving everything down, appears like looking up
		drawY -= this->player->getZAngle();
		// block offset from ground
		drawY -= block.zOffset* (1/length);

		// skip drawing if block is completly covered by the previous block
		if (drawX >= lastX && drawX + width <= lastX + lastWidth && drawY >= lastY && drawY + height <= lastY + lastHeight)
			return;
		else
		{
			// save all proberties in a line object
			Line line;
			line.drawX = drawX;
			line.drawY = drawY;
			line.width = width;
			line.height = height;
			
			line.blockX = blockX;
			line.blockY = blockY;
			line.side = side;
			line.collision = collision;
			line.textureNum = block.type;
			
			blockLines.push_back(line);
			
			lastX = drawX;
			lastY = drawY;
			lastWidth = width;
			lastHeight = height;
		}
	}
	
	
}

void Camera::renderBlockLines(std::vector<Line> &blockLines, int offsetX)
{
	for (int i = 0; i < blockLines.size(); i++)
	{
		Line line = blockLines[i];
		
		//x coordinate on the texture
		int textureX = -1;

		if (line.side == 3)
			textureX = (line.collision.x - line.blockX) * textureWidth;
		else if (line.side == 1)
			textureX = textureWidth - (line.collision.x - line.blockX) * textureWidth;
		else if (line.side == 2)
			textureX = (line.collision.y - line.blockY) * textureWidth;
		else if (line.side == 4)
			textureX = textureWidth - (line.collision.y - line.blockY) * textureWidth;

		// use darker texture for every second texture
		if (line.textureNum == 1)
		{
			if (line.side == 1 || line.side == 3)
				this->engine.setColor("lightblue");
			else
				this->engine.setColor("blue");
		}
		else
		{
			if (line.side == 1 || line.side == 3)
				this->engine.setColor("lightgreen");
			else
				this->engine.setColor("green");
		}
				
		double textureY = 0;
		double textureW = 1;
		double textureH = textureHeight;
			
		//this->engine.fillRect(screenWidth / 2 + offsetX, line.drawY, 1, line.height);
		this->engine.drawImage(this->textures[line.textureNum], textureX, textureY, textureW, textureH, line.drawX, line.drawY, line.width, line.height);
	}
}
