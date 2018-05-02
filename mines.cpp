#include<iostream>
#include<SFML/Graphics.hpp>
#include<string>
#include<sstream>
#include <time.h>

int WIDTH= 10;
int HEIGHT= 10;
int BOMBS = 2;
int sTS = 32; //source tile size
int TS = 40;
const char * SKINTYPE = "minesweeper2.bmp";

typedef std::vector< std::vector<int> > grid;
//typedef std::vector< std::vector<bool> > boolGrid;

// function to generate a random grid with bombs and numbers
grid makeGrid(int height, int width, int bombs) // makes a numerical grid
{
    //create an empty grid
    grid numGrid;
    std::vector<int> zeroRow; // create empty row
    for (int j=0; j<width; j++)
        {zeroRow.push_back(0);}
    for (int i =0; i<height; i++)
    {numGrid.push_back(zeroRow);}

    // place bombs and hints
    for (int b = 0; b<BOMBS; b++)
    {
        int x = rand()% width;
        int y = rand()% height;

        while (numGrid[y][x]>=9)
        {
            x = rand()% width;
            y = rand()% height;
        }

        numGrid[y][x] = 9;
        // add one hint each of the surrounding bombs
        if (x>0 && x<width-1)
        {
            numGrid[y][x-1] +=1;
            numGrid[y][x+1] +=1;

            if (y<height-1)
            {
                numGrid[y+1][x-1] +=1;
                numGrid[y+1][x] +=1;
                numGrid[y+1][x+1] +=1;
            }
            if (y>0)
            {
                numGrid[y-1][x-1] +=1;
                numGrid[y-1][x] +=1;
                numGrid[y-1][x+1] +=1;

            }


        }
        else if (x<=0)
        {
            numGrid[y][1] +=1;

            if (y<height-1)
            {
                numGrid[y+1][0] +=1;
                numGrid[y+1][1] +=1;
            }
            if (y>0)
            {
                numGrid[y-1][0] +=1;
                numGrid[y-1][1] +=1;
            }


        }
        else if (x>=width-1)
        {
            numGrid[y][width-2] +=1;

            if (y<width-1)
            {
                numGrid[y+1][width-1] +=1;
                numGrid[y+1][width-2] +=1;
            }
            if (y>0)
            {
                numGrid[y-1][width-1] +=1;
                numGrid[y-1][width-2] +=1;
            }

    }


    }

    return numGrid;

}

// function to make the grid a vertex array for use of the drawable class
sf::VertexArray makeGArray(grid numGrid)
{
    sf::VertexArray GArray;
    GArray.resize(numGrid.size() * numGrid[0].size() *4);
    GArray.setPrimitiveType(sf::Quads);

    int v = 0;
    for (int a=0; a<numGrid.size(); a++)
    {
        for (int b=0; b<numGrid[0].size(); b++)
        {

            GArray[v].position = sf::Vector2f(b*TS, a*TS);
            GArray[v+1].position = sf::Vector2f(b*TS+TS, a*TS);
            GArray[v+2].position = sf::Vector2f(b*TS+TS, a*TS+TS);
            GArray[v+3].position = sf::Vector2f(b*TS, a*TS+TS);

            int number = numGrid[a][b];
            int X; int Y;
            if (number>=9){X=1; Y=4;}
            else
            {X=number%3; Y=number/3;}

            GArray[v].texCoords = sf::Vector2f(X*sTS,Y*sTS);
            GArray[v+1].texCoords = sf::Vector2f(X*sTS+sTS,Y*sTS);
            GArray[v+2].texCoords = sf::Vector2f(X*sTS+sTS,Y*sTS+sTS);
            GArray[v+3].texCoords = sf::Vector2f(X*sTS,Y*sTS+sTS);

            v+=4;
        }
    }
    return GArray;
    return GArray;
}

