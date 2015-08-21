// 717310 C++ SDL Framework

// This includes:
#include "game.h"

// Local includes:
#include "backbuffer.h"
#include "inputhandler.h"
#include "logmanager.h"
#include "sprite.h"
#include "player.h"
#include "AnimatedSprite.h"
#include "texture.h"
#include "map.h"
#include "trap.h"
#include "spear.h"

// Library includes:
#include <cassert>
#include <SDL.h>

// Static Members:
Game* Game::sm_pInstance = 0;

Game&
Game::GetInstance()
{
	if (sm_pInstance == 0)
	{ 
		sm_pInstance = new Game();
	}

	assert(sm_pInstance);

	return (*sm_pInstance);
}

void 
Game::DestroyInstance()
{
	delete sm_pInstance; 
	sm_pInstance = 0;
}

Game::Game()
: m_pBackBuffer(0)
, m_pInputHandler(0)
, m_level(0)
, m_looping(true)
, m_executionTime(0)
, m_elapsedSeconds(0)
, m_frameCount(0)
, m_FPS(0)
, m_numUpdates(0)
, m_lastTime(0)
, m_lag(0)
{
	
}

Game::~Game()
{
	delete m_pBackBuffer;
	m_pBackBuffer = 0;
}

bool 
Game::Initialise()
{
	const int width = 800;
	const int height = 600;

	m_pBackBuffer = new BackBuffer();
	if (!m_pBackBuffer->Initialise(width, height))
	{
		LogManager::GetInstance().Log("BackBuffer Init Fail!");
		return (false);
	}

	m_pInputHandler = new InputHandler();
	if (!m_pInputHandler->Initialise())
	{
		LogManager::GetInstance().Log("InputHandler Init Fail!");
		return (false);
	}

	/*m_level = new Level("assets\\map.txt", *m_pBackBuffer);
	m_level->LoadMap();
	m_level->DrawMap();*/


	/*Sprite* m_sprite = m_pBackBuffer->CreateSprite("assets\\player.png");
	m_sprite->SetX(400);
	m_sprite->SetY(500);
	m_pPlayer = new Player(m_sprite);*/

	map = new Map();
	map->load("assets\\test.map");

	SpawnTraps(200, 270);
	SpawnTraps(100, 220);

	InitPlayerAnimation();

	SpawnDiamond();

	m_lastTime = SDL_GetTicks();
	m_lag = 0.0f;

	return (true);
}

void 
Game::InitPlayerAnimation(){
	Sprite* sprite = m_pBackBuffer->CreateSprite("assets\\player_sprite.png");
	Texture* tex = sprite->GetTexture();

	an_sprite = new AnimatedSprite();
	an_sprite->Initialise(*tex);
	an_sprite->SetFrameWidth(32);
	an_sprite->SetLooping(true);
	//an_sprite->Pause();
	an_sprite->SetFrameSpeed(0.15f);
	an_sprite->AddFrame(0);
	an_sprite->AddFrame(32);
	an_sprite->AddFrame(64);
	an_sprite->AddFrame(96);
	an_sprite->AddFrame(128);
	an_sprite->SetX(0);
	an_sprite->SetY(0);
	m_pPlayer = new Player(an_sprite);
}

bool 
Game::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;

	assert(m_pInputHandler);
	m_pInputHandler->ProcessInput(*this);
	
	if (m_looping)
	{
		int current = SDL_GetTicks();
		float deltaTime = (current - m_lastTime) / 1000.0f;
		m_lastTime = current;

		m_executionTime += deltaTime;

		m_lag += deltaTime;

		while (m_lag >= stepSize)
		{
			Process(stepSize);

			m_lag -= stepSize;

			++m_numUpdates;
		}
		
		Draw(*m_pBackBuffer);
	}

	SDL_Delay(1);

	return (m_looping);
}

void 
Game::Process(float deltaTime)
{

	m_elapsedSeconds += deltaTime;


	if (m_elapsedSeconds > 1)
	{
		m_elapsedSeconds -= 1;
		m_FPS = m_frameCount;
		m_frameCount = 0;
	}

	if (m_pPlayer->GetHorizontalVelocity() == 0){
		an_sprite->StartAnimating();
	}

	m_pPlayer->Process(deltaTime);
	an_sprite->Process(deltaTime);

	jewel->Process(deltaTime);
	
	for (int i = 0; i < explosions.size(); ++i){
		AnimatedSprite* sprite = explosions[i];
		if (sprite->GetCurrentFrame() == 4){
			explosions.erase(explosions.begin() + i);
		}
		else{
			sprite->Process(deltaTime);
		}

	}

	for (int i = 0; i < m_spears.size(); ++i){
		Spear* spear = m_spears[i];
		spear->Process(deltaTime);
		for (int j = 0; j < m_traps.size(); ++j){
			Trap* trap = m_traps[j];
			if (spear->IsCollidingWith(*trap)){
				/*spear->SetDead(true);
				trap->SetDead(true);*/
				m_traps.erase(m_traps.begin() + j);
				m_spears.erase(m_spears.begin() + i);
				SpawnExplosion(trap->GetPositionX() - 4, trap->GetPositionY() -10);
			}
		}
	}

	m_pPlayer->HandleMovement(deltaTime, map);

	for (int i = 0; i < m_traps.size(); ++i)
	{
		Trap* trap = m_traps[i];
		if (m_pPlayer->IsCollidingWith(*trap)){
			trap->SetDead(true);
			m_traps.erase(m_traps.begin() + i);
			m_pPlayer->SetDead(true);
			SpawnExplosion(m_pPlayer->GetPositionX() , m_pPlayer->GetPositionY() );
		}
	}


	if (m_pPlayer->GetJumped()){
		m_pPlayer->SetVerticalVelocity(m_pPlayer->GetVerticalVelocity() + (500.0f * deltaTime));
	}

}

