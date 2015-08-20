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

	/*Sprite* m_sprite = m_pBackBuffer->CreateSprite("assets\\player.png");
	m_sprite->SetX(400);
	m_sprite->SetY(500);
	m_pPlayer = new Player(m_sprite);*/


	//------ Animated Sprite ------
	Sprite* sprite = m_pBackBuffer->CreateSprite("assets\\player_sprite.png");
	an_sprite = new AnimatedSprite();
	m_pPlayer = new Player(an_sprite);
	an_sprite->Initialise(*sprite->GetTexture());
	an_sprite->SetFrameWidth(32);
	an_sprite->SetFrameSpeed(10.0f);
	an_sprite->AddFrame(0);
	an_sprite->AddFrame(32);
	an_sprite->AddFrame(64);
	an_sprite->AddFrame(96);
	an_sprite->AddFrame(128);

	m_lastTime = SDL_GetTicks();
	m_lag = 0.0f;

	return (true);
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

	m_pPlayer->Process(deltaTime);
	an_sprite->Process(deltaTime);
	
	if (m_pPlayer->GetJumped()){
		m_pPlayer->SetVerticalVelocity(m_pPlayer->GetVerticalVelocity() + (700.0f * deltaTime));
	}

}

void 
Game::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;

	backBuffer.Clear();
	//---- old static sprite---
	//backBuffer.DrawSprite(m_pPlayer->GetSprite());


	//---animated sprite---
	an_sprite->Draw(backBuffer);
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
	m_pPlayer->SetHorizontalVelocity(-300.0f);
}

void
Game::MovePlayerRight()
{
	m_pPlayer->SetHorizontalVelocity(300.0f);
}

void
Game::PlayerJump(){
	m_pPlayer->SetVerticalVelocity(-500.0f);
	m_pPlayer->SetJumped(true);
}

void
Game::MoveStop(){
	m_pPlayer->SetHorizontalVelocity(0);
}