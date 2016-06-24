int debugSize = 20;

//display grid
void drawGrid(NextEngine& engine, Map& map)
{
	int drawX = 0;
	int drawY = 0;
	bool end = false;
	while (!end) 
	{
		engine.setColor("white");
		engine.strokeRect(drawX*debugSize, drawY*debugSize, debugSize,debugSize);
		drawX++;
		
		if (drawX >= map.getWidth())
		{
			drawX = 0;
			drawY ++;
		}
		if (drawY >= map.getHeight())
			end = true;
	}
}

// go through the map array and mark all blocks blue
void drawMap(NextEngine& engine, Map& map)
{
	for (int x = 0; x < map.getWidth(); x++)
	{
		for (int y = 0; y < map.getHeight(); y++)
		{
			Block block = *map.getBlock(x,y);
			if (block.type != 0)
			{
				if (block.type == 1)
					engine.setColor("blue");
				else
					engine.setColor("green");
				
				engine.fillRect(x*debugSize, y*debugSize, debugSize, debugSize);
			}
		}
	}
}

// display circles around every border point
void drawBorderCrossCircle(NextEngine& engine, std::vector<RayCollision> collisions)
{
	int size  = 0.3 * debugSize;
	for (int i = 0; i < collisions.size(); i++)
	{
		double posX = collisions[i].x;
		double posY = collisions[i].y;
		
		engine.setColor("red");
		engine.strokeRect(posX*debugSize - (double)size / 2,posY*debugSize - (double)size / 2,size,size);
	}
}
