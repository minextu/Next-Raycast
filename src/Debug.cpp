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
				block->zHeight += delta(10);
			else if (debugMode == 3)
				block->zOffset += delta(10);
		}
		if (mouse.right)
		{
			if(debugMode == 1)
			{
				block->type--;
				mouse.right = false;
			}
			else if (debugMode == 2)
				block->zHeight -= delta(10);
			else if (debugMode == 3)
				block->zOffset-= delta(10);
		}
	}
			
	if (keyboard.one)
		debugMode = 1;
	if (keyboard.two)
		debugMode = 2;
	if (keyboard.three)
		debugMode = 3;	
}

Map benchmarkMap;
void initBenchmarkMap()
{
	std::vector<Block> mapArr  = 
	{
				{2,9999},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2,9999},
				{2},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{2},
				{2},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{2},
				{2},{0},{4,900,0},{4,900,100},{4,900,200},{4,900,300},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{2},
				{2},{0},{0},{0},{0},{1},{1},{1},{1},{1},{1},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{2},
				{2},{0},{0},{0},{0},{1},{0},{0},{0},{0},{1},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{2},
				{2},{0},{0},{0},{0},{1},{0},{1},{1},{0},{1},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{2},
				{2},{0},{0},{0},{0},{1},{0},{1},{1},{0},{1},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{2},
				{2},{0},{0},{0},{0},{3},{0},{1},{1},{0},{1},{0},{0},{0},{0},{6},{6},{6},{6},{0},{0},{0},{0},{0},{3,100,0},{2},
				{2},{0},{0},{0},{0},{3},{0},{1},{1},{0},{1},{0},{0},{0},{0},{6},{5,900,600},{6},{0},{0},{0},{0},{0},{0},{3,100,100},{2},
				{2},{0},{0},{0},{0},{3},{0},{3},{0},{0},{0},{0},{0},{0},{0},{6},{6},{6},{6},{0},{0},{0},{0},{0},{3,100,200},{2},
				{2},{0},{0},{0},{0},{3},{0},{3},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{3,100,300},{2},
				{2},{0},{0},{0},{0},{3},{0},{3},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{3,100,400},{2},
				{2},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{3,100,500},{2},
				{2},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{3,100,600},{2},
				{2},{0},{0},{0},{0},{0},{0},{0},{7,600},{7,800},{7,900},{7,1000},{7,1100},{7,1200},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{3,100,700},{2},
				{2},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{3,100,800},{2},
				{2},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{3,100,900},{2},
				{2},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{3,100,0},{3,100,100},{3,100,200},{3,100,300},{3,100,400},{3,100,500},{3,100,600},{3,100,700},{3,100,800},{3,100,900},{3,100,1000},{2},
				{2,99999},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2,9999}
	};
			
	benchmarkMap.set(mapArr);
	benchmarkMap.setSize(26, 20);
}

int isBenchmark = false;
int useBenchmarkMap = false;
int benchmarkState = 0;
Player* benchmarkPlayer = nullptr;

void checkBenchmark(Keyboard& keyboard, Player& player, Camera &camera)
{
	if (keyboard.nine)
	{
		useBenchmarkMap = !useBenchmarkMap;
		initBenchmarkMap();
		keyboard.nine = false;
		player.setX(2);
		player.setY(2);
		player.setZ(400);
		player.setAngle(0);
		player.setZAngle(0);
	}
	if (keyboard.zero)
	{
		if (!isBenchmark)
		{
			delete benchmarkPlayer;
			benchmarkPlayer = new Player();
			benchmarkState = 0;
			
			initBenchmarkMap();
			
			camera.setPlayer(benchmarkPlayer);
			
			useBenchmarkMap = true;
		}
		else
		{
			camera.setPlayer(&player);
			useBenchmarkMap = false;
		}
		
		isBenchmark = !isBenchmark;
		keyboard.zero = false;
	}
	
	if (isBenchmark)
	{
		if (benchmarkState == 0)
		{
			if (benchmarkPlayer->getZAngle() < 250)
			{
				benchmarkPlayer->setZ(benchmarkPlayer->getZ()+delta(4));
				benchmarkPlayer->setZAngle(benchmarkPlayer->getZAngle()+delta(1));
			}
			else
				benchmarkState++;
		}
		else if (benchmarkState == 1)
		{
			if (benchmarkPlayer->getAngle() < 0.6)
				benchmarkPlayer->setAngle(benchmarkPlayer->getAngle()+delta(0.05));
			else
				benchmarkState++;
		}
		else if (benchmarkState == 2)
		{
			if (benchmarkPlayer->getY() <= 15)
				benchmarkPlayer->moveForward();
			else
				benchmarkState++;
		}
		else if (benchmarkPlayer->getAngle() > -2.5)
			benchmarkPlayer->setAngle(benchmarkPlayer->getAngle()-delta(0.05));
		else if (benchmarkPlayer->getX() >= 1)
			benchmarkPlayer->moveForward();
		else
			benchmarkState = 0;
	}
}
