#include "game.h"
#include <iostream>

void inicijalizirajPlocu(double ploca[REDCI][STUPCI])
{
    for (int i = 0; i < REDCI; i++)
        for (int j = 0; j < STUPCI; j++)
            ploca[i][j] = 0;
}

// Ostale funkcije iz originalnog koda (ispisiPlocu, ubaciZnak, provjeriPobjedu)
