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

		//�t�@�C�����J���̂Ɏ��s�����ꍇ�̏���
		if (!file.is_open())
		{
			std::cerr << "�t�@�C�����J���̂Ɏ��s���܂����B\n";
			exit(EXIT_FAILURE);
		}

		file.seekg(0, std::ifstream::end);
		//�t�@�C���ʒu = �T�C�Y
		size = static_cast<int>(file.tellg());
		*buffer = new char[size];

		file.seekg(0, std::ifstream::beg);

		file.read(*buffer, size);

		if (!buffer)
		{
			std::cerr << "�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B\n";
			exit(EXIT_FAILURE);
		}

	}
	//����������
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