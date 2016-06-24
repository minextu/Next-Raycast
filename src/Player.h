class Player
{
	private:
		double angle = 0, zAngle = 0;
		double x = 2, y = 2, z = 400, height = 400;
		double moveSpeed = 0.05, flySpeed = 5;
	public:
		double getAngle();
		double getZAngle();
		double getX();
		double getY();
		double getZ();
		
		void moveLeft();
		void moveRight();
		void moveForward();
		void moveBackward();
		void move(double,double);
		void moveUp();
		void moveDown();
		void rotate(double);
		void addZAngle(double);
};
