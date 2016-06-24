class Camera
{
	private:
		Player* player;
		int fov = 100;
		double currentFov;
		NextEngine engine;
		NextImage* textures;
		
	public:
		void setTextures(NextImage[]);
		void setPlayer(Player*);
		void setEngine(NextEngine&);
		void renderMap(Map&);
	private:
		void renderRay(Map&, double, double, int, double);
		void generateBlockLine(std::vector<Line>&, double, double, RayCollision collision, double, double, Map&, double, int);
		void renderBlockLines(std::vector<Line>&, int);

};
