#include "Spear.h"


Spear::Spear(Sprite* sprite) : Entity()
{
	Entity::Initialise(sprite);
}

void
Spear::Process(float deltaTime){
	Entity::Process(deltaTime);
}

Spear::~Spear()
{
}