void 
Game::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;

	backBuffer.Clear();

	//---animated sprite---

	map->draw(backBuffer);
	

	if (m_pPlayer->IsDead() != true){
		bool left = m_pPlayer->FacingLeft();
		an_sprite->Draw(backBuffer, left);
	}

	for (int i = 0; i < m_traps.size(); ++i){
		m_traps[i]->Draw(backBuffer);
	}

	for (int i = 0; i < explosions.size(); ++i){
		AnimatedSprite* exp = explosions[i];
		exp->Draw(backBuffer, false);
	}

	for (int i = 0; i < m_spears.size(); ++i){
		Spear* spear = m_spears[i];
		spear->Draw(backBuffer);
	}

	jewel->Draw(backBuffer, false);

	backBuffer.Present();
}

void 
Game::Quit()
{
	m_looping = false;
}

void 
Game::MovePlayerLeft()
{ 
	m_pPlayer->SetHorizontalVelocity(-170.0f);
	m_pPlayer->setFacingLeft(true);
}

void
Game::MovePlayerRight()
{
	m_pPlayer->SetHorizontalVelocity(170.0f);
	m_pPlayer->setFacingLeft(false);
}

void
Game::PlayerJump(){
	m_pPlayer->SetVerticalVelocity(-230.0f);
	m_pPlayer->SetJumped(true);
}

void
Game::MoveStop(){
	m_pPlayer->SetHorizontalVelocity(0);
}

void
Game::SpawnSpear(){
	Sprite* sprite = m_pBackBuffer->CreateSprite("assets\\spear.png");
	sprite->SetX(m_pPlayer->GetPositionX());
	sprite->SetY(m_pPlayer->GetPositionY() + 15);

	Spear* spear = new Spear(sprite);
	spear->SetPositionX(m_pPlayer->GetPositionX());
	spear->SetPositionY(m_pPlayer->GetPositionY() + 15);
	
	if (m_pPlayer->FacingLeft()){
		spear->SetHorizontalVelocity(-210.0f);
	}
	else{

		spear->SetHorizontalVelocity(210.0f);
	}

	m_spears.push_back(spear);
}

void
Game::SpawnTraps(int x, int y){
	
	Sprite* s_trap = m_pBackBuffer->CreateSprite("assets\\trap.png");

	//set sprite position
	s_trap->SetX(x);
	s_trap->SetY(y);

	Trap* trap = new Trap(s_trap, x, y);
	trap->SetPositionX(x);
	trap->SetPositionY(y);
	m_traps.push_back(trap);
}

void
Game::SpawnExplosion(int x, int y){

	Sprite* sprite = m_pBackBuffer->CreateSprite("assets\\explosion.png");
	Texture* tex = sprite->GetTexture();

	an_explosion = new AnimatedSprite();
	an_explosion->Initialise(*tex);
	an_explosion->SetFrameWidth(36);
	an_explosion->SetLooping(false);
	an_explosion->SetFrameSpeed(0.1f);
	an_explosion->AddFrame(0);
	an_explosion->AddFrame(36);
	an_explosion->AddFrame(72);
	an_explosion->AddFrame(108);
	an_explosion->AddFrame(144);
	an_explosion->SetX(x);
	an_explosion->SetY(y);
	explosions.push_back(an_explosion);
}

void
Game::SpawnDiamond(){

	Sprite* sprite = m_pBackBuffer->CreateAnimatedSprite("assets\\spin_diammond.png");
	Texture *tex = sprite->GetTexture();

	jewel = new AnimatedSprite();
	jewel->Initialise(*tex);
	jewel->SetFrameWidth(31);
	jewel->SetLooping(true);
	jewel->SetFrameSpeed(0.5f);
	jewel->AddFrame(0);
	jewel->AddFrame(31);
	jewel->AddFrame(63);
	jewel->AddFrame(94);
	jewel->SetX(300);
	jewel->SetY(10);
}

bool
Game::CheckInValid(){
	return m_pPlayer->IsDead();
}