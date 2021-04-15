#include<iostream>
#include<fstream>
using namespace std;

//�Q�[���I�u�W�F�N�g
enum Object
{
	OBJ_SPACE,
	OBJ_PLAYER,
	OBJ_BLOCK,
	OBJ_WALL,
	OBJ_UNNKOWN,
};
void ReadFile(const char* fileName, char** buffer, int& size);
//������
void Init(const char* data, Object* obj, bool* IsGoal, const int size, int& width, int& height);
//�X�V����
void Update(Object* obj, char input, const int width, const int height);
//�o�͏���
void Show(Object* obj, bool* IsGoal, const int width, const int height);
//�N���A����
bool IsClear(Object* obj, bool* IsGoal, const int width, const int height);

int main(int argc, char** argv)
{
_RESTART:

	char* buffer = nullptr;
	Object* obj = nullptr;//�I�u�W�F�N�g�f�[�^�̕ۑ�
	bool* IsGoal = nullptr;//�������S�[�����ǂ����ۑ�
	int width = 0;//�X�e�[�W�̕�
	int height = 0;//�X�e�[�W�̍���
	int size = 0;//�X�e�[�W�f�[�^�̃f�[�^�T�C�Y

	ReadFile("stageData.txt", &buffer, size);

	//������
	obj = new Object[size];
	IsGoal = new bool[size];
	Init(buffer, obj, IsGoal, size, width, height);

	delete[] buffer;
	buffer = nullptr;
	while (true)
	{
		bool IsEnd = false;
		char input;

		//�o�͏���
		Show(obj, IsGoal, width, height);

		//�N���A����
		if (IsClear(obj, IsGoal, width, height))
		{
			std::cout << "�N���A�I\n";


			//���X�^�[�g���邩�ǂ������߂�
			do
			{
				std::cout << "������x�v���C���܂����H\n";
				std::cin >> input;
				//���X�^�[�g����ꍇ
				if (input == 'y' || input == 'Y')
				{
					std::cout << "���X�^�[�g���܂�...\n";
					delete[] obj;
					delete[] IsGoal;
					obj = nullptr;
					IsGoal = nullptr;
					goto _RESTART;
				}
				//���X�^�[�g���Ȃ��ꍇ(�I������ꍇ)
				else if (input == 'n' || input == 'N')
				{
					IsEnd = true;
					break;
				}
				else
				{
					std::cout << "������x���͂��Ă��������B\n";
				}

			} while (true);

		}
		//�I�����Ȃ��ꍇ��
		if (!IsEnd)
		{
			std::cout << "a:Left d:Right w:Up s:Down\n";
			std::cin >> input;
		}
		//�I������ꍇ
		if (input == 'Q' || input == 'q' || IsEnd)
		{
			std::cout << "�I�����܂�...\n";
			break;
		}
		else if (input == 'R' || input == 'r')
		{
			std::cout << "���X�^�[�g���܂�...\n";
			delete[] obj;
			delete[] IsGoal;
			obj = nullptr;
			IsGoal = nullptr;
			goto _RESTART;
		}
		//�X�V����
		Update(obj, input, width, height);
	}
	
	//�I������ꍇ�͕K���������J������
	delete[] obj;
	delete[] IsGoal;
	obj = nullptr;
	IsGoal = nullptr;
}

void ReadFile(const char* fileName, char** buffer, int& size)
{
	ifstream file(fileName);

	//�t�@�C�����J���̂Ɏ��s�����ꍇ�̏���
	if (!file.is_open())
	{
		std::cerr << "�t�@�C�����J���̂Ɏ��s���܂����B\n";
		exit(EXIT_FAILURE);
	}

	file.seekg(0, ifstream::end);
	//�t�@�C���ʒu = �T�C�Y
	size = static_cast<int>(file.tellg());
	*buffer = new char[size];

	file.seekg(0, ifstream::beg);

	file.read(*buffer, size);

	if (!buffer)
	{
		std::cout << "�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B\n";
		exit(EXIT_FAILURE);
	}

}

