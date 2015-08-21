
#ifndef __ENTITY_H__
#define __ENTITY_H__

// Forward Declarations:
class BackBuffer;
class Sprite;
class Map;

class Entity
{
	//Member Methods:
public:
	Entity();
	~Entity();

	bool Initialise(Sprite* sprite);

	void Process(float deltaTime);
	virtual void Draw(BackBuffer& backBuffer);

	void SetDead(bool dead);
	bool IsDead();

	bool IsCollidingWith(Entity& e);

	float GetPositionX();
	float GetPositionY();

	void SetPositionX(float x);
	void SetPositionY(float y);

	float GetHorizontalVelocity();
	void SetHorizontalVelocity(float x); 

	float GetVerticalVelocity();
	void SetVerticalVelocity(float y);

	bool HorizontalCollision(int x, int y, int &tilePosY, Map* map);
	bool VerticalCollision(int x, int y, int &tilePosX, Map* map);

	void HandleMovement(float deltaTime, Map* map);

	Sprite& GetSprite();


private:
	Entity(const Entity& entity);
	Entity& operator=(const Entity& entity);

protected:
	Sprite* m_pSprite;

	float m_x;
	float m_y;

	float m_velocityX;
	float m_velocityY;

	bool m_dead;

};

#endif //__ENTITY_H__
