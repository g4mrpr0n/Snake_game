#include <iostream>
#include <time.h>
#include <fstream>
#include <Windows.h>
#include <conio.h>
#include <string>
using namespace std;

void MainMenu();
int GameOverScreen();
void Setup();
void Draw();
void Input();
void Logic();
void copyData();

const int width = 30;
const int height = 30;
ofstream savefile;
ifstream saveFile("save.txt");
fstream fuckinghighscore("hs.txt");
bool gameOver, continuegame = 0, launch, restart, savehighscore = 0, foreverstained=0;
bool goingleft, goingright, goingup, goingdown;
int x, y, fruitX, fruitY, score, hscore;
int xc, yc, fruitXc, fruitYc, scorec, nTailc, gamemodec, directionc;
int tailX[100], tailY[100], tailXc[100], tailYc[100];
int nTail, choice, gamemode, direction, decision;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;
clock_t tStart;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

int GameOverScreen()
{
	system("cls");
	if (gameOver)
	{
		SetConsoleTextAttribute(hConsole, 12);
		cout << "You lost! Sad :(\n\n";
	}
	else SetConsoleTextAttribute(hConsole, 3);
	cout << "Some statistics:\nScore : " << score << "\nTail length : " << nTail << "\nPlaytime : " << (double)(clock() - tStart) / CLOCKS_PER_SEC << "\nHigh score : " << hscore << endl;;
	cout << "\nPlease press the buttons corresponding to the menu options to access them." << endl;
	if (gameOver)
		cout << "\n1 - Start New Game\n2 - Go to Main Menu\n3 - Quit\n\n"; 
	else cout << "\n0 - Continue\n1 - Start New Game\n2 - Go to Main Menu\n3 - Quit\n\n";
	cin >> decision;
	return decision;
}

void copyData()
{
	x = xc;
	y = yc;
	fruitX = fruitXc;
	fruitY = fruitYc;
	score = scorec;
	nTail = nTailc;
	for (int k = 0; k < nTail; k++)
	{
		tailX[k] = tailXc[k];
		tailY[k] = tailYc[k];
	}
	gamemode = gamemodec;
	direction = directionc;
}

void Setup()
{
	srand(time(NULL));
	gameOver = false;
	dir = STOP;
	//if the player wants to continue from a savefile, not after losing or quitting
	if (continuegame) 
	{
		SetConsoleTextAttribute(hConsole, 16);
		fuckinghighscore >> hscore;
		if (foreverstained)
		{
			copyData();
		}
		else {
			saveFile >> xc >> yc >> fruitXc >> fruitYc >> scorec >> nTailc;
			for (int k = 0; k < nTailc; k++)
			{
				saveFile >> tailXc[k] >> tailYc[k];
			}
			saveFile >> gamemodec >> directionc;
			copyData();
		}
		dir = (eDirection)direction;

		//variable used to fix the game continue bug where it would be an instant loss when using saves
		launch = true; 
	}
	else {
		SetConsoleTextAttribute(hConsole, 192);
		fuckinghighscore >> hscore;
		x = width / 2;
		y = height / 2;
		fruitX = rand() % width;
		fruitY = rand() % height;
		score = 0;
		nTail = 0;
		goingleft = 0; 
		goingright = 0;
		goingup = 0; 
		goingdown = 0;
		for (int i = 0; i < nTail; i++)
		{
			tailX[i] = 0;
			tailY[i] = 0;
		}
	}
}

void Draw()
{
	system("cls");
	//draw the top part
	for (int i = 0; i < width + 2; i++)
		cout << "#";
	cout << endl;

	//draw the middle 
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (j == 0)
				cout << "#";
			if (i == y && j == x)
				cout << "0";
			else if (i == fruitY && j == fruitX)
				cout << "F";
			else
			{
				bool print = false;
				for (int k = 0; k < nTail; k++)
				{
					if (tailX[k] == j && tailY[k] == i)
					{
						cout << "o";
						print = true;
					}
				}
				if (!print)
					cout << " ";
			}
			if (j == width - 1)
				cout << "#";
		}
		cout << endl;
	}

	//draw the bottom
	for (int i = 0; i < width + 2; i++)
		cout << "#";
	cout << endl;

	//bottom information
	cout << "Score:" << score << "\nHighscore: " << hscore << endl;
	cout << "\nPress 'q' to save the game and go to the main menu, and 'x' to quit.";

	//for the first time launch after continuing a game pause it before continuing
	if (launch)
		system("pause>nul");
}

