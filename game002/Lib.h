#pragma once
#include<fstream>
#include<iostream>
namespace
{
	enum Object
	{
		OBJ_SPACE,
		OBJ_PLAYER,
		OBJ_BLOCK,
		OBJ_WALL,
		OBJ_UNNKOWN,
	};
	void ReadFile(const char* fileName, char** buffer, int& size)
	{
		std::ifstream file(fileName);

		//ファイルを開くのに失敗した場合の処理
		if (!file.is_open())
		{
			std::cerr << "ファイルを開くのに失敗しました。\n";
			exit(EXIT_FAILURE);
		}

		file.seekg(0, std::ifstream::end);
		//ファイル位置 = サイズ
		size = static_cast<int>(file.tellg());
		*buffer = new char[size];

		file.seekg(0, std::ifstream::beg);

		file.read(*buffer, size);

		if (!buffer)
		{
			std::cerr << "ファイルの読み込みに失敗しました。\n";
			exit(EXIT_FAILURE);
		}

	}
	//文字数検索
	int FindChar(char* str, char cha)
	{
		int count = 0;
		for (int i = 0; str[i] != '\0'; i++)
		{
			if (str[i] == cha)
			{
				++count;
			}
		}

		return count;
	}
}