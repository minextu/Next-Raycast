#include <iostream>
#include "NextEngine.cpp"
#include "NextImage.cpp"

#include "Player.cpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
Player player;

int startLoop(NextEngine, NextImage);
int game(NextEngine, bool[], int);

int main()
{
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
	player.checkKeys(keys);
	player.draw(engine);
}