void Input()
{
	if (_kbhit()) {

		//Beep(1000, 100); for a nice little beep
		switch (_getch())
		{
		case 'a':
			//check if it was going in the opposite direction and don't allow it
			if (goingright) {
				dir = RIGHT;
				goingleft = 0;
				break;
			}

			//otherwise go the normal direction and reset the pairwise opposite directions
			dir = LEFT;
			goingleft = true;
			goingup = 0;
			goingdown = 0;
			break;


		//repeat for all cases
		case 'd':
			if (goingleft) {
				dir = LEFT;
				goingright = 0;
				break;
			}
			dir = RIGHT;
			goingright = true;
			goingup = 0;
			goingdown = 0;
			break;

		case 'w':
			if (goingdown) {
				dir = DOWN;
				goingup = 0;
				break;
			}
			dir = UP;
			goingup = true;
			goingleft = 0;
			goingright = 0;
			break;

		case 's':
			if (goingup) {
				dir = UP;
				goingdown = 0;
				break;
			}
			dir = DOWN;
			goingdown = true;
			goingleft = 0;
			goingright = 0;
			break;

		case 'q':
			SetConsoleTextAttribute(hConsole, 10);
			//open the savefile and remove all contents
			savefile.open("save.txt", ofstream::out | ofstream::trunc);
			fuckinghighscore.open("hs.txt", ofstream::out | ofstream::trunc);
			//then output every single game statistic
			fuckinghighscore << hscore;
			savefile << x << " " << y << " " << fruitX << " " << fruitY << " " << score << " " << nTail << " ";
			for (int k = 0; k < nTail; k++) {
				savefile << tailX[k] << " " << tailY[k] << " ";
			}
			savefile << gamemode << " " << (int)dir;
			fuckinghighscore.close();
			savefile.close();
			savehighscore = 1;

			//at gameover screen get to choose to start over or not
			decision = GameOverScreen();
			if (decision == 0)
			{
				break;
			}
			if (decision == 1)
			{
				restart = true;
				gameOver = false;
			}
			else if (decision == 2)
			{
				MainMenu();
			}
			else {
				system("cls");
				SetConsoleTextAttribute(hConsole, 13);
				cout << "Thank you for playing my shitty game :D\nPress any key to exit." << endl;
				gameOver = true; 
			}
			break;


		case 'x':
			system("cls");
			SetConsoleTextAttribute(hConsole, 14);
			cout << "Are you sure?\nType y for yes or n for no.\n\n";
			system("pause>nul");
			char x;
			cin >> x;
			if (x == 'y')
			{ 
				system("cls");
				SetConsoleTextAttribute(hConsole, 13);
				cout << "Thank you for playing my shitty game :D\nPress any key to exit." << endl;
				gameOver = true;
			}
			else {
				launch = true;
			}
		}
	}
}

