#pragma once
#include<vector>
#include<graphics.h>
#include "Block.h"


using namespace std;
class Tetris
{
public:
	Tetris(int rows,int cols,int left,int top,int blockSize);

	void init();

	void play();

private:
	void keyEvent();
	void updateWindow();
	int getDelay();
	void drop();
	void clearLine();
	void moveLeftRight(int offset);
	void rotate();
	void drawScore();
	void drawLineCount();
	void drawLevel();
	void updateHighestScore();
	void checkOver();
	void displayOver();


private:
	int delay;
	bool update;

	vector<vector<int>>map;

	int rows;
	int cols;
	int leftMargin;
	int topMargin;
	int blockSize;
	IMAGE imgBg;
	IMAGE imgOver;
	IMAGE imgWin;

	Block* curBlock;
	Block* nextBlock;
	Block bakBlock;

	int score;
	int highestScore;
	int highestScore_updateFlag;
	int level;
	int lineCount;
	bool gameOver;
};

