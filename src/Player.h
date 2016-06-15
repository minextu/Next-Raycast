class Camera;

class Player
{
	private:
		double moveSpeed = 0.05; //squares/second
		double rotSpeed = 0.05; //radians/second
		double blockMargin = 0.1;
		double currentJumpSpeed = 0, jumpSpeed = 50;
	public:
		bool isDown = false;
		double posX = 8, posY = 3; // intial position
		double dirX = -1, dirY = 0; // initial direction
		double posZ = 0;

	public:
		void moveForward(int[]);
		void moveBackward(int[]);
		void moveLeft(int[]);
		void moveRight(int[]);
		void move(int[], double, double);
		void rotate(Camera&, bool);
		void rotate(Camera&, double);
		void jump();
		void checkJump();
};
