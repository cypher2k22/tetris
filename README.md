# tetris
Falling Puzzle Game (SFML)
A simple falling-block puzzle game developed using C++ and SFML.
This project demonstrates basic game mechanics such as piece movement, collision detection, line clearing, and mouse-based controls.

🎮 Features

Falling block puzzle gameplay (similar to classic block games)
Randomly generated pieces
Grid-based collision detection
Line clearing logic
Mouse drag support to move pieces horizontally
Automatic gravity-based falling
Game Over detection and display

🧱 Game Controls

Mouse Drag (Left / Right) – Move the falling piece horizontally
Pieces fall automatically over time
Game ends when blocks reach the top of the grid

🛠️ Technologies Used

Language: C++
Graphics Library: SFML (Simple and Fast Multimedia Library)
Concepts Used:
Object-Oriented Programming (OOP)
Vectors and STL algorithms
Event handling
Real-time rendering
Collision detection

📁 Project Structure
.
├── main.cpp        // Game source code
├── README.md       // Project documentation

⚙️ Requirements

C++17 or later
SFML 2.6 or compatible version

A C++ compiler (g++, clang, or MSVC)

▶️ How to Build and Run
1. Install SFML

Make sure SFML is installed on your system.

2. Compile

Example using g++:

g++ main.cpp -o FallingPuzzle -lsfml-graphics -lsfml-window -lsfml-system

3. Run
./FallingPuzzle

🧠 Game Logic Overview

Blocks are represented using sf::RectangleShape

Pieces consist of multiple blocks with offset-based movement

A 2D grid tracks occupied positions

Pieces lock into place when they can no longer fall

Full rows are detected, cleared, and upper blocks shift downward

Game ends when a new piece overlaps the top row

🚀 Future Improvements

Keyboard controls (left, right, rotate)

Piece rotation

Score tracking

Next-piece preview

Sound effects and animations

Better color variation per piece

📜 License

This project is created for learning and educational purposes.
You are free to modify and use it as needed.
