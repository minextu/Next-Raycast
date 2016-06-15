const int MAP_WIDTH = 24;

int convert(int x, int y)
{
	int newX = y*MAP_WIDTH + x;
	return newX;
}

double currentFps = 999900;
double delta(double x)
{
	x = x / currentFps * 60.0;
}

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
void rotateDir(double &x, double &y, int degrees)
{
	double rot = degrees * (double)M_PI/180;
	
	double oldX = x;
	x = x * cos(rot) - y * sin(rot);
	y = oldX * sin(rot) + y * cos(rot);
}

#include <iostream>
#include "NextEngine.cpp"
#include "NextImage.cpp"

#include "Camera.cpp"
#include "Player.cpp"
#include "Map.cpp"


const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

int startLoop(NextEngine, NextImage[]);
int game(NextEngine, bool[], int, NextImage[]);


int worldMap[]=
{
	4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7,
	4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7,
	4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,
	4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,
	4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7,
	4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7,
	4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1,
	4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8,
	4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1,
	4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8,
	4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1,
	4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1,
	6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6,
	8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
	6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6,
	4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3,
	4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2,
	4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2,
	4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2,
	4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2,
	4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2,
	4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2,
	4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2,
	4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3
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
	
	SDL_SetRelativeMouseMode(SDL_TRUE);
	
	
	const std::string resPath = getResourcePath();
	
	int textureNum = 8;
	std::string imageName;
	NextImage images[textureNum+1];
	images[0].setEngine(engine);
	images[0].load(resPath + "block_overlay.png");
	
	for (int i = 1; i <= textureNum; i++)
	{
		images[i].setEngine(engine);
		
		imageName = "texture" + std::to_string(i) + ".png";
		images[i].load(resPath + imageName);
	}
	
	startLoop(engine, images);
	
	return 0;
}
bool disableMouse = false;
int mouseX, mouseY;
void checkEvents(SDL_Event e, bool* quit, bool keys[])
{
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
			*quit = true;
		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE || e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
		{
			disableMouse = true;
			SDL_SetRelativeMouseMode(SDL_FALSE);
			mouseX = 0;
			mouseY = 0;
		}
		
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			disableMouse = false;
			SDL_GetRelativeMouseState(&mouseX, &mouseY);
			SDL_SetRelativeMouseMode(SDL_TRUE);
		}
		
		if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
		{
			bool down = false;
			if (e.type == SDL_KEYDOWN)
				down = true;
			
			switch (e.key.keysym.sym)
			{
				case SDLK_LEFT:
					keys[0] = down;
					break;
				case SDLK_a:
					keys[0] = down;
					break;
				case SDLK_RIGHT:
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
				case SDLK_h:
					keys[4] = down;
					break;
				case SDLK_n:
					keys[5] = down;
					break;
			}
		}
	}	
}

int startLoop(NextEngine engine, NextImage images[])
{
	bool quit = false;
	
	long double lastTime = SDL_GetTicks();
	long double currentTime = SDL_GetTicks();
	
	std::cout << currentTime - lastTime;
	
	int loopNum = 0;
	bool keys[] = { false, false, false, false };
	SDL_Event e;
	while (!quit)
	{
		loopNum++;
		engine.clear();
		
		checkEvents(e, &quit, keys);
		if (!disableMouse)
			SDL_GetRelativeMouseState(&mouseX, &mouseY);
		
		game(engine, keys, loopNum, images);
		
		engine.render();
		
		currentTime = SDL_GetTicks();
		currentFps = 1000 / (currentTime - lastTime);
		
		lastTime = currentTime;

		
		if (loopNum % 100 == 0)
			std::cout << currentFps << ": " << delta(currentFps) << ",";
	}
}
int game(NextEngine engine, bool keys[], int loopNum, NextImage images[])
{
	camera.calculateRay(engine, worldMap, player, images);
	map.render2d(engine, worldMap, player);
	
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
		player.moveRight(worldMap);
	}
	//rotate to the left
	if (keys[1])
	{
		player.moveLeft(worldMap);
	}
	
	if (keys[4])
		camera.angle += delta(3);
	if (keys[5])
		camera.angle -= delta(3);
	
	// rotate on mouse move (left and right)
	player.rotate(camera, (double)(-mouseX) * 0.001);
	
	camera.angle += (double) (-mouseY);
	
	if (camera.angle > 1000)
		camera.angle = 1000;
	
	if (camera.angle < -1000)
		camera.angle = -1000;

}
