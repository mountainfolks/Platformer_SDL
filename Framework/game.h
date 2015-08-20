
#ifndef __GAME_H__
#define __GAME_H__

class BackBuffer;
class InputHandler;
class Sprite;
class Player;
class AnimatedSprite;

class Game
{
	//Member Methods:
public:
	static Game& GetInstance();
	static void DestroyInstance();
	~Game();

	bool Initialise();
	bool DoGameLoop();
	void Quit();

	void MovePlayerLeft();
	void MovePlayerRight();
	void PlayerJump();
	void MoveStop();

	void SpawnEnemy(int x, int y);
	void SpawnExplosion(int x, int y);

	
protected:
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);
	void InitPlayerAnimation();

private:
	Game(const Game& game);
	Game& operator=(const Game& game);
	
	Game();

	//Member Data:
public:

protected:
	static Game* sm_pInstance;
	BackBuffer* m_pBackBuffer;
	InputHandler* m_pInputHandler;
	bool m_looping;

	float m_elapsedSeconds;
	float m_lag;
	float m_executionTime;
	int m_lastTime;
	int m_frameCount;
	int m_FPS;
	int m_numUpdates;
	bool m_drawDebugInfo;

	Player* m_pPlayer;
	AnimatedSprite* an_sprite;

private:

};

#endif // __GAME_H__
