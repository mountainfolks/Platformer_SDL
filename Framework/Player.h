#pragma once
#include "entity.h"
#include "AnimatedSprite.h"

class AnimatedSprite;

class Player : public Entity
{
public:
	Player(AnimatedSprite* sprite);
	~Player();

	void SetJumped(bool set);
	bool GetJumped();
	void Process(float deltaTime);

	bool FacingLeft();
	void setFacingLeft(bool face);

private:
	bool m_jump;
	bool m_left;

protected:

};