// draws the vertex array for the back of the minesweeper board
class gMap : public sf::Drawable, public sf::Transformable
{
public:
    void loadMap(sf::VertexArray GArray, const char * text)
    {
        this->m_vertices = GArray;
        this->m_tileset.loadFromFile(text);
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.texture = &m_tileset;
        states.transform *= getTransform();
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};

// obtain the 8 or less numbers surrounding that number in the grid
std::vector<int> getNearby(int n, grid numGrid)
{
    int h = numGrid.size();
    int w = numGrid[0].size();
    std::vector<int> nearby;
    int X=n%w; int Y=n/w;
    if (X>0)
    {
        nearby.push_back(n-1);

        if (Y>0)
        {
            nearby.push_back(n-w);
            nearby.push_back(n-w-1);
        }

        if (Y<h-1)
        {
            nearby.push_back(n+w);
            nearby.push_back(n+w-1);
        }
    }

    if (X<w-1)
    {
        nearby.push_back(n+1);

        if (Y>0)
        {
            nearby.push_back(n-w);
            nearby.push_back(n-w+1);
        }

        if (Y<h-1)
        {
            nearby.push_back(n+w);
            nearby.push_back(n+w+1);
        }
    }
    return nearby;
}


// function to make the cover grid
grid makeCGrid(int height, int width) // makes a grid (0 for nothing, 1 for cover, 2 for flag, 3 for qmark)
{
        //create an empty grid
        grid CGrid;
        std::vector<int> zeroRow; // create empty row
        for (int j=0; j<width; j++)
            {zeroRow.push_back(1);}
        for (int i =0; i<height; i++)
        {CGrid.push_back(zeroRow);}


        return CGrid;
}

// function make a new vertex array out of the cover grid for use of drawable class
sf::VertexArray makeCArray(grid CGrid, int mouseX, int mouseY)
{
    sf::VertexArray CArray;
    int coverSize = 0;
    for (int i = 0; i<WIDTH*HEIGHT; i++)
    {
        if (CGrid[i/WIDTH][i%WIDTH])
        {
            coverSize++;
        }
    }

    CArray.resize(coverSize * 4);
    CArray.setPrimitiveType(sf::Quads);

    int v = 0;
    for (int a=0; a<CGrid.size(); a++)
    {
        for (int b=0; b<CGrid[0].size(); b++)
        {
            if (CGrid[a][b])
            {
            CArray[v].position = sf::Vector2f(b*TS, a*TS);
            CArray[v+1].position = sf::Vector2f(b*TS+TS, a*TS);
            CArray[v+2].position = sf::Vector2f(b*TS+TS, a*TS+TS);
            CArray[v+3].position = sf::Vector2f(b*TS, a*TS+TS);
            int X; int Y;

            if (a == mouseY && b == mouseX)
            {
                {X = 2, Y = 4;}
            }

            else if (CGrid[a][b]==2)
            {X=2, Y=3;}

            else if (CGrid[a][b]==3)
            {X=1, Y=3;}

            else if (CGrid[a][b]==4)
            {X=0, Y=4;}

            else { X= 0; Y = 3;}

            CArray[v].texCoords = sf::Vector2f(X*sTS,Y*sTS);
            CArray[v+1].texCoords = sf::Vector2f(X*sTS+sTS,Y*sTS);
            CArray[v+2].texCoords = sf::Vector2f(X*sTS+sTS,Y*sTS+sTS);
            CArray[v+3].texCoords = sf::Vector2f(X*sTS,Y*sTS+sTS);

            v+=4;
            }
        }
    }
    return CArray;
}

// modifies the cover grid to flag a tile (my first attempt at pointers haha)
grid flagBox(grid &CGrid, int lmouseX, int lmouseY, int &bombsLeft)
{
    int &b = bombsLeft;
    int &i = CGrid[lmouseY][lmouseX];
    if (i==1)
    {b--;}
    if (i==2)
    {b++;}
    if (i==3)
    {i=1;}
    else {i++;}
    return CGrid;
}

//opens the boxes related to one box
bool openBox(grid &CGrid, int lmouseX, int lmouseY, grid numGrid, int &boxesOpened)
{
    int &bo = boxesOpened;
    int h = numGrid.size();
    int w = numGrid[0].size();
    std::vector<int> boxWZero;
    int &b = CGrid[lmouseY][lmouseX];
    b = 0;
    bo++;
    boxWZero.push_back(lmouseY*w+lmouseX);

    while (boxWZero.size()>0)
    {
        std::vector<int> nearby = getNearby(boxWZero[0], numGrid);
        for (int j=0; j<nearby.size(); j++)
        {
            int num = nearby[j];

            if (numGrid[num/w][num%w] == 0 && CGrid[num/w][num%w] == 1)
            {
                boxWZero.push_back(num);
            }
            if (CGrid[num/w][num%w]==1)
            {
                int &b = CGrid[num/w][num%w];
                b = 0;
                bo++;

            }

        }
        boxWZero.erase(boxWZero.begin());

    }
    return 0;
}

//opens all the boxes with bombs when the game is lost
int openBomb(grid &CGrid, grid numGrid, int lmouseX, int lmouseY)
{

    for (int i=0; i<numGrid.size(); i++)
    {
        for (int j=0; j<numGrid[0].size(); j++)
        {
            if (numGrid[i][j] >= 9)
            {
                int &p = CGrid[i][j];
                p = 0;
            }
        }
    }

    int &a = CGrid[lmouseY][lmouseX];
    a = 4;

    return 0;
}

int main()
{

    sf::RenderWindow window(sf::VideoMode(WIDTH*TS,HEIGHT*TS+3*TS), "Minesweeper", sf::Style::Close);
    sf::Font berlin;
    berlin.loadFromFile("BRLNSDB.ttf");
    sf::Clock clock;

    int mouseX = -1; int mouseY = -1;
    int lmouseX = -1; int lmouseY = -1;

    //main menu
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(WIDTH*TS, 3*TS));
    rectangle.setFillColor(sf::Color(245,190,120));
    sf::Sprite restart;
    sf::Texture restartT;
    restartT.loadFromFile("restart.bmp");
    restart.setPosition((WIDTH*TS-86-10), 15);
    restart.setTexture(restartT);

