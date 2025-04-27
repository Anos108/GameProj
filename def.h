#ifndef _DEF__H
#define _DEF__H
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float GRAVITY = 0.2f;

struct Car {
    float x, y;
    float velocityX, velocityY;
    float angle;
    SDL_Texture* carTexture;
    SDL_Texture* wheelTexture;
    float fuel = 100.0f;
    float accelerationX = 0.0f;
    float acceleration_power = 0.08f;
    float braking_power = 0.1f;
    float hesomasat = 0.97f;
    float max_speed = 6.0f;
};
struct FuelCan {
    float x, y;
    SDL_Texture* fuelCanTexture;
};

struct Terrain {
    std::vector<SDL_Point> points;
};

struct Obstacle {
    float x, y;
    int width, height;
    SDL_Texture* obstacleTexture;
    bool passed = false;
};

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* backgroundTexture = nullptr;
SDL_Texture* terrainTexture = nullptr;
SDL_Texture* grassTexture = nullptr;
Mix_Music* backgroundMusic = nullptr;
Mix_Chunk* engineSound = nullptr;
SDL_Texture* gameOverTexture = nullptr;
SDL_Rect gameOverRect;
Car car;
Terrain terrain;
FuelCan fuelCan;
bool engineOn = false;
Obstacle obstacle;
std::vector<Obstacle> obstacles;
std::vector<FuelCan> fuelCans;
bool gameover = false;

int score = 0;
TTF_Font* font = nullptr;
SDL_Texture* scoreTexture = nullptr;
SDL_Rect scoreRect = {10, 30, 0, 0};

#endif
