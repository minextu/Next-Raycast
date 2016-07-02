struct Keyboard
{
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool space = false;
	bool h = false;
	bool n = false;
	bool zero = false;
	bool one = false;
	bool two = false;
	bool three = false;
	bool nine = false;
	bool q = false;
	bool x = false;
	bool y = false;
	bool shift = false;
	
	void getKey(SDL_Event e);
};
