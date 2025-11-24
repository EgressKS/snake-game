# 🐍 Snake Game (C++ Console)

A classic Snake Game implemented in C++ for the Windows console.  
This project demonstrates real-time keyboard handling, cursor movement, OOP design, and a complete game loop in a console environment.


## 🎮 Features

- Real-time snake movement  
- Supports Arrow Keys and W/A/S/D  
- Snake grows when it eats food  
- Live score display  
- Screen wrapping (snake appears on opposite edges)  
# 🐍 Snake Game (C++ Console)

A simple, fast Snake game written for the Windows console. This fork adds borders, dynamic speed, pause/restart, and high-score persistence.

## 🎮 Highlights

- Real-time movement with Arrow Keys or W/A/S/D
- Pause (`P`), Restart after game-over (`R`), Quit (`Q`)
- Visible border (hitting a wall ends the game)
- Score and persistent high score saved to `highscore.txt`
- Dynamic speed: game speeds up as your score increases

## 🕹️ Controls

| Key / Input | Action |
|-------------|--------|
| Arrow keys or `W`/`A`/`S`/`D` | Move snake |
| `P` | Pause (press any key to resume) |
| `R` (after game over) | Restart the game |
| `Q` (after game over) | Quit the program |

## ⚙️ Requirements

- Windows OS (uses `windows.h` and WinAPI console functions)
- A C++ compiler that targets Windows (MinGW/GCC or MSVC)

Note: This program will not run under WSL or a Linux terminal because it depends on Windows-specific headers.

## 🛠️ Build & Run (PowerShell / Command Prompt)

Using MinGW / g++:

```powershell
g++ -std=c++17 -O2 -o snake.exe .\game.cpp
.\snake.exe
```

Using MSVC (Developer Command Prompt):

```powershell
cl /EHsc /std:c++17 game.cpp
game.exe
```

When you run the game the first time it will create (or update) `highscore.txt` in the same folder to store the best score.

## 📂 What's in the code

- `game.cpp` — main game source: `Snake` and `Board` classes, input handling, rendering, and main loop.
- `highscore.txt` — created at runtime to persist the top score (not committed to repo)

## 🧠 Design Notes

- The `Snake` class manages body segments and movement.
- The `Board` class handles drawing, spawning food, input, scoring, borders, and saving/loading high score.
- Speed is calculated using `max(40, 200 - score * 10)` so the game gets gradually more challenging.

## 📸 Sample Output

The console will display the snake (`O`), food (`o`), a top status line with `Score` and `High`, and a `#` border around the playfield.

## Next improvements (ideas)

- Add color output to differentiate head/food/border
- Add a simple start menu with difficulty selection
- Add levels or obstacles

## Contributing

PRs and issues welcome — small, focused changes are easiest to review.

## License

MIT

---

Author: Krishna Sumit

Enjoy! Pull requests welcome.
---
