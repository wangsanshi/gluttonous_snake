// gluttonous_snake.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

#include <stdio.h>
#include <windows.h>          //光标设置的api
#include <stdlib.h>
#include <time.h>             //食物随机
#include <conio.h>            //按键监控

//---------------------------数据设计---------------------------

//地图的宽度
#define MAP_WIDTH 80
//地图的高度
#define MAP_HEIGHT 26
//蛇最大的长度
#define SNAKE_MAX_SIZE 53

//食物
struct Food 
{
	int x;
	int y;
};

Food food;

//蛇
struct Snake
{
	//因为蛇是由很多点构成，所以用两个一维数组来保存蛇的x、y坐标
	int x[SNAKE_MAX_SIZE];
	int y[SNAKE_MAX_SIZE];
	//蛇的长度
	int length;
	//蛇的移动速度
	int speed;
};

Snake snake;

//蛇初始移动方向
int key = 'A';
//蛇变化的标记
int changeFlag = 0;
//姓名
char name[20];
//分数
int score = 0;

//-------------------------------------------------------------


//---------------------------模块设计---------------------------

//1.欢迎界面
void welcome();
//2.绘制地图
void drawMap();
//3.产生食物
bool createFood();
//4.按键操作
void keyDown();
//5.蛇状态监控，用来判断是否结束游戏
int snakeStatus();
//6.辅助函数--光标移动
void gotoXY(int, int);
//7.结束界面
void finish();

//-------------------------------------------------------------

int main()
{
	system("color 0B");
	welcome();
	drawMap();

	while (true) {
		if (!createFood()) {
			break;
		}
		Sleep(snake.speed);
		keyDown();

		if (!snakeStatus()) {
			break;
		}
	}

	finish();

	return 0;
}

/*
 * ■  蛇身
 * ⊙ 食物
 */
void drawMap() {
	//随机函数种子
	srand((unsigned int)time(NULL));

	//1.确定一个范围，绘制地图边界
	for (int i = 0; i <= MAP_HEIGHT;i++) {
		gotoXY(0, i);
		printf("■");
		gotoXY(MAP_WIDTH, i);
		printf("■");
	}
	//i+=2 是因为 ■ 字符占用两个字节
	for (int i = 0; i <= MAP_WIDTH;i+=2) {
		gotoXY(i, 0);
		printf("■");
		gotoXY(i,MAP_HEIGHT);
		printf("■");
	}

	gotoXY(85, 10);
	printf("Hello %s,Welcome to play", name);
	gotoXY(85, 15);
	printf("Your score is : %d", score);
	gotoXY(85, 20);
	printf("This game is created from bilibili");

	//2.绘制蛇
	//2.1确定蛇的属性
	snake.length = 3;//初始为3节
	snake.speed = 300;//初始速度
	//2.2蛇的头部在屏幕中间
	snake.x[0] = MAP_WIDTH / 2;
	snake.y[0] = MAP_HEIGHT / 2;
	//2.3绘制蛇头
	gotoXY(snake.x[0], snake.y[0]);
	printf("■");
	//2.4绘制身体
	for (int i = 1; i < snake.length;i++) {
		snake.x[i] = snake.x[i - 1] + 2;//后一节的x坐标等于前一节x坐标+2(2表示■占两个字符)
		snake.y[i] = snake.y[i - 1];
		gotoXY(snake.x[i], snake.y[i]);
		printf("■");
	}

	//3.绘制食物，食物的x坐标必须为偶数
	while (true){
		food.x = rand() % (MAP_WIDTH - 4) + 2;
		food.y = rand() % (MAP_HEIGHT - 2) + 1;

		if ((food.x & 1) == 0) {
			break;
		}
	}
	gotoXY(food.x, food.y);
	printf("⊙");
}

bool createFood() {
	//蛇把食物吃了，会产生新的食物
	if (snake.x[0] == food.x && snake.y[0] == food.y) {
		srand((unsigned int)time(NULL));

		//产生的食物不能在蛇的身上，并且其x坐标只能为偶数
		while (1) {
			food.x = rand() % (MAP_WIDTH - 4) + 2;
			food.y = rand() % (MAP_HEIGHT - 2) + 1;

			//如果该食物的x坐标为偶数，则说明该食物不合法，需重新产生
			if ((food.x & 1) == 1) {
				continue;
			}	

			bool flag = true;

			for (int i = 0; i < snake.length;i++) {
				if ((snake.x[i] == food.x) && (snake.y[i] == food.y)) {
					flag = false;
					break;
				}
			}

			if (flag) {
				break;
			}
		}

		gotoXY(food.x, food.y);
		printf("⊙");

		//吃了食物后，蛇的长度+1
		snake.length++;
		score += 20;
		gotoXY(85, 15);
		printf("Your score is : %d", score);

		if (snake.length >= 53) {
			return false;
		}

		changeFlag = 1;//蛇的标记是1
		return true;
	}
}

void keyDown() {
	if (_kbhit()) {
		//有按键
		fflush(stdin);
		key = _getch();
	}

	//擦除蛇身体的最后一节
	if (!changeFlag) {
		gotoXY(snake.x[snake.length - 1], snake.y[snake.length - 1]);
		printf("  ");
	}
	for (int i = snake.length - 1; i > 0;i--) {
		snake.x[i] = snake.x[i - 1];
		snake.y[i] = snake.y[i - 1];
	}

	//移动方向的处理
	switch (key){
		case 'w':
		case 'W':
			snake.y[0]--;
			break;
		case 's':
		case 'S':
			snake.y[0]++;
			break;
		case 'a':
		case 'A':
			snake.x[0] -= 2;
			break;
		case 'd':
		case 'D':
			snake.x[0] += 2;
			break;

		default:
			break;
	}
	gotoXY(snake.x[0], snake.y[0]);
	printf("■");
	changeFlag = 0;
	gotoXY(MAP_HEIGHT + 2, 0);//移动光标
}

int snakeStatus() {
	if (snake.x[0] == 2 || snake.x[0] == MAP_WIDTH - 2 
		|| snake.y[0] == 0 || snake.y[0] == MAP_HEIGHT - 1) {
		return 0;
	}

	//蛇头不能撞自己
	for (int i = 1; i < snake.length;i++) {
		if (snake.x[0] == snake.x[i] && snake.y[0] == snake.y[i]) {
			return 0;
		}
	}

	return 1;
}

void gotoXY(int x, int y) {
	//1.找到控制台的窗口
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	//2.光标的结构体
	COORD coord;
	//3.设置坐标
	coord.X = x;
	coord.Y = y;
	//4.同步到控制台
	SetConsoleCursorPosition(handle,coord);
}

void welcome() {
	gotoXY(15, 10);
	printf("/**********************************************************/");
	gotoXY(15, 20);
	printf("/**********************************************************/");
	gotoXY(18, 13);
	printf("Welcome to the game of retro snake");
	gotoXY(18, 15);
	printf("请在英文输入法中操作，反向移动等同于吃到自己，wasd控制");
	gotoXY(18, 17);
	printf("Please enter your name : ");
	scanf_s("%s", &name, 10);
	system("cls");//清屏
}

void finish()
{
	system("cls");
	gotoXY(15, 10);
	printf("/**********************************************************/");
	gotoXY(15, 20);
	printf("/**********************************************************/");
	gotoXY(25, 13);
	printf("GAME   OVER      o(*￣▽￣*)o");
	gotoXY(25, 15);
	printf("Your score is %d ", score);
	gotoXY(25, 17);
	printf("还不错哦，     继续努力O(∩_∩)O");
	gotoXY(0, 27);
	system("pause");
}