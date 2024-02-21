#define SDL_MAIN_HANDLED
#include <conio.h>
#include <SDL.h>
#include <SDL_ttf.h>
using namespace std;

enum direction {STOP = 0, UP, DOWN, LEFT, RIGHT};
const int width = 40;
const int height = 40;
extern char buffer[width + 2][height + 2];
extern int x, y; 
extern int fX, fY; 
extern int Score; 
extern int tailX[100], tailY[100]; 
extern int tailLen; 
extern bool isGameOver;
extern direction sDir;
extern SDL_Window* window;
extern SDL_Renderer* renderer;

void showStartMenu(SDL_Renderer* renderer);
void init();
void Draw(SDL_Renderer* renderer);
void Input();
void update();
void collision();
void gameOver();