void Init(const char* data, Object* obj, bool* IsGoal, const int size, int& width, int& height)
{
	int x = 0;
	int y = 0;

	int a = 0;

	for (int i = 0; i < size; i++)
	{
		Object o = OBJ_UNNKOWN;
		bool b = false;
		//�X�e�[�W�f�[�^�̓ǂݍ��݂Ƃ��̏��̎擾
		switch (data[i])
		{
		case ' ':
			o = OBJ_SPACE;
			b = false;
			x++;
			break;
		case '.':
			o = OBJ_SPACE;
			b = true;
			x++;
			break;
		case'#':
			o = OBJ_WALL;
			b = false;
			x++;
			break;
		case 'o':
			o = OBJ_BLOCK;
			b = false;
			x++;
			break;
		case'p':
			o = OBJ_PLAYER;
			b = false;
			x++;
			break;
		case'O':
			o = OBJ_BLOCK;
			b = true;
			x++;
			break;
		case 'P':
			o = OBJ_PLAYER;
			b = true;
			x++;
			break;
		case'\n':
			y++;
			break;
		default:
			//�������Ȃ�
			break;
		}
		//�I�u�W�F�N�g���w�肵�Ă�����̂ł������͑��
		if (o != OBJ_UNNKOWN)
		{
			obj[a] = o;
			IsGoal[a] = b;
			++a;
		}

	}
	//���A�����̐ݒ�
	height = y;
	x /= y;//���̎Z�o
	width = x;

}

void Update(Object* obj, char input, const int width, const int height)
{
	//�v���C���[�̍��W
	int y = 0;
	int x = 0;
	//�v���C���[�̍��W����
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			if (obj[y * width + x] == OBJ_PLAYER)
			{
				goto _BREAK;
			}
		}
	}
_BREAK:
	//�ړ����̃x�N�g��
	int vx = 0;
	int vy = 0;
	switch (input)
	{
	case 'w':
		vy -= 1;
		break;
	case 's':
		vy += 1;
		break;
	case 'a':
		vx -= 1;
		break;
	case 'd':
		vx += 1;
		break;
		//�ςȂ��̂��������ꍇ�͖����Ƃ���
	default:
		return;
		break;
	}

	//���̃v���C���[�̍��W
	int ty = y + vy;
	int tx = x + vx;

	//���A�����𒴂��Ă��Ȃ����`�F�b�N
	if ((ty < 0 || height <= ty) || (tx < 0 || width <= tx))
	{
		return;
	}
	//�ǂɂԂ����Ă��Ȃ����`�F�b�N
	else if (obj[ty * width + tx] == OBJ_WALL)
	{
		return;
	}
	//���̍��W���u���b�N�ł���ꍇ
	else if (obj[ty * width + tx] == OBJ_BLOCK)
	{
		//�u���b�N�̉����ꂽ�ꍇ�̍��W
		int bx = tx + vx;
		int by = ty + vy;
		//���A�����𒴂��Ă��Ȃ����`�F�b�N
		if ((by < 0 || height <= by) || (bx < 0 || width <= bx))
		{
			return;
		}
		//�ǂɂԂ����Ă��Ȃ����`�F�b�N
		if (obj[by * width + bx] == OBJ_WALL)
		{
			return;
		}
		//�ړ�����
		else
		{
			obj[by * width + bx] = OBJ_BLOCK;
			obj[ty * width + tx] = OBJ_PLAYER;
			obj[y * width + x] = OBJ_SPACE;
		}
	}
	//�ړ�����
	else
	{
		obj[ty * width + tx] = OBJ_PLAYER;
		obj[y * width + x] = OBJ_SPACE;
	}

}

//�o�͏���
void Show(Object* obj, bool* IsGoal, const int width, const int height)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			char c = NULL;
			switch (obj[y * width + x])
			{
			case OBJ_SPACE:
				c = (IsGoal[y * width + x]) ? '.' : ' ';
				break;
			case OBJ_WALL:
				c = (IsGoal[y * width + x]) ? '#' : '#';
				break;
			case OBJ_BLOCK:
				c = (IsGoal[y * width + x]) ? 'O' : 'o';
				break;
			case OBJ_PLAYER:
				c = (IsGoal[y * width + x]) ? 'P' : 'p';
				break;
			default:
				//�������Ȃ�
				break;
			}
			//c�ɉ��������Ă��Ȃ�(������NULL�̂܂܂�)�ꍇ�ȊO�͏o��
			if (c != NULL)
			{
				printf("%c", c);
			}
		}
		//���S�Ă��s��������s
		printf("\n");
	}

}

//�N���A����
bool IsClear(Object* obj, bool* IsGoal, const int width, const int height)
{
	//�S�ẴS�[���Ƀu���b�N������Ă��Ȃ��ꍇ��false����ȊO��true
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (IsGoal[y * width + x] == true)
			{
				if (obj[y * width + x] != OBJ_BLOCK)
				{
					return false;
				}
			}
		}
	}

	return true;
}
