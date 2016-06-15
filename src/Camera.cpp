#include "Player.h"
#include "Camera.h"

#define texWidth 64
#define texHeight 64

Camera::Camera(int w, int h)
{
	this->w = w;
	this->h = h;
}

void Camera::calculateRay(NextEngine engine, int worldMap[], Player& player, NextImage images[])
{
	int w = this->w;
	int h = this->h;
	
	double posX = player.posX;
	double posY = player.posY;
	double dirX = player.dirX;
	double dirY = player.dirY;
	
	double planeX = this->planeX;
	double planeY = this->planeY;
	
	for(int x = 0; x < w; x++)
    {
		//calculate ray position and direction
		double cameraX = 2 * x / double(w) - 1; //x-coordinate in camera space
		double rayPosX = posX;
		double rayPosY = posY;
		double rayDirX = dirX + planeX * cameraX; // add for zoom
		double rayDirY = dirY + planeY * cameraX;
		
		
		//which box of the map we're in
		int mapX = int(rayPosX);
		int mapY = int(rayPosY);
		
		//length of ray from current position to next x or y-side
		double sideDistX;
		double sideDistY;

		//length of ray from one x or y-side to next x or y-side
		double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
		double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
		double perpWallDist;

		//what direction to step in x or y-direction (either +1 or -1)
		int stepX;
		int stepY;

		int hit = 0; //was there a wall hit?
		int side; //was a NS or a EW wall hit?
		
		//calculate step and initial sideDist
		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (rayPosX - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
		}
		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (rayPosY - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
		}
		
		//perform DDA
		while (hit == 0)
		{
			//jump to next map square, OR in x-direction, OR in y-direction
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			//Check if ray has hit a wall
			if (worldMap[convert(mapX,mapY)] > 0) 
				hit = 1;
			
			//Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
			if (side == 0)
				perpWallDist = (mapX - rayPosX + (1 - stepX) / 2) / rayDirX;
			else          
				perpWallDist = (mapY - rayPosY + (1 - stepY) / 2) / rayDirY;
			
			//Calculate height of line to draw on screen
			int lineHeight = (int)(h / perpWallDist);

			//calculate lowest and highest pixel to fill in current stripe
			int drawStart = -lineHeight / 2 + h / 2 + angle;
			int drawEnd = lineHeight / 2 + h / 2 + angle;

			
			if (hit == 1)
			{
				//texturing calculations
				int texNum = worldMap[convert(mapX,mapY)];

				//calculate value of wallX
				double wallX; //where exactly the wall was hit
				if (side == 0) 
					wallX = rayPosY + perpWallDist * rayDirY;
				else          
					wallX = rayPosX + perpWallDist * rayDirX;
				
				wallX -= floor((wallX));
					
				//x coordinate on the texture
				int texX = int(wallX * double(texWidth));
				if(side == 0 && rayDirX > 0) 
					texX = texWidth - texX - 1;
				if(side == 1 && rayDirY < 0) 
					texX = texWidth - texX - 1;
				
				double drawStartZ = drawStart -player.posZ*(1/perpWallDist)*1;
				engine.drawImage(images[texNum], texX, 0, 1, texHeight, x, drawStartZ, 1, drawEnd - drawStart);
				
				if (side == 1)
					engine.drawImage(images[0], texX, 0, 1, texHeight, x, drawStartZ, 1, drawEnd - drawStart);
			}
		} 
	}
}
