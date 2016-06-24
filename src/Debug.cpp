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

int debugMode = 1;
void modifyMap(Mouse& mouse, Keyboard& keyboard, Player& player, Map& map)
{
	if (mouse.right || mouse.left)
	{
		Ray ray(player.getX(), player.getY(), player.getAngle());
		ray.setMapSize(map.getWidth(), map.getHeight());
		std::vector<RayCollision> collisions = ray.send(true, true, &map);
		
		double blockX = std::floor(collisions[0].x);
		double blockY = std::floor(collisions[0].y);
		Block* block = map.getBlock(blockX,blockY);
		
		if (mouse.left)
		{
			if(debugMode == 1)
			{
				block->type++;
				mouse.left = false;
			}
			else if (debugMode == 2)
				block->zHeight += 10;
			else if (debugMode == 3)
				block->zOffset+=10;
		}
		if (mouse.right)
		{
			if(debugMode == 1)
			{
				block->type--;
				mouse.right = false;
			}
			else if (debugMode == 2)
				block->zHeight -= 10;
			else if (debugMode == 3)
				block->zOffset-=10;
		}
	}
			
	if (keyboard.one)
		debugMode = 1;
	if (keyboard.two)
		debugMode = 2;
	if (keyboard.three)
		debugMode = 3;	
}
