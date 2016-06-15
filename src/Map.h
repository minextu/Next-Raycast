class Player;

class Map
{
	private:
	
	public:
		int* map;
		int w, h;
		
	public:
		void setSize(int,int);
		void set(int[]);
		void render2d(NextEngine, int[], Player, NextImage[]);
};
