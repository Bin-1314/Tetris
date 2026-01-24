#include "Tetris.h"
#include<ctime>
#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include<graphics.h>
#include<tchar.h>
#include<string>
#include<string.h>
#include<fstream>
#include<wchar.h>
#include"Block.h"

#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")

#define MAX_LEVEL 5
#define RECORDER_FILE "recorder.txt"

using namespace std;
//const int SPEED_NORMAL = 500; //ms
const int SPEED_NORMAL[MAX_LEVEL] = { 500,300,150,100,80 };
const int SPEED_QUICK = 30;


Tetris::Tetris(int rows, int cols, int left, int top, int blockSize)
{
	this->rows = rows;
	this->cols = cols;
	this->leftMargin = left;
	this->topMargin = top;
	this->blockSize = blockSize;

	for (int i = 0; i < rows; i++)
	{
		vector<int>mapRow;
		for (int j = 0; j < cols; j++)
		{
			mapRow.push_back(0);
		}
		map.push_back(mapRow);
	}

}

void Tetris::init()
{
	mciSendString(_T("play res/bg.mp3 repeat"), 0, 0, 0);

	delay = SPEED_NORMAL[0];

	//配置随机数种子
	srand(time(NULL));


	//创建游戏窗口
	initgraph(938,896);
	//initgraph(938, 896, EW_NOCLOSE | EW_NOMINIMIZE | EW_NOMAXIMIZE);
	//initgraph(938, 896, EW_NOCLOSE | EW_NOMINIMIZE);
	//initgraph(938, 896, SHOWCONSOLE);
	//initgraph(938, 896,NULL);

	//HWND hwnd = GetHWnd();
	//SetWindowPos(hwnd, HWND_TOP, 0, 0, 938, 896, SWP_SHOWWINDOW);

	//加载背景图片
	loadimage(&imgBg, _T("res/bg2.png"));

	loadimage(&imgOver, _T("res/over.png"));
	loadimage(&imgWin, _T("res/win.png"));

	//初始化游戏区的数据、
	char data[20][10];
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			map[i][j] = 0;
		}
	}
	score = 0;
	level = 1;
	lineCount = 0;
	highestScore_updateFlag = 0;

	ifstream file(RECORDER_FILE);
	if (!file.is_open())
	{
		cout << RECORDER_FILE << "打开失败" << endl;
		highestScore = 0;
	}
	else
	{
		file >> highestScore;
	}
	//if (highestScore_updateFlag == 1)
	//{
	//	ofstream file(RECORDER_FILE);
	//	file << highestScore << endl;
	//	file.close();
	//}
	file.close();


	gameOver = false;

}

void Tetris::play()
{
	init();

	nextBlock = new Block;
	curBlock = nextBlock;
	nextBlock = new Block;


	int timer = 0;


	while (1)
	{
		//接受用户输入
		keyEvent();

		timer += getDelay();

		if (timer > delay)
		{
			timer = 0;
			drop();
			//渲染游戏画面
			update = true;
			

		}
		if (update)
		{
			update = false;
			updateWindow();

			clearLine();

		}
		if (gameOver)
		{
			updateHighestScore();
			displayOver();
			system("pause");
			//getchar();
			init();
		}
	}
}

void Tetris::keyEvent()
{
	unsigned char ch;
	bool rotateFlag = false;
	int dx = 0;
	if (_kbhit())
	{
		ch = _getch();
		if (ch == 224)
		{
			ch = _getch();
			switch (ch)
			{
			case 72:
				rotateFlag = true;
				break;
			case 80:
				delay = SPEED_QUICK;
				break;
			case 75:
				dx = -1;
				break;
			case 77:
				dx = 1;
				break;
			default:
				break;
			}
		}
	}
	if (rotateFlag)
	{
		//todo
		//
		rotate();
		update = true;
	}

	
	if (dx != 0)
	{
		moveLeftRight(dx);
		update = true;
	}

}

void Tetris::updateWindow()
{
	putimage(0, 0, &imgBg);//绘制背景图片

	IMAGE** imgs = Block::getImages();
	BeginBatchDraw();
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (map[i][j] == 0)
				continue;
			int x = j * blockSize + leftMargin;
			int y = i * blockSize + topMargin;
			putimage(x, y, imgs[map[i][j] - 1]);
		}
	}

	curBlock->draw(leftMargin, topMargin);
	nextBlock->draw(689, 150);

	drawScore();
	updateHighestScore();

	EndBatchDraw();
}

