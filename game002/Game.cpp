#include "Game.h"
#include<cstdio>
#include<cstdlib>

#define WINWIDTH 1024
#define WINHEIGHT 800

Game::Game()
	:
	mWindow{ nullptr }, 
	mIsEnd{ false },
	PlayerVector{ 0,0 },
	IsPressed{ true },
	PlayerPos{ 0,0 },
	mIsRestart{false},
	mTicksCount{0}
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("SDL���C�u�����̏������Ɏ��s���܂����B:%s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}


	mWindow = SDL_CreateWindow(
		"First Window",
		100,
		100,
		WINWIDTH,
		WINHEIGHT,
		0
		);

	if (!mWindow)
	{
		printf("�E�B���h�E�̍쐬�Ɏ��s���܂����B:%s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
		);

	if (!mRenderer)
	{
		printf("�����_���[�̍쐬�Ɏ��s���܂����B:%s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	//�t�@�C���f�[�^�̓ǂݍ���
	char* buffer = nullptr;

	int size = 0;

	ReadFile("stageData03.txt", &buffer, size);

	int height = FindChar(buffer, '\n');
	int width = 0;
	int Maxw = 0;
	for (int i = 0; buffer[i] != '\0'; i++)
	{
		
		if (buffer[i] == '\n')
		{
			width = (Maxw > width) ? Maxw : width;
			Maxw = 0;
		}
		else
		{
			switch (buffer[i])
			{
			case ' ':
			case '.':
			case'#':
			case 'o':
			case'p':
			case'O':
			case 'P':
				++Maxw;
				break;
			default:
				break;
			}
		}
	}

	mIsTarget.SetSize(width, height);
	mObject.SetSize(width, height);

	if (mObject.GetWidth() != mIsTarget.GetWidth() || mObject.GetHeight() != mIsTarget.GetHeight())
	{
		std::cerr << "�T�C�Y�G���[���������܂����B\n";
		exit(EXIT_FAILURE);
	}

	//���W
	int x = 0;
	int y = 0;

	for (int i = 0; buffer[i] != '\0'; i++)
	{
		
			Object o = OBJ_UNNKOWN;
			bool b = false;
			//�X�e�[�W�f�[�^�̓ǂݍ��݂Ƃ��̏��̎擾
			switch (buffer[i])
			{
			case ' ':
				o = OBJ_SPACE;
				b = false;
				break;
			case '.':
				o = OBJ_SPACE;
				b = true;
				break;
			case'#':
				o = OBJ_WALL;
				b = false;
				break;
			case 'o':
				o = OBJ_BLOCK;
				b = false;
				break;
			case'p':
				o = OBJ_PLAYER;
				b = false;
				break;
			case'O':
				o = OBJ_BLOCK;
				b = true;
				break;
			case 'P':
				o = OBJ_PLAYER;
				b = true;
				break;
			default:
				//�������Ȃ�
				break;
			}

			if (o != OBJ_UNNKOWN)
			{
				if ((0 <= x && x < mObject.GetWidth()) && (0 <= y && y < mObject.GetHeight()))
				{

					mObject(x, y) = o;
					mIsTarget(x, y) = b;
					x++;
					if (mObject.GetWidth() <= x)
					{
						x = 0;
						y++;
					}

				}
			}

	}

	delete[] buffer;
}

Game::~Game()
{
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
}

int kame = 0;
void Game::Input()
{
	
	SDL_Event event;
	PlayerVector = { 0,0 };

	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsEnd = true;
			break;
		case SDL_KEYDOWN:
			if (IsPressed)
			{
				printf("%d\n",++kame);
				const Uint8* KeyState = SDL_GetKeyboardState(NULL);
				if (KeyState[SDL_SCANCODE_ESCAPE])
				{
					mIsEnd = true;
				}
				
				if (KeyState[SDL_SCANCODE_A])
				{
					PlayerVector.x -= 1;
				}
				if (KeyState[SDL_SCANCODE_D])
				{
					PlayerVector.x += 1;
				}
				if (KeyState[SDL_SCANCODE_W])
				{
					PlayerVector.y -= 1;
				}
				if (KeyState[SDL_SCANCODE_S])
				{
					PlayerVector.y += 1;
				}
				if (KeyState[SDL_SCANCODE_R])
				{
					mIsRestart = true;
				}
				IsPressed = false;
			}
			break;
			case SDL_KEYUP:
				IsPressed = true;
				break;
		}
	}

}

void Game::Update()
{

	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	//�v���C���[�̍��W����
	int y = 0;
	int x = 0;
	for (y = 0; y < mObject.GetHeight(); y++)
	{
		for (x = 0; x < mObject.GetWidth(); x++)
		{

			if (mObject(x, y) == OBJ_PLAYER)
			{
				goto _BREAK;
			}

		}
	}
_BREAK:

	//���̃v���C���[�̍��W
	int ty = y + PlayerVector.y ;
	int tx = x + PlayerVector.x ;

	if (x != tx || y != ty)
	{
		//���A�����𒴂��Ă��Ȃ����`�F�b�N
		if ((ty < 0 || mObject.GetHeight() <= ty) || (tx < 0 || mObject.GetWidth() <= tx))
		{
			return;
		}
		//�ǂɂԂ����Ă��Ȃ����`�F�b�N
		else if (mObject(tx,ty) == OBJ_WALL)
		{
			return;
		}
		//���̍��W���u���b�N�ł���ꍇ
		else if (mObject(tx, ty) == OBJ_BLOCK)
		{
			//�u���b�N�̉����ꂽ�ꍇ�̍��W
			int bx = tx + PlayerVector.x;
			int by = ty + PlayerVector.y;
			//�u���b�N�����A�����𒴂��Ă��Ȃ����`�F�b�N
			if ((by < 0 || mObject.GetHeight() <= by) || (bx < 0 || mObject.GetWidth() <= bx))
			{
				return;
			}
			//�u���b�N���ǂɂԂ����Ă��Ȃ����`�F�b�N
			if (mObject(bx, by) == OBJ_WALL|| mObject(bx, by) == OBJ_BLOCK)
			{
				return;
			}
			//�ړ�����
			else
			{
				mObject(bx, by) = OBJ_BLOCK;
				mObject(tx, ty) = OBJ_PLAYER;
				mObject(x, y) = OBJ_SPACE;
			}
		}
		//�ړ�����
		else
		{
			mObject(tx, ty) = OBJ_PLAYER;
			mObject(x, y) = OBJ_SPACE;
		}
	}
}

void Game::Output()
{

	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);

	SDL_RenderClear(mRenderer);

	/*��������`��*/

	for (int y = 0; y < mObject.GetHeight(); y++)
	{
		for (int x = 0; x < mObject.GetWidth(); x++)
		{
			char c = NULL;
			switch (mObject(x, y))
			{
			case OBJ_SPACE:
				c = (mIsTarget(x,y)) ? '.' : ' ';
				if (mIsTarget(x, y))
				{
					SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
				}
				else
				{
					SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
				}
				break;
			case OBJ_WALL:
				c = (mIsTarget(x, y)) ? '#' : '#';
				if (mIsTarget(x, y))
				{
					SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
				}
				else
				{
					SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
				}
				break;
			case OBJ_BLOCK:
				c = (mIsTarget(x, y)) ? 'O' : 'o';
				if (mIsTarget(x, y))
				{
					SDL_SetRenderDrawColor(mRenderer, 0, 255, 255, 255);
				}
				else
				{
					SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 255);
				}
				break;
			case OBJ_PLAYER:
				c = (mIsTarget(x, y)) ? 'P' : 'p';
				if (mIsTarget(x, y))
				{
					SDL_SetRenderDrawColor(mRenderer, 255, 0, 255, 255);
				}
				else
				{
					SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);
				}
				break;
			default:
				//�������Ȃ�
				continue;
				break;
			}
			//c�ɉ��������Ă��Ȃ�(������NULL�̂܂܂�)�ꍇ�ȊO�͏o��
			if (c != NULL)
			{
				printf("%c", c);
			}

			DrawBox(x, y, WINWIDTH / mObject.GetWidth(), WINHEIGHT / mObject.GetHeight());
		}
		//���S�Ă��s��������s
		printf("\n");
	}

	/**/


	SDL_RenderPresent(mRenderer);
}

bool Game::IsClear()
{
	//�S�ẴS�[���Ƀu���b�N������Ă��Ȃ��ꍇ��false����ȊO��true
	for (int y = 0; y < mObject.GetHeight(); y++)
	{
		for (int x = 0; x < mObject.GetWidth(); x++)
		{
			if (mIsTarget(x, y) == true)
			{
				if (mObject(x, y) != OBJ_BLOCK)
				{
					return false;
				}
			}
		}
	}

	return true;
}

bool Game::GetIsEnd()
{
	return mIsEnd;
}

bool Game::GetIsRestart()
{
	return mIsRestart;
}

void Game::DrawBox(int x, int y, int width, int height)
{

	if (width > height)
	{
		width = height;
	}
	else if (height > width)
	{
		height = width;
	}

	SDL_Rect Box{
		x+(x * width),
		y+(y * height),
		width,
		height,
	};

	SDL_RenderFillRect(mRenderer, &Box);
}
