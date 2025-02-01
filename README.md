# Strategic Board Game
A 7x7 board game implementation with AI opponent using the Minimax algorithm. This project was developed as part of the CSE462 course at Yeditepe University.
## Game Description
This is a strategic board game played on a 7x7 grid where two players (AI vs Human) compete to capture each other's pieces through tactical movements and positioning.
## Features
* GUI implementation using Raylib
* AI opponent using Minimax algorithm with Alpha-Beta pruning
* Turn-based gameplay with move validation
* Multiple capture mechanics
* Game state tracking and win condition detection

## Game Rules and Board Setup
* 7x7 grid board
* Player 1 (AI) uses triangle pieces
* Player 2 (Human) uses circle pieces
* Each player starts with 4 pieces in predefined positions
* AI moves first

## Movement Rules
* Pieces can move horizontally and vertically (no diagonal moves)
* If a player has more than one piece, they must make two consecutive moves with different pieces
* If a player has only one piece remaining, they make a single move

## Capturing Rules

* If the player’s single piece or group of pieces are between the wall and the opponent
  piece, they are captured
* If the player’s single piece or group of pieces are between two opponent pieces, it is
  captured
* If both player’s pieces are between two opponent pieces, all of these pieces are captured

## Game Conditions

### Draw conditions:

* Both players have no pieces remaining
* Both players have only one piece remaining
* After 50 moves, both players have the same number of pieces

### Win conditions:
* One player has pieces while the opponent has none
* After 50 moves, the player with more pieces wins



## Technical Details and Dependencies

Raylib graphics library  and C++ Standard Library

## Key Components

* game.cpp: Main game loop and window management
* board.cpp: Game logic, AI implementation, and board state management

## AI Implementation
The game uses the Minimax algorithm with Alpha-Beta pruning for the AI player. The evaluation function considers:

* Piece count
* Board position (center control)
* Piece safety and Capturing opponents pieces
* Movement flexibility

## Building and Running
### Prequisites
* CMake (minimum version 3.10)
* C++ compiler supporting C++11 or higher
* Raylib library
### Build Instructions 
* Clone the repository
* Create and navigate to a build directory:
``mkdir build && cd build``
* Build the project:
``cmake .. # Generates build files using the CMakeLists.txt from parent directory``
`` make # Compiles the project using the generated build files``
* Run the executable:
``./StrategicBoardGame``
### Alternative Manual Compilation
If you prefer not to use CMake, you can compile directly:
``g++ game.cpp board.cpp -o game -lraylib``

## Controls

* Use mouse left-click to select and move pieces
* Click a piece to select it
* Click a valid destination (highlighted in green) to move the selected piece
* The game state and move count are displayed at the bottom of the window

