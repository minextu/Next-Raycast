void Player::moveForward(int worldMap[])
{
	// Only move, if no wall is in front
	if( worldMap[convert(this->posX + this->dirX * this->moveSpeed, this->posY)] == 0 &&
		worldMap[convert(this->posX,this->posY + this->dirY * this->moveSpeed)] == 0 )
	{
		this->posX += this->dirX * this->moveSpeed;
		this->posY += this->dirY * this->moveSpeed;
	}
}

void Player::moveBackward(int worldMap[])
{
	
	// Only move, if no wall is behind
	if( worldMap[convert(this->posX - this->dirX * this->moveSpeed, this->posY)] == 0 &&
		worldMap[convert(this->posX,this->posY - this->dirY * this->moveSpeed)] == 0 )
	{
		this->posX -= this->dirX * this->moveSpeed;
		this->posY -= this->dirY * this->moveSpeed;
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
	
	//both camera direction and camera plane must be rotated
	double oldDirX = this->dirX;
	this->dirX = this->dirX * cos(rot) - this->dirY * sin(rot);
	this->dirY = oldDirX * sin(rot) + this->dirY * cos(rot);
	
	double oldPlaneX = camera.planeX;
	camera.planeX = camera.planeX * cos(rot) - camera.planeY * sin(rot);
	camera.planeY = oldPlaneX * sin(rot) + camera.planeY * cos(rot);
}
