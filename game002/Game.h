#pragma once
#include"SDL.h"
#include"Lib.h"
#include"Array2D.h"

struct Vector2D
{
	float x;
	float y;
};

class Game
{
public://主要関数
	Game();//コンストラクタ
	~Game();//デストラクタ
public://ヘルパー関数
	void Input();
	void Update();
	void Output();

	bool IsClear();

	bool GetIsEnd();
	bool GetIsRestart();

	void DrawBox(int x, int y, int width, int height);
private:

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	bool mIsEnd;
	bool mIsRestart;
	Uint32 mTicksCount;

private:
	Array2D<bool> mIsTarget;
	Array2D<Object> mObject;
	//Object* obj = nullptr;//オブジェクトデータの保存
	//bool* IsGoal = nullptr;//そこがゴールかどうか保存
	Vector2D PlayerVector;
	Vector2D PlayerPos;
	bool IsPressed;
};