void Logic()
{
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;
	for (int i = 1; i < nTail; i++)
	{
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}
	switch (dir)
	{
	case LEFT:
		if (goingright) {
			x++;
			break;
		}
		x--;
		break;

	case RIGHT:
		if (goingleft) {
			x--;
			break;
		}
		x++;
		break;
	
	case UP:
		if (goingdown) {
			y++;
			break;
		}
		y--;
		break;
		
	case DOWN:
		if (goingup) {
			y--;
			break;
		}
		y++;
		break;

	default:
		break;
	}
	if (x > width || x < 0 || y > height || y < 0)
	{
		if (gamemode == 1)
		{
			gameOver = true;
			foreverstained = true;
			decision = GameOverScreen();
			if (decision == 1)
			{
				restart = true;
				gameOver = false;
			}
			else if (decision == 2)
			{
				MainMenu();
			}
			else {
				system("cls");
				SetConsoleTextAttribute(hConsole, 13);
				cout << "Thank you for playing my shitty game :D\nPress any key to exit." << endl;
			}
		}
	} 
	if (x >= width && gamemode == 2) x = 0;
		else if (x < 0 && gamemode == 2) x = width - 1;
	if (y >= height && gamemode == 2) y = 0;
		else if (y < 0 && gamemode == 2) y = height - 1;

	for (int i = 0; i < nTail; i++)
		if (tailX[i] == x && tailY[i] == y && !launch && !restart)
		{
			gameOver = true;
			if (gamemode == 1 || gamemode == 2)
			{
				foreverstained = true;
				decision = GameOverScreen();
				if (decision == 1)
				{
					restart = true;
					gameOver = false;
				}
				else if (decision == 2)
				{
					MainMenu();
				}
				else {
					system("cls");
					SetConsoleTextAttribute(hConsole, 13);
					cout << "Thank you for playing my shitty game :D\nPress any key to exit." << endl;
				}
			}
		}
	
	if (x == fruitX && y == fruitY)
	{
		score += 10;
		fruitX = rand() % width;
		fruitY = rand() % height;
		nTail++;
	}
	launch = 0;
}

void MainMenu()
{
	SetConsoleTextAttribute(hConsole, 11);
	system("cls");
	cout << "Hello! Welcome to this shitty Snake copycat made by Annannas :)" << endl;
	cout << "\nPlease press the buttons corresponding to the menu options to access them." << endl;
	cout << "1 - New Game\n2 - Continue Game\n3 - Instructions\n4 - Quit\n\n";
	cin >> choice;
	switch (choice)
	{
	case 1:
	{
		system("cls");
		SetConsoleTextAttribute(hConsole, 6);
		cout << "1 - Normal Mode\n2 - Endless Mode\n3 - Back\n\n";
		cin >> gamemode;
		if (gamemode == 1 || gamemode == 2) {
			continuegame = 0;
			Setup();
			tStart = clock();
			while (!gameOver) {
				Draw();
				Input();
				Logic();
				if (hscore < score)
				{
					hscore = score;
					fuckinghighscore.open("hs.txt", ofstream::out | ofstream::trunc);
					fuckinghighscore << hscore;
					fuckinghighscore.close();
				}			
				Sleep(50);
				if (restart)
				{
					Setup();
					Draw();
					Input();
					Logic();
					Sleep(50);
					restart = 0;
				}
			}
		}
		else {
			MainMenu();
		}
		if (!savehighscore)
		{
			fuckinghighscore.open("hs.txt", ofstream::out | ofstream::trunc);
			fuckinghighscore << hscore;
			fuckinghighscore.close();
			savehighscore = 1;
		}
		break;
	}
	case 2:
		continuegame = true;
		Setup();
		tStart = clock();
		while (!gameOver) {
			Draw();
			Input();
			Logic();
			if (hscore < score)
			{
				hscore = score;
				fuckinghighscore.open("hs.txt", ofstream::out | ofstream::trunc);
				fuckinghighscore << hscore;
			}
			Sleep(50);
			continuegame = false;
			if (restart)
			{
				Setup();
				Draw();
				Input();
				Logic();
				Sleep(50);
				restart = 0;
			}
		}
		if (!savehighscore)
		{
			fuckinghighscore.open("hs.txt", ofstream::out | ofstream::trunc);
			fuckinghighscore << hscore;
			fuckinghighscore.close();
			savehighscore = 1;
		}
		break;
	case 3:
		system("cls");
		cout << "In Snake, your main objective is getting a score as high as possible by eating fruits while not\ncolliding with the wall or the Snake's tail.\n\nEndless mode removes the wall boundaries and allows for free roam except for the Snake's tail." << endl;
		cout << "\nPress any key to go back to the main menu.";
		system("pause >nul");
		MainMenu();
		break;
	case 4:
		system("cls");
		SetConsoleTextAttribute(hConsole, 13);
		cout << "Thank you for playing my shitty game :D\nPress any key to exit." << endl;
		break;
	}
	savehighscore = 0;
	system("pause >nul");
}

int main() {
	system("mode 100,50");
	MainMenu();
}