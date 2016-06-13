#include "Player.h"

void Player::checkKeys(bool keys[])
{
	if (keys[0])
		this->drawX += this->speed;
	if (keys[1])
		this->drawX -= this->speed;
	if (keys[2])
		this->drawY += this->speed;
	if (keys[3])
		this->drawY -= this->speed;
}

void Player::draw(NextEngine engine)
{
	engine.setFillColor("red");
	engine.fillRect(this->drawX,this->drawY,this->width,this->height);
}
