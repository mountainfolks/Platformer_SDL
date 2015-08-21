#include "Player.h"
#include "Map.h"

Player::Player(AnimatedSprite* sprite) : Entity()
, m_left(0)
{
	Entity::Initialise(sprite);
}


Player::~Player()
{
}

void
Player::Process(float deltaTime)
{
	Entity::Process(deltaTime);
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

bool
Player::FacingLeft(){
	return m_left;
}

void
Player::setFacingLeft(bool left){
	m_left = left;
}