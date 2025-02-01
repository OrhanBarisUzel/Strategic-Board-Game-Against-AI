#include "raylib.h"
#include "board.cpp"

int main() {
    const int width= 700;
    const int hegiht= 740;
    InitWindow(width, hegiht, "7x7 Board Game");
    Board board(100);
    board.Init();
    SetTargetFPS(60);

    double lastMoveTime= GetTime();
    const double AI_MOVE_DELAY= 2.0;
    while (!WindowShouldClose()) {
        double currentTime= GetTime();
        //For AI
        if ((board.getCurrentState()== GameState::AI_FIRST_TURN ||
             board.getCurrentState()== GameState::AI_SECOND_TURN) &&
            currentTime - lastMoveTime >= AI_MOVE_DELAY) {
            board.processAITurn();
            lastMoveTime= currentTime;
        }
        //For player
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            board.HandleMouseClick(GetMouseX(), GetMouseY());
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);
        board.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}