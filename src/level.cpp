 /**
 * ========================
 * 
 * Filename: level.cpp
 * 
 * Author: NehZio <leo.gaspard@outlook.fr>
 * 
 * Created: 28/08/15
 * =========================
 **/

#include <iostream>
#include <fstream>
#include "level.h"

using namespace std;

/*******************
 *   CONSTRUCTOR   * 
 *******************/

 
 Level::Level()
 {
   scrollX = 0;
   if(!font.loadFromFile("SuperMario256.ttf"))
       cout << "ERROR : Can't load the font";  
   if(!headUp.loadFromFile("pictures/HUD.png"))
       cout << "ERROR : Can't load HUD";
  
 }
 
/*******************
 *    DESTRUCTOR   *
 *******************/
 
 Level::~Level()
 {
   //nothing
 }
 
/*******************
 *    LOAD MAP     *
 *******************/
 
 void Level::loadMap(string fileName)
 {
   string parse;
   ifstream flux(fileName.c_str());
   
   if(flux)
   {     
     do
     {
       getline(flux, parse);
     }while(parse != "#size");
     
     flux >> mapWidth;
     flux >> mapHeight;
     
     for(int i = 0; i < mapHeight; i++)
       map.push_back(vector<int>(mapWidth));
     for(int i = 1; i < mapWidth; i++)
       map[0].push_back(0);     
     
     do
     {
       getline(flux, parse);
     }while(parse != "#level");
     
     for(int i = 0; i < mapHeight; i++)
     {
       for(int j = 0; j < mapWidth; j++)
	 flux >> map[i][j];
     }  
   }
   else
     cout << "ERROR : Can't open the file " << fileName << endl;
   
   if(!tileSet.loadFromFile("pictures/tileSet.png"))
      cout << "ERROR : Can't load image" << endl;
   if(!bg.loadFromFile("pictures/background.png"))
       cout << "ERROR : Can't load image" << endl;
   
 }
 
/******************
 *     UPDATE     *
 ******************/

 void Level::update(sf::RenderWindow& window, sf::Sprite playerSprite, int nbrLives, int nbrCoins)
 {
   int posX = 0, posY = 0;
   
   sf::Sprite tile;
   sf::Text lives;
   sf::Text coins;
   
   
   
   tile.setTexture(tileSet);
   lives.setFont(font);
   coins.setFont(font);
   lives.setColor(sf::Color::Black);
   coins.setColor(sf::Color::Black);
   
   char* temporary;
   sprintf(temporary,"%d",nbrLives);
   lives.setString(string(temporary));
   lives.setCharacterSize(24);
   lives.setPosition(sf::Vector2f(1225,-2));
   
   sprintf(temporary, "%d",nbrCoins);
   coins.setString(string(temporary));
   coins.setCharacterSize(24);
   coins.setPosition(sf::Vector2f(1225,25));
   
   
   backGround.setTexture(bg);
   
   window.draw(backGround);
    
  for(int i = 0; i < mapHeight; i++)
   {
            for(int j = (scrollX/TILE_WIDTH);scrollX == mapWidth*TILE_WIDTH - WINDOW_WIDTH ? j < ((scrollX+WINDOW_WIDTH)/TILE_WIDTH) : j <= ((scrollX+WINDOW_WIDTH)/TILE_WIDTH); j++)
            {
                int x = 0, y = 0;
                x = map[i][j] * 32;
                while(x >= 320)
                {
                        x -= 320;
                        y += 32;	 
                }
                
                posX = 32*(j-(scrollX/TILE_WIDTH))- scrollX%32;
                posY = 32*i;
                
                if(getLayer(map[i][j]) < 2)
                {
                    tile.setTextureRect(sf::IntRect(x,y,32,32));
                    tile.setPosition(sf::Vector2f(posX,posY)); 
                    window.draw(tile);
                }
            
            }
       
  }
  
  window.draw(playerSprite);
  
  for(int i = 0; i < mapHeight; i++)
   {
        for(int j = (scrollX/TILE_WIDTH);scrollX == mapWidth*TILE_WIDTH - WINDOW_WIDTH ? j < ((scrollX+WINDOW_WIDTH)/TILE_WIDTH) : j <= ((scrollX+WINDOW_WIDTH)/TILE_WIDTH); j++)
        {
            int x = 0, y = 0;
            x = map[i][j] * 32;
            while(x >= 320)
            {
                    x -= 320;
                    y += 32;     
            }
            
            posX = 32*(j-(scrollX/TILE_WIDTH))- scrollX%32;
            posY = 32*i;
            
            if(getLayer(map[i][j]) >= 2)
            {
                tile.setTextureRect(sf::IntRect(x,y,32,32));
                tile.setPosition(sf::Vector2f(posX,posY)); 
                window.draw(tile);
            }             
        }
  }
  
  HUD.setTexture(headUp);
  window.draw(HUD);
  window.draw(lives);
  window.draw(coins);
   
 }

