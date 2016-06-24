#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <iostream>
#include <vector>
#include <algorithm>

#include "NextEngine.cpp"
#include "NextImage.cpp"

#include "Keyboard.cpp"
#include "Mouse.cpp"

int screenWidth = 1024;
int screenHeight = 768;

int const textureWidth = 1024;
int const textureHeight = 1024;

double delta(double x);

#include "Map/Map.cpp"
#include "Player.cpp"
#include "Camera/Camera.cpp"

#include "Debug.cpp"


double currentFps = 999900;
double delta(double x)
{
	x = x / currentFps * 60.0;
}

int startLoop(NextEngine&, NextImage[]);
int game(NextEngine&, int, NextImage[]);


// loads textures and fonts to vram
int textureNum = 9;
void loadMedia(NextEngine& engine, NextImage images[])
{	
	const std::string resPath = getResourcePath();
	std::string imageName;
	
	images[0].setEngine(engine);
	images[0].load(resPath + "textures/block_overlay.png");
	
	for (int i = 1; i <= textureNum; i++)
	{
		images[i].setEngine(engine);
		imageName = "textures/texture" + std::to_string(i) + ".png";
		images[i].load(resPath + imageName);
	}
	
	engine.setFont(resPath + "fonts/OpenSans-Regular.ttf", 22);
}

Map map;
Player player;
Camera camera;
void newGame(NextEngine& engine, NextImage images[])
{
	std::vector<Block> mapArr  = 
	{
		{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},
		{2},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{2},
		{2},{0},{0},{0},{0},{0},{0},{1},{0},{0},{0},{0},{2},
		{2},{0},{0},{0},{0},{0},{0},{1},{0},{0},{0},{0},{2},
		{2},{0},{0},{0},{1},{0},{0},{0},{2},{0},{0},{0},{2},
		{2},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{2},
		{2},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{2},
		{2},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{2},
		{2},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{2},
		{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2},{2}
	};


	
	map.set(mapArr);
	map.setSize(13,10);
	
	//player
	
	camera.setEngine(engine);
	camera.setPlayer(&player);
	camera.setTextures(images);
}

int main()
{	
	NextEngine engine;
	
	char title[] = "Game Test";
	engine.createWindow(title, screenWidth, screenHeight);
	
	SDL_SetRelativeMouseMode(SDL_TRUE);
	
	NextImage images[textureNum+1];
	loadMedia(engine, images);
	
	newGame(engine, images);
	startLoop(engine, images);
	
	return 0;
}

Keyboard keyboard;
Mouse mouse;

void checkEvents(SDL_Event e, bool* quit)
{
	while (SDL_PollEvent(&e))
	{
		// close button
		if (e.type == SDL_QUIT)
			*quit = true;
		// esc key or focus lost
		else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE || e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
		{
			mouse.enabled = false;
			SDL_SetRelativeMouseMode(SDL_FALSE);
			mouse.x = 0;
			mouse.y = 0;
		}
		// clicked inside the window when previously lost focus
		else if (!mouse.enabled && e.type == SDL_MOUSEBUTTONDOWN)
		{
			mouse.enabled = true;
			SDL_GetRelativeMouseState(&mouse.x, &mouse.y);
			SDL_SetRelativeMouseMode(SDL_TRUE);
		}
		// resized window
		else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED)
		{
			screenWidth = e.window.data1;
			screenHeight = e.window.data2;
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
		{
			bool down = false;
			if (e.type == SDL_MOUSEBUTTONDOWN)
				down = true;
			
			if (e.button.button == SDL_BUTTON_LEFT)
				mouse.left = down;
			else if (e.button.button == SDL_BUTTON_RIGHT)
				mouse.right = down;
		}
		// other keys (will be handled by struct Keyboard
		else
			keyboard.getKey(e);

	}	
}

int startLoop(NextEngine& engine, NextImage images[])
{
	bool quit = false;
	
	long double lastTime = SDL_GetTicks();
	long double currentTime = SDL_GetTicks();
	
	std::cout << currentTime - lastTime;
	
	int loopNum = 0;
	SDL_Event e;
	while (!quit)
	{
		loopNum++;
		
		checkEvents(e, &quit);
		if (mouse.enabled)
			SDL_GetRelativeMouseState(&mouse.x, &mouse.y);
		
		engine.clear();
		
		// main game loop
		game(engine, loopNum, images);
		
		// show fps
		engine.setColor("white");
		engine.fillText("FPS:" + std::to_string(currentFps), 0,screenHeight - 40);
		
		// draw all changes
		engine.render();
		
		// calculate FPS
		currentTime = SDL_GetTicks();
		currentFps = 1000 / (currentTime - lastTime);
		
		lastTime = currentTime;
	}
}

int game(NextEngine& engine, int loopNum, NextImage images[])
{
	//debug
	engine.setColor("gray");
	engine.fillRect(0, 0, debugSize*13,debugSize*10);
	
		
	
	// debug: handle Benchmark
	checkBenchmark(keyboard, player, camera);
	
	if (!useBenchmarkMap)
		camera.renderMap(map);
	else
		camera.renderMap(benchmarkMap);
	
	if (!isBenchmark)
	{
		if (keyboard.up)
			player.moveForward();
		if (keyboard.down)
			player.moveBackward();
		if (keyboard.left)
			player.moveLeft();
		if (keyboard.right)
			player.moveRight();
		if (keyboard.h)
			player.moveUp();
		if (keyboard.n)
			player.moveDown();
		
		player.rotate(mouse.x*0.001);
		player.addZAngle(mouse.y);
	}
	
	// debug: resize, change texture of block
	modifyMap(mouse, keyboard, player, map);
	
	// debug: show map
	drawGrid(engine,map);
	drawMap(engine,map);
}
