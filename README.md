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

Sample variables.

`WIDTH = 20; HEIGHT = 10;`

 <img src="/screenshots/start.png" width="400px"></img>
 
`WIDTH = 25; HEIGHT = 15;` 

 <img src="/screenshots/game.png" width="400px"></img>
 
 
 * `BOMBS` - number of bombs planted into the game
 * `char * SKINTYPE` - source file name for Minesweeper skin
 
Here are some available designs:

![alt text](/minesweeper2.bmp) (Tile size: 24px)

<img src="minesweeper.bmp" width="96px"></img> (Tile size: 200px)

 
 * `sTS` - Tile size of source image in pixels
 * `TS` - tile size as seen by viewer.
 




## Implementation

### Data structures
Program makes use of three 2D `std::vectors` to store the following information:
 * Number of neighbouring bombs (any number >=9 signifies bomb within itself) 
 * Boolean cover (whether or not the tile has been revealed)
 * Tile icon (whether the tile has been flagged/marked)

### Algorithms

I considered two algorithms for implementing this:
#### Current implementation: Breadth-first search
  **How it works**:
   * Start opening adjacent tiles after the user has chosen a tile to click.
   * Uses breadth-first search algorithm to look for all the tile nearby until numbered tiles are found.

  **Pros**:
   * Implementation is simplest

  **Cons**:
   * Takes a while to load when number of tiles to be opened is large.

#### Alternative implementation (original idea): Union-find Algorithm
  **How it works**:
   * Identify tiles within the same “island” (group of tiles surrounded by numbered tiles in standard Minesweeper game) before game starts
   * Use union-find algorithm to find all these tiles
   * Not possible to implement simply as certain numbered tiles could belong to different islands depending on how the user clicks on the tiles during the game.
 
## Technologies used
The following were used to create this game: 
 * C++
 * Simple and Fast Multimedia Library (SFML)

## Credits

### SFML tutorial
Coding Made Easy: https://www.youtube.com/channel/UCas000yWtwjvFzD2zB9Nzmw
