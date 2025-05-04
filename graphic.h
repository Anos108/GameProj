#ifndef GRAPHIC_H_INCLUDED
#define GRAPHIC_H_INCLUDED


bool loadMedia()
{
    car.carTexture = IMG_LoadTexture(renderer, "car.png");
    car.wheelTexture = IMG_LoadTexture(renderer, "wheel.png");
    fuelCan.fuelCanTexture = IMG_LoadTexture(renderer, "FuelCan.png");
    backgroundMusic = Mix_LoadMUS("background.mp3");
    terrainTexture = IMG_LoadTexture(renderer, "terrain.png");
    grassTexture = IMG_LoadTexture(renderer, "grass.png");
    obstacle.obstacleTexture = IMG_LoadTexture(renderer,"obstacle.png") ;
    font = TTF_OpenFont("font/arial.ttf", 25);
    return car.carTexture && car.wheelTexture  && fuelCan.fuelCanTexture && backgroundMusic && terrainTexture && grassTexture
           && obstacle.obstacleTexture  && font;
}

bool initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return false;
    window = SDL_CreateWindow("Hill Climb Racing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) return false;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_Log("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        SDL_Log("Failed to initialize SDL_Image with PNG support: %s", IMG_GetError());

        return false;
    }
    if (TTF_Init() == -1)
    {
        SDL_Log("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }
    return true;
}


#endif
