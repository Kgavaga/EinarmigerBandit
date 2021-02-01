#include "Graphics.h"
#include "SlotCharacters.h"
#include <stdio.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

const char UP_G = 'A';
const char DOWN_G = 'B';
const char FORWARD_G = 'C';
const char BACKWARD_G = 'D';

void setCursor(int x, int y)
{
    printf("\e[%d;%dH", x, y);
}

void setCursorColor(int color)
{
    printf("\e[%dm", color);
}

void setCursorBold()
{
    printf("\e[1m");
}

void resetCursorFont()
{
    printf("\e[0m");
}

void eraseCurrentLine()
{
    printf("\e[K");
}

void moveCursorInDirection(char c)
{
    printf("\e[%c", c);
}

void clearScreen()
{
    system("cls");
}

void printCharacters(int a, int b, int c)
{
    int characters[] = {a, b, c};
    for (int row = 0; row < CHARACTER_SIZE; row++)
    {
        for (int wheelNumber = 0; wheelNumber < NUMBER_OF_WHEELS; wheelNumber++)
        {
            for (int character = 0; character < CHARACTER_SIZE; character++)
            {
                printf("%c", SLOT_CHARACTER[characters[wheelNumber]][character + row * CHARACTER_SIZE]);
            }
            printf("   ");
        }
        printf("\n");
    }
}

void fadeText(const char *text, int duration)
{
    for (int i = 0; text[i] != '\0'; i++)
    {
        printf("%c", text[i]);
        Sleep(duration);
    }
}