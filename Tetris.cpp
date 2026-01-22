#include "Tetris.h"
#include<ctime>
#include<cstdlib>
#include<conio.h>
#include<windows.h>
#include"Block.h"

const int SPEED_NORMAL = 500; //ms
const int SPEED_QUICK = 50;


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
	delay = SPEED_NORMAL;

	//配置随机数种子
	srand(time(NULL));


	//创建游戏窗口
	initgraph(938,896);
	

	//加载背景图片
	loadimage(&imgBg, _T("res/bg2.png"));

	//初始化游戏区的数据、
	char data[20][10];
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			map[i][j] = 0;
		}
	}
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
	}
	delay = SPEED_NORMAL;
}

void Tetris::clearLine()
{



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

