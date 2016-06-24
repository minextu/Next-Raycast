class Player
{
	private:
		double angle = 0, zAngle = 0;
		double x = 2, y = 2, z = 400, height = 400;
		double moveSpeed = 0.05, flySpeed = 10;
	public:
		double getAngle();
		double getZAngle();
		double getX();
		double getY();
		double getZ();
		
		void setAngle(double);
		void setZAngle(double);
		void setX(double);
		void setY(double);
		void setZ(double);
		
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
