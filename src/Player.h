class Camera;

class Player
{
	private:
		double moveSpeed = 0.1; //squares/second
		double rotSpeed = 0.05; //radians/second
	public:
		double posX = 22, posY = 12; // intial position
		double dirX = -1, dirY = 0; // initial direction

	public:
		void moveForward(int[]);
		void moveBackward(int[]);
		void rotate(Camera&, bool);
};
