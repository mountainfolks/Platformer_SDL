#include "Trap.h"
#include "sprite.h"

Trap::Trap(Sprite* sprite, int x, int y) : Entity()
{
	Entity::Initialise(sprite);
}

void
Trap::Process(float deltaTime){
	Entity::Process(deltaTime);
}


Trap::~Trap()
{

}
