struct Keyboard
{
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool space = false;
	bool h = false;
	bool n = false;
	
	void getKey(SDL_Event e);
};
