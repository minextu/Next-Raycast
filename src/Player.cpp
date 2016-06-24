#include "Player.h"

void rotateDir(double &x, double &y, int degrees)
{
	double rot = degrees * (double)M_PI/180;
	
	double oldX = x;
	x = x * cos(rot) - y * sin(rot);
	y = oldX * sin(rot) + y * cos(rot);
}

double Player::getAngle()
{
	return this->angle;
}

double Player::getZAngle()
{
	return this->zAngle;
}

double Player::getX()
{
	return this->x;
}

double Player::getY()
{
	return this->y;
}

double Player::getZ()
{
	return this->z;
}

void Player::setAngle(double angle)
{
	this->angle = angle;
}

void Player::setZAngle(double angle)
{
	this->zAngle = angle;
}

void Player::setX(double x)
{
	this->x = x;
}

void Player::setY(double y)
{
	this->y = y;
}

void Player::setZ(double z)
{
	this->z = z;
}

void Player::moveForward()
{
	double speedX = std::cos(this->angle);
	double speedY = std::sin(this->angle);
	this->move( speedX, speedY);
}

void Player::moveBackward()
{
	double speedX = std::cos(this->angle);
	double speedY = std::sin(this->angle);
	this->move(-speedX, -speedY);
}

void Player::moveLeft()
{
	double x = std::cos(this->angle);
	double y = std::sin(this->angle);
	rotateDir(x,y,-90);
	
	this->move(x, y);
}
void Player::moveRight()
{
	double x = std::cos(this->angle);
	double y = std::sin(this->angle);
	rotateDir(x,y,90);
	
	this->move(x, y);
}

void Player::move(double speedX, double speedY)
{
	// Only move, if no wall is in front
	/*if( worldMap[convert(this->posX + dirX * delta(this->moveSpeed) + dirX * this->blockMargin, this->posY)] == 0)*/
	this->x += delta(speedX*this->moveSpeed);

	/*if(worldMap[convert(this->posX,this->posY + dirY * delta(this->moveSpeed) + dirY * this->blockMargin)] == 0)*/
	this->y += delta(speedY*this->moveSpeed);
}

void Player::moveUp()
{
	this->z += delta(this->flySpeed);
}

void Player::moveDown()
{
	this->z -= delta(this->flySpeed);
}

void Player::rotate(double speed)
{
	this->angle += speed;
}

void Player::addZAngle(double angle)
{
	this->zAngle += angle;
}
