# Minesweeper


## Running the program

### Windows
  Run `mines.exe` file

### Mac

  1. Run `make` 
  2. Then `./mines`


## Tile collections

![alt text](/minesweeper2.bmp)  <img src="minesweeper.bmp" width="96px"></img>

## Screenshots

Below are some in-game screenshots:

<img src="/screenshots/start.png" width="400px"></img>

<img src="/screenshots/detonate.png" width="400px"></img>

## Implementation

### Data structures
Program makes use of three 2D arrays to store the following information:
 * Number of neighbouring bombs (any number >=9 signifies bomb within itself) 
 * Boolean cover (whether or not the tile has been revealed)
 * Tile icon (whether the tile has been flagged/marked)

### Algorithms

I considered two algorithms for implementing this:
#### Breadth-first search
How it works:
 * Start opening adjacent tiles after the user has chosen a tile to click.
 * Uses breadth-first search algorithm to look for all the tile nearby until numbered tiles are found.
Pros:
 * Implementation is simplest
Cons:
 * Takes a while to load when number of tiles to be opened is large.

#### Breadth-first search
Alternative implementation (original idea):
How it works:
 * Identify tiles within the same “island” (group of tiles surrounded by numbered tiles in standard Minesweeper game) before game starts
 * Use union-find algorithm to find all these tiles
 * Not possible to implement simply as certain numbered tiles could belong to different islands depending on how the user clicks on the tiles during the game.
 
## Tutorial credits

### SFML tutorial
Coding Made Easy: https://www.youtube.com/channel/UCas000yWtwjvFzD2zB9Nzmw
