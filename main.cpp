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

void prikaziLeaderboard()
{
    ifstream datoteka("leaderboard.txt");
    if (!datoteka.is_open())
    {
        cout << "\n=== LEADERBOARD ===" << endl;
        cout << "Nema još uvijek zabilježenih rezultata." << endl;
        cout << "Riješite kocku i spremite rezultat da vidite leaderboard!" << endl;
        return;
    }

    Rezultat svi_rezultati[MAX_REZULTATA];
    Rezultat rijeseni[MAX_REZULTATA];
    int broj_svih = 0;
    int broj_rijesenih = 0;

    char linija[200];

    // Učitaj sve rezultate
    while (datoteka.getline(linija, sizeof(linija)) && broj_svih < MAX_REZULTATA)
    {
        if (strlen(linija) == 0)
            continue;

        // Parsiraj liniju (format: ime,vrijeme,potezi,status,datum)
        char *token = strtok(linija, ",");
        if (token == NULL)
            continue;
        strcpy(svi_rezultati[broj_svih].ime, token);

        token = strtok(NULL, ",");
        if (token == NULL)
            continue;
        svi_rezultati[broj_svih].vrijeme = atof(token);

        token = strtok(NULL, ",");
        if (token == NULL)
            continue;
        svi_rezultati[broj_svih].broj_poteza = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL)
            continue;
        svi_rezultati[broj_svih].rijeseno = (strcmp(token, "RIJESENO") == 0);

        token = strtok(NULL, ",");
        if (token == NULL)
            continue;
        strcpy(svi_rezultati[broj_svih].datum, token);

        if (svi_rezultati[broj_svih].rijeseno && broj_rijesenih < MAX_REZULTATA)
        {
            rijeseni[broj_rijesenih] = svi_rezultati[broj_svih];
            broj_rijesenih++;
        }

        broj_svih++;
    }
    datoteka.close();

    // Sortiraj riješene po vremenu
    sortirajRezultate(rijeseni, broj_rijesenih);

    cout << "\n=========================================" << endl;
    cout << "               LEADERBOARD               " << endl;
    cout << "=========================================" << endl;

    if (broj_rijesenih == 0)
    {
        cout << "\nJoš uvijek nema riješenih kocki!" << endl;
        cout << "Budite prvi koji će riješiti kocku!" << endl;
    }
    else
    {
        cout << "\nNAJBOLJI REZULTATI (RIJEŠENI):" << endl;
        cout << "-----------------------------------------" << endl;
        cout << "POS.  IME              VRIJEME   POTEZI" << endl;
        cout << "-----------------------------------------" << endl;

        int max_prikaz = (broj_rijesenih < 10) ? broj_rijesenih : 10;
        for (int i = 0; i < max_prikaz; i++)
        {
            printf("%2d.   %-15s %7.0fs    %4d\n",
                   i + 1,
                   rijeseni[i].ime,
                   rijeseni[i].vrijeme,
                   rijeseni[i].broj_poteza);
        }

        cout << "-----------------------------------------" << endl;
    }

    // Prikaži SVI POKUŠAJI - i riješeni i neriješeni
    cout << "\n--- SVI POKUŠAJI SVIH IGRAČA ---" << endl;
    if (broj_svih > 0)
    {
        cout << "IME              VRIJEME   POTEZI   STATUS      DATUM" << endl;
        cout << "-------------------------------------------------------" << endl;

        for (int i = 0; i < broj_svih; i++)
        {
            printf("%-15s %7.0fs    %4d     %-10s %s\n",
                   svi_rezultati[i].ime,
                   svi_rezultati[i].vrijeme,
                   svi_rezultati[i].broj_poteza,
                   svi_rezultati[i].rijeseno ? "RIJEŠENO" : "NEDOVRŠENO",
                   svi_rezultati[i].datum);
        }
        cout << "-------------------------------------------------------" << endl;
    }

    // Statistike
    cout << "\nSTATISTIKE:" << endl;
    cout << "Ukupno pokušaja: " << broj_svih << endl;
    cout << "Riješeno: " << broj_rijesenih << endl;
    if (broj_svih > 0)
    {
        double postotak = (double(broj_rijesenih) / broj_svih) * 100.0;
        printf("Postotak uspjeha: %.1f%%\n", postotak);
    }
    else
    {
        cout << "Postotak uspjeha: 0.0%" << endl;
    }

    if (broj_rijesenih > 0)
    {
        printf("Najbolje vrijeme: %.0fs (%s)\n", rijeseni[0].vrijeme, rijeseni[0].ime);

        // Pronađi najbolji broj poteza
        int min_potezi = rijeseni[0].broj_poteza;
        int najbolji_index = 0;
        for (int i = 1; i < broj_rijesenih; i++)
        {
            if (rijeseni[i].broj_poteza < min_potezi)
            {
                min_potezi = rijeseni[i].broj_poteza;
                najbolji_index = i;
            }
        }
        printf("Najmanji broj poteza: %d (%s)\n", min_potezi, rijeseni[najbolji_index].ime);
    }

    // Pokaži rezultate trenutnog igrača (ako postoji)
    if (strlen(trenutni_igrac) > 0)
    {
        cout << "\n--- REZULTATI TRENUTNOG IGRAČA (" << trenutni_igrac << ") ---" << endl;
        bool imaPokusaja = false;

        for (int i = 0; i < broj_svih; i++)
        {
            if (strcmp(svi_rezultati[i].ime, trenutni_igrac) == 0)
            {
                if (!imaPokusaja)
                {
                    cout << "VRIJEME   POTEZI   STATUS     DATUM" << endl;
                    cout << "-------------------------------------" << endl;
                    imaPokusaja = true;
                }
                printf("%7.0fs   %4d     %-10s %s\n",
                       svi_rezultati[i].vrijeme,
                       svi_rezultati[i].broj_poteza,
                       svi_rezultati[i].rijeseno ? "RIJEŠENO" : "NEDOVRŠENO",
                       svi_rezultati[i].datum);
            }
        }

        if (!imaPokusaja)
        {
            cout << "Još nema zabilježenih pokušaja za ovog igrača." << endl;
        }
    }
}
void zapocniNovuIgru()
{
    // Prvo tražimo ime igrača
    cout << "\n========================================" << endl;
    cout << "            NOVA IGRA                   " << endl;
    cout << "========================================" << endl;
    cout << "Unesite ime igrača: ";
    cin.ignore(); // Očisti buffer
    cin.getline(trenutni_igrac, sizeof(trenutni_igrac));

    // Provjeri da li je ime prazno
    if (strlen(trenutni_igrac) == 0)
    {
        strcpy(trenutni_igrac, "Nepoznati_Igrac");
        cout << "Koristi se ime: " << trenutni_igrac << endl;
    }

    cout << "Broj poteza za miješanje (preporučeno 15-25): ";
    int br_poteza;
    cin >> br_poteza;
    if (br_poteza < 1 || br_poteza > 100)
        br_poteza = 20;

    inicijalizirajKocku();
    promijesajKocku(br_poteza);

    // Započni igru
    pocetno_vrijeme = trenutno_vrijeme();
    igra_aktivna = true;
    ukupan_broj_poteza = 0;

    cout << "\n========================================" << endl;
    cout << "Igrač: " << trenutni_igrac << endl;
    cout << "Igra započeta! Vrijeme se mjeri..." << endl;
    cout << "========================================" << endl;
}

void zavrsiIgru()
{
    if (!igra_aktivna)
    {
        cout << "Nema aktivne igre za završetak!" << endl;
        return;
    }

    igra_aktivna = false;
    double ukupno_vrijeme = trenutno_vrijeme() - pocetno_vrijeme;

    cout << "\n========================================" << endl;
    cout << "             IGRA ZAVRŠENA              " << endl;
    cout << "========================================" << endl;
    cout << "Igrač: " << trenutni_igrac << endl;
    printf("Vrijeme: %.0f sekundi\n", ukupno_vrijeme);
    cout << "Broj poteza: " << ukupan_broj_poteza << endl;

    if (jeLiRiješeno())
    {
        cout << "Status: KOCKA JE RIJEŠENA! ČESTITAMO!" << endl;
    }
    else
    {
        cout << "Status: Kocka nije riješena" << endl;
    }

    // Automatski spremi rezultat
    automatskoSpremiRezultat();
}