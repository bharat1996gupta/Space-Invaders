#include "Game.h"
#include <conio.h>
#include<iostream>
#include<Windows.h>
#pragma comment(lib, "winmm.lib")

//### TODO FUNCTIONS ***************************************
//-----------------------------------------------------
void Game::Ship_Laser_Create() {
	//### TODO
	// scrap test code as comment
	/*int count = 0;
	for (int i = 0; i < Nb_ShipLasers-1; i++)
	{
		if (!(Ship_Lasers[i]->isAlive))
		{
			count++;
		}
	}
	Nb_ShipLasers -= count;*/ 
	// condition to stop creating new ship laser objects 
	if (Nb_ShipLasers < Max_Nb_Ship_Lasers)
	{
		Ship_Lasers[Nb_ShipLasers] = new Laser_Ship(My_Ship.coord.X, My_Ship.coord.Y - 1);
		Nb_ShipLasers++;
	}
	// loop to re-create lasers 
	for (int i = 0; i < Nb_ShipLasers; i++)
	{
		if (!(Ship_Lasers[i]->isAlive))
		{
			Ship_Lasers[i]->restart(My_Ship.coord.X, My_Ship.coord.Y - 1);
		}
	}
	// scrap test code as comment 
	/*if (Nb_ShipLasers > 0 && !(Ship_Lasers[Nb_ShipLasers-1]->isAlive) && Nb_ShipLasers == Max_Nb_Ship_Lasers)
	{
		Ship_Lasers[Nb_ShipLasers - 1]->restart(My_Ship.coord.X, My_Ship.coord.Y - 1);
		Nb_ShipLasers = 0;
	}
	Ship_Lasers[Nb_ShipLasers] = new Laser_Ship(My_Ship.coord.X, My_Ship.coord.Y - 1);
	Nb_ShipLasers++;
	/*if (Nb_ShipLasers == Max_Nb_Ship_Lasers)
	{
		Ship_Lasers[0]->restart(My_Ship.coord.X, My_Ship.coord.Y - 1);
		Nb_ShipLasers = 0;
	}*/
}

//-----------------------------------------------------
void Game::Enemy_Lasers_Move_All() {
	//### TODO	
		// moves enemy lasers
		for (int i = 0; i < Nb_EnemyLasers; i++)
		{
			Enemy_Lasers[i]->move();
		}
	
		// loop to re-create lasers for alive enemies only
		for (int i = 0; i < Nb_EnemyLasers; i++)
		{
			if (!(Enemy_Lasers[i]->isAlive))
			{
				if (ET_List[i]->isAlive)
				{
					Enemy_Lasers[i]->restart(ET_List[i]->coord.X, ET_List[i]->coord.Y + 1);
				}
			}
		}
	
}

//-----------------------------------------------------
bool Game::Check_Ship_Killed() {
	//### TODO	
	// loop to check if ship is hit by any enemy laser  
	for (int i = 0; i < Nb_EnemyLasers; i++)
	{
		if (My_Ship.coord.X == Enemy_Lasers[i]->coord.X && My_Ship.coord.Y == Enemy_Lasers[i]->coord.Y)
		{
			return true;
		}
	}
	return false;
}

//-------------------------------------------------------
void Game::Ship_Lasers_Move_All() {
	//### TODO	
	// moves ship lasers
	for (int i = 0; i < Nb_ShipLasers; i++)
	{
		Ship_Lasers[i]->move();
	}
}

//-------------------------------------------------------
void Game::Create_Enemy_Laser(int x, int y) {
	//### TODO	
	// creates enemy lasers 
	if (Nb_EnemyLasers < Max_Nb_Enemy_Lasers)
	{
		Enemy_Lasers[Nb_EnemyLasers] = new Laser_Enemies(x, y);
		Nb_EnemyLasers++;
	}
	
}

//-------------------------------------------------------
void Game::ETs_Move_All() {
	//### TODO	
	// moves the enemies
	for (int i = 0; i < Nb_ETs; i++)
	{
		ET_List[i]->move();
	}
	// cout << "***" << sizeof(ET_List[0]); // this commented line is to check size of one element of array ET_List[]
	
}

//-------------------------------------------------------
void Game::Check_ETs_Killed() {
	//### TODO
	// loop for each ship laser
	for (int i = 0; i < Nb_ShipLasers; i++)
	{
		// loop for each enemy
		for (int j = 0; j < Nb_ETs; j++)
		{
			// condition to check enemy killed or not for each ship laser alive
			if (ET_List[j]->coord.X == Ship_Lasers[i]->coord.X && ET_List[j]->coord.Y == Ship_Lasers[i]->coord.Y)
			{
				// enemy killed sound
 				PlaySound(TEXT("invaderkilled.wav"), NULL, SND_FILENAME | SND_ASYNC);
				score += ET_List[j]->kill();
				Enemy_Lasers[j]->kill();
				Nb_ETs--;
				Nb_EnemyLasers--;
			}
		}
	}
}

//-------------------------------------------------------
void Game::Enemies_Create() {
	clock_t t;

	t = clock() - last_ET_time;
	if ((((float)t) / CLOCKS_PER_SEC) > ET_Period) {
		last_ET_time = clock();
		//### TODO	
		// check to not create more enemies if array is full
		if (Nb_ETs < sizeof(ET_List)/ET_Period) // size of array is 60 as each index has 4 bytes of storage so we divide by 4 to get actual number of index in array
		{
			ET_List[Nb_ETs] = new Enemy_Fast;
			Create_Enemy_Laser(ET_List[Nb_ETs]->coord.X, ET_List[Nb_ETs]->coord.Y + 1);
			Nb_ETs++;
			last_ET_time = clock();// time when the last ennemy was created
		}
	}
}

