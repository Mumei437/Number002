#pragma once
#include<vector>

template<typename TYPE>
class Array2D
{
public:
	Array2D();
	Array2D(int w, int h);
	~Array2D();

	int GetWidth() { return width; }
	int GetHeight() { return height; }

	void Add(int i, TYPE data);
	void SetSize(int w, int h);

	TYPE& operator()(int x, int y);
	const TYPE& operator()(int x, int y)const;


private:
	int width;
	int height;
	TYPE* mArray;
};

template<typename TYPE>
inline Array2D<TYPE>::Array2D()
	:
	width(0),
	height(0)
{
	mArray = nullptr;
}

template<typename TYPE>
inline Array2D<TYPE>::Array2D(int w, int h)
{
	this->width = w;
	this->height = h;
	mArray = new TYPE[w * h];

}

template<typename TYPE>
inline Array2D<TYPE>::~Array2D()
{
	delete[]mArray;
	mArray = nullptr;
}

template<typename TYPE>
inline void Array2D<TYPE>::Add(int i, TYPE data)
{
	if (0 <= i && i < (width * height))
	{
		mArray[i] = data;
	}
}

template<typename TYPE>
inline void Array2D<TYPE>::SetSize(int w, int h)
{
	width = w;
	height = h;
	mArray = new TYPE[w * h];
}

template<typename TYPE>
inline TYPE& Array2D<TYPE>::operator()(int x, int y)
{
	if (0 <= x && x < width && 0 <= y && y < height)
	{
		return mArray[width * y + x];
	}
	else
	{
		std::cerr << "書き込みエラーです。(" << x << "," << y << ")\n";
		exit(EXIT_FAILURE);
	}
}

template<typename TYPE>
inline const TYPE& Array2D<TYPE>::operator()(int x, int y) const
{
	if (0 <= x && x < width && 0 <= y && y < height)
	{
		return mArray[width * y + x];
	}
	else
	{
		std::cerr << "読み込みエラーです。\n";
		exit(EXIT_FAILURE);
	}
}