    bool playAgain = true;
    bool playing = true;
    bool win;

    while (playAgain)
    {

    srand (time(NULL));

    //ask for the number of bombs and the size of the board.
    /**std::cout << "Width: ";
    std::cin >> WIDTH;
    std::cout << "Height: ";
    std::cin >> HEIGHT;

    while (BOMBS >= WIDTH*HEIGHT)
    {
        std::cout << "Number of Bombs: ";
        std::cin >> BOMBS;
    }
    **/


    //grid
    grid numGrid = makeGrid(HEIGHT,WIDTH,BOMBS);
    sf::VertexArray GArray = makeGArray(numGrid);
    gMap myMap;
    myMap.setPosition(0,TS*3);
    myMap.loadMap(GArray, SKINTYPE);
    grid CGrid = makeCGrid(HEIGHT, WIDTH);

    //boxes opened text
    int boxesOpened = 0;
    std::string rBoxesOpened;
    std::ostringstream convert;
    convert << boxesOpened;
    rBoxesOpened = convert.str();
    sf::Text text("Boxes Opened: " + rBoxesOpened, berlin);
    text.setPosition(10,7);
    text.setCharacterSize(TS/1.5);
    text.setColor(sf::Color(10,150,160));

    //bombs left text
    int bombsLeft = BOMBS;
    std::string rBombsLeft;
    std::ostringstream convert2;
    convert2 << bombsLeft;
    rBombsLeft = convert2.str();
    sf::Text text2("Bombs Left: " + rBombsLeft, berlin);
    text2.setPosition(10,TS/1.5 + 10);
    text2.setCharacterSize(TS/1.5);
    text2.setColor(sf::Color(7,121,131));

    //Timing text
    int timing = 999;
    std::string rTiming;
    std::ostringstream convert3;
    convert3 << timing;
    rTiming = convert3.str();
    sf::Text text3("Time Left: " + rTiming, berlin);
    text3.setPosition(10,TS/.75 + 13);
    text3.setCharacterSize(TS/1.5);
    text3.setColor(sf::Color(10,150,160));

    playAgain = false;
    playing = true;
    win = false;
    clock.restart();

    //clears the event queue;
    sf::Event event;
    while (window.pollEvent(event))
    {;}

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape))
                {
                window.close();
                }

            if (event.type == sf::Event::MouseButtonPressed)
            {

                mouseX = event.mouseButton.x/TS;
                mouseY = event.mouseButton.y/TS-3;


                if (event.mouseButton.y <= restart.getPosition().y+30 &&
                    event.mouseButton.y >= restart.getPosition().y &&
                    event.mouseButton.x >= restart.getPosition().y &&
                    event.mouseButton.x <= restart.getPosition().x + 86 )
                    {
                        playAgain = true;
                    }

            }

            if (event.type == sf::Event::MouseButtonReleased)
            {

                lmouseX = event.mouseButton.x/TS;
                lmouseY = event.mouseButton.y/TS-3;

                mouseX = -1; mouseY=-1;

                if (event.mouseButton.button == sf::Mouse::Left&&lmouseX>=0&&lmouseY>=0)
                {
                    //check for bomb
                    if (numGrid[lmouseY][lmouseX] >= 9)
                    {
                        openBomb(CGrid, numGrid, lmouseX, lmouseY);
                        playing = false;
                    }

                    if (CGrid[lmouseY][lmouseX] && playing)
                    {
                        if (numGrid[lmouseY][lmouseX]==0)
                        {
                            openBox(CGrid, lmouseX, lmouseY, numGrid, boxesOpened);
                        }

                        else //opens that box only
                        {
                            CGrid[lmouseY][lmouseX]=0;
                            boxesOpened++;

                        }

                        //edits the number of boxes opened
                        std::ostringstream convert;
                        convert << boxesOpened;
                        rBoxesOpened = convert.str();
                        text.setString("Boxes Opened: " + rBoxesOpened);

                        // check for a win
                        if (boxesOpened==WIDTH*HEIGHT-BOMBS)
                        {
                            playing = false;
                            win = true;
                        }
                    }

                }

                if (event.mouseButton.button == sf::Mouse::Right&&lmouseX>=0&&lmouseY>=0)
                {
                    if (CGrid[lmouseY][lmouseX]) // if there is still a cover
                    {
                        {
                        flagBox(CGrid, lmouseX, lmouseY, bombsLeft);

                        //edits the number of bombs left
                        std::ostringstream convert2;
                        convert2 << bombsLeft;
                        rBombsLeft = convert2.str();
                        text2.setString("Bombs Left: " + rBombsLeft);}
                    }

                }

            }

        }

        timing = 999 - (int)clock.getElapsedTime().asSeconds();
        if (timing<0){timing=0;}
        std::ostringstream convert3;
        convert3 << timing;
        rTiming = convert3.str();
        text3.setString("Timing: " + rTiming);

        window.clear();
        window.draw(rectangle);
        window.draw(restart);
        window.draw(myMap); // draws the back of the board
        sf::VertexArray CArray = makeCArray(CGrid, mouseX, mouseY); // makes array for cover
        gMap myCMap; //makes a new map (a drawable with a vertex array and a picture)
        myCMap.setPosition(0,TS*3);
        myCMap.loadMap(CArray, SKINTYPE);
        window.draw(myCMap);
        window.draw(text);
        window.draw(text2);

        window.draw(text3);
        window.display();

        if (win)
        {
            sf::RenderWindow winner(sf::VideoMode(600,200), "Congratulations!", sf::Style::Close);
            sf::RectangleShape rect;
            rect.setSize(sf::Vector2f(600, 100));
            rect.setFillColor(sf::Color(245,190,120));

            sf::RectangleShape rect2;
            rect2.setSize(sf::Vector2f(600, 100));
            rect2.setPosition(sf::Vector2f(0, 100));
            rect2.setFillColor(sf::Color(245,220,150));

            sf::Text congratulations("CONGRATULATIONS!", berlin);
            congratulations.setPosition(10, 10);
            congratulations.setColor(sf::Color(10,150,160));
            congratulations.setCharacterSize(60);

            sf::Text points("You finished with " + rTiming + " seconds left to spare.", berlin);
            points.setPosition(200, 80);
            points.setColor(sf::Color(10,150,160));
            points.setCharacterSize(20);

            winner.draw(rect);
            winner.draw(rect2);
            winner.draw(congratulations);
            winner.draw(points);
            winner.display();

            winner.setActive(true);
            while (winner.isOpen())
            {

            std::string str;
            sf::Text name;

                sf::Event wEvent;
                while (winner.pollEvent(wEvent))
                {
                    if (wEvent.type == sf::Event::Closed)
                    {
                        winner.close();
                        playAgain=true;
                    }


                    if (wEvent.type == sf::Event::KeyPressed)

                    {
                        std::cout << "HI";
                        // Handle ASCII characters only
                        if (wEvent.TextEvent.unicode < 128)
                        {
                            str += static_cast<char>(event.key.code);
                            name.setString(str);
                            std::cout << str;
                        }
                    }
                }
                winner.draw(name);

            }



        }
        if (!playing && !win)
        {
            sf::RenderWindow loser(sf::VideoMode(600,200), "Better Luck Next Time");

            sf::RectangleShape rect;
            rect.setSize(sf::Vector2f(600, 100));
            rect.setFillColor(sf::Color(178,2,0));

            sf::RectangleShape rect2;
            rect2.setSize(sf::Vector2f(600, 100));
            rect2.setPosition(sf::Vector2f(0, 100));
            rect2.setFillColor(sf::Color(255,65,63));

            sf::Text youLose("MINE DETONATED!", berlin);
            youLose.setPosition(10, 10);
            youLose.setColor(sf::Color(0,0,0));
            youLose.setCharacterSize(60);

            loser.draw(rect);
            loser.draw(rect2);
            loser.draw(youLose);
            loser.display();

            loser.setActive(true);
            while (loser.isOpen())
            {
                sf::Event wEvent;
                while (loser.pollEvent(wEvent))
                {
                    if (wEvent.type == sf::Event::Closed ||
                    (wEvent.type == sf::Event::KeyPressed))
                        {
                        //window.create(sf::VideoMode(WIDTH*TS,HEIGHT*TS+3*TS), "Minesweeper", sf::Style::Close);
                        loser.close();
                        playAgain=true;
                        }

                }
            }

        }


        if (playAgain)
        {
            break;
        }
    }

    }
    return 0;
}
