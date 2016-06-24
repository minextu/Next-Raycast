#include "Keyboard.h"

void Keyboard::getKey(SDL_Event e)
{
	if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
	{
		bool down = false;
		if (e.type == SDL_KEYDOWN)
			down = true;
			
		switch (e.key.keysym.sym)
		{
			case SDLK_LEFT:
			case SDLK_a:
				this->left = down;
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				this->right = down;
				break;
			case SDLK_DOWN:
			case SDLK_s:
				this->down = down;
				break;
			case SDLK_UP:
			case SDLK_w:
				this->up = down;
				break;
			case SDLK_h:
				this->h = down;
				break;
			case SDLK_n:
				this->n = down;
				break;
			case SDLK_SPACE:
				this->space = down;
				break;
			case SDLK_1:
				this->one = down;
				break;
			case SDLK_2:
				this->two = down;
				break;
			case SDLK_3:
				this->three = down;
				break;
			case SDLK_9:
				this->nine = down;
				break;
			case SDLK_0:
				this->zero = down;
				break;
		}
	}
}
