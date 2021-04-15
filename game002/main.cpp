#include"Game.h"

Game* gGame;

int main(int argc, char** argv)
{

	while (true)
	{	
		if (!gGame)
		{
			gGame = new Game();
			gGame->Output();
		}
		
		gGame->Input();
		gGame->Update();
		gGame->Output();
		if (gGame->GetIsEnd())
		{
			delete gGame;
			gGame = nullptr;
			break;
		}

		if (gGame->IsClear() || gGame->GetIsRestart())
		{
			delete gGame;
			gGame = nullptr;
		}

	}

	return 0;
}