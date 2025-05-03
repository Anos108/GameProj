#ifndef _KEYBOARD__H
#define _KEYBOARD__H

void handleInput(bool& running, bool& gameover)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) running = false;
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
        {
            if (!isSpacePressed)
            {
                isSpacePressed = true;
                spacePressStartTime = SDL_GetTicks();
            }
        }
        if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE)
        {
            isSpacePressed = false;
        }
    }
    const Uint8* keys = SDL_GetKeyboardState(NULL);

    car.accelerationX = 0.0f;
    if (car.fuel > 0)
    {
        bool moving = false;
        car.accelerationX += car.acceleration_power;
        car.fuel -= 0.05f;
        moving = true;


        if (keys[SDL_SCANCODE_SPACE])
        {

            car.velocityX = std::clamp(car.velocityX, -car.max_speed, car.max_speed);
            car.x += car.velocityX -1;
            car.y += car.velocityY -1;

            if (isSpacePressed)
            {
                Uint32 currentTime = SDL_GetTicks();
                if (currentTime - spacePressStartTime >= 500)
                {
                    gameover = true;
                    resetGame(gameover);
                }
            }
        }


    }
    else
    {
        car.velocityX = 0;
        car.velocityY = 0;
        gameover = true;
        resetGame(gameover);

    }
}
#endif
