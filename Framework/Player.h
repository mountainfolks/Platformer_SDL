#pragma once
#include "entity.h"
#include "AnimatedSprite.h"

class Player : public Entity
{
public:
	Player(AnimatedSprite* sprite);
	~Player();

	void SetJumped(bool set);
	bool GetJumped();
	void Process(float deltaTime);
private:
	bool m_jump;

protected:

};

