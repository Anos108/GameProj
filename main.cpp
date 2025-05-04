#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <iostream>
#include "def.h"
#include "logic.h"
#include "map.h"
#include "keyboard.h"
#include "graphic.h"

void render()
{
    int cameraX = (int)car.x - SCREEN_WIDTH / 2;
    SDL_SetRenderDrawColor(renderer, 171, 220, 241, 255);
    SDL_RenderClear(renderer);



    std::vector<SDL_Point> terrainPolygon;
    for (const auto& point : terrain.points)
    {
        terrainPolygon.push_back({point.x - cameraX, point.y});
    }

    if (!terrain.points.empty())
    {
        terrainPolygon.push_back({terrain.points.back().x - cameraX, SCREEN_HEIGHT});
        terrainPolygon.push_back({terrain.points.front().x - cameraX, SCREEN_HEIGHT});
    }


    for (size_t i = 0; i < terrain.points.size() - 1; ++i)
    {
        int x1 = terrain.points[i].x - cameraX;
        int y1 = terrain.points[i].y;
        int x2 = terrain.points[i + 1].x - cameraX;
        int y2 = terrain.points[i + 1].y;


        SDL_Point polygon[4] =
        {
            {x1, y1},
            {x2, y2},
            {x2, SCREEN_HEIGHT},
            {x1, SCREEN_HEIGHT}
        };

        SDL_Rect soilRect = {x1, std::min(y1, y2), x2 - x1, SCREEN_HEIGHT - std::min(y1, y2)};
        SDL_RenderCopy(renderer, terrainTexture, nullptr, &soilRect);
    }

    for (size_t i = 0; i < terrain.points.size() - 1; ++i)
    {
        int grassWidth = 13;
        int grassHeight = 14;
        int startX = terrain.points[i].x;
        int endX = terrain.points[i + 1].x;
        float dx = terrain.points[i + 1].x - terrain.points[i].x;
        float dy = terrain.points[i + 1].y - terrain.points[i].y;
        float angle = atan2(dy, dx) * 180 / M_PI;


        float segmentLength = sqrt(dx * dx + dy * dy);

        int step = grassWidth / 2;

        for (float dist = 0; dist < segmentLength; dist += step)
        {
            float t = dist / segmentLength;
            int x = terrain.points[i].x + t * dx;
            int y = terrain.points[i].y + t * dy;
            SDL_Rect grassRect = {x - cameraX - grassWidth / 2, y - grassHeight, grassWidth, grassHeight};
            SDL_RenderCopyEx(renderer, grassTexture, nullptr, &grassRect, angle, nullptr, SDL_FLIP_NONE);
        }
    }
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    std::vector<SDL_Point> visiblePoints;
    for (auto& point : terrain.points)
    {
        visiblePoints.push_back({point.x - cameraX, point.y});
    }
    SDL_RenderDrawLines(renderer, visiblePoints.data(), visiblePoints.size());

    int carWidth = 40, carHeight = 20, wheelSize = 10;
    int frontWheelOffsetX = 12, rearWheelOffsetX = -13, wheelOffsetY = -4;
    float carGroundY = car.y, wheelGroundY = car.y + carHeight / 2 - wheelOffsetY;
    SDL_Point center = {20, 10};

    for (size_t i = 0; i < terrain.points.size() - 1; i++)
    {
        if (car.x >= terrain.points[i].x && car.x <= terrain.points[i + 1].x)
        {
            carGroundY = terrain.points[i].y - carHeight / 2;
            wheelGroundY = terrain.points[i].y - wheelSize / 2 + wheelOffsetY;
        }
    }


    SDL_Rect frontWheelRect = {(int)(car.x + frontWheelOffsetX - wheelSize / 2) - cameraX, (int)wheelGroundY, wheelSize, wheelSize};
    SDL_Rect rearWheelRect = {(int)(car.x + rearWheelOffsetX - wheelSize / 2) - cameraX, (int)wheelGroundY, wheelSize, wheelSize};
    static float wheelAngle = 0;
    wheelAngle += car.velocityX * 3;
    SDL_Point wheelCenter = {wheelSize / 2, wheelSize / 2};
    SDL_RenderCopyEx(renderer, car.wheelTexture, NULL, &frontWheelRect, wheelAngle, &wheelCenter, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, car.wheelTexture, NULL, &rearWheelRect, wheelAngle, &wheelCenter, SDL_FLIP_NONE);

    SDL_Rect carRect = {(int)car.x - cameraX - carWidth / 2, (int)carGroundY - carHeight / 2, carWidth, carHeight};
    SDL_RenderCopyEx(renderer, car.carTexture, NULL, &carRect, car.angle, &center, SDL_FLIP_NONE);

    SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255);
    SDL_Rect fuelBar = {10, 10, static_cast<int>(car.fuel * 2.5), 10};
    SDL_RenderFillRect(renderer, &fuelBar);


    for (const auto& obs : obstacles)
    {
        int renderY = (int)obs.y;
        if (terrain.points.size() >= 2)
        {
            for (size_t i = 0; i < terrain.points.size() - 1; ++i)
            {
                if (obs.x >= terrain.points[i].x && obs.x <= terrain.points[i + 1].x)
                {
                    renderY = terrain.points[i].y - obs.height;
                    break;
                }
            }
        }
        SDL_Rect obsRect = {(int)obs.x - cameraX, renderY, obs.width, obs.height};
        SDL_RenderCopy(renderer, obstacle.obstacleTexture, nullptr, &obsRect);
    }

    for (auto& fuelCan : fuelCans)
    {
        if (!fuelCan.fuelCanTexture)
        {
            printf("FuelCan texture is null at x: %f\n", fuelCan.x);
            continue;
        }
        int renderY = (int)fuelCan.y;
        if (terrain.points.size() >= 2)
        {
            for (size_t i = 0; i < terrain.points.size() - 1; ++i)
            {
                if (fuelCan.x >= terrain.points[i].x && fuelCan.x <= terrain.points[i + 1].x)
                {
                    renderY = terrain.points[i].y - 20;
                    break;
                }
            }
        }
        SDL_Rect fuelRect = {(int)fuelCan.x - cameraX - 10, renderY - 10, 20, 20};
        SDL_RenderCopy(renderer, fuelCan.fuelCanTexture, nullptr, &fuelRect);
    }
    if (font)
    {
        char scoreText[32];
        sprintf(scoreText, "Score: %d", score);
        SDL_Color white = {255, 255, 255, 255};
        SDL_Surface* surface = TTF_RenderText_Solid(font, scoreText, white);
        if (surface)
        {
            if (scoreTexture) SDL_DestroyTexture(scoreTexture);
            scoreTexture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
            if (scoreTexture)
            {
                SDL_QueryTexture(scoreTexture, nullptr, nullptr, &scoreRect.w, &scoreRect.h);
                SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);
            }
        }
    }
    SDL_RenderPresent(renderer);
}


void musicFinished()
{
    Mix_PlayMusic(backgroundMusic, 0);
}

void cleanup()
{
    Mix_FreeMusic(backgroundMusic);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(car.carTexture);
    SDL_DestroyTexture(car.wheelTexture);
    SDL_DestroyTexture(obstacle.obstacleTexture) ;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_CloseFont(font);
    SDL_Quit();
    Mix_CloseAudio();

}

int main(int argc, char* argv[])
{
    if (!initSDL() || !loadMedia()) return -1;
    srand(time(0));
    createRandomTerrain();
    createFuelCans();
    createObstacles();
    car.x = 100;
    car.y = terrain.points[0].y - 20;
    car.velocityX = car.velocityY = 0;
    bool running = true;
    Mix_HookMusicFinished(musicFinished);


    Mix_PlayMusic(backgroundMusic, 0);
    while (running)
    {
        if (!gameover)
        {
            handleInput(running,gameover );
            updatePhysics();
            updateTerrain();
            updateObstacles();
            updateFuelCans();
            checkFuelPickup();
            checkObstacleCollision(running);

        }
        render();
        SDL_Delay(16);
    }
    cleanup();
    return 0;
}
