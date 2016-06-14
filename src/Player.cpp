void Player::moveForward(int worldMap[])
{
	if(worldMap[convert(this->posX + this->dirX * this->moveSpeed, this->posY)] == false) 
		this->posX += this->dirX * this->moveSpeed;
	if(worldMap[convert(this->posX,this->posY + this->dirY * this->moveSpeed)] == false) 
		this->posY += this->dirY * this->moveSpeed;
}

void Player::moveBackward(int worldMap[])
{
	if(worldMap[convert(this->posX - this->dirX * this->moveSpeed, this->posY)] == false) 
		this->posX -= this->dirX * this->moveSpeed;
	if(worldMap[convert(this->posX,this->posY - this->dirY * this->moveSpeed)] == false) 
		this->posY -= this->dirY * this->moveSpeed;
}

void Player::rotate(Camera& camera, bool left)
{
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
