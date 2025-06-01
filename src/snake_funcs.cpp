#include "snake_funcs.hpp"
#include <iostream>
#include <string>

//renderText function
SDL_Texture* renderText(const std::string& message, const std::string& fontFile, SDL_Color color, int fontSize, SDL_Renderer* renderer) {
  
    // Open the font
    TTF_Font* font = TTF_OpenFont(fontFile.c_str(), fontSize);
    if (font == nullptr) {
        std::cerr << "Failed to load font: " << TTF_GetError() <<std::endl;
        return nullptr;
    }

    // Render the message to an SDL_Surface
    SDL_Surface* surf = TTF_RenderText_Solid(font, message.c_str(), color);
    if (surf == nullptr) {
        TTF_CloseFont(font);
        std::cerr << "Failed to render text: " << TTF_GetError() << std::endl;
        return nullptr;
    }

    // Convert the surface to a texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
    if (texture == nullptr) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
    }

    // Clean up the surface and font
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);

    return texture;
}


//Start Menu
void showStartMenu(SDL_Renderer* renderer) {
    // Set the text for the start menu
    const char* message = "Snake!";
    const char* enter = "Press Enter";

    // Set the color of the text
    SDL_Color white = {255, 255, 255, 255};
    
    // Variables for flashing text
    SDL_Texture* enterTexture = nullptr;
    SDL_Rect enterRect = {1280 / 2 - 100, 720 / 2 + 25, 200, 50};
    bool showEnterText = true;
    Uint32 lastToggle = SDL_GetTicks();
    const Uint32 toggleInterval = 250; // Time in milliseconds

    // Render the text
    SDL_Texture* messageTexture = renderText(message, "C:/Windows/Fonts/arial.ttf", white, 12, renderer);
    SDL_Rect messageRect = {1280 / 2 - 100, 720 / 2 - 25, 200, 50}; // Adjust these values as needed
    SDL_RenderCopy(renderer, messageTexture, NULL, &messageRect);

    // Update the screen
    SDL_RenderPresent(renderer);

    // Wait for the user to press Enter
    SDL_Event event;
    bool startGame = false;
    while (!startGame) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                isGameOver = true;
                startGame = true;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    startGame = true;
                }
            }
        }
            // Toggle "Press Enter" visibility
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastToggle > toggleInterval) {
            showEnterText = !showEnterText;
            lastToggle = currentTime;
        }

        //Clear screen
        SDL_RenderClear(renderer);

        // Render main message
        SDL_RenderCopy(renderer, messageTexture, NULL, &messageRect);

        // Conditionally render "Press Enter" text
        if (showEnterText) {
            if (!enterTexture) { // Lazy load the texture
                enterTexture = renderText(enter, "C:/Windows/Fonts/arial.ttf", white, 12, renderer);
            }
            SDL_RenderCopy(renderer, enterTexture, NULL, &enterRect);
        }
        // Update the screen
        SDL_RenderPresent(renderer);
    }
}



//Initializng game variables
void init() {
    isGameOver = false;
    sDir = STOP;
    x = width / 2;
    y = height / 2;
    fX = rand() % width;
    fY = rand() % height;
    tailLen = 0;
    Score = 0;
}



//Printing game state to screen
void Draw(SDL_Renderer* renderer) {

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
    SDL_RenderClear(renderer);

    //Drawing border to the buffer
    SDL_SetRenderDrawColor(renderer, 144, 238, 144, 255);
    SDL_Rect border = { 320, 50, width * 16, height * 16 };
    SDL_RenderDrawRect(renderer, &border);

    //Drawing snake's head to the buffer
    SDL_Rect head = { 320 + x * 16,50 + y * 16, 16, 16 };
    SDL_SetRenderDrawColor(renderer, 173, 216, 230, 255);
    SDL_RenderFillRect(renderer, &head);

    // Drawing snake's food to the buffer
    SDL_Rect food = {320 + fX * 16, 50 + fY * 16, 16, 16 };
    SDL_SetRenderDrawColor(renderer, 255, 102, 102, 255);
    SDL_RenderFillRect(renderer, &food);

    // Drawing snake's tail to the buffer
    for (int i = 0; i <= tailLen; i++) {
        SDL_Rect tail = { 320 + tailX[i] * 16, 50 + tailY[i] * 16, 16, 16 };
        SDL_SetRenderDrawColor(renderer, 173, 216, 230, 255);
        SDL_RenderFillRect(renderer, &tail);
    }


    //Score text code:
    // Loading the font
    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/arialbd.ttf", 20);
    if (font == nullptr) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }

    // Convert the score to a string
    std::string scoreStr = std::to_string(Score);
    std::string scoreText = "Score: " + scoreStr;
    
    // Create a surface from the score string
    SDL_Color textColor = {255, 255, 255}; // White text
    SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);

    // Create a texture from the surface
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);

    // Create a rect for the score texture
    SDL_Rect scoreRect;
    scoreRect.x = 10; // 10 pixels from the left edge of the window
    scoreRect.y = 690; // 10 pixels from the top edge of the window
    SDL_QueryTexture(scoreTexture, NULL, NULL, &scoreRect.w, &scoreRect.h); // Get the width and height of the texture

    // Render the score texture
    SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);

    // Free the surface and texture
    SDL_FreeSurface(scoreSurface);
    SDL_DestroyTexture(scoreTexture);

    // Update screen
    SDL_RenderPresent(renderer);
}



//User input function
void Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            isGameOver = true;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {

            case SDLK_UP:
            case SDLK_w:
                if (sDir != DOWN) {
					sDir = STOP;
					sDir = UP;
				}
                break;
            case SDLK_DOWN:
            case SDLK_s:
                if (sDir != UP) {
                sDir = STOP;
                sDir = DOWN;
                }
                break;
            case SDLK_LEFT:
            case SDLK_a:
                if (sDir != RIGHT) {
                sDir = STOP;
                sDir = LEFT;
                }
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                if (sDir != LEFT) {
                sDir = STOP;
                sDir = RIGHT;
                }
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}


//Game state update function
void update() {

    switch (sDir) {
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    default:
        break;
    }

    if (x == fX && y == fY) {
        Score += 10;
        fX = rand() % width;
        fY = rand() % height;
        tailLen++;
    }

    for (int i = tailLen; i > 0; i--) {
        tailX[i] = tailX[i - 1];
        tailY[i] = tailY[i - 1];
    }

    tailX[0] = x;
    tailY[0] = y;

}


//Collision Detection
void collision() {
    if (tailLen > 0) {
        for (int i = 1; i <= tailLen; i++) {
            if (x == tailX[i] && y == tailY[i]) {
                isGameOver = true;
            }
        }
    }
    if (x <= -1 || x >= width || y <= -1 || y >= height) {
        isGameOver = true;
    }
}


//Game Over function
void gameOver() {
    cout << "Game Over!\n";
    cout << "Press any key to exit...";
    _getch();
}
