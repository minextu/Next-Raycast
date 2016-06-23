
#include "Player.h"
#include "Camera.h"

struct Sprite
{
  double x;
  double y;
  int texture;
};

#define numSprites 19

Sprite sprite[numSprites] =
{
  {20.5, 11.5, 10}, //green light in front of playerstart
  //green lights in every room
  {18.5,4.5, 10},
  {10.0,4.5, 10},
  {10.0,12.5,10},
  {3.5, 6.5, 10},
  {3.5, 20.5,10},
  {3.5, 14.5,10},
  {14.5,20.5,10},

  //row of pillars in front of wall: fisheye test
  {18.5, 10.5, 9},
  {18.5, 11.5, 9},
  {18.5, 12.5, 9},

  //some barrels around the map
  {21.5, 1.5, 8},
  {15.5, 1.5, 8},
  {16.0, 1.8, 8},
  {16.2, 1.2, 8},
  {3.5,  2.5, 8},
  {9.5, 15.5, 8},
  {10.0, 15.1,8},
  {10.5, 15.8,8},
};
//arrays used to sort the sprites
int spriteOrder[numSprites];
double spriteDistance[numSprites];

//function used to sort the sprites
//sort algorithm
void combSort(int* order, double* dist, int amount)
{
  int gap = amount;
  bool swapped = false;
  while(gap > 1 || swapped)
  {
    //shrink factor 1.3
    gap = (gap * 10) / 13;
    if(gap == 9 || gap == 10) gap = 11;
    if (gap < 1) gap = 1;
    swapped = false;
    for(int i = 0; i < amount - gap; i++)
    {
      int j = i + gap;
      if(dist[i] < dist[j])
      {
        std::swap(dist[i], dist[j]);
        std::swap(order[i], order[j]);
        swapped = true;
      }
    }
  }
}

#define texWidth 1024
#define texHeight 1024

Camera::Camera(int* w, int* h)
{
	this->w = w;
	this->h = h;
}

void Camera::calculateRay(NextEngine engine, int worldMap[], Player& player, NextImage images[])
{
	int w = *this->w;
	int h = *this->h;
	//1D Zbuffer
	double ZBuffer[w];
	
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
				
				double drawY = drawStart -player.posZ*(1/perpWallDist)*1;
				engine.drawImage(images[texNum], texX, 0, 1, texHeight, x, drawY, 1, drawEnd - drawStart);
				
				if (side == 1)
					engine.drawImage(images[0], texX, 0, 1, texHeight, x, drawY, 1, drawEnd - drawStart);
				
				
				
				//SET THE ZBUFFER FOR THE SPRITE CASTING
				ZBuffer[x] = perpWallDist; //perpendicular distance is used
				
				/*//FLOOR CASTING
				double floorXWall, floorYWall; //x, y position of the floor texel at the bottom of the wall
			
				//4 different wall directions possible
				if(side == 0 && rayDirX > 0)
				{
					floorXWall = mapX;
					floorYWall = mapY + wallX;
				}
				else if(side == 0 && rayDirX < 0)
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
				}

				double distWall, distPlayer, currentDist;

				distWall = perpWallDist;
				distPlayer = 0.0;

				if (drawEnd < 0) 
					drawEnd = h; //becomes < 0 when the integer overflows

				//draw the floor from drawEnd to the bottom of the screen
				for(int y = drawEnd + 1; y < h; y++)
				{
					currentDist = h / (2.0 * y - h); //you could make a small lookup table for this instead

					double weight = (currentDist - distPlayer) / (distWall - distPlayer);

					double currentFloorX = weight * floorXWall + (1.0 - weight) * posX;
					double currentFloorY = weight * floorYWall + (1.0 - weight) * posY;

					int floorTexX, floorTexY;
					floorTexX = int(currentFloorX * texWidth) % texWidth;
					floorTexY = int(currentFloorY * texHeight) % texHeight;

					//floor
					engine.drawImage(images[3], floorTexX, floorTexY, texWidth * floorTexY, 1, x, y, texWidth * floorTexY, 1);
					//buffer[y][x] = (texture[3][texWidth * floorTexY + floorTexX] >> 1) & 8355711;
					
					//ceiling (symmetrical!)
					engine.drawImage(images[5], floorTexX, floorTexY, texWidth * floorTexY, 1, x, h - y, texWidth * floorTexY, 1);
					//buffer[h - y][x] = texture[6][texWidth * floorTexY + floorTexX];
				}*/
			}
		} 
	}	
	//SPRITE CASTING
    //sort sprites from far to close
    for(int i = 0; i < numSprites; i++)
    {
      spriteOrder[i] = i;
      spriteDistance[i] = ((posX - sprite[i].x) * (posX - sprite[i].x) + (posY - sprite[i].y) * (posY - sprite[i].y)); //sqrt not taken, unneeded
    }
    combSort(spriteOrder, spriteDistance, numSprites);

    //after sorting the sprites, do the projection and draw them
    for(int i = 0; i < numSprites; i++)
    {
      //translate sprite position to relative to camera
      double spriteX = sprite[spriteOrder[i]].x - posX;
      double spriteY = sprite[spriteOrder[i]].y - posY;

      //transform sprite with the inverse camera matrix
      // [ planeX   dirX ] -1                                       [ dirY      -dirX ]
      // [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
      // [ planeY   dirY ]                                          [ -planeY  planeX ]

      double invDet = 1.0 / (planeX * dirY - dirX * planeY); //required for correct matrix multiplication

      double transformX = invDet * (dirY * spriteX - dirX * spriteY);
      double transformY = invDet * (-planeY * spriteX + planeX * spriteY); //this is actually the depth inside the screen, that what Z is in 3D

      int spriteScreenX = int((w / 2) * (1 + transformX / transformY));

      //calculate height of the sprite on screen
      int spriteHeight = abs(int(h / (transformY))); //using "transformY" instead of the real distance prevents fisheye
      //calculate lowest and highest pixel to fill in current stripe
      int drawStartY = -spriteHeight / 2 + h / 2;
      if(drawStartY < 0) drawStartY = 0;
      int drawEndY = spriteHeight / 2 + h / 2;
      if(drawEndY >= h) drawEndY = h - 1;

      //calculate width of the sprite
      int spriteWidth = abs( int (h / (transformY)));
      int drawStartX = -spriteWidth / 2 + spriteScreenX;
      if(drawStartX < 0) drawStartX = 0;
      int drawEndX = spriteWidth / 2 + spriteScreenX;
      if(drawEndX >= w) drawEndX = w - 1;

      //loop through every vertical stripe of the sprite on screen
      for(int stripe = drawStartX; stripe < drawEndX; stripe++)
      {
        int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;
        //the conditions in the if are:
        //1) it's in front of camera plane so you don't see things behind you
        //2) it's on the screen (left)
        //3) it's on the screen (right)
        //4) ZBuffer, with perpendicular distance
        if(transformY > 0 && stripe > 0 && stripe < w && transformY < ZBuffer[stripe])
		{
			double drawY = drawStartY + this->angle - player.posZ*(1/ZBuffer[stripe])*1;
			
			engine.drawImage(images[9], texX, 0, 1, texHeight, stripe, drawY, 1, drawEndY - drawStartY);
		}
      }
    }
}