/******************
 *   SET SCROLL   *
 ******************/ 

void Level::setScroll(int scroll)
{
  scrollX += scroll;
  if(scrollX > (mapWidth*TILE_WIDTH - WINDOW_WIDTH))
    scrollX = mapWidth*TILE_WIDTH - WINDOW_WIDTH;
  
  if(scrollX < 0)
    scrollX = 0;
}

/******************
 *    GET MAP     *
 ******************/

int Level::getMap(int x, int y)
{
  return map[x][y];
}

/******************
 *   GET SCROLL   *
 ******************/

int Level::getScroll()
{
  return scrollX;
}

/******************
 *  GET MAPWIDTH  *
 ******************/

int Level::getMapWidth()
{
  return mapWidth;
}

/******************
 *BOTTOM COLLISION*
 ******************/

bool Level::bottomCollision(int posX, int posY, bool stateDying)
{
    if(posY + PLAYER_HEIGHT >= 0 && posY + PLAYER_HEIGHT <= WINDOW_HEIGHT && !stateDying && posY +PLAYER_HEIGHT < 480)
        return (getLayer(map[(posY + PLAYER_HEIGHT) / TILE_HEIGHT][(posX + scrollX + 10) / TILE_WIDTH]) == 1) || (getLayer(map[(posY + PLAYER_HEIGHT) / TILE_HEIGHT][(posX + PLAYER_WIDTH + scrollX - 10) / TILE_WIDTH]) == 1);
    else
        return false;
}

/******************
 * RIGHT COLLISION*
 ******************/

bool Level::rightCollision(int posX, int posY, int speed, bool stateDying)
{
    if(posY + PLAYER_HEIGHT >= 0 && posY >= 0 && posY + PLAYER_HEIGHT <= WINDOW_HEIGHT && posY <= WINDOW_HEIGHT && !stateDying)
        return (getLayer(map[(posY + PLAYER_HEIGHT - 18) / TILE_HEIGHT][(posX + PLAYER_WIDTH + speed + scrollX) / TILE_WIDTH]) == 1) || (getLayer(map[posY / TILE_HEIGHT][(posX + PLAYER_WIDTH + speed + scrollX) / TILE_WIDTH]) == 1);
    else
        return false;
    
}

/******************
 * LEFT COLLISION *
 ******************/

bool Level::leftCollision(int posX, int posY, int speed, bool stateDying)
{
    if(posY + PLAYER_HEIGHT >= 0 && posY >= 0 && posY + PLAYER_HEIGHT <= WINDOW_HEIGHT && posY <= WINDOW_HEIGHT && !stateDying)
        return (getLayer(map[(posY + PLAYER_HEIGHT - 18) / TILE_HEIGHT][(posX - speed + scrollX) / TILE_WIDTH]) == 1) || (getLayer(map[posY / TILE_HEIGHT][(posX - speed + scrollX) / TILE_WIDTH]) == 1);
    else
        return false;
}

/******************
 * TOP COLLISION  *
 ******************/

bool Level::topCollision(int posX, int posY, bool stateDying)
{
    if(posY >= 0 && posY <= WINDOW_HEIGHT && !stateDying)
        return (getLayer(map[posY / TILE_HEIGHT][(posX + scrollX + 10) / TILE_WIDTH]) == 1) || (getLayer(map[posY / TILE_HEIGHT][(posX + PLAYER_WIDTH + scrollX - 10) / TILE_WIDTH]) == 1);
     else
        return false; 
}
