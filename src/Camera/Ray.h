class Ray
{
	private:
		double x, y, angle;
		int mapWidth, mapHeight;
	public:
		Ray(double, double, double);
		void setMapSize(int, int);
		std::vector<RayCollision> send();
};
