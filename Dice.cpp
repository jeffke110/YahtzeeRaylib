#include "Dice.h"


Dice::Dice() {
	rightArrow = LoadTexture("resources/rightArrow.png");
	calculatedScores =
	{
		{"Total Ones      ", 0},
		{"Total Twos       ", 0},
		{"Total Threes    ", 0},
		{"Total Fours      ", 0},
		{"Total Fives       ", 0},
		{"Total Sixes       ", 0},
		{"Chance            ", 0},
		{"3 of a Kind       ",  0},
		{"4 of a Kind       ",  0},
		{"Small Straight     ", 0},
		{"Large Straight     ", 0},
		{"Full House      ",    0},
		{"5 of a Kind       ",  0}
	};
	gameScores = {
	{0, false}, {0, false}, {0, false}, {0, false}, {0, false}, {0, false},
	{0, false}, {0, false}, {0, false}, {0, false}, {0, false}, {0, false},
	{0, false}
	};
}
Dice::~Dice() {
	UnloadTexture(rightArrow);
}

void Dice::rollDice() {


	if (savedFaces.empty()) {
		std::generate(rolledFaces.begin(),
			rolledFaces.end(),
			[]() { return rand() % 6 + 1; });
	}
	else {
		for (int i = 0; i < rolledFaces.size(); i++) {
			if (!savedFaces.contains(i)) {
				rolledFaces[i] = rand() % 6 + 1;
			}
		}
	}
	std::sort(rolledFaces.begin(), rolledFaces.end());
	savedFaces.clear();
}


