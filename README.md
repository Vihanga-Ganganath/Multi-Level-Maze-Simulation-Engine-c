# Multi-Level-Maze-Simulation-Engine-C

A terminal-based strategy game engine written in C that simulates three players navigating a 3D, multi-floor maze environment. The system features dynamic map generation via configuration files and implements complex movement logic, including collision detection and state-based player effects.

## 🚀 Features
* **3D Navigation:** Players move across multiple floors using stairs (up/down) and poles (slide down).
* **Dynamic Map Loading:** The game environment (Walls, Stairs, Poles, Flag location) is parsed from external `.txt` files, allowing for custom map creation without recompiling.
* **State Machine Logic:** Players are affected by "Bawana" cells which trigger states like "Disoriented" (random movement), "Food Poisoned" (skip turns), or "Triggered" (movement multipliers).
* **Collision System:** robust checking for walls, boundaries, and restricted areas.
* **Event Logging:** Automatically generates a `log.txt` file to record detailed game events and errors for debugging.

## 🛠 Technical Highlights
* **Dynamic Memory Management:** Uses `calloc` and `realloc` to handle variable numbers of game objects (stairs/walls) loaded from files.
* **File I/O:** Custom parsers using `sscanf` to interpret structured data from configuration files.
* **Struct Implementation:** utilized complex structs to manage Player states and Game Object properties.

## 🎮 How to Run
1.  Clone the repository.
2.  Compile the source code:
    ```bash
    gcc main.c game.c -o maze_game
    ```
3.  Run the executable:
    * **Windows:** `maze_game.exe`
    * **Mac/Linux:** `./maze_game`

## 📂 Configuration Files
* `walls.txt`: Defines boundaries to block movement.
* `stairs.txt`: Connects floors (upward/downward movement).
* `poles.txt`: One-way transport to lower floors.
* `seed.txt`: Controls the Random Number Generator (RNG) for reproducible gameplay.
