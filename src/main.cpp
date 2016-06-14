const int MAP_WIDTH = 24;

int convert(int x, int y)
{
	int newX = y*MAP_WIDTH + x;
	return newX;
}

#include <iostream>
#include "NextEngine.cpp"
#include "NextImage.cpp"

#include "Map.cpp"
#include "Camera.cpp"
#include "Player.cpp"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

int startLoop(NextEngine, NextImage);
int game(NextEngine, bool[], int);


int worldMap[]=
{
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1,
	1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1,
	1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};
Map map;
Player player;
Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT);


int main()
{
	map.setSize(24,24);
	map.set(worldMap);
	
	int status;
	
	NextEngine engine;
	engine.createWindow("test", SCREEN_WIDTH, SCREEN_HEIGHT);
	
	const std::string resPath = getResourcePath();
	
	NextImage image(engine);
	image.load(resPath + "image.bmp");
	
	startLoop(engine, image);
	
	return 0;
}

void checkEvents(SDL_Event e, bool* quit, bool keys[])
{
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
			*quit = true;
		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
			*quit = true;
		
		if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
		{
			bool down = false;
			if (e.type == SDL_KEYDOWN)
				down = true;
			
			switch (e.key.keysym.sym)
			{
				case SDLK_RIGHT:
					keys[0] = down;
					break;
				case SDLK_a:
					keys[0] = down;
					break;
				case SDLK_LEFT:
					keys[1] = down;
					break;
				case SDLK_d:
					keys[1] = down;
					break;
				case SDLK_DOWN:
					keys[2] = down;
					break;
				case SDLK_s:
					keys[2] = down;
					break;
				case SDLK_UP:
					keys[3] = down;
					break;
				case SDLK_w:
					keys[3] = down;
					break;
			}
		}
	}	
}

int startLoop(NextEngine engine, NextImage image)
{
	bool quit = false;
	int loopNum = 0;
	bool keys[] = { false, false, false, false };
	
	SDL_Event e;
	while (!quit)
	{
		loopNum++;
		engine.clear();
		
		checkEvents(e, &quit, keys);
		
		game(engine, keys, loopNum);
		
		engine.render();
	}
}

int game(NextEngine engine, bool keys[], int loopNum)
{
	camera.calculateRay(engine, worldMap, player);
	
	//move forward
    if (keys[3])
    {
		player.moveForward(worldMap);
	}
	//move backwards
	if (keys[2])
	{
		player.moveBackward(worldMap);
	}
	//rotate to the right
	if (keys[0])
	{
		player.rotate(camera, false);
	}
	//rotate to the left
	if (keys[1])
	{
		player.rotate(camera, true);
	}
}
