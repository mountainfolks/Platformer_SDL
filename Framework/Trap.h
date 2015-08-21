#pragma once
#include "entity.h"

class Trap : public Entity
{
public:
	Trap(Sprite* sprite, int x, int y);
	~Trap();

	void Process(float deltaTime);
};

