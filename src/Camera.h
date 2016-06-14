class Camera
{
	int w, h;
	
	public:
		double planeX = 0, planeY = 0.66, angle = 0; //the 2d raycaster version of camera plane
	
	public:
		Camera(int,int);
		void calculateRay(NextEngine, int[], Player&);
};
