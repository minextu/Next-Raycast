void Player::moveForward(int worldMap[])
{
	this->move(worldMap, this->dirX, this->dirY);
}

void Player::moveBackward(int worldMap[])
{
	this->move(worldMap, -this->dirX, -this->dirY);
}

void Player::moveLeft(int worldMap[])
{
	double x = this->dirX;
	double y = this->dirY;
	rotateDir(x,y,-90);
	
	this->move(worldMap, x, y);
}
void Player::moveRight(int worldMap[])
{
	double x = this->dirX;
	double y = this->dirY;
	rotateDir(x,y,90);
	
	this->move(worldMap, x, y);
}

void Player::move(int worldMap[], double dirX, double dirY)
{
	// Only move, if no wall is in front
	if( worldMap[convert(this->posX + dirX * this->moveSpeed, this->posY)] == 0 &&
		worldMap[convert(this->posX,this->posY + dirY * this->moveSpeed)] == 0 )
	{
		this->posX += dirX * this->moveSpeed;
		this->posY += dirY * this->moveSpeed;
	}
}
void Player::rotate(Camera& camera, bool left)
{
	//rotate Speed
	double rot;
	
	if (left == false)
		rot = this->rotSpeed;
	else
		rot = -this->rotSpeed;
	
	this->rotate(camera, rot);
}

void Player::rotate(Camera& camera, double rot)
{
	//both camera direction and camera plane must be rotated
	double oldDirX = this->dirX;
	this->dirX = this->dirX * cos(rot) - this->dirY * sin(rot);
	this->dirY = oldDirX * sin(rot) + this->dirY * cos(rot);
	
	double oldPlaneX = camera.planeX;
	camera.planeX = camera.planeX * cos(rot) - camera.planeY * sin(rot);
	camera.planeY = oldPlaneX * sin(rot) + camera.planeY * cos(rot);
}
