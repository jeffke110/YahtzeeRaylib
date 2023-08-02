#include "Game.h"



Game::Game() {
	InitWindow(screenWidth, screenHeight, "Yahtzee");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(MIN_WIDTH, MIN_HEIGHT);
	SetTargetFPS(60);

    diceController = new Dice();
    rollDiceButton = { (float)screenWidth / 2 - 100, 200 , 200, 30 };
    resetGameButton = { (float)screenWidth / 2 + 300,  (float)screenHeight / 2 - 250, 200, 30 };
}


Game::~Game() {
    delete diceController;
}

void Game::draw() {

    BeginDrawing();

    ClearBackground(DARKGREEN); 
    
    //Draw Instructions
    drawInstructions();

    //Display Dice and Scores
    diceController->displayDie();
    diceController->displayGameScores();
    diceController->displayCalculatedScores();

    //Draw Buttons
    DrawRectangleRec(rollDiceButton, LIGHTGRAY);
    DrawText(TextFormat("ROLL DICE : %d", rollCount), rollDiceButton.x + 40, rollDiceButton.y + 5, 20, DARKGRAY);
    DrawRectangleRec(resetGameButton, LIGHTGRAY);
    DrawText("RESET GAME", resetGameButton.x + 40, resetGameButton.y + 5, 20, DARKGRAY);
    DrawRectangle((float)screenWidth / 2 + 300, (float)screenHeight / 2 - 200, 200, 125, LIGHTGRAY);
    if (roundCount == 14) {
        DrawText(("ROUND:\n   13"), (float)screenWidth / 2 + 300 + 40, (float)screenHeight / 2 - 200 + 5, 35, DARKGRAY);
    }else{
        DrawText(TextFormat("ROUND:\n   %d", roundCount), (float)screenWidth / 2 + 300 + 40, (float)screenHeight / 2 - 200 + 5, 35, DARKGRAY);
    }
    
    EndDrawing();

}


void Game::handleInput() {
    //Handle input
    rollDiceButton = { (float)screenWidth / 2 - 100, 200 , 200, 30 };
    resetGameButton = { (float)screenWidth / 2 + 300,  (float)screenHeight / 2 - 250, 200, 30 };

    //Press Roll Button
    if (CheckCollisionPointRec(GetMousePosition(), rollDiceButton) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && rollCount != 0 && roundCount < 14) {
        diceController->rollDice();
        diceController->calculateScores();
        rollCount--;
        gameRunning = true;
    }
    //Press Reset Game button
    if (CheckCollisionPointRec(GetMousePosition(), resetGameButton) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        diceController->resetFaces();
        diceController->resetCalculatedScores();
        diceController->resetGameScores();
        diceController->resetSavedFaces();
        diceController->resetTotal();
        rollCount = 3;
        roundCount = 1;
        gameRunning = false;
    }
}


void Game::drawInstructions() {
    DrawRectangle(screenWidth / 2 - 250, 250, 500, 175, LIGHTGRAY);
    DrawText("Instructions: ", screenWidth / 2 - 50, 255, 20, BLACK);
    if (roundCount > 13) {
        DrawText("GAME OVER \npress reset button to play again ", screenWidth / 2 - 240, 300, 20, BLACK);
    }
    else if (rollCount == 3 && !gameRunning) {
        DrawText("Roll die (press roll die button) \nto start the game ", screenWidth / 2 - 240, 300, 20, BLACK);
    }
    else if (rollCount == 3 && gameRunning) {
        DrawText("Roll die (press roll die button) \nto start the next round ", screenWidth / 2 - 240, 300, 20, BLACK);
    }
    else if (rollCount < 3 && gameRunning && (rollCount == 2 || rollCount == 0)) {
        DrawText(TextFormat("Roll die again (%d more tries) \nand press which die you want to keep \nand then select which score you want to keep \nfrom this round", rollCount), screenWidth / 2 - 240, 300, 20, BLACK);
    }
    else if (rollCount < 3 && gameRunning && rollCount == 1) {
        DrawText(TextFormat("Roll die again (%d more try) \nand press which die you want to keep \nand then select which score you want to keep \nfrom this round", rollCount), screenWidth / 2 - 240, 300, 20, BLACK);
    }
    else if (rollCount == 0 && gameRunning) {
        DrawText("You must select one of the scores \ninorder to proceed to the next round \nbecause there are no more dice to roll", screenWidth / 2 - 240, 300, 20, BLACK);
    }

}

void Game::update() {

    // Get the new window size when resized
    if (IsWindowResized())
    {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        if (screenWidth < MIN_WIDTH)
            screenWidth = MIN_WIDTH;
        if (screenHeight < MIN_HEIGHT)
            screenHeight = MIN_HEIGHT;
        diceController->setScreenResolution(screenWidth, screenHeight);
    }

    // Save Dice for next roll
    if (gameRunning && rollCount != 0 && roundCount < 14 && rollCount != 3) {
       diceController->saveDice();
    }

    // Save game scores from rolls
    if (rollCount < 3 && roundCount < 14) {
        if (diceController->changeGameScores()) {
            roundCount++;
            rollCount = 3;
            diceController->resetFaces();
            diceController->calculateScores();
            diceController->resetSavedFaces();
        }
    }
    handleInput();
    draw();
}


void Game::run() {
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        update();
    }
    CloseWindow();
}