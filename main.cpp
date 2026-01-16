#include "Tetris.h"
#include<iostream>
#include<conio.h>
#include<windows.h>
using namespace std;

int main()
{
	//测试git功能
	//继续测试
	//nihao

	Tetris game(20, 10, 263, 133, 36);

	game.play();



	/*while (1)
	{
		unsigned char ch;
		bool rotateFlag = false;
		int dx = 0;
		if (_kbhit() != 0)
		{
			ch = _getch();
			if (ch == 224)
			{
				ch = _getch();
				switch (ch)
				{
				case 72:
					cout << "上箭头" << endl;
					break;
				case 80:
					cout << "下箭头" << endl;
					break;
				case 75:
					cout << "左箭头" << endl;
					break;
				case 77:
					cout << "右箭头" << endl;
					break;
				default:
					break;
				}
			}
		}
	}*/



	return 0;

}

