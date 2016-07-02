#include "Ray.cpp"
#include "Line.cpp"
#include "Camera.h"

//gpu sound
//int quality = 20;
int quality = 1;
bool drawFloor = false;

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

void Camera::drawBackground()
{
	if (!drawFloor)
	{
		int skyHeight = screenHeight - this->player->getZAngle();
		int groundX = skyHeight;
		int groundHeight = screenHeight - skyHeight;
		
		engine.setColor(0,0,0);
		engine.fillRect(0, 0, screenWidth, skyHeight);
		engine.setColor(30,30,30);
		engine.fillRect(0, groundX, screenWidth, groundHeight);
	}
	else
	{
		engine.setColor(0,0,0);
		engine.fillRect(0, 0, screenWidth, screenHeight);
	}
}
void Camera::renderMap(Map& map)
{	
	// adapt fov to current screen ratio
	this->currentFov = (double)this->fov / (4.0/3) * ((double)screenWidth/screenHeight);
	
	double rayStartX = this->player->getX();
	double rayStartY = this->player->getY();
	double startAngle = this->player->getAngle();
	
	// send rays in screenWidth different directions (going left and right from center)
	for (int screenX = 0; screenX <= screenWidth / 2; screenX+=quality)
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

int lastX;
int lastY;
int lastHeight;
int lastWidth;

void Camera::renderRay(Map& map, double rayStartX, double rayStartY, int screenX, double rayAngle)
{
	lastX = 0;
	lastY = 0;
	lastHeight = 0;
	lastWidth = 0;
	
	// Detect every possible border the line crosses
	Ray ray(rayStartX, rayStartY, rayAngle);
	ray.setMapSize(map.getWidth(), map.getHeight());
	std::vector<RayCollision> collisions = ray.send();
	
	// debug
	if (screenX == 0 || screenX == screenWidth / 2 || screenX == -screenWidth / quality)
		drawBorderCrossCircle(this->engine, collisions);
	
	//this->engine.setTargetTexture();
	//this->engine.clear();
	//this->engine.clearTarget();
	
	std::vector<Line> blockLines;
	for (int i = 0; i < collisions.size(); ++i)
	{
		int blockX = (int)(collisions[i].x);
		int blockY = (int)(collisions[i].y);
		
		
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
	
	this->renderFloor(blockLines);
	
	
	// render blocks further away first, so closer ones can overwrite them (if jumping would not be allowed, only the first block has to be rendered)
	std::reverse(blockLines.begin(), blockLines.end());
	this->renderBlockLines(blockLines, screenX);
}

void Camera::generateBlockLine(std::vector<Line> &blockLines, int blockX, int blockY, RayCollision collision, double rayStartX, double rayStartY, Map& map, double offsetX, int side)
{
	Block *blockPtr = map.getBlock(blockX, blockY);
	
	if (blockPtr != nullptr && blockPtr->type != 0)
	{
		Block block = *blockPtr;
		
		// distance between ray start point and block
		// Formular: sqrt( (x2 - x1)^2 + (y2 - y1)^2 )
		double fishEyeLength = sqrt( pow(rayStartX - collision.x, 2) + pow(rayStartY - collision.y, 2) );
		
		// -fov/2 for the most left ray, 0 for the center and fov/2 for the most right ray. (After that convert from degress to radiens)
		double angleOffset = ((this->currentFov / 2) / (screenWidth) * offsetX) * M_PI / 180;
		// because the screen is flat, every line would appear distorted (fish eye effect)
		// to fix this, the length of each ray has to be corrected, by multiplying it with cos(angleOffset)
		double length = fishEyeLength * cos(angleOffset);
		
		double height = block.zHeight / 768.0 * screenHeight;
		height = height / (length);
		double width = quality;
		
		double drawX =  screenWidth / 2 + offsetX;
		double drawY = screenHeight - height;
		int tHeight = textureHeight;
		
		// player Z position (move closer blocks faster than others)
		drawY += this->player->getZ() / 768.0 * screenHeight * (1/length);
		// viewing angle (moving everything down, appears like looking up
		drawY -= this->player->getZAngle();
		// block offset from ground
		drawY -= block.zOffset* (1/length);

		// skip drawing if block is not in screen
		//if (drawY + height < 0 || drawX + width < 0 || drawY > screenHeight || drawX > screenWidth)
		//	return;
		// skip drawing if block is completly covered by the previous block
		if (lastX == drawX && lastY <= 0 && lastHeight >= screenHeight
				|| lastX == drawX && lastY <= drawY && lastY + lastHeight >= drawY + height) // causes bugs when standing under a block
			return;
			
		// only draw part of block if not complety covered by the previous block
		else if (drawX == lastX && drawY + height >= lastY && drawY <= lastY + lastHeight && drawY <= lastY &&
				 !(drawY < lastY && drawY + height > lastY + lastHeight))// also check if the last block was bigger than the last
		{
			double newHeight = lastY - drawY;
			tHeight = textureHeight / height * newHeight;
				
			height = newHeight;
			lastHeight = height + lastHeight;
			lastY = drawY;
		}
		else
		{
			lastX = drawX;
			lastY = drawY;
			lastWidth = width;
			lastHeight = height;
		}
		
		// save all proberties in a line object
		Line line;
		line.drawX = drawX;
		line.drawY = drawY;
		line.width = width;
		line.height = height;
		line.length = length;
			
		line.blockX = blockX;
		line.blockY = blockY;
		line.side = side;
		line.collision = collision;
		line.textureNum = block.type;
		line.zOffset = block.zOffset* (1/length);
		
		line.textureHeight = tHeight;
			
		blockLines.push_back(line);
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
		double textureW = (double) textureWidth / (screenWidth / 2 / quality);
		double textureH = line.textureHeight;
			
		//this->engine.fillRect(screenWidth / 2 + offsetX, line.drawY, 1, line.height);
		this->engine.drawImage(this->textures[line.textureNum], textureX, textureY, textureW, textureH, line.drawX, line.drawY, line.width, line.height);
		
		// floor reflection
		this->engine.setAlpha(5);
		this->engine.drawImage(this->textures[line.textureNum], textureX, textureY, textureW, textureH, line.drawX, line.drawY + line.height + 2*line.zOffset, line.width, line.height);
		this->engine.setAlpha(255);
	}
}

void Camera::renderFloor(std::vector<Line> &blockLines)
{
	if (drawFloor)
	{
		for (int i = 0; i < blockLines.size(); i++)
		{
			Line line = blockLines[i];
			int drawX = line.drawX;
			int drawY = line.drawY + line.height;
			int width = line.width;
			int height = screenHeight - drawY;

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
			
			int textureY = 0;
			int textureW = 1;
			int textureH = textureHeight;
			
			//this->engine.drawImage(this->textures[line.textureNum], textureX, textureY, textureW, textureH, drawX, drawY, width, height);
			engine.setColor(30,30,30);
			engine.fillRect(drawX, drawY, width, height);
		}
	}
}
/*void Camera::renderFloor(Line &line)
{
	//FLOOR CASTING
	double floorXWall, floorYWall; //x, y position of the floor texel at the bottom of the wall
	
	//4 different wall directions possible
	//if(side == 0 && rayDirX > 0)
	//{
		floorXWall = line.blockX;
		floorYWall = line.blockY;
	//}
	/*else if(side == 0 && rayDirX < 0)
	{
		floorXWall = mapX + 1.0;
		floorYWall = mapY + wallX;
	}
	else if(side == 1 && rayDirY > 0)
	{
		floorXWall = mapX + wallX;
		floorYWall = mapY;
	}
	else
	{
		floorXWall = mapX + wallX;
		floorYWall = mapY + 1.0;
	}*/
	
	/*double distWall, distPlayer, currentDist;
	
	distWall = perpWallDist;
	distPlayer = 0.0;

	//draw the floor from drawEnd to the bottom of the screen
	for(int y = line.drawY + line.height + 1; y < screenHeight; y++)
	{
		currentDist = h / (2.0 * y - h); //you could make a small lookup table for this instead
		double weight = (currentDist - distPlayer) / (distWall - distPlayer);
		
		double currentFloorX = weight * floorXWall + (1.0 - weight) * posX;
		double currentFloorY = weight * floorYWall + (1.0 - weight) * posY;

		int floorTexX, floorTexY;
		floorTexX = int(currentFloorX * texWidth) % texWidth;
		floorTexY = int(currentFloorY * texHeight) % texHeight;

        //floor
        buffer[y][x] = (texture[3][texWidth * floorTexY + floorTexX] >> 1) & 8355711;
      }	
}*/
