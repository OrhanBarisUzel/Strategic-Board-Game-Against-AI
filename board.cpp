#include "raylib.h"
#include <vector>
#include <limits>
#include <iostream>

struct MinimaxResult{
    int pieceScore;
    std::pair<std::pair<int, int>, std::pair<int, int>> move;
    MinimaxResult(int s, std::pair<int, int> from, std::pair<int, int> to): pieceScore(s), move(from, to){}
};

enum Piece{
    NONE,
    AI,
    PLAYER
};
enum class GameState{
    AI_FIRST_TURN,
    PLAYER_FIRST_TURN,
    AI_SECOND_TURN,
    PLAYER_SECOND_TURN
};
class Board{
private:
    std::vector<std::vector<Piece>> grid;
    int cellSize;
    int moveCount=0;
    std::string gameResult;
    Vector2 selectedPiece;
    Vector2 lastMovedPiece;
    Vector2 lastAIMovedPiece={-1,-1};
    bool isValid(int row1, int col1, int row2, int col2){
        if(row2 < 0 || row2 >= 7 || col2 < 0 || col2 >= 7){
            return false;
        }
        int rowDiff= abs(row1- row2);
        int colDiff= abs(col1- col2);
        if((rowDiff+ colDiff)   != 1){
            return false;
        }
        if(grid[row2][col2]!= NONE){
            return false;
        }
        return true;
    }
    int countPieces(Piece player){
        int count= 0;
        for(const auto& row: grid){
            for(Piece piece: row){
                if(piece== player){
                    count++;
                }
            }
        }
        return count;
    }

