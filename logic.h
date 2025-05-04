#ifndef _LOGIC__H
#define _LOGIC__H
#include "map.h"
#include "def.h"

void createObstacles()
{
    for (int i = 0; i < 5; i++)
    {
        int locateX = 500 * (i + 1);
        int locateY = terrain.points.back().y - 28;
        obstacles.push_back({(float) locateX, (float) locateY,20,20, obstacle.obstacleTexture}) ;

    }
}

void updateObstacles()
{
    int lastScoreMilestone = 0;
    if (!obstacles.empty() && car.x > obstacles.back().x - SCREEN_WIDTH)
    {
        int locateX = obstacles.back().x + 250;
        int locateY = terrain.points.back().y - 25;
        obstacles.push_back({(float) locateX, (float) locateY, 20, 20, obstacle.obstacleTexture}) ;
    }
    for (auto& obs : obstacles)
    {
        if (car.x > obs.x && !obs.passed)
        {
            score += 1;
            obs.passed = true;

            if (score % 2 == 0 && score > lastScoreMilestone)
            {
                lastScoreMilestone = score;
                car.max_speed += 5.0f;
                car.max_speed = std::min(car.max_speed, 150.0f);
                SDL_Log("Speed has increased 5.");
            }
        }
    }
}



void createFuelCans()
{


    for (int i = 0; i < 5; ++i)
    {
        int spawnX = 700 * (i + 1);
        int spawnHeight = terrain.points.back().y - 40;
        fuelCans.push_back({(float)spawnX, (float)spawnHeight, fuelCan.fuelCanTexture});
    }

}
void updateFuelCans()
{
    srand(time(NULL));
    int random = rand() % 100;
    if (random < 65)
    {

        if (!fuelCans.empty() && car.x > fuelCans.back().x - SCREEN_WIDTH)
        {
            int spawnX = fuelCans.back().x + 300;
            int spawnHeight = terrain.points.back().y - 40;
            fuelCans.push_back({(float)spawnX, (float)spawnHeight, fuelCan.fuelCanTexture});
        }

        fuelCans.erase(std::remove_if(fuelCans.begin(), fuelCans.end(),
                                      [](const FuelCan& fuelCan)
        {
            return fuelCan.x < car.x - SCREEN_WIDTH;
        }),fuelCans.end());
    }
}

void checkFuelPickup()
{
    int cameraX = car.x - SCREEN_WIDTH / 2;
    for (auto& fuelCan : fuelCans )
    {
        if (fabs(car.x - fuelCan.x) < 20)
        {
            car.fuel = std::min(car.fuel + 3, 100.0f);
            fuelCan.x = -100;
        }
    }
}

void updatePhysics()
{
    car.velocityY += GRAVITY;
    car.velocityX *= car.hesomasat;
    car.velocityX += car.accelerationX;
    car.velocityX = std::clamp (car.velocityX, -car.max_speed, car.max_speed);
    car.x += car.velocityX;
    car.y += car.velocityY;
    for (size_t i = 0; i < terrain.points.size() - 1; i++)
    {
        if (car.x >= terrain.points[i].x && car.x <= terrain.points[i + 1].x)
        {
            float groundY = terrain.points[i].y;
            if (car.y >= groundY - 20)
            {
                car.y = groundY - 20;
                car.velocityY = 0;

            }
        }
    }
}


void resetGame(bool &gameover)
{
    car.x = 100;
    car.velocityX = 0.0f;
    car.velocityY = 0.0f;
    car.fuel = 100.0f;
    gameover = false;
    isSpacePressed = false;
    spacePressStartTime = 0;
    score = 0;
    fuelCans.clear();
    obstacles.clear();
    terrain.points.clear();
    createFuelCans();
    createObstacles();
    Mix_HaltMusic();
    createRandomTerrain();
    if (!terrain.points.empty())
    {
        car.y = terrain.points[0].y - 20;
    }
    else
    {
        car.y = 300 - 20;
    }
}


void checkObstacleCollision(bool& running)
{
    for (auto& obs : obstacles)
    {
        SDL_Rect carRect = {(int)car.x - 20, (int)car.y - 10, 40, 20};
        SDL_Rect obsRect = {(int)obs.x, (int)obs.y, obs.width, obs.height};
        const Uint8* key = SDL_GetKeyboardState(NULL);
        if (fabs(car.x - obs.x) < 20)
        {
            if (!key [SDL_SCANCODE_SPACE])
            {
                car.velocityX = 0;
                car.velocityY = 0;
                gameover = true;
                resetGame(gameover);
                break;
            }
        }
    }
}



#endif
