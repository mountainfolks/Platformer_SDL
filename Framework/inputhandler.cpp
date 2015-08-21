
// This include:
#include "inputhandler.h"

// Local includes:
#include "game.h"

// Library includes:
#include <cassert>

InputHandler::InputHandler()
{

}

InputHandler::~InputHandler()
{
}

bool 
InputHandler::Initialise()
{
	return (true);
}

void 
InputHandler::ProcessInput(Game& game)
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			game.Quit();
		}
		else if (e.type == SDL_KEYDOWN)
		{
			if (game.CheckInValid() == false){
				switch (e.key.keysym.sym)
				{
				case SDLK_LEFT:
					game.MovePlayerLeft();
					break;
				case SDLK_RIGHT:
					game.MovePlayerRight();
					break;
				case SDLK_SPACE:
					game.PlayerJump();
					break;
				case SDLK_z:
					game.SpawnSpear();
					break;
				}
			}
			
		}

		else if (e.type == SDL_KEYUP){
			switch (e.key.keysym.sym)
			{
			case SDLK_LEFT:
				game.MoveStop();
				break;
			case SDLK_RIGHT:
				game.MoveStop();
				break;
			}
		}
	}
}
