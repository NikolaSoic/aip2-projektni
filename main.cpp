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

void izvrsiPotez(const char *potez)
{
    ukupan_broj_poteza++;

    if (strcmp(potez, "F") == 0)
        rotirajF();
    else if (strcmp(potez, "F'") == 0)
        rotirajFPrim();
    else if (strcmp(potez, "R") == 0)
        rotirajR();
    else if (strcmp(potez, "R'") == 0)
        rotirajRPrim();
    else if (strcmp(potez, "U") == 0)
        rotirajU();
    else if (strcmp(potez, "U'") == 0)
        rotirajUPrim();
    else if (strcmp(potez, "L") == 0)
        rotirajL();
    else if (strcmp(potez, "L'") == 0)
        rotirajLPrim();
    else if (strcmp(potez, "D") == 0)
        rotirajD();
    else if (strcmp(potez, "D'") == 0)
        rotirajDPrim();
    else if (strcmp(potez, "B") == 0)
        rotirajB();
    else if (strcmp(potez, "B'") == 0)
        rotirajBPrim();
    else
    {
        cout << "Nepoznat potez: " << potez << endl;
        ukupan_broj_poteza--; // Vrati brojač jer potez nije valjan
    }
}

void promijesajKocku(int broj_poteza)
{
    cout << "Miješam kocku sa " << broj_poteza << " poteza..." << endl;

    const char *mogući_potezi[] = {"F", "F'", "R", "R'", "U", "U'", "L", "L'", "D", "D'", "B", "B'"};
    int broj_mogućih = 12;

    // Jednostavan način generiranja pseudo-slučajnih brojeva
    int seed = (int)(trenutno_vrijeme() * 1000) % 32768;

    for (int i = 0; i < broj_poteza; i++)
    {
        seed = (seed * 1103515245 + 12345) % (1 << 31);
        int index = abs(seed) % broj_mogućih;

        const char *potez = mogući_potezi[index];

        // Izvršava stvarni potez na kocki bez brojanja
        ukupan_broj_poteza--; // Kompenzacija jer izvrsiPotez uvećava brojač
        izvrsiPotez(potez);

        cout << "Potez " << (i + 1) << ": " << potez << " ";
        if ((i + 1) % 10 == 0)
            cout << endl;
    }
    cout << endl
         << "Kocka je pomiješana!" << endl;
}

bool jeLiRiješeno()
{
    for (int lice = 0; lice < 6; lice++)
    {
        char boja = kocka[lice][0][0];
        for (int i = 0; i < VELICINA; i++)
        {
            for (int j = 0; j < VELICINA; j++)
            {
                if (kocka[lice][i][j] != boja)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

void spremiRezultat(const char *ime)
{
    double vrijeme_igre = igra_aktivna ? (trenutno_vrijeme() - pocetno_vrijeme) : 0;

    // Spremi u tekstualnu datoteku
    ofstream datoteka("leaderboard.txt", ios::app);
    if (datoteka.is_open())
    {
        char datum[30];
        trenutni_datum(datum);

        datoteka << ime << "," << vrijeme_igre << "," << ukupan_broj_poteza << ",";
        datoteka << (jeLiRiješeno() ? "RIJESENO" : "NEDOVRSENO") << "," << datum << endl;
        datoteka.close();
        cout << "Rezultat uspješno spremljen u leaderboard!" << endl;
        printf("Ime: %s, Vrijeme: %.0f sekundi, Potezi: %d\n", ime, vrijeme_igre, ukupan_broj_poteza);
    }
    else
    {
        cout << "Greška pri spremanju rezultata!" << endl;
    }
}

// Automatsko spremanje rezultata na kraju igre
void automatskoSpremiRezultat()
{
    double vrijeme_igre = igra_aktivna ? (trenutno_vrijeme() - pocetno_vrijeme) : 0;

    // Spremi u tekstualnu datoteku
    ofstream datoteka("leaderboard.txt", ios::app);
    if (datoteka.is_open())
    {
        char datum[30];
        trenutni_datum(datum);

        datoteka << trenutni_igrac << "," << vrijeme_igre << "," << ukupan_broj_poteza << ",";
        datoteka << (jeLiRiješeno() ? "RIJESENO" : "NEDOVRSENO") << "," << datum << endl;
        datoteka.close();

        cout << "\n========================================" << endl;
        cout << "        REZULTAT AUTOMATSKI SPREMLJEN   " << endl;
        cout << "========================================" << endl;
        printf("Igrač: %s\n", trenutni_igrac);
        printf("Vrijeme: %.0f sekundi\n", vrijeme_igre);
        printf("Broj poteza: %d\n", ukupan_broj_poteza);
        printf("Status: %s\n", jeLiRiješeno() ? "RIJEŠENO" : "NEDOVRŠENO");
        cout << "========================================" << endl;
    }
    else
    {
        cout << "Greška pri spremanju rezultata!" << endl;
    }
}

// Jednostavna implementacija sortiranja za nizove
void sortirajRezultate(Rezultat *rezultati, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (rezultati[j].vrijeme > rezultati[j + 1].vrijeme)
            {
                Rezultat temp = rezultati[j];
                rezultati[j] = rezultati[j + 1];
                rezultati[j + 1] = temp;
            }
        }
    }
}
