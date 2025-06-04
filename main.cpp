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
void inicijalizirajKocku()
{
    char boje[] = {BIJELA, NARANCASTA, ZELENA, CRVENA, PLAVA, ZUTA};
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < VELICINA; j++)
        {
            for (int k = 0; k < VELICINA; k++)
            {
                kocka[i][j][k] = boje[i];
            }
        }
    }
}

void ispisiKvadratBoje(char boja)
{
    switch (boja)
    {
    case CRVENA:
        cout << "[\x1b[41m \x1b[0m]";
        break;
    case ZELENA:
        cout << "[\x1b[42m \x1b[0m]";
        break;
    case PLAVA:
        cout << "[\x1b[44m \x1b[0m]";
        break;
    case ZUTA:
        cout << "[\x1b[43m \x1b[0m]";
        break;
    case NARANCASTA:
        cout << "[\x1b[48;5;202m \x1b[0m]";
        break;
    case BIJELA:
        cout << "[\x1b[47m \x1b[0m]";
        break;
    }
}

void ispisiKocku()
{
    cout << "\n=== RUBIKOVA KOCKA ===" << endl;

    // Prikaz gornjeg lica
    cout << "      GORNJE (W)" << endl;
    for (int i = 0; i < VELICINA; i++)
    {
        cout << "    ";
        for (int j = 0; j < VELICINA; j++)
        {
            ispisiKvadratBoje(kocka[GORNJE][i][j]);
        }
        cout << endl;
    }

    // Prikaz srednjeg reda
    cout << "\nL(O) F(G) R(R) B(B)" << endl;
    for (int i = 0; i < VELICINA; i++)
    {
        for (int j = 0; j < VELICINA; j++)
        {
            ispisiKvadratBoje(kocka[LIJEVO][i][j]);
        }
        cout << " ";
        for (int j = 0; j < VELICINA; j++)
        {
            ispisiKvadratBoje(kocka[PREDNJE][i][j]);
        }
        cout << " ";
        for (int j = 0; j < VELICINA; j++)
        {
            ispisiKvadratBoje(kocka[DESNO][i][j]);
        }
        cout << " ";
        for (int j = 0; j < VELICINA; j++)
        {
            ispisiKvadratBoje(kocka[STRAZNJE][i][j]);
        }
        cout << endl;
    }

    // Prikaz donjeg lica
    cout << "\n      DONJE (Y)" << endl;
    for (int i = 0; i < VELICINA; i++)
    {
        cout << "    ";
        for (int j = 0; j < VELICINA; j++)
        {
            ispisiKvadratBoje(kocka[DONJE][i][j]);
        }
        cout << endl;
    }
    cout << endl;
} 

void rotirajLiceUSmjeruKazaljke(int lice)
{
    char temp[VELICINA][VELICINA];
    for (int i = 0; i < VELICINA; i++)
    {
        for (int j = 0; j < VELICINA; j++)
        {
            temp[i][j] = kocka[lice][i][j];
        }
    }
    for (int i = 0; i < VELICINA; i++)
    {
        for (int j = 0; j < VELICINA; j++)
        {
            kocka[lice][j][VELICINA - 1 - i] = temp[i][j];
        }
    }
}

void rotirajLiceProtivKazaljke(int lice)
{
    rotirajLiceUSmjeruKazaljke(lice);
    rotirajLiceUSmjeruKazaljke(lice);
    rotirajLiceUSmjeruKazaljke(lice);
}

// Rotacija prednjeg lica (F)
void rotirajF()
{
    rotirajLiceUSmjeruKazaljke(PREDNJE);
    char temp[VELICINA];
    for (int i = 0; i < VELICINA; i++)
    {
        temp[i] = kocka[GORNJE][2][i];
    }
    for (int i = 0; i < VELICINA; i++)
    {
        kocka[GORNJE][2][i] = kocka[LIJEVO][2 - i][2];
    }
    for (int i = 0; i < VELICINA; i++)
    {
        kocka[LIJEVO][2 - i][2] = kocka[DONJE][0][2 - i];
    }
    for (int i = 0; i < VELICINA; i++)
    {
        kocka[DONJE][0][2 - i] = kocka[DESNO][i][0];
    }
    for (int i = 0; i < VELICINA; i++)
    {
        kocka[DESNO][i][0] = temp[i];
    }
}

void rotirajFPrim()
{
    rotirajF();
    rotirajF();
    rotirajF();
}

