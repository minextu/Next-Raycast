#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifdef EMSCRIPTEN
#include <emscripten.h>
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
	return x;
}

int startLoop(NextEngine&, NextImage[]);


// loads textures and fonts to ram
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
	map.setSize(13,10);
	
	/*std::vector<Block> mapArr =
	{
		{8},{8},{8},{8},{8},{8},{8},{8},{8},{8},{8},{4},{4},{6},{4},{4},{6},{4},{6},{4},{4},{4},{6},{4},
		{8},{0},{0},{0},{0},{0},{0},{0},{0},{0},{8},{4},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{4},
		{8},{0},{3},{3},{0},{0},{0},{0},{0},{8},{8},{4},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{6},
		{8},{0},{0},{3},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{6},
		{8},{0},{3},{3},{0},{0},{0},{0},{0},{8},{8},{4},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{4},
		{8},{0},{0},{0},{0},{0},{0},{0},{0},{0},{8},{4},{0},{0},{0},{0},{0},{6},{6},{6},{0},{6},{4},{6},
		{8},{8},{8},{8},{0},{8},{8},{8},{8},{8},{8},{4},{4},{4},{4},{4},{4},{6},{0},{0},{0},{0},{0},{6},
		{7},{7},{7},{7},{0},{7},{7},{7},{7},{0},{8},{0},{8},{0},{8},{0},{8},{4},{0},{4},{0},{6},{0},{6},
		{7},{7},{0},{0},{0},{0},{0},{0},{7},{8},{0},{8},{0},{8},{0},{8},{8},{6},{0},{0},{0},{0},{0},{6},
		{7},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{8},{6},{0},{0},{0},{0},{0},{4},
		{7},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{8},{6},{0},{6},{0},{6},{0},{6},
		{7},{7},{0},{0},{0},{0},{0},{0},{7},{8},{0},{8},{0},{8},{0},{8},{8},{6},{4},{6},{0},{6},{6},{6},
		{7},{7},{7},{7},{0},{7},{7},{7},{7},{8},{8},{4},{0},{6},{8},{4},{8},{3},{3},{3},{0},{3},{3},{3},
		{2},{2},{2},{2},{0},{2},{2},{2},{2},{4},{6},{4},{0},{0},{6},{0},{6},{3},{0},{0},{0},{0},{0},{3},
		{2},{2},{0},{0},{0},{0},{0},{2},{2},{4},{0},{0},{0},{0},{0},{0},{4},{3},{0},{0},{0},{0},{0},{3},
		{2},{0},{0},{0},{0},{0},{0},{0},{2},{4},{0},{0},{0},{0},{0},{0},{4},{3},{0},{0},{0},{0},{0},{3},
		{1},{0},{0},{0},{0},{0},{0},{0},{1},{4},{4},{4},{4},{4},{6},{0},{6},{3},{3},{0},{0},{0},{3},{3},
		{2},{0},{0},{0},{0},{0},{0},{0},{2},{2},{2},{1},{2},{2},{2},{6},{6},{0},{0},{5},{0},{5},{0},{5},
		{2},{2},{0},{0},{0},{0},{0},{2},{2},{2},{0},{0},{0},{2},{2},{0},{5},{0},{5},{0},{0},{0},{5},{5},
		{2},{0},{0},{0},{0},{0},{0},{0},{2},{0},{0},{0},{0},{0},{2},{5},{0},{5},{0},{5},{0},{5},{0},{5},
		{1},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{5},
		{2},{0},{0},{0},{0},{0},{0},{0},{2},{0},{0},{0},{0},{0},{2},{5},{0},{5},{0},{5},{0},{5},{0},{5},
		{2},{2},{0},{0},{0},{0},{0},{2},{2},{2},{0},{0},{0},{2},{2},{0},{5},{0},{5},{0},{0},{0},{5},{5},
		{2},{2},{2},{2},{1},{2},{2},{2},{2},{2},{2},{1},{2},{2},{2},{5},{5},{5},{5},{5},{5},{5},{5},{5}
	};
	map.setSize(24,24);*/

	
	map.set(mapArr);
	
	//player
	
	camera.setEngine(engine);
	camera.setPlayer(&player);
	camera.setTextures(images);
}

NextEngine engine;
NextImage *images;

int main()
{	
	
	char title[] = "Game Test";
	engine.createWindow(title, screenWidth, screenHeight);
	
	SDL_SetRelativeMouseMode(SDL_TRUE);
	
	images = new NextImage[textureNum+1];
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
			engine.updateSize(screenWidth, screenHeight);
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
int loopNum = 0;
bool quit = false;
SDL_Event e;
long double lastTime;
long double currentTime = 0;

void loop()
{
	loopNum++;
		
	checkEvents(e, &quit);
	if (mouse.enabled)
		SDL_GetRelativeMouseState(&mouse.x, &mouse.y);
		
	engine.clear();
		
	camera.drawBackground();
	
	// main game loop
	//debug
	engine.setColor("gray");
	engine.fillRect(0, 0, debugSize*map.getWidth(),debugSize*map.getHeight());
	
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
		if (keyboard.space)
			player.moveUp();
		if (keyboard.shift)
			player.moveDown();
		
		player.rotate(mouse.x*0.001);
		player.addZAngle(mouse.y);
	}
	
	// debug: resize, change texture of block
	modifyMap(mouse, keyboard, player, map);
	
	// debug: show map
	drawGrid(engine,map);
	drawMap(engine,map);
	
	// debug: switch quality key
	if (keyboard.q)
	{
		quality++;
		keyboard.q = false;
	}
	if (keyboard.y)
	{
		quality--;
		if (quality < 1)
			quality = 1;
		keyboard.y = false;
	}
	if (keyboard.x)
	{
		drawFloor = !drawFloor;
		keyboard.x = false;
	}
		
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

int startLoop(NextEngine& engine, NextImage images[])
{

	#ifdef EMSCRIPTEN
	emscripten_set_main_loop(loop, 0, 1);
	#else
	while (!quit) 
	{
		loop();
	}
	#endif
}

int game()
{
}