    void checkCaptures(int row, int col){
        std::vector<std::pair<int, int>> mark;
        Piece currentPiece= grid[row][col];
        if(currentPiece== NONE) return;

        Piece oppositePiece=(currentPiece== AI) ? PLAYER : AI;
        //Normal capture logic
       //Horizontal captures
       {
            int left= col- 1;
            while(left >= 0 && grid[row][left]== oppositePiece){
                left--;
            }
            if(left < col- 1){
                bool capture= false;
                if(left >= 0 && grid[row][left]== currentPiece){
                    capture= true;
                } else if(left < 0){
                    capture= true;
                }
                if(capture){
                    for(int j= left+ 1; j < col; j++){
                       //grid[row][j]= NONE;
                        mark.push_back({row, j});
                    }
                }
            }
        }
       //Capture to the right
       {
            int right= col+ 1;
            while(right < 7 && grid[row][right]== oppositePiece){
                right++;
            }
            if(right > col+ 1){
                bool capture= false;
                if(right < 7 && grid[row][right]== currentPiece){
                    capture= true;
                } else if(right== 7){
                    capture= true;
                }
                if(capture){
                    for(int j= col+ 1; j < right; j++){
                       //grid[row][j]= NONE;
                        mark.push_back({row, j});
                    }
                }
            }
        }
       //Check if current piece is captured(horizontal)
       {
            int left= col- 1;
            while(left >= 0 && grid[row][left]== oppositePiece){
                left--;
            }
            int right= col+ 1;
            while(right < 7 && grid[row][right]== oppositePiece){
                right++;
            }

            if(left >= 0 && right < 7 && grid[row][left]== oppositePiece && grid[row][right]== oppositePiece){
               //grid[row][col]= NONE;
                mark.push_back({row, col});
            }
        }
       //Vertical captures
       //Capture upwards
       {
            int up= row- 1;
            while(up >= 0 && grid[up][col]== oppositePiece){
                up--;
            }

            if(up < row- 1){
                bool capture= false;
                if(up >= 0 && grid[up][col]== currentPiece){
                    capture= true;
                } else if(up < 0){
                    capture= true;
                }

                if(capture){
                    for(int i= up+ 1; i < row; i++){
                       //grid[i][col]= NONE;
                        mark.push_back({i, col});
                    }
                }
            }
        }
       //Capture downwards
       {
            int down= row+ 1;
            while(down < 7 && grid[down][col]== oppositePiece){
                down++;
            }
            if(down > row+ 1){
                bool capture= false;
                if(down < 7 && grid[down][col]== currentPiece){
                    capture= true;
                } else if(down== 7){
                    capture= true;
                }

                if(capture){
                    for(int i= row+ 1; i < down; i++){
                       //grid[i][col]= NONE;
                        mark.push_back({i, col});
                    }
                }
            }
        }
       //Check if current piece is captured(vertical)
       {
            int up= row- 1;
            while(up >= 0 && grid[up][col]== oppositePiece){
                up--;
            }

            int down= row+ 1;
            while(down < 7 && grid[down][col]== oppositePiece){
                down++;
            }
            if(up >= 0 && down < 7 && grid[up][col]== oppositePiece && grid[down][col]== oppositePiece){
               //grid[row][col]= NONE;
                mark.push_back({row, col});
            }
        }
       //Mutual captures
       //Check for mutual horizontal capture
       {
            int left= col- 1;
            while(left >= 0 && grid[row][left]== currentPiece){
                left--;
            }

            int right= col+ 1;
            while(right < 7 && grid[row][right]== currentPiece){
                right++;
            }
            if(left >= 0 && right < 7 && grid[row][left]== oppositePiece && grid[row][right]== oppositePiece){
               //grid[row][col]= NONE;
                mark.push_back({row, col});
            }
        }
       //Check for mutual vertical capture
       {
            int up= row- 1;
            while(up >= 0 && grid[up][col]== currentPiece){
                up--;
            }
            int down= row+ 1;
            while(down < 7 && grid[down][col]== currentPiece){
                down++;
            }
            if(up >= 0 && down < 7 && grid[up][col]== oppositePiece && grid[down][col]== oppositePiece){
               //grid[row][col]= NONE;
                mark.push_back({row, col});
            }
        }
        //Reverse capture logic
       //Horizontal reverse capture
       {
            int left= col- 1;
            while(left >= 0 && grid[row][left]== currentPiece){
                left--;
            }
            int right= col+ 1;
            while(right < 7 && grid[row][right]== currentPiece){
                right++;
            }

            if((left >= 0 && grid[row][left]== oppositePiece) &&(right < 7 && grid[row][right]== oppositePiece)){
                for(int j= left+ 1; j < right; j++){
                    if(grid[row][j]== currentPiece){
                        mark.push_back({row, j});
                    }//grid[row][j]= NONE;
                }
            }
        }
       //Vertical reverse capture
       {
            int up= row- 1;
            while(up >= 0 && grid[up][col]== currentPiece) up--;
            int down= row+ 1;
            while(down < 7 && grid[down][col]== currentPiece) down++;
            if((up >= 0 && grid[up][col]== oppositePiece) &&(down < 7 && grid[down][col]== oppositePiece)){
                for(int i= up+ 1; i < down; i++){
                    if(grid[i][col]== currentPiece){
                        mark.push_back({i, col});
                    }//grid[i][col]= NONE;
                }
            }
        }
        //Reverse capture against walls(horizontal)
       {
            int left= col- 1;
            while(left >= 0 && grid[row][left]== currentPiece) left--;
            if(left < 0){//If left hits wall
                    int right= col+ 1;
                while(right < 7 && grid[row][right]== currentPiece) right++;
                if(right < 7 && grid[row][right]== oppositePiece){
                    for(int j= left+ 1; j < right; j++){
                        mark.push_back({row, j});
                    }//grid[row][j]= NONE;
                }
            }
        }
       {
            int right= col+ 1;
            while(right < 7 && grid[row][right]== currentPiece) right++;
            if(right== 7){//If right hits wall
                int left= col- 1;
                while(left >= 0 && grid[row][left]== currentPiece) left--;
                if(left >= 0 && grid[row][left]== oppositePiece){
                    for(int j= left+ 1; j < right; j++){
                       //grid[row][j]= NONE; 
                        mark.push_back({row, j});
                    }
                }
            }
        }
       //Reverse capture against walls(vertical)
       {
            int up= row- 1;
            while(up >= 0 && grid[up][col]== currentPiece) up--;
            if(up < 0){//If up hits wall
                int down= row+ 1;
                while(down < 7 && grid[down][col]== currentPiece) down++;
                if(down < 7 && grid[down][col]== oppositePiece){
                    for(int i= up+ 1; i < down; i++){
                       //grid[i][col]= NONE;
                        mark.push_back({i, col});
                    }
                }
            }
        }
       {
            int down= row+ 1;
            while(down < 7 && grid[down][col]== currentPiece) down++;
            if(down== 7){
                int up= row- 1;
                while(up >= 0 && grid[up][col]== currentPiece) up--;
                if(up >= 0 && grid[up][col]== oppositePiece){
                    for(int i= up+ 1; i < down; i++){
                       //grid[i][col]= NONE; 
                        mark.push_back({i, col});
                    }
                }
            }
        }
        for(auto& position: mark){
            grid[position.first][position.second]= NONE;
        }
        mark.clear();
    }
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> PossibleMovesets(Piece player){
        std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> moves;
        for(int row1= 0; row1 < 7;++row1){
            for(int col1= 0; col1 < 7;++col1){
                if(player== AI && currentState== GameState::AI_SECOND_TURN &&
                    row1== lastAIMovedPiece.y && col1== lastAIMovedPiece.x){
                    continue;
                }
                if(grid[row1][col1]== player){
                    int directions[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

                    for(const auto& [dirRow, dirCol] : directions){
                        int row2= row1+ dirRow;
                        int col2= col1+ dirCol;
                        if(isValid(row1, col1, row2, col2)){
                            moves.push_back({{row1, col1},{row2, col2}});
                        }
                    }
                }
            }
        }
        return moves;
    }
    int evaluate() {
        int pieceScore= 0;
        //Counting pieces and positions(more focused on checking centers)
        for(int row= 0; row < 7; row++) {
            for(int col= 0; col < 7; col++) {
                if(grid[row][col]== AI) {
                    pieceScore+= 50;
                    if(row >= 2 && row <= 4 && col >= 2 && col <= 4) {
                        pieceScore+= 20;
                    }
                    if(row== 3 && col== 3) {
                        pieceScore+= 30;
                    }
                    if(row== 0 || row== 6 || col== 0 || col== 6) {
                        pieceScore-= 25;
                    }
                } else if(grid[row][col]== PLAYER) { // player version
                    pieceScore-= 50;  
                    if(row >= 2 && row <= 4 && col >= 2 && col <= 4) {
                        pieceScore-= 20;
                    }
                    if(row== 3 && col== 3) {
                        pieceScore-= 30;  
                    }
                    if(row== 0 || row== 6 || col== 0 || col== 6){
                        pieceScore+= 25;
                    }
                }
            }
        }
        //piece safety
        for(int row= 0; row < 7; row++) {
            for(int col= 0; col < 7; col++) {
                if(grid[row][col]== AI) {
                    //horizontal
                    if(col > 0 && col < 6) {
                        if((grid[row][col-1]== PLAYER || col== 1) &&(grid[row][col+1]== PLAYER || col== 5)) {
                            pieceScore-= 40;
                        }
                    }
                    //vertical
                    if(row > 0 && row < 6) {
                        if((grid[row-1][col]== PLAYER || row== 1) &&(grid[row+1][col]== PLAYER || row== 5)) {
                            pieceScore-= 40;  
                        }
                    }
                    //capture threats
                    if(col > 0 && col < 6) {
                        if(grid[row][col-1]== PLAYER && grid[row][col+1]== NONE) {
                            pieceScore+= 15;
                        }
                        if(grid[row][col+1]== PLAYER && grid[row][col-1]== NONE) {
                            pieceScore+= 15;
                        }
                    }
                    if(row > 0 && row < 6) {
                        if(grid[row-1][col]== PLAYER && grid[row+1][col]== NONE) {
                            pieceScore+= 15;
                        }
                        if(grid[row+1][col]== PLAYER && grid[row-1][col]== NONE) {
                            pieceScore+= 15;
                        }
                    }
                }
            }
        }
        //mobility
        for(int row= 0; row < 7; row++) {
            for(int col= 0; col < 7; col++) {
                if(grid[row][col]== AI) {
                    int availableMoves= 0;
                    int directions[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
                    for(const auto& [dirRow, dirCol]: directions) {
                        int newRow= row + dirRow;
                        int newCol= col + dirCol;
                        if(newRow >= 0 && newRow < 7 && newCol >= 0 && newCol < 7 && grid[newRow][newCol]== NONE) {
                            availableMoves++;
                        }
                    }
                    pieceScore+= availableMoves* 10;
                }
            }
        }
        return pieceScore;
    }

    bool gameCondition(){
        int aiCount= countPieces(AI);
        int playerCount= countPieces(PLAYER);
        if(aiCount==0 && playerCount== 0){
           //draw
            gameResult= "Draw. Both players have no pieces left.";
            return true;
        }
        if(aiCount==1 && playerCount== 1){
            gameResult= "Draw. Both players have only one piece left.";
            return true;
        }
        if(aiCount==0){
           //player wins
            gameResult= "Player wins";
            return true;
        }
        if(playerCount== 0){
            gameResult= "AI wins";
            return true;
           //ai wins
        }
        if(moveCount >= 50){
            if(aiCount== playerCount){
                gameResult= "After 50 moves. both players have \nsame amount of pieces. Its Draw";
               //draw
            }
            else if(playerCount > aiCount){
                gameResult= "After 50 moves. Player has more \npieces than AI. Player wins";
               //player wins
            }
            else{
                gameResult= "After 50 moves. AI has more pieces \nthan player. AI wins";
               //ai wins
            }
            return true;
        }
        return false;
    }

public:
    GameState currentState;

    Board(int size= 100): cellSize(size), currentState(GameState::AI_FIRST_TURN), selectedPiece({-1,-1}), lastMovedPiece({-1,-1}){
        grid= std::vector<std::vector<Piece>>(7, std::vector<Piece>(7, NONE));
    }
    void Init(){
       //AI pieces
        grid[0][0]= AI;
        grid[2][0]= AI;
        grid[4][6]= AI;
        grid[6][6]= AI;
       //Player pieces
        grid[0][6]= PLAYER;
        grid[2][6]= PLAYER;
        grid[4][0]= PLAYER;
        grid[6][0]= PLAYER;
    }
    void Draw(){
        for(int row= 0; row < 7; row++){
            for(int col= 0; col < 7; col++){
                int x= col* cellSize;
                int y= row* cellSize;

                DrawRectangleLines(x, y, cellSize, cellSize, GRAY);
                if(grid[row][col]== AI){
                    DrawTriangle(
                          {static_cast<float>(x + (cellSize/2)), static_cast<float>(y+ 10)},
                          {static_cast<float>(x + 10), static_cast<float>(y+ cellSize- 10)},
                          {static_cast<float>(x + cellSize- 10), static_cast<float>(y+ cellSize- 10)},
                            RED
                    );
                } else if(grid[row][col]== PLAYER){
                    DrawCircle(x+ cellSize/2, y+ cellSize/2, cellSize/3, RED);
                }
                if(row== selectedPiece.y && col== selectedPiece.x){
                    DrawRectangleLines(x, y, cellSize, cellSize, GREEN);
                    if(currentState== GameState::PLAYER_FIRST_TURN || currentState== GameState::PLAYER_SECOND_TURN){
                        if(isValid(row, col, row+ 1, col)){//Down
                            DrawCircle(x+ cellSize/2, y+ cellSize+ cellSize/2, cellSize/5, GREEN);
                        }
                        if(isValid(row, col, row- 1, col)){//Up
                            DrawCircle(x+ cellSize/2, y- cellSize+ cellSize/2, cellSize/5, GREEN);
                        }
                        if(isValid(row, col, row, col+ 1)){//Right
                            DrawCircle(x+ cellSize+ cellSize/2, y+ cellSize/2, cellSize/5, GREEN);
                        }
                        if(isValid(row, col, row, col- 1)){//Left
                            DrawCircle(x- cellSize+ cellSize/2, y+ cellSize/2, cellSize/5, GREEN);
                        }
                    }
                }
            }
            if(gameCondition()) popup();
        }
        //game state
        std::string state, turn;
        switch(currentState){
            case GameState::AI_FIRST_TURN:
                state= "AI's Turn(First Move)";
                break;
            case GameState::AI_SECOND_TURN:
                state= "AI's Turn(Second Move)";
                break;
            case GameState::PLAYER_FIRST_TURN:
                state= "PLAYER's Turn(First Move)";
                break;
            case GameState::PLAYER_SECOND_TURN:
                state= "PLAYER's Turn(Second Move)";
                break;
        }
        turn= "Total Move "+ std::to_string(moveCount);
        DrawText(state.c_str(), 10, GetScreenHeight()- 30, 20, BLACK);
        DrawText(turn.c_str(), 500, GetScreenHeight()-30, 20, BLACK);
    }
    void popup(){
        int popupWidth= 400;
        int popupHeight= 200;
        int x=(GetScreenWidth()- popupWidth)/2;
        int y=(GetScreenHeight()- popupHeight)/2;
        DrawRectangle(x, y, popupWidth, popupHeight, Fade(GRAY, 0.8f));
        DrawRectangleLines(x, y, popupWidth, popupHeight, DARKGRAY);
        DrawText(gameResult.c_str(), x+ 20, y+ 40, 20, DARKBLUE);
        int buttonWidth= 100;
        int buttonHeight= 40;
        int buttonX= x+(popupWidth- buttonWidth)/2;
        int buttonY= y+ popupHeight- buttonHeight- 20;

        DrawRectangle(buttonX, buttonY, buttonWidth, buttonHeight, LIGHTGRAY);
        DrawRectangleLines(buttonX, buttonY, buttonWidth, buttonHeight, DARKGRAY);
        DrawText("OK", buttonX+ 30, buttonY+ 10, 20, BLACK);

        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            int mouseX= GetMouseX();
            int mouseY= GetMouseY();
            if(mouseX >= buttonX && mouseX <= buttonX+ buttonWidth &&
                mouseY >= buttonY && mouseY <= buttonY+ buttonHeight){
                CloseWindow();
            }

        }
    }
    void moveset(const std::pair<int, int>& from, const std::pair<int, int>& to){
        if(isValid(from.first, from.second, to.first, to.second)){
            grid[to.first][to.second]= grid[from.first][from.second];
            grid[from.first][from.second]= NONE;
            checkCaptures(to.first, to.second);
        }
    }
    MinimaxResult minimax(int depth, bool isMax, int alpha, int beta){
        if(depth== 0 || gameCondition()){
            return MinimaxResult(evaluate(),{-1,-1},{-1,-1});
        }
        auto moves= PossibleMovesets(isMax ? AI : PLAYER);
        if(isMax){
            int maxEvaluation= std::numeric_limits<int>::min();
            std::pair<int, int> bestFrom={-1,-1};
            std::pair<int, int> bestTo={-1,-1};
            for(const auto& [from, to] : moves){
                Board tempBoard= *this;
                tempBoard.moveset(from, to);
                auto evaluation= tempBoard.minimax(depth- 1, false, alpha, beta).pieceScore;
                if(evaluation > maxEvaluation){
                    maxEvaluation= evaluation;
                    bestFrom= from;
                    bestTo= to;
                }
                alpha= std::max(alpha, maxEvaluation);
                if(beta <= alpha) break;
            }
            return MinimaxResult(maxEvaluation, bestFrom, bestTo);
        } else{
            int minEvaluation= std::numeric_limits<int>::max();
            std::pair<int, int> bestFrom= {-1,-1};
            std::pair<int, int> bestTo= {-1,-1};
            for(const auto& [from, to] : moves){
                Board tempBoard= *this;
                tempBoard.moveset(from, to);
                auto evaluation= tempBoard.minimax(depth- 1, true, alpha, beta).pieceScore;
                if(evaluation < minEvaluation){
                    minEvaluation= evaluation;
                    bestFrom= from;
                    bestTo= to;
                }
                beta= std::min(beta, minEvaluation);
                if(beta <= alpha) break;
            }
            return MinimaxResult(minEvaluation, bestFrom, bestTo);
        }
    }
    void AImovement(){
        int depth= 8;
        auto result= minimax(depth, true, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
        const auto &[from, to]= result.move;

        if(from.first!=-1 && from.second !=-1 &&
            to.first!=-1 && to.second !=-1){
            grid[to.first][to.second]= grid[from.first][from.second];
            grid[from.first][from.second]= NONE;
            if(currentState== GameState::AI_FIRST_TURN){
                lastAIMovedPiece={static_cast<float>(to.second), static_cast<float>(to.first)};
            }
            checkCaptures(to.first, to.second);
        }
    }
    void processAITurn(){
        if(currentState== GameState::AI_FIRST_TURN){
            AImovement();
            moveCount++;
            if(countPieces(AI) > 1){
                currentState= GameState::AI_SECOND_TURN;
            } else{
                currentState= GameState::PLAYER_FIRST_TURN;
                lastAIMovedPiece={-1,-1};
            }
        } else if(currentState== GameState::AI_SECOND_TURN){
            AImovement();
            moveCount++;
            currentState= GameState::PLAYER_FIRST_TURN;
            lastAIMovedPiece={-1,-1};
        }
        gameCondition();
    }

    bool HandleMouseClick(int mouseX, int mouseY){
        int col= mouseX/cellSize;
        int row= mouseY/cellSize;
        if(row < 0 || row >= 7 || col < 0 || col >= 7){
            return false;
        }
        Piece currentPlayer=(currentState== GameState::AI_FIRST_TURN || currentState== GameState::AI_SECOND_TURN) ? AI : PLAYER;
        int maxMoves=(countPieces(currentPlayer) > 1)? 2 : 1;
        if(selectedPiece.x==-1){
            if(grid[row][col]== currentPlayer){
                if(currentState== GameState::PLAYER_SECOND_TURN){
                    if(row== lastMovedPiece.y && col== lastMovedPiece.x){
                        return false;
                    }
                }
                selectedPiece={static_cast<float>(col), static_cast<float>(row)};
                return true;
            }
        } else{
            if(isValid(selectedPiece.y, selectedPiece.x, row, col)){
                grid[row][col]= grid[selectedPiece.y][selectedPiece.x];
                grid[static_cast<int>(selectedPiece.y)][static_cast<int>(selectedPiece.x)]= NONE;
                checkCaptures(row, col);
                lastMovedPiece={static_cast<float>(col), static_cast<float>(row)};

                switch(currentState){
                    case GameState::PLAYER_FIRST_TURN:
                        currentState=(maxMoves > 1) ? GameState::PLAYER_SECOND_TURN : GameState::AI_FIRST_TURN;
                        break;
                    case GameState::PLAYER_SECOND_TURN:
                        currentState= GameState::AI_FIRST_TURN;
                        lastMovedPiece={-1,-1};
                        break;
                    default:
                        break;
                }
                selectedPiece={-1,-1};
                moveCount++;
                gameCondition();
                return true;
            } else{
                selectedPiece={-1,-1};
                return false;
            }
        }
        return false;
    }
    GameState getCurrentState() const{
        return currentState;
    }
};