class Camera
{
	private:
		Player* player;
		int fov = 90;
		NextEngine engine;
		NextImage* textures;
		
	public:
		void setTextures(NextImage[]);
		void setPlayer(Player*);
		void setEngine(NextEngine&);
		void renderMap(Map&);
	private:
		void renderRay(Map&, double, double, int, double);
		void renderBlockLine(double, double, RayCollision collision, double, double, Map&, double, int);

};
