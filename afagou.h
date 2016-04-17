/*
'-'表示空
‘*’表示白棋
‘+’表示黑棋
*/
#include "stdafx.h"
const int RAW = 19;
const int COLUMN = 19;
const char  background = '-';
const char  white = '*';
const char  black = '+';
using namespace std;
class weiqi{
public:
	char qipan[RAW][COLUMN];
	char winner;
public:
	weiqi();
	void move(int raw, int col, char colour);
	void whowin(int raw, int col, char colour);
	void show();
	~weiqi();
};