//-------------------------------------------------------
int Game::Enemies_Get_Total_Alive() {
	//### TODO	
	return(Nb_ETs);// none were ever created, so don't return 0
}

//-------------------------------------------------------
bool Game::Enemies_Check_All_Killed() {
	//### TODO	
	if (score == 500)
	{
		return true;
	}
	else
	{
		return(false);
	}
}

//*****************************************************************
// Constructor --------------------------------------------------
Game::Game()
{
	// initialize game variables
	score = 0;
	level = 1;
	Nb_ShipLasers = 0;
	Nb_EnemyLasers = 0;
	Nb_ETs = 0;
	ET_Period = 5;// seconds
}

// destructor --------------------------------------
Game::~Game()
{
	// delete the ETs Ennemy_Fast  --------------------------
	for (int i = 1; i <= Nb_ETs; i++) {
		delete ET_List[i - 1];
	}

	// delete the ship lasers --------------------------
	for (int i = 1; i <= Nb_ShipLasers; i++) {
		delete Ship_Lasers[i - 1];
	}

	// delete the enemy lasers --------------------------
	for (int i = 1; i <= Nb_EnemyLasers; i++) {
		delete Enemy_Lasers[i - 1];
	}
}

int Game::getScore() {
	return score;
}

int Game::getLevel() {
	return level;
}

// ---------------------------------------------------------------
// Play() function starts and ends the game
// return: 0=>Game Over, 1=>You win, 2=>You Quit
// type e to quit/exit game
// --------------------------------------------------------------
int Game::Play() {

	char Key_Pressed; // key pressed by player on keyboard

	//----------------------------------------------------
	// set window dimension 100 columns, 42 lines
	system("CLS");
	UIKit::setWindowDimensions(0, 0, 100, 42);

	// left game frame with ship etc... 
	// the game field is 40 columns X 40 lines
	// so make the frame around 41 columns, 43 lines, color 15
	UIKit::frame(0, 0, 41, 42, 15);

	// right frame for level display, color 11
	UIKit::frame(49, 4, 70, 6, 11);

	// right frame for score display, color 11
	UIKit::frame(49, 6, 70, 8, 11);

	// show the ship
	My_Ship.show();
	//My_Ship.move();//moves ship
	// create at least 1 ET (Enemy_Fast), those move from left to right
	//ET_List[0] = new Enemy_Fast;
	//Nb_ETs = 1;
	//Create_Enemy_Laser(ET_List[0]->coord.X, ET_List[0]->coord.Y + 1);
	//last_ET_time = clock();// time when the last ennemy was created

	Game_Paused = false;

	// MASTER GAME LOOP ---------------------------------------------
	while (true) {

		Sleep(50); // slow down the game loop

		//Read keyboard input -----------------------
		if (_kbhit()) {
			Key_Pressed = _getch();
		}
		else {
			Key_Pressed = 0;
		}

		if (Key_Pressed == 'h') {
			// pause the game
			Game_Paused = true;
			continue; // skip the rest and restart from top of while loop
		}
		else if (Key_Pressed == 0 and Game_Paused) {
			// no key pressed, stay paused
			continue; // skip the rest and restart from top of while loop
		}
		else {
			// key was pressed, resume game
			Game_Paused = false;
		}

		switch (Key_Pressed) {
		case 'e':
			return(2);// User quit the game before the end

		case 32:
			// spacebar ASCII code = 32
			// Create new ship lasers
			// ### TODO
			//cout << "******" << Nb_ShipLasers; // counter to test after how many presses the ship lasers hang
			Ship_Laser_Create();
			break;

		case 'k':
		case 'l':
			// Move ship ---------------------------------
			My_Ship.move(Key_Pressed);
			break;
		}
		// end of switch ------------------------------------------


		// move the enemy lasers ----------------------------
		// ### TODO
		Enemy_Lasers_Move_All();

		// check if ship killed by enemy fire ---------------
		// ### TODO
		if (Check_Ship_Killed()) return(0);// game over

		// move the ship lasers -----------------------------
		// ### TODO
		Ship_Lasers_Move_All();

		// move the ETs (Enemy_Fast) ------------------------
		// ### TODO
		ETs_Move_All();

		// check ETs killed ---------------------------------
		// ### TODO
		Check_ETs_Killed();

		// Count how many enemies are alive
		// stop game if 15 enemies alive (as per problem requirement)
		// ### TODO
		if (Enemies_Get_Total_Alive() >= 15) {
			return(0); // Game Over
		}

		// increase level condition
		if (score == 250)
		{
			level = 2;
		}


		// Check if all enemies were killed
		// End game or change level
		// ### TODO
		if (Enemies_Check_All_Killed()) {
			return(1); // you win
		}

		// periodically create more ETs  ---------------------
		// ### TODO 
		Enemies_Create();

		// Update game info in the right frame ---------------
		UIKit::color(Game_Info_Color);

		// level 
		UIKit::gotoXY(50, 5); // go in level box
		std::cout << "Level    : " << level;

		// score
		UIKit::gotoXY(50, 7); // go in score box
		std::cout << "Score    : " << score;
	}

}


