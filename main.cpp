#include <iostream>
#include <fstream>
#include <algorithm>
#include <string.h>
#include <numeric>
#include <cmath>
#include <ctime>

using namespace std;

#define VELICINA 3
#define CRVENA 'R'
#define ZELENA 'G'
#define PLAVA 'B'
#define ZUTA 'Y'
#define NARANCASTA 'O'
#define BIJELA 'W'
#define MAX_REZULTATA 100

char kocka[6][VELICINA][VELICINA];

// Globalne varijable za mjerenje vremena i leaderboard
int ukupan_broj_poteza = 0;
double pocetno_vrijeme = 0;
bool igra_aktivna = false;
char trenutni_igrac[50] = ""; // Ime trenutnog igrača
                              // Struktura za rezultat
struct Rezultat
{
    char ime[50];
    double vrijeme;
    int broj_poteza;
    char datum[30];
    bool rijeseno;
};

// Enum za lica kocke
enum Lice
{
    GORNJE = 0,   // Bijela (W)
    LIJEVO = 1,   // Narančasta (O)
    PREDNJE = 2,  // Zelena (G)
    DESNO = 3,    // Crvena (R)
    STRAZNJE = 4, // Plava (B)
    DONJE = 5     // Žuta (Y)
};

// Funkcije za vrijeme
double trenutno_vrijeme()
{
    return (double)time(NULL);
}

void trenutni_datum(char *buffer)
{
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 30, "%Y-%m-%d %H:%M:%S", timeinfo);
}