// Rotacija desnog lica (R)
void rotirajR()
{
    rotirajLiceUSmjeruKazaljke(DESNO);
    char temp[VELICINA];
    for (int i = 0; i < VELICINA; i++)
    {
        temp[i] = kocka[GORNJE][i][2];
    }
    for (int i = 0; i < VELICINA; i++)
    {
        kocka[GORNJE][i][2] = kocka[PREDNJE][i][2];
    }
    for (int i = 0; i < VELICINA; i++)
    {
        kocka[PREDNJE][i][2] = kocka[DONJE][i][2];
    }
    for (int i = 0; i < VELICINA; i++)
    {
        kocka[DONJE][i][2] = kocka[STRAZNJE][2 - i][0];
    }
    for (int i = 0; i < VELICINA; i++)
    {
        kocka[STRAZNJE][2 - i][0] = temp[i];
    }
}

void rotirajRPrim()
{
    rotirajR();
    rotirajR();
    rotirajR();
}

// Rotacija gornjeg lica (U)
void rotirajU()
{
    rotirajLiceUSmjeruKazaljke(GORNJE);
    char temp[VELICINA];
    for (int i = 0; i < VELICINA; i++)
    {
        temp[i] = kocka[PREDNJE][0][i];
    }
    for (int i = 0; i < VELICINA; i++)
    {
        kocka[PREDNJE][0][i] = kocka[DESNO][0][i];
    }
    for (int i = 0; i < VELICINA; i++)
    {
        kocka[DESNO][0][i] = kocka[STRAZNJE][0][i];
    }
    for (int i = 0; i < VELICINA; i++)
    {
        kocka[STRAZNJE][0][i] = kocka[LIJEVO][0][i];
    }
    for (int i = 0; i < VELICINA; i++)
    {
        kocka[LIJEVO][0][i] = temp[i];
    }
}

void rotirajUPrim()
{
    rotirajU();
    rotirajU();
    rotirajU();
}

// Rotacija lijevog lica (L)
void rotirajL()
{
    rotirajLiceUSmjeruKazaljke(LIJEVO);
    char temp[VELICINA];
    for (int i = 0; i < VELICINA; i++)
    {
        temp[i] = kocka[GORNJE][i][0];
    }
    for (int i = 0; i < VELICINA; i++)
    {
        kocka[GORNJE][i][0] = kocka[STRAZNJE][2 - i][2];
    }
    for (int i = 0; i < VELICINA; i++)
    {
        kocka[STRAZNJE][2 - i][2] = kocka[DONJE][i][0];
    }
    for (int i = 0; i < VELICINA; i++)
    {
        kocka[DONJE][i][0] = kocka[PREDNJE][i][0];
    }
    for (int i = 0; i < VELICINA; i++)
    {
        kocka[PREDNJE][i][0] = temp[i];
    }
}

void rotirajLPrim()
{
    rotirajL();
    rotirajL();
    rotirajL();
}

// Rotacija donjeg lica (D)
void rotirajD()
{
    rotirajLiceUSmjeruKazaljke(DONJE);
    char temp[VELICINA];
    for (int i = 0; i < VELICINA; i++)
    {
        temp[i] = kocka[PREDNJE][2][i];
    }
    for (int i = 0; i < VELICINA; i++)
    {
        kocka[PREDNJE][2][i] = kocka[LIJEVO][2][i];
    }
    for (int i = 0; i < VELICINA; i++)
    {
        kocka[LIJEVO][2][i] = kocka[STRAZNJE][2][i];
    }
    for (int i = 0; i < VELICINA; i++)
    {
        kocka[STRAZNJE][2][i] = kocka[DESNO][2][i];
    }
    for (int i = 0; i < VELICINA; i++)
    {
        kocka[DESNO][2][i] = temp[i];
    }
}

void rotirajDPrim()
{
    rotirajD();
    rotirajD();
    rotirajD();
}

// Rotacija stražnjeg lica (B)
void rotirajB()
{
    rotirajLiceUSmjeruKazaljke(STRAZNJE);
    char temp[VELICINA];
    for (int i = 0; i < VELICINA; i++)
    {
        temp[i] = kocka[GORNJE][0][i];
    }
    for (int i = 0; i < VELICINA; i++)
    {
        kocka[GORNJE][0][i] = kocka[DESNO][i][2];
    }
    for (int i = 0; i < VELICINA; i++)
    {
        kocka[DESNO][i][2] = kocka[DONJE][2][2 - i];
    }
    for (int i = 0; i < VELICINA; i++)
    {
        kocka[DONJE][2][2 - i] = kocka[LIJEVO][2 - i][0];
    }
    for (int i = 0; i < VELICINA; i++)
    {
        kocka[LIJEVO][2 - i][0] = temp[i];
    }
}

void rotirajBPrim()
{
    rotirajB();
    rotirajB();
    rotirajB();
}
