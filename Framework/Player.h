#pragma once
#include "entity.h"

class Player : public Entity
{
public:
	Player(Sprite* sprite);
	~Player();

	void SetJumped(bool set);
	bool GetJumped();
	void Process(float deltaTime);

private:
	bool m_jump;

protected:

};

