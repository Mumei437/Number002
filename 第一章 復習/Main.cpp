#include<iostream>
#include<fstream>
using namespace std;

//ゲームオブジェクト
enum Object
{
	OBJ_SPACE,
	OBJ_PLAYER,
	OBJ_BLOCK,
	OBJ_WALL,
	OBJ_UNNKOWN,
};
void ReadFile(const char* fileName, char** buffer, int& size);
//初期化
void Init(const char* data, Object* obj, bool* IsGoal, const int size, int& width, int& height);
//更新処理
void Update(Object* obj, char input, const int width, const int height);
//出力処理
void Show(Object* obj, bool* IsGoal, const int width, const int height);
//クリア判定
bool IsClear(Object* obj, bool* IsGoal, const int width, const int height);

int main(int argc, char** argv)
{
_RESTART:

	char* buffer = nullptr;
	Object* obj = nullptr;//オブジェクトデータの保存
	bool* IsGoal = nullptr;//そこがゴールかどうか保存
	int width = 0;//ステージの幅
	int height = 0;//ステージの高さ
	int size = 0;//ステージデータのデータサイズ

	ReadFile("stageData.txt", &buffer, size);

	//初期化
	obj = new Object[size];
	IsGoal = new bool[size];
	Init(buffer, obj, IsGoal, size, width, height);

	delete[] buffer;
	buffer = nullptr;
	while (true)
	{
		bool IsEnd = false;
		char input;

		//出力処理
		Show(obj, IsGoal, width, height);

		//クリア判定
		if (IsClear(obj, IsGoal, width, height))
		{
			std::cout << "クリア！\n";


			//リスタートするかどうか決める
			do
			{
				std::cout << "もう一度プレイしますか？\n";
				std::cin >> input;
				//リスタートする場合
				if (input == 'y' || input == 'Y')
				{
					std::cout << "リスタートします...\n";
					delete[] obj;
					delete[] IsGoal;
					obj = nullptr;
					IsGoal = nullptr;
					goto _RESTART;
				}
				//リスタートしない場合(終了する場合)
				else if (input == 'n' || input == 'N')
				{
					IsEnd = true;
					break;
				}
				else
				{
					std::cout << "もう一度入力してください。\n";
				}

			} while (true);

		}
		//終了しない場合に
		if (!IsEnd)
		{
			std::cout << "a:Left d:Right w:Up s:Down\n";
			std::cin >> input;
		}
		//終了する場合
		if (input == 'Q' || input == 'q' || IsEnd)
		{
			std::cout << "終了します...\n";
			break;
		}
		else if (input == 'R' || input == 'r')
		{
			std::cout << "リスタートします...\n";
			delete[] obj;
			delete[] IsGoal;
			obj = nullptr;
			IsGoal = nullptr;
			goto _RESTART;
		}
		//更新処理
		Update(obj, input, width, height);
	}
	
	//終了する場合は必ずメモリ開放する
	delete[] obj;
	delete[] IsGoal;
	obj = nullptr;
	IsGoal = nullptr;
}

void ReadFile(const char* fileName, char** buffer, int& size)
{
	ifstream file(fileName);

	//ファイルを開くのに失敗した場合の処理
	if (!file.is_open())
	{
		std::cerr << "ファイルを開くのに失敗しました。\n";
		exit(EXIT_FAILURE);
	}

	file.seekg(0, ifstream::end);
	//ファイル位置 = サイズ
	size = static_cast<int>(file.tellg());
	*buffer = new char[size];

	file.seekg(0, ifstream::beg);

	file.read(*buffer, size);

	if (!buffer)
	{
		std::cout << "ファイルの読み込みに失敗しました。\n";
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
		//ステージデータの読み込みとその情報の取得
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
			//何もしない
			break;
		}
		//オブジェクトが指定しているものである限りは代入
		if (o != OBJ_UNNKOWN)
		{
			obj[a] = o;
			IsGoal[a] = b;
			++a;
		}

	}
	//幅、高さの設定
	height = y;
	x /= y;//幅の算出
	width = x;

}

void Update(Object* obj, char input, const int width, const int height)
{
	//プレイヤーの座標
	int y = 0;
	int x = 0;
	//プレイヤーの座標検索
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
	//移動分のベクトル
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
		//変なものが入った場合は無効とする
	default:
		return;
		break;
	}

	//次のプレイヤーの座標
	int ty = y + vy;
	int tx = x + vx;

	//幅、高さを超えていないかチェック
	if ((ty < 0 || height <= ty) || (tx < 0 || width <= tx))
	{
		return;
	}
	//壁にぶつかっていないかチェック
	else if (obj[ty * width + tx] == OBJ_WALL)
	{
		return;
	}
	//次の座標がブロックである場合
	else if (obj[ty * width + tx] == OBJ_BLOCK)
	{
		//ブロックの押された場合の座標
		int bx = tx + vx;
		int by = ty + vy;
		//幅、高さを超えていないかチェック
		if ((by < 0 || height <= by) || (bx < 0 || width <= bx))
		{
			return;
		}
		//壁にぶつかっていないかチェック
		if (obj[by * width + bx] == OBJ_WALL)
		{
			return;
		}
		//移動処理
		else
		{
			obj[by * width + bx] = OBJ_BLOCK;
			obj[ty * width + tx] = OBJ_PLAYER;
			obj[y * width + x] = OBJ_SPACE;
		}
	}
	//移動処理
	else
	{
		obj[ty * width + tx] = OBJ_PLAYER;
		obj[y * width + x] = OBJ_SPACE;
	}

}

//出力処理
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
				//何もしない
				break;
			}
			//cに何も入っていない(初期のNULLのままの)場合以外は出力
			if (c != NULL)
			{
				printf("%c", c);
			}
		}
		//幅全てを行ったら改行
		printf("\n");
	}

}

//クリア判定
bool IsClear(Object* obj, bool* IsGoal, const int width, const int height)
{
	//全てのゴールにブロックが乗っていない場合はfalseそれ以外はtrue
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
