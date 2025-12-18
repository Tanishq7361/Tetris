# 🎮Tetra Twist🎮

# Tetra Twist is a Tetris Game in C++
This is a console-based implementation of the classic Tetris game using C++. The project includes all the standard Tetris mechanics such as falling tetrominoes, line clearing, scoring, and game-over conditions.

## 🚀 Features

- **🧩 Tetromino Pieces**: All seven standard tetrominoes (I, O, T, S, Z, J, L) are implemented.
- **📊 Grid Representation**: The game board is a 10x20 grid.
- **🎮 Piece Movement**: Players can move pieces left, right, rotate, and drop them.
- **⏳ Gravity**: Tetrominoes fall at a constant speed, which increases as the game progresses.
- **🚫 Collision Detection**: Prevents blocks from overlapping or moving out of bounds.
- **🧹 Line Clearing**: Full rows are removed, and the grid shifts downward.
- **🏆 Scoring System**: Points are awarded for cleared lines.

## 🕹️ Controls

&ensp;&ensp;`A`  or  **⬅️ Left Arrow (←)** : Move left (to the left on the screen).<br>
&ensp;&ensp;`S`  or  **🔽 Down Arrow (↓)** : Soft drop (move the piece down faster).<br>
&ensp;&ensp;`D`  or  **➡️ Right Arrow (→)** : Move right (to the right on the screen).<br>
&ensp;&ensp;`W` : Rotate clockwise 90 degree.<br>
&ensp;&ensp;`Q` : Pause or quit the game.<br>
&ensp;&ensp;`␣`  or  **Spacebar** : Hard drop (instantly drop the piece to the bottom).<br>
&ensp;&ensp; **Enter** : Press Enter to play again. <br>
&ensp;&ensp; **Escape (ESC)** : Opens Quit menu. <br>

## 🎲 Game Progression

- The falling speed of tetrominoes increases after a certain number of cleared lines.
- The current score is displayed on-screen.
- Highscore till now is displayed.
- The game ends when blocks reach the top of the grid.

## 💻 Technical Details

- **🖥️ Language**: C++ (C++17 or later recommended).
- **📂 Data Structures**: Structs or classes are used to represent tetrominoes.
- **📊 Grid Representation**: 2D arrays or vector-based grids are used for the game board.
- **⌨️ Input Handling**: Real time user input is handled efficiently.
- **🚫 Collision Detection**: Efficient collision detection and game logic are implemented.

## 🛠️ How to Run

1. Clone the repository to your local machine.
2. Compile the source code using a C++ compiler (e.g., `g++`).
3. Run the executable file to start the game.

```bash
g++ -o tetris main.cpp
./tetris
```

## 📊 Data Structure Analysis
### Data Structure Used 
This game primarily utilizes the following data structures:

1. **Vector (`std::vector`):**
   - Game Board (board): A 2D vector (vector<vector<int>>) representing the Tetris grid, where 1 indicates a filled cell and 0 represents an empty cell.
   - Color Board (colorBoard): Similar to board, but stores color information for each block.
   - Tetromino Shapes (tetrominoes): A vector of Tetromino structures, each containing a 2D vector to represent its shape.
   - Tetromino Colors (COLORS): A vector storing available color codes.
   - Game Board (board): Defined as vector<vector<int>> board(HEIGHT, vector<int>(WIDTH, 0));
   - Stores 1 for filled cells and 0 for empty cells.
   - Color Board (colorBoard): Stores the color of each occupied cell to manage rendering.
3. **Struct (`struct`):**
   - shape - A 2D vector representing the shape of the tetromino.
   - x and y - Position of the tetromino on the board.
   - color - An integer representing the tetromino's color.

## Credits💳:

👨‍💻[Tanishq Shah](https://github.com/Tanishq7361) <br>
👨‍💻[Yash Oza](https://github.com/Yash-Oza-ui) <br>
👨‍💻[Dhvanit Shah](https://github.com/shahdhvanit) <br>
👨‍💻[Yash Vaghela](https://github.com/Yash071-ma) <br>
👨‍💻[Om Sutariya](https://github.com/thunder1907)

## License
This project is licensed under the **MIT License.** 

     