int Tetris::getDelay()
{
	static unsigned long long lastTime = 0;

	unsigned long long currentTime = GetTickCount();
	if (lastTime == 0)
	{
		lastTime = currentTime;
		return 0;
	}
	else
	{
		int ret = currentTime - lastTime;
		lastTime = currentTime;
		return ret;
	}
	
}

void Tetris::drop()
{
	bakBlock = *curBlock;
	curBlock->drop();

	if (!curBlock->blockInMap(map))
	{
		bakBlock.solidify(map);
		delete curBlock;
		curBlock = nextBlock;
		nextBlock = new Block;


		checkOver();
	}
	delay = SPEED_NORMAL[level - 1];
}

void Tetris::clearLine()
{
	int lines = 0;
	int k = rows - 1;
	for (int i = rows -1; i >= 0; i--)
	{
		int count = 0;
		for (int j = 0; j < cols; j++)
		{
			if (map[i][j])
			{
				count++;
			}
			map[k][j] = map[i][j];
		}
		if (count < cols)
		{
			k--;
		}
		else
		{
			lines++;
		}
		
	}
	if (lines > 0)
	{
		//计算得分
		//to do.
		int addScore[4] = { 10,30,60,80 };

		score += addScore[lines - 1];
		if (score > highestScore)
		{
			highestScore = score;
			highestScore_updateFlag = 1;
		}

		mciSendString(_T("play res/xiaochu1.mp3"), 0, 0, 0);
		update = true;

		level = (score + 99) / 100;
		if (level > MAX_LEVEL)
		{
			gameOver = true;
		}

		lineCount += lines;
	}


}

void Tetris::moveLeftRight(int offset)
{
	bakBlock = *curBlock;
	curBlock->moveLeftRight(offset);
	if (!curBlock->blockInMap(map))
	{
		*curBlock = bakBlock;
	}
}

void Tetris::rotate()
{
	if (curBlock->getBlockType() == 7)
	{
		return;
	}
	bakBlock = *curBlock;
	curBlock->rotate();
	if (!curBlock->blockInMap(map))
	{
		*curBlock = bakBlock;
	}
}

void Tetris::drawScore()
{
	wchar_t scoreText[32];
	swprintf_s(scoreText, sizeof(scoreText)/sizeof(wchar_t), L"%d", score);
	setcolor(RGB(180,180,180));
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 60;
	f.lfWidth = 30;
	f.lfQuality = NONANTIALIASED_QUALITY;
	wcscpy_s(f.lfFaceName ,sizeof(f.lfFaceName)/ sizeof(wchar_t),_T("Segoe UI Black"));
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	outtextxy(670, 730, scoreText);

	swprintf_s(scoreText, sizeof(scoreText) / sizeof(wchar_t), L"%d", lineCount);
	gettextstyle(&f);
	int xPos = 224 - f.lfWidth * wcslen(scoreText);
	outtextxy(xPos, 817, scoreText);

	swprintf_s(scoreText, sizeof(scoreText) / sizeof(wchar_t), L"%d", level);
	gettextstyle(&f);
	xPos = 224 - f.lfWidth;
	outtextxy(xPos, 730, scoreText);

	swprintf_s(scoreText, sizeof(scoreText) / sizeof(wchar_t), L"%d", highestScore);
	outtextxy(670, 817, scoreText);


}

void Tetris::drawLineCount()
{

}

void Tetris::drawLevel()
{
	
}

void Tetris::updateHighestScore()
{
	if (highestScore_updateFlag == 1)
	{
		ofstream file(RECORDER_FILE);
		file << highestScore << endl;
		file.close();
	}
}

void Tetris::checkOver()
{
	gameOver = (curBlock->blockInMap(map) == false);

}

void Tetris::displayOver()
{
	mciSendString(_T("stop res/bg.mp3"), 0, 0, 0);
	if (level <= MAX_LEVEL)
	{
		putimage(262, 361, &imgOver);
		mciSendString(_T("play res/over.mp3"), 0, 0, 0);
	}
	else
	{
		putimage(262, 361, &imgWin);
		mciSendString(_T("play res/win.mp3"), 0, 0, 0);
	}
}

