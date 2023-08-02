#pragma once
#ifndef DICE_H 
#define DICE_H

#include "raylib.h"

#include <algorithm>
#include <set>
#include <vector>
#include <iostream>
#include <numeric>
#include <tuple>



class Dice {


public:

	Dice();
	~Dice();

	//Dice Methods
	void rollDice();
	void displayDice(int x, int y, int face);
	void displayDie();
	void saveDice();

	//Score methods
	bool changeGameScores();
	void displayGameScores();
	void calculateScores();
	void displayCalculatedScores();
	bool patternMatch(const std::vector<uint8_t> vDice, const std::string& sPattern);

	//Rest and screen resolution update
	void resetFaces();
	void resetGameScores();
	void resetSavedFaces();
	void resetCalculatedScores();
	void resetTotal();
	void setScreenResolution(int width, int height);

private:
	Texture2D rightArrow;
	std::set<uint8_t> savedFaces;
	std::vector<uint8_t> rolledFaces{ 0, 0, 0, 0, 0 };
	std::vector<std::pair<const char *, int>> calculatedScores;
	std::vector<std::pair<int, bool>> gameScores;

	int screenWidth = 1200;
	int screenHeight = 1000;
	int bonus = 0;
	int total = 0;

	

};




#endif // DICE_H
