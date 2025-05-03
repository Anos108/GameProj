#ifndef _MAP__H
#define _MAP__H
#include "logic.h"


void createRandomTerrain()
{
    int currentX = 0;
    int currentY = 300;

    terrain.points.push_back({currentX, currentY});
    for (int i = 1; i < SCREEN_WIDTH; i += 10)
    {
        int changeY = (rand() % 30) - 15;
        currentY = std::max(100, std::min(500, currentY + changeY));
        currentX += 10;
        terrain.points.push_back({currentX, currentY});
    }
}
void updateTerrain()
{
    if (terrain.points.size() > 0 && car.x > terrain.points.back().x - SCREEN_WIDTH)
    {
        int currentX = terrain.points.back().x;
        int currentY = terrain.points.back().y;

        for (int i = 0; i < 100; ++i)
        {
            int changeY = (rand() % 30) - 15;
            currentY = std::max(100, std::min(500, currentY + changeY));
            currentX += 10;
            terrain.points.push_back({currentX, currentY});
        }
    }

    while (terrain.points.size() > 0 && terrain.points[0].x < car.x - SCREEN_WIDTH)
    {
        terrain.points.erase(terrain.points.begin());
    }
}
#endif
