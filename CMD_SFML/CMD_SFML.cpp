#include <iostream>
#include "Game.h"

int main()
{
	std::srand(static_cast<unsigned>(time(NULL)));

	Game game;

	//create window
	while (game.running())
	{
		game.update();
		game.render();
	}
	return 0;
}