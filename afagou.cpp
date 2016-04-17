#include "stdafx.h"
#include "afagou.h"

weiqi::weiqi()
{
	for (int i = 0; i < RAW; i++)
	for (int j = 0; j < COLUMN; j++)
		qipan[i][j] = background;
	winner = '0';
}
void weiqi::move(int raw, int col, char colour)
{
	cout << raw << endl << col << endl;
	qipan[raw][col] = colour;
}
void weiqi::whowin(int raw, int col, char colour)
{
	int r1, r2, c1, c2;
	r1 = r2 = raw;
	c1 = c2 = col;
	int count = 1;
	//竖直方向
	r1--;
	while (qipan[r1][col] == colour)
	{
		count++;
		r1--;
	}
	r2++;

	while (qipan[r2][col] == colour)
	{
		count++;
		r2++;
	}
	if (count == 5)
	{
		if (colour == '*')
			winner = '1';
		else
			winner = '2';
		return;
	}
	////水平方向
	r1 = r2 = raw;
	c1 = c2 = col;
	count = 1;
	c1--;
	while (qipan[raw][c1] == colour)
	{
		count++;
		c1--;
	}
	c2++;
	while (qipan[raw][c2] == colour)
	{
		count++;
		c2++;
	}
	if (count == 5)
	{
		if (colour == '*')
			winner = '1';
		else
			winner = '2';
		return;
	}
	//左上右下方向
	r1 = r2 = raw;
	c1 = c2 = col;
	count = 1;
	r1--; c1--;
	while (qipan[r1][c1] == colour)
	{
		count++; c1--;
		r1--; c1--;
	}
	r2++;
	while (qipan[r2][c2] == colour)
	{
		count++;
		r2++; c2++;
	}
	if (count == 5)
	{
		if (colour == '*')
			winner = '1';
		else
			winner = '2';
		return;
	}
	//左下右上方向
	r1 = r2 = raw;
	c1 = c2 = col;
	count = 1;
	r1--; c1++;
	while (qipan[r1][c1] == colour)
	{
		count++;
		r1--; c1++;
	}
	r2++; c2--;
	while (qipan[r2][c2] == colour)
	{
		count++;
		r2++; c2--;
	}
	if (count == 5)
	{
		if (colour == '*')
			winner = '1';
		else
			winner = '2';
		return;
	}
}
void weiqi::show()
{
	cout << "棋谱:" << endl << ' ';
	for (int i = 0; i < RAW * 2; i++)
		cout << '-';
	cout << endl;
	for (int i = 0; i < RAW; i++)
	{
		cout << '|';
		for (int j = 0; j < COLUMN; j++)
			cout << qipan[i][j] << " ";
		cout << '|' << endl;
	}
	cout << ' ';
	for (int i = 0; i < RAW * 2; i++)
		cout << '-';
}
weiqi::~weiqi()
{

}