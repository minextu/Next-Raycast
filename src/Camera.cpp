#include "Player.h"
#include "Camera.h"

Camera::Camera(int w, int h)
{
	this->w = w;
	this->h = h;
}

void Camera::calculateRay(NextEngine engine, int worldMap[], Player& player)
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
		double rayDirX = dirX + planeX * cameraX;
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
			int drawStart = -lineHeight / 2 + h / 2;
			if(drawStart < 0)
				drawStart = 0;
			
			int drawEnd = lineHeight / 2 + h / 2;
			if(drawEnd >= h)
				drawEnd = h - 1;
			
			if (hit == 1)
			{
				//choose wall color
				std::string color;
				switch(worldMap[convert(mapX,mapY)])
				{
					case 1:  color = "red";  break;
					case 2:  color = "green";  break;
					case 3:  color = "blue";   break;
					case 4:  color = "gray";  break;
					default: color = "yellow"; break;
				}

				//give x and y sides different brightness
				if (side == 1) 
					color = "light" + color;
				engine.setColor(color);
				
				//draw the pixels of the stripe as a vertical line
				engine.drawLine(x, drawStart, x, drawEnd);
			}
		} 
	}
}
