/**
* ========================
* 
* Filename: player.cpp
* 
* Author: NehZio <leo.gaspard@outlook.fr>
* 
* Description: The player source
* 
* Created: 28/08/15
* =========================
**/

#include <iostream>
#include "player.h"

using namespace std;

//CONSTRUCTOR

Player::Player()
{
posX = posY = 0;
stateWalking = stateJumping = stateFalling = stateDying = false;
direction = true;
lives = 3;
maxLives = 3; 
playerHeight = 50;
playerWidth = 40; 
jumpSpeed = 0.0f;
if(!spriteSheet.loadFromFile("pictures/SpriteSheet.png"))
cout << "ERROR : Can't load SpriteSheet";
}

//DESTRUCTOR

Player::~Player()
{
//nothing
}

//MOVEMENT

void Player::movement(Level& level, sf::RenderWindow& window)
{
    playerSprite.setTexture(spriteSheet);
    playerSprite.setTextureRect(sf::IntRect(0,0,playerWidth,playerHeight));

    if(level.getMap((posY+playerHeight) / TILE_HEIGHT, (posX + level.getScroll() + 10) / TILE_WIDTH) != 0)
    {
        stateFalling = false; 
        jumpSpeed = 0.0f;
    }
    else if(level.getMap((posY + playerHeight) / TILE_HEIGHT, (posX + playerWidth + level.getScroll() - 10) / TILE_WIDTH) != 0)
    {
        stateFalling = false;
        jumpSpeed = 0.0f;
    }
    else if(level.getMap((posY+playerHeight) / TILE_HEIGHT, (posX + level.getScroll() + playerWidth - 10) / TILE_WIDTH) == 0) 
    {
        stateFalling = true;
    }  

    sf::Event event;
    while(window.pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Closed:
                window.close();
            break;
            
            case sf::Event::KeyPressed:
        switch(event.key.code)
        {
            case sf::Keyboard::Escape:
                window.close();
            break;
            
            default:
                inputMap[event.key.code] = true;
            break;
        }
            break;
            
            case sf::Event::KeyReleased:
                inputMap[event.key.code] = false;
            break;
            
            default:
        break;	
        }
    }

    if(inputMap[sf::Keyboard::Up])
    {
        if(!stateFalling)
        {
            jumpSpeed = 10.0f;
            stateFalling = true;
        }
    }

    if(inputMap[sf::Keyboard::Left])
    {
        stateWalking = true;
        direction = false;
    }  
    else if(inputMap[sf::Keyboard::Right])
    {
        stateWalking = true;
        direction = true;
    }
    else
        stateWalking = false;  

    if(stateFalling)
    {
        posY-=(int)jumpSpeed;
        jumpSpeed -= 0.5;
        
        while(jumpSpeed > 0 && level.getMap((posY - jumpSpeed) / TILE_HEIGHT, (posX + level.getScroll() + 10) / TILE_WIDTH) != 0 || level.getMap((posY - jumpSpeed) / TILE_HEIGHT, (posX + playerWidth + level.getScroll() - 10) / TILE_WIDTH) != 0)
        {
            jumpSpeed--;
        }

        if(posY > 480)
            stateDying = true;
    }

    if(stateWalking)
    {
        speed = 3;
        bool moveTest;
        
        
        if(!direction)
        {
            moveTest = true;
        }

        if(direction && posX < (1280-playerWidth))
        {
            while(level.getMap((posY + playerHeight - 18) / TILE_HEIGHT, (posX + playerWidth + speed + level.getScroll()) / TILE_WIDTH) != 0 || level.getMap((posY) / TILE_HEIGHT, (posX + playerWidth + speed + level.getScroll()) / TILE_WIDTH) != 0)
            {
                speed--;                
                if(speed == 0)
                    break;                
            }
           
            if(posX > 1000 && level.getScroll() < (level.getMapWidth()*TILE_WIDTH - WINDOW_WIDTH) && direction)
                level.setScroll(speed);
            else
                posX += speed;
        }

        if(!direction && posX > 0 && moveTest)
        {
            while(level.getMap((posY + playerHeight - 18) / TILE_HEIGHT, (posX - speed + level.getScroll()) / TILE_WIDTH) != 0 || level.getMap((posY) / TILE_HEIGHT, (posX - speed + level.getScroll()) / TILE_WIDTH) != 0)
            {
                speed--;
                if(speed == 0)
                    break;
            } 
            
            if(posX < 250 && level.getScroll() > 0)
                level.setScroll(-speed);
            else
                posX -= speed;           
        }
    }

    if(level.getMap((posY+playerHeight) / TILE_HEIGHT, (posX + level.getScroll() + 10)/TILE_WIDTH) != 0 || level.getMap((posY+playerHeight) / TILE_HEIGHT, (posX + level.getScroll() + playerWidth - 10) / TILE_WIDTH) != 0)
        posY = ((posY+playerHeight)/ TILE_HEIGHT) * TILE_HEIGHT - playerHeight;

    playerSprite.setPosition(sf::Vector2f(posX, posY));
    window.draw(playerSprite);
    
}