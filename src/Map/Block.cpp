#include "Block.h"

Block::Block(int type)
{
	this->type = type;
}

Block::Block(int type, int zHeight)
{
	this->type = type;
	this->zHeight = zHeight;
}

Block::Block(int type, int zHeight, int zOffset)
{
	this->type = type;
	this->zHeight = zHeight;
	this->zOffset = zOffset;
}
