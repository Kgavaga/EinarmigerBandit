#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "SlotCharacters.h"
#include "Graphics.h"

#define ACTION_SPIN 1
#define ACTION_LEAVE 2

#define FREE_PLAYS 10

#define CHOOSE_MENU_1 0
#define CHOOSE_MENU_2 1

#define COST_OF_ROUND 0.2

const float PRIZES[] = {0.2, 0.4, 0.8, 1.6, 3.2, FREE_PLAYS};

float credit = 0;
int numberOfFreeGames = 0;
int rounds = 1;
float wonMoney = 0;

void setupRandomGenerator()
{
    time_t rawtime;
    time(&rawtime);
    //srand((unsigned int)rawtime); //Seed randomness with system time
    srand(4);
}

//[lowerBound, upperbound]
int getRandomNumber(int lowerBound, int upperBound)
{
    return (lowerBound + rand() % ((upperBound + 1) - lowerBound));
}

void setup()
{
    setupRandomGenerator();
    clearScreen();
}

void clearGame()
{
    clearScreen();
    resetCursorFont();
}

void askForMoney()
{
    int notSatisfied = 1;
    while (notSatisfied)
    {
        fadeText("Mit wie viel Geld moechtest du ins Spiel rein gehen?\n", MEDIUM);
        scanf("%f", &credit);
        if (credit >= 0.1 && credit <= 100)
        {
            notSatisfied = 0;
        }
        else
        {
            printf("Der Betrag ist unzulaessig!\n");
        }
    }
}

int spinTheWheel()
{
    int randomNumber = getRandomNumber(1, 10);

    if (randomNumber >= 1 && randomNumber < 4)
    {
        return A_EM;
    }
    else if (randomNumber >= 4 && randomNumber < 6)
    {
        return B_EM;
    }
    else if (randomNumber >= 6 && randomNumber < 8)
    {
        return C_EM;
    }
    else if (randomNumber == 8)
    {
        return D_EM;
    }
    else if (randomNumber == 9)
    {
        return E_EM;
    }
    else
    {
        return SEVEN_EM;
    }
}

float checkForWin(int a, int b, int c)
{
    if (a == b && a == c)
    {
        return PRIZES[a];
    }
    return 0;
}

int getResponseFromUser(int menu)
{
    int response = 0;
    if (menu == CHOOSE_MENU_1)
    {
        setCursor(9, 0);
        printf("1) Um zu drehen!\n2) Um zu gehen!\n");
        scanf("%d", &response);
        return response;
    }
    else if (menu == CHOOSE_MENU_2)
    {
        setCursor(12, 0);
        for (int i = 0; i < 3; i++)
        {
            eraseCurrentLine();
            moveCursorInDirection(UP_G);
        }
        eraseCurrentLine();
        fadeText("1) Moechtest du es versuchen zu verdoppeln?\n2) Moechtest fortfahren?\n", FAST);
        scanf("%d", &response);
        return response;
    }
}

void printStats()
{
    setCursor(1, 40);
    setCursorBold();
    printf("Geld: ");
    setCursorColor(RED);
    printf("%.2f", credit);

    setCursorColor(BLACK);
    setCursor(2, 40);
    printf("Freispiele: ");
    setCursorColor(RED);
    printf("%d", numberOfFreeGames);

    setCursorColor(BLACK);
    setCursor(3, 40);
    printf("Runde: ");
    setCursorColor(RED);
    printf("%d", rounds);
    resetCursorFont();
}

float fakeSpin()
{
    int firstWheel = E_EM;
    int secondWheel = E_EM;
    int thirdWheel = E_EM;

    printCharacters(firstWheel, secondWheel, thirdWheel);
    return checkForWin(firstWheel, secondWheel, thirdWheel);
}

float spin()
{
    int firstWheel = spinTheWheel();
    int secondWheel = spinTheWheel();
    int thirdWheel = spinTheWheel();

    printCharacters(firstWheel, secondWheel, thirdWheel);
    return checkForWin(firstWheel, secondWheel, thirdWheel);
}

void processDoubling(float *earnings)
{

    if (getRandomNumber(1, 1) == 1)
    {
        *earnings *= 2;
        setCursorColor(GREEN);
        fadeText("Du hast es verdoppeln koennen!\n", FAST);
        resetCursorFont();
    }
    else
    {
        *earnings = 0;
        setCursorColor(RED);
        fadeText("Du hast verloren!\n", FAST);
        resetCursorFont();
    }
}

int main()
{
    int running = 1;
    int response = 0;
    setup();
    askForMoney();
    clearScreen();

    while (running)
    {
        printStats();
        response = getResponseFromUser(CHOOSE_MENU_1);

        if (response == ACTION_SPIN && credit >= COST_OF_ROUND)
        {
            if (numberOfFreeGames == 0)
            {
                credit -= COST_OF_ROUND;
                printStats();
            }
            else
            {
                numberOfFreeGames--;
            }
            setCursor(0, 0);

            float earnings = 0;
            if (!(rounds % 10) && wonMoney < rounds * COST_OF_ROUND)
            {
                earnings = fakeSpin();
            }
            else
            {
                earnings = spin();
            }

            setCursor(4, 40);
            if (earnings == FREE_PLAYS)
            {
                setCursorBold();
                setCursorColor(GREEN);
                fadeText("Du hast 10 zusaetzliche Freispiele erhalten!", FAST);
                resetCursorFont();
                numberOfFreeGames += 10;
                wonMoney += 2;
            }
            else if (earnings > 0)
            {
                setCursorColor(32);
                printf("Du hast %.2f gewonnen!", earnings);
                resetCursorFont();

                setCursorBold();
                while (earnings != 0 && 2 * earnings < 12.81)
                {
                    Sleep(1500);
                    response = getResponseFromUser(CHOOSE_MENU_2);
                    if (response == 1)
                    {
                        processDoubling(&earnings);
                        setCursor(4, 40);
                        setCursorColor(32);
                        printf("Du hast %.2f gewonnen!", earnings);
                        resetCursorFont();
                    }
                    else
                    {
                        fadeText("OK!\n", FAST);
                        break;
                    }
                }
                wonMoney += earnings;
                credit += earnings;
            }
            else
            {
                printf("Du hast nichts gewonnen!");
            }
            Sleep(1500);
            clearScreen();
            rounds++;
        }
        else if (response == ACTION_LEAVE)
        {
            char message[1];
            sprintf(message, "Die %.2f Euro werden dir ausbezahlt.\nBye!", credit);
            fadeText(message, MEDIUM);
            Sleep(1500);
            running = 0;
        }
        else
        {
            setCursorColor(31);
            fadeText("Du hast nicht mehr genuegend Geld um zu Spielen!\n", MEDIUM);
            resetCursorFont();
            Sleep(1500);
            running = 0;
        }
    }
    clearGame();
    return 0;
}
