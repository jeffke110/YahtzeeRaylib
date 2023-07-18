#pragma once
#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "Dice.h"

#include <iostream>


class Game {

public:

	Game();
	~Game();
	void run();
	void draw();
	void update();
	void drawInstructions();
	void handleInput();


private:
	Dice * diceController;

	int rollCount = 3;
	int roundCount = 1;
	bool gameRunning = false;

	int screenWidth = 1200;
	int screenHeight = 1000;
	const int MIN_WIDTH = 1200;
	const int MIN_HEIGHT = 1000;

	Rectangle rollDiceButton;
	Rectangle resetGameButton;

};


#endif // !GAME_H
