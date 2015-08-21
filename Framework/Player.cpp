#include "Player.h"
#include "Map.h"

Player::Player(AnimatedSprite* sprite) : Entity()
{
	Entity::Initialise(sprite);
}


Player::~Player()
{
}

void
Player::Process(float deltaTime, Map* map)
{
	Entity::Process(deltaTime, map);
}

void
Player::SetJumped(bool set)
{
	m_jump = set;
}

bool
Player::GetJumped()
{
	return m_jump;
}