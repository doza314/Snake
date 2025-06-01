#include "snake_funcs.hpp"
#include <iostream>

//Variables 
direction sDir = STOP;
char buffer[width + 2][height + 2];
int x, y;
int fX, fY;
int Score;
int tailX[100], tailY[100];
int tailLen;
bool isGameOver;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

//Game loop
int main(int argc, char* argv[]) {

    //Initializing SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    //Initializing SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
        return 1;
    }
    //Creating the window
    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create renderer for window
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    showStartMenu(renderer);
    
    init();
    //Main game loop
    while (!isGameOver) {
        //Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
        SDL_RenderClear(renderer);

        Draw(renderer);
        Input();
        update();
        collision();

        // Update screen
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }

    if (isGameOver) {
    char gameOverMessage[50];
    sprintf(gameOverMessage, "You lost. \nYour score was %d.", Score);

    const SDL_MessageBoxButtonData buttons[] = {
        { /* .flags, .buttonid, .text */        0, 0, "OK" },
    };
    const SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_INFORMATION, /* .flags */
        NULL, /* .window */
        "Game Over", /* .title */
        gameOverMessage, /* .message */
        SDL_arraysize(buttons), /* .numbuttons */
        buttons, /* .buttons */
        NULL /* .colorScheme */
    };
    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
        SDL_Log("error displaying message box");
        return 1;
    }
    if (buttonid == -1) {
        SDL_Log("no selection");
    } else {
        SDL_Log("selection was %s", buttons[buttonid].text);
    }
}

    // Destroy window and renderer
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}