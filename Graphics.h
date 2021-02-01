#ifndef Graphics
#define Graphics

#define SUPER_FAST 2
#define FAST 10
#define MEDIUM 50
#define SLOW 250

#define A_EM 0
#define B_EM 1
#define C_EM 2
#define D_EM 3
#define E_EM 4
#define SEVEN_EM 5

#define BLACK 30
#define RED 31
#define GREEN 32

#define CHARACTER_SIZE 7
#define NUMBER_OF_WHEELS 3


extern const char UP_G;
extern const char DOWN_G;
extern const char FORWARD_G;
extern const char BACKWARD_G;

void setCursor(int x, int y);

void setCursorColor(int color);

void setCursorBold();

void resetCursorFont();

void eraseCurrentLine();

void moveCursorInDirection(char c);

void clearScreen();

void printCharacters(int a, int b, int c);

void fadeText(const char *text, int duration);

#endif