void Dice::saveDice() {

	Rectangle diceOne = { (float)screenWidth / 2 - 350, 75, 100, 100 };
	Rectangle diceTwo = { (float)screenWidth / 2 - 200, 75, 100, 100 };
	Rectangle diceThree = { (float)screenWidth / 2 - 50, 75, 100, 100 };
	Rectangle diceFour = { (float)screenWidth / 2 + 100, 75, 100, 100 };
	Rectangle diceFive = { (float)screenWidth / 2 + 250, 75, 100, 100 };
	std::vector<Rectangle> dieRectangle = { diceOne , diceTwo , diceThree, diceFour, diceFive };

	uint8_t idx = 0;
	for (const auto& element : dieRectangle) {
		if (CheckCollisionPointRec(GetMousePosition(), element) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
		{
			//if index is not in set
			if (!savedFaces.contains(idx)) {
				savedFaces.insert(idx);
			}
			else {
				savedFaces.erase(idx);
			}
		}
		idx++;
	}
}


void Dice::displayDice(int x, int y, int index) {
	int face = rolledFaces[index];
	if (savedFaces.contains(index)) {
		DrawRectangle(x, y, 100, 100, GREEN);
	}
	else {
		DrawRectangle(x, y, 100, 100, WHITE);
	}
	if (face & 1)
	{
		DrawCircle(x + 50, y + 50, 5, BLACK); // middle
	}
	if (face > 1)
	{
		DrawCircle(x + 25, y + 25, 5, BLACK); // top left
		DrawCircle(x + 75, y + 75, 5, BLACK); // bottom right
	}
	if (face > 3)
	{
		DrawCircle(x + 75, y + 25, 5, BLACK); // top right
		DrawCircle(x + 25, y + 75, 5, BLACK); // bottome left
	}
	if (face == 6)
	{
		DrawCircle(x + 25, y + 50, 5, BLACK); // middle left
		DrawCircle(x + 75, y + 50, 5, BLACK); // middle right
	}
}


void Dice::displayDie() {
	displayDice(screenWidth / 2 - 350, 75, 0);
	displayDice(screenWidth / 2 - 200, 75, 1);
	displayDice(screenWidth / 2 - 50, 75, 2);
	displayDice(screenWidth / 2 + 100, 75, 3);
	displayDice(screenWidth / 2 + 250, 75, 4);
	
}


bool Dice::patternMatch(const std::vector<uint8_t> vDice, const std::string& sPattern) {
	bool match = true;
	uint8_t n = 0;

	for (size_t idx = 0; idx < 5; idx++)
	{
		if (sPattern[idx] == 'n')
		{
			if (n == 0)
			{
				n = vDice[idx];
			}
			else
			{
				match &= (vDice[idx] == n);
			}
		}
		else if (sPattern[idx] == '?')
		{
			match &= true;
		}
		// is Face Value
		else 
		{
			match &= ((sPattern[idx] - '0') == vDice[idx]);
		}

	}
	return match;
}




void Dice::calculateScores() {

	int threeOfAKind = 0;
	int fourOfAKind = 0;
	int FiveOfAKind = 0;
	int smallStraight = 0;
	int largeStraight = 0;
	int fullHouse = 0;

	if (patternMatch(rolledFaces, "nnnnn") && !(patternMatch(rolledFaces, "00000")))
	{
		FiveOfAKind = 50;
	}
	// nnnn?, ?nnnn - four of a kind
	if (patternMatch(rolledFaces, "nnnn?") || patternMatch(rolledFaces, "?nnnn"))
	{
		fourOfAKind = 4 * rolledFaces[2];
	}
	// nnn??, ?nnn?, ??nnn  - three of a kind
	if (patternMatch(rolledFaces, "nnn??") || patternMatch(rolledFaces, "?nnn?") || patternMatch(rolledFaces, "??nnn"))
	{
		threeOfAKind = 3 * rolledFaces[2];
	}
	// 1234?, ?2345, 2345?, ?3456 - Small Straight
	if (patternMatch(rolledFaces, "1234?") 
		|| patternMatch(rolledFaces, "2345?") 
		|| patternMatch(rolledFaces, "?3456") 
		|| patternMatch(rolledFaces, "?2345"))
	{
		smallStraight = 30;
	}
	// 12345, 23456 - Large Straight
	if (patternMatch(rolledFaces, "12345") || patternMatch(rolledFaces, "23456"))
	{
		largeStraight = 40;
	}
	// nnn?? & ???nn, nn??? & ??nnn - Full House
	if (((patternMatch(rolledFaces, "nnn??") 
		&& patternMatch(rolledFaces, "???nn")) 
		|| (patternMatch(rolledFaces, "??nnn") 
			&& patternMatch(rolledFaces, "nn???"))) 
		&& !(patternMatch(rolledFaces, "00000")))
	{
		fullHouse = 25;
	}

	//calculate scores
	calculatedScores =
	{
		{"Total Ones      ", std::count(rolledFaces.begin(), rolledFaces.end(), 1) * 1},
		{"Total Twos       ", std::count(rolledFaces.begin(), rolledFaces.end(), 2) * 2},
		{"Total Threes    ", std::count(rolledFaces.begin(), rolledFaces.end(), 3) * 3},
		{"Total Fours      ", std::count(rolledFaces.begin(), rolledFaces.end(), 4) * 4},
		{"Total Fives       ", std::count(rolledFaces.begin(), rolledFaces.end(), 5) * 5},
		{"Total Sixes       ", std::count(rolledFaces.begin(), rolledFaces.end(), 6) * 6},
		{"Chance            ", std::accumulate(rolledFaces.begin(), rolledFaces.end(), 0)},
		{"3 of a Kind       ",  threeOfAKind},
		{"4 of a Kind       ",  fourOfAKind},
		{"Small Straight     ", smallStraight},
		{"Large Straight     ", largeStraight},
		{"Full House      ",    fullHouse},
		{"5 of a Kind       ",  FiveOfAKind}
	};


	//calculate bonus
	auto it = std::next(gameScores.begin(), 6);
	int tempBonus = std::accumulate(gameScores.begin(), it, 0,
		[](int total, const std::pair<int, bool>& pair) {
			return total + pair.first;
		});
	if (tempBonus >= 63) {
		bonus = 35;
	}
	//calculate total
	total = std::accumulate(gameScores.begin(), gameScores.end(), 0,
		[](int total, const std::pair<int, bool>& pair) {
			return total + pair.first;
		});
	total += bonus;
}


void Dice::displayCalculatedScores() {
	//display scores
	int height = 0;
	DrawText("Scores", screenWidth / 2 - 100, screenHeight / 2 - 50, 20, WHITE);
	for (const auto& i : calculatedScores) {
		DrawText(i.first, screenWidth / 2 - 300, screenHeight / 2 + height, 20, WHITE);
		DrawText(TextFormat("%d", i.second), screenWidth / 2 - 100, screenHeight / 2 + height, 20, WHITE);
		height += 30;
	}
	DrawText("Upper Section Bonus", screenWidth / 2 - 300, screenHeight / 2 + height, 20, WHITE);
	height += 30;
	DrawText("Total", screenWidth / 2 - 300, screenHeight / 2 + height, 20, WHITE);
}

bool Dice::changeGameScores() {
	int height = 0;
	int idx = 0;
	for (auto& i : gameScores) {
		Rectangle scoreButton = { static_cast<float>(screenWidth / 2 - 6.25), (float)screenHeight / 2 + height - 4 , 40, 25 };
		
		if (CheckCollisionPointRec(GetMousePosition(), scoreButton) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && i.second == false) {
			i.first = calculatedScores[idx].second;
			i.second = true;
			return true;
		}
		height += 30;
		idx++;
	}

	return false;
}

void Dice::displayGameScores() {
	int height = 0;
	DrawText("Kept Scores", screenWidth / 2, screenHeight / 2 - 50, 20, WHITE);
	for (auto& i : gameScores) {
		Rectangle scoreButton = { static_cast<float>(screenWidth / 2 - 6.25), (float)screenHeight / 2 + height - 4 , 40, 25 };
		if (i.second == true || CheckCollisionPointRec(GetMousePosition(), scoreButton)) {
			DrawTexture(rightArrow, screenWidth / 2 - 75, screenHeight / 2 + height - 3, WHITE);
			DrawRectangleLines(screenWidth / 2 - 6.25, screenHeight / 2 + height - 4, 40, 25, GREEN);
		}
		else {
			DrawRectangleLines(screenWidth / 2 - 6.25, screenHeight / 2 + height - 4, 40, 25, RED);
		}

		DrawText(TextFormat("%d", i.first), screenWidth / 2, screenHeight / 2 + height, 20, WHITE);
		height += 30;
	}
	



	DrawText(TextFormat("%d", bonus), screenWidth / 2, screenHeight / 2 + height, 20, WHITE);
	height += 30;
	DrawText(TextFormat("%d", total), screenWidth / 2, screenHeight / 2 + height, 20, WHITE);
}


void Dice::setScreenResolution(int width, int height){
	screenWidth = width;
	screenHeight = height;
}

void Dice::resetFaces() {
	rolledFaces = { 0, 0, 0, 0, 0 };
}

void Dice::resetGameScores() {
	gameScores = {
	{0, false}, {0, false}, {0, false}, {0, false}, {0, false}, {0, false},
	{0, false}, {0, false}, {0, false}, {0, false}, {0, false}, {0, false},
	{0, false}
	};
}

void Dice::resetSavedFaces() {
	savedFaces.clear();
}

void Dice::resetTotal() {
	total = 0;
	bonus = 0;
}