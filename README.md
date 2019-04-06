# Minesweeper


## Running the program

### Windows

  1. Download the entire repository.
  2. Run `mines.exe` file by double-clicking on file.
  
  Game screen should show up immediately.
  
### Linux/MacOS

  1. Download the entire repository.
  2. Open repo in Terminal and run the following commands
```
 $ make
 $ ./mines
```
  
  Game screen should show up immediately.

## Screenshots

Below are some in-game screenshots:

<img src="/screenshots/start.png" width="400px"></img>

<img src="/screenshots/detonate.png" width="400px"></img>

## Instructions

Play this game like your standard Minesweeper game. Right-click to flag tiles that you think have bombs underneath. Do this before time runs out.


## Customizability

#### Variables in `mines.cpp`

The following are variables that can be edited in  file:
 * `WIDTH`and `HEIGHT` - number of tiles in the Minesweeper board 

| Variable Configuration | Screenshot |
| -- | -- |
| `WIDTH = 20; HEIGHT = 10;` | <img src="/screenshots/start.png" width="400px"></img> |
|`WIDTH = 25; HEIGHT = 15;` | <img src="/screenshots/game.png" width="400px"></img> |
 
 * `BOMBS` - number of bombs planted into the game
 * `TS` - tile size in your screen
 * `char * SKINTYPE` - source file name for Minesweeper skin
 * `sTS` - Tile size of source image in pixels
 
Here are some available skins:

| File Name       | Tile           | Source tile size  |
| ------------- |-------------| -----|
| `minesweeper2.bmp` | ![alt text](/minesweeper2.bmp) | 24px |
| `minesweeper.bmp`  | <img src="minesweeper.bmp" width="96px"></img>     | 200px |

## Implementation (Data structures)
Program makes use of three 2D `std::vectors` to store the following information:
 * Number of neighbouring bombs (any number >=9 signifies bomb within itself) 
 * Boolean cover (whether or not the tile has been revealed)
 * Tile icon (whether the tile has been flagged/marked)

## Implementation (Algorithms)

I considered two algorithms for implementing this:
### Breadth-first search (Current implementation)
  **How it works**:
   * The program will only begin to look for adjacent tiles to open once user has clicked on a tile.
   * In the function, `openBox`, we breadth-first search algorithm to look for all the tile nearby until numbered tiles are found.

  **Pros**:
   * Simple to implement

  **Cons**:
   * Might take a while to load when there is a large number of tiles to be opened.

### Union-find (Alternative implementation)
  **How it works**:
   * Identify tiles within the same “island” (group of tiles surrounded by numbered tiles to be opened during a single tile click) before game starts
   * Use union-find algorithm to find all these tiles
   * Not possible to implement simply as certain numbered tiles could belong to different islands depending on how the user clicks on the tiles during the game.
 
## Technologies used
The following were used to create this game: 
 * C++
 * Simple and Fast Multimedia Library (SFML)
 
## Future Development

* Function calls to be made recursive and elegant

## Credits

Developed by: Tiffany Chong

SFML tutorial: Coding Made Easy (https://www.youtube.com/channel/UCas000yWtwjvFzD2zB9Nzmw)


