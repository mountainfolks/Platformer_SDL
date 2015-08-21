
#include "entity.h"

// Local includes:
#include "sprite.h"
#include "backbuffer.h"
#include "Map.h"
#include "game.h"
#include <SDL.h>

// Library includes:
#include <cassert>

Entity::Entity()
: m_pSprite(0)
, m_x(0.0f)
, m_y(0.0f)
, m_velocityX(0.0f)
, m_velocityY(0.0f)
, m_dead(false)
{

}

Entity::~Entity()
{
}

bool
Entity::Initialise(Sprite* sprite)
{
	assert(sprite);
	m_pSprite = sprite;

	return (true);
}

void 
Entity::Process(float deltaTime)
{
	m_x = m_pSprite->GetX() + (m_velocityX * deltaTime);
	m_pSprite->SetX(m_x);

	m_y = m_pSprite->GetY() + (m_velocityY * deltaTime);
	m_pSprite->SetY(m_y);

}

void 
Entity::Draw(BackBuffer& backBuffer)
{
	assert(m_pSprite);
	m_pSprite->Draw(backBuffer);
}

void 
Entity::HandleMovement(float deltaTime, Map* map)
{

	int tilePos;

	//if (m_velocityX > 0){

	//	if (VerticalCollision((m_x + (m_velocityX/ 10) + m_pSprite->GetWidth()), m_y, tilePos, map))
	//	{
	//		m_x = (tilePos * 32) - m_pSprite->GetWidth() + 4;
	//		m_pSprite->SetX(m_x);
	//	}
	//	else
	//	{
	//		m_x = m_pSprite->GetX() + (m_velocityX * deltaTime);
	//		m_pSprite->SetX(m_x);
	//	}
	//}
	//else if (m_velocityX < 0){

	//	if (VerticalCollision((m_x + (m_velocityX / 10)), m_y, tilePos, map)){
	//		m_x = (tilePos + 1) * 32;
	//		m_pSprite->SetX(m_x);
	//	}
	//	else{
	//		m_x = m_pSprite->GetX() + (m_velocityX * deltaTime);
	//		m_pSprite->SetX(m_x);
	//	}
	//	
	//	}

	

	if (m_velocityY > 0){

		if (HorizontalCollision(m_x, (m_y + (m_velocityY / 15 ) + m_pSprite->GetHeight()), tilePos, map)){
			m_y = (tilePos * 32) - m_pSprite->GetHeight() + 2;
			m_pSprite->SetY(m_y);
			m_velocityY = 0;
		}
		else{
			m_y = m_pSprite->GetY() + (m_velocityY * deltaTime);
			m_pSprite->SetY(m_y);
		}
	}
	else{

		if (HorizontalCollision(m_x, (m_y + (m_velocityY / 15)), tilePos, map)){
			m_y = (tilePos + 1) * 32;
			m_velocityY = 0;
			m_pSprite->SetY(m_y);
		}
			
			m_velocityY += (500.0f * deltaTime);

			m_y = m_pSprite->GetY() + (m_velocityY * deltaTime);
			m_pSprite->SetY(m_y);
		
	}
}

bool
Entity::HorizontalCollision(int x, int y, int &tilePosY, Map* _map){

	int TILE_HEIGHT = 32;
	int TILE_WIDTH = 32;

	// Calculate the Y position (in terms of map tiles) of the tile to test.
	tilePosY = y / TILE_HEIGHT;

	// Calculate the X position
	int tileXPixels = x - (x % TILE_WIDTH);

	// Calculate the X position (in terms of map tiles) of the first tile to test.
	int tilePosX = tileXPixels / TILE_WIDTH;

	// Testing for tile collisions beginning from the start to the end height of the character.
	while (tileXPixels < (x + TILE_WIDTH))
	{
		// A collision (of type non-zero) has occured.
		if (_map->get(tilePosX, tilePosY) != 0)
		{
			return true;
		}

		tilePosX++;
		tileXPixels += TILE_WIDTH;
	}

	return false;
}

bool
Entity::VerticalCollision(int x, int y, int &tilePosX, Map* _map){

	int TILE = 32;

	tilePosX = x / TILE; // Example: 50/32 = 1

	// Calculate the Y position
	int tileYPixels = y - (y%TILE); // Example: 100 - (100%32) = 100 - 4 = 96

	// Calculate the Y position (in terms of map tiles) of the first tile to test.
	int tilePosY = tileYPixels / TILE; 

	while (tileYPixels < (y + TILE))
	{
		// A collision (of type non-zero) has occured.
		if (_map->get(tilePosX, tilePosY) != 0)
		{
			return true;
		}

		tilePosY++;
		tileYPixels += TILE;
	}

	return false;
}

bool
Entity::IsCollidingWith(Entity& e){
	SDL_Rect rect1;
	SDL_Rect rect2;

	rect1.x = this->GetPositionX();
	rect1.y = this->GetPositionY();
	rect1.w = 32;
	rect1.h = 32;

	rect2.x = e.GetPositionX();
	rect2.y = e.GetPositionY();
	rect2.w = 16;
	rect2.h = 16;

	if (rect1.y >= rect2.y + rect2.h){
		return false;
	}
	if (rect1.x >= rect2.x + rect2.w){
		return false;
	}
	if (rect1.y + rect1.h <= rect2.y){
		return false;
	}
	if (rect1.x + rect1.w <= rect2.x){
		return false;
	}
	return true;
}

//
//bool
//Entity::IsCollidingWith(Entity& e)
//{
//	return (false); // Ex006.4 Change return value!
//}

void 
Entity::SetDead(bool dead)
{
	m_dead = dead;
}

float 
Entity::GetPositionX()
{
	return (m_x);
}

float 
Entity::GetPositionY()
{
	return (m_y);
}

float 
Entity::GetHorizontalVelocity()
{
	return (m_velocityX);
}

float 
Entity::GetVerticalVelocity()
{
	return (m_velocityY);
}

void 
Entity::SetHorizontalVelocity(float x)
{
	m_velocityX = x;
}

void 
Entity::SetVerticalVelocity(float y)
{
	m_velocityY = y;
}

Sprite&
Entity::GetSprite(){
	return *m_pSprite;
}

void
Entity::SetPositionX(float x){
	m_x = x;
}

void 
Entity::SetPositionY(float y){
	m_y = y;
}

bool
Entity::IsDead(){
	return m_dead;
}