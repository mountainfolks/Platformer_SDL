#pragma once
#include "entity.h"

class Spear : public Entity
{
public:
	Spear(Sprite* spear);
	~Spear();
	void Process(float deltaTime);
};

