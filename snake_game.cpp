// snake_game.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <windows.h>
#include <cstdlib>
#include <conio.h> // A library for console input/output apis

#define SNAKE_MAX_LENGTH 20
#define SLEEP_TIME_MS 100

using namespace std;
bool gameOver = false;
enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN};
// Snake
class snake{
public:
	int snake_position_x[SNAKE_MAX_LENGTH];
	int snake_position_y[SNAKE_MAX_LENGTH];
	int tail_length;
	eDirecton dir;
	snake(){
		tail_length = 0;
		std::fill(snake_position_x, snake_position_x+SNAKE_MAX_LENGTH, -1);
		std::fill(snake_position_y, snake_position_y+SNAKE_MAX_LENGTH, -1);
		dir = STOP;
	}

	void random_init_position(){
		snake_position_x[0] = rand() % SNAKE_MAX_LENGTH;
		snake_position_y[0] = rand() % SNAKE_MAX_LENGTH;
	}

};
// Playground
class game{
public:
	char play_ground[SNAKE_MAX_LENGTH][SNAKE_MAX_LENGTH];
	char play_ground_temp[SNAKE_MAX_LENGTH][SNAKE_MAX_LENGTH];
	int fruitX, fruitY;
	int score;
	game(){
		for (int i = 0; i<SNAKE_MAX_LENGTH; i++)
			for (int j = 0; j<SNAKE_MAX_LENGTH; j++)
			{
				play_ground[i][j] = '.';
			}
	}

	void game_setup(snake &snake_object){
		snake_object.random_init_position();
		fruitX = rand() % SNAKE_MAX_LENGTH;
		fruitY = rand() % SNAKE_MAX_LENGTH;
		score = 0;
	}

	void draw(const snake &snake_object){
		system("cls");
		// First bar
		for (int i = 0; i<SNAKE_MAX_LENGTH+2; i++)
			std::cout << "#";
		std::cout << endl;

		//Get a copy from the playground
		for (int i = 0; i<SNAKE_MAX_LENGTH; i++)
			for (int j = 0; j<SNAKE_MAX_LENGTH; j++)
			{
				play_ground_temp[i][j] = play_ground[i][j];
			}
		// Update the playground copy with fruit and snake position
		play_ground_temp[fruitX][fruitY] = 'F';
		play_ground_temp[snake_object.snake_position_x[0]][snake_object.snake_position_y[0]] = 'O';
		for (int i = 1; i<=snake_object.tail_length; i++)
			play_ground_temp[snake_object.snake_position_x[i]][snake_object.snake_position_y[i]] = 'o';

		// Draw playground
		for (int i = 0; i<SNAKE_MAX_LENGTH; i++)
		{
			std::cout << "#" ;
			for (int j = 0; j<SNAKE_MAX_LENGTH; j++)
			{
				std::cout << play_ground_temp[i][j];
			}
			std::cout << "#" << endl;
		}
		//Last bar
		for (int i = 0; i<SNAKE_MAX_LENGTH+2; i++)
			std::cout << "#";
		std::cout << endl;
	}

	void Input(snake &snake_object)
	{
		if (_kbhit())
		{
			switch (_getch())
			{
			case 'a':
				snake_object.dir = LEFT;
				break;
			case 'd':
				snake_object.dir = RIGHT;
				break;
			case 'w':
				snake_object.dir = UP;
				break;
			case 's':
				snake_object.dir = DOWN;
				break;
			case 'q':
				gameOver = true;
				break;
			case 'x':
				//snake_object.dir = STOP;
				this->TelemetryPrint(snake_object);
				getchar();
				break;
			}
		}
	}
	
	void TelemetryPrint(const snake &snake_object){
		std::cout << "X - snake head = " << snake_object.snake_position_x[0] << endl;
		std::cout << "Y - snake head = " << snake_object.snake_position_y[0] << endl;
		std::cout << "Tail Length = " << snake_object.tail_length << endl;
		for (int i=1; i<=snake_object.tail_length;i++){
			std::cout << "X - snake tail " << i << " = " << snake_object.snake_position_x[i] << endl;
			std::cout << "Y - snake tail " << i << " = " << snake_object.snake_position_y[i] << endl;
		}
		std::cout << "Score = " << score << endl;
	}

	void Logic(snake &snake_object){
		/*
		ToDo:
		1. Update the position of the head of the snake
		2. Check if the head is on a fruit or not. Hence, Update the score. Update fruit
		3. If yes, increase the tail length of the snake.
		4. Update the position of the snake tail.
		5. Check if it hits the wall - terminate the game
		*/

		//1. Update the position of the head of the snake
		int old_x = snake_object.snake_position_x[0];
		int old_y = snake_object.snake_position_y[0];
		switch (snake_object.dir)
		{
		case UP:
			snake_object.snake_position_x[0]--;
			break;
		case DOWN:
			snake_object.snake_position_x[0]++;
			break;
		case LEFT:
			snake_object.snake_position_y[0]--;
			break;
		case RIGHT:
			snake_object.snake_position_y[0]++;
			break;
		default:
			break;
		}

		//2. Check if the head is on a fruit or not. Hence, Update the score, and update the fruit
		if (snake_object.snake_position_y[0] == fruitY && snake_object.snake_position_x[0] == fruitX){
			score += 10;
			fruitX = rand() % SNAKE_MAX_LENGTH;
			fruitY = rand() % SNAKE_MAX_LENGTH;
			snake_object.tail_length += 1;
		}

		//3. Update the tail position - we want to swap different position
		for (int i=1; i<=snake_object.tail_length;i++){
			int posX = snake_object.snake_position_x[i];
			int posY = snake_object.snake_position_y[i];

			snake_object.snake_position_x[i] = old_x;
			snake_object.snake_position_y[i] = old_y;

			old_x = posX;
			old_y = posY;
		}
		std::cout << "Score = " << score << endl;
		//5. Check if it hits the wall - terminate the game
		if (snake_object.snake_position_y[0] >= SNAKE_MAX_LENGTH || snake_object.snake_position_x[0] >= SNAKE_MAX_LENGTH
			|| snake_object.snake_position_y[0] < 0 || snake_object.snake_position_x[0] < 0){
				gameOver = true;
		}
		// Check if it hits its own body - terminate the game - two nested loops
		// MOD: I need only to check the head against the rest of the tail. No need for two loops
		for (int i = 1;i <= snake_object.tail_length;i++)
		{
			if ( (snake_object.snake_position_y[0] == snake_object.snake_position_y[i]) && 
				(snake_object.snake_position_x[0] == snake_object.snake_position_x[i]) ){
					gameOver = true;
			}
		}

	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	snake snake_1;
	game game_board;
	game_board.game_setup(snake_1);
	int x = 0;
	while (!gameOver){
		game_board.Input(snake_1);
		game_board.Logic(snake_1);
		game_board.draw(snake_1);
		Sleep(SLEEP_TIME_MS);
	}
	return 0;
}