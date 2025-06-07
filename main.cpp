
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string.h>
#include <numeric>
#include <ctime>
#include <cstdlib>
#include <vector>

using namespace std;

#define VELICINA 3
#define CRVENA 'R'
#define ZELENA 'G'
#define PLAVA 'B'
#define ZUTA 'Y'
#define NARANCASTA 'O'
#define BIJELA 'W'
#define MAX_REZULTATA 1000

// Struktura za čuvanje rezultata
struct Rezultat
{
    char ime[50];
    double vrijeme;
    int broj_poteza;
    bool rijeseno;
    char datum[30];
};

// Struktura za čuvanje stanja kocke
struct StanjeKocke
{
    char kocka[6][3][3];
    char ime_igraca[50];
    int broj_poteza;
    double proteklo_vrijeme;
};

// Globalne varijable
char kocka[6][3][3]; // 6 strana kocke, svaka 3x3
char trenutni_igrac[50] = "";
bool igra_aktivna = false;
double pocetno_vrijeme = 0;
int ukupan_broj_poteza = 0;

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

// Deklaracije funkcija
void automatskoSpremiRezultat();
void zavrsiIgru();

// Funkcije za rad s vremenom
double trenutno_vrijeme()
{
    return (double)time(NULL);
}

// Funkcija za formatiranje datuma
void trenutni_datum(char *buffer)
{
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 30, "%Y-%m-%d %H:%M:%S", timeinfo);
}

// Inicijalizacija riješene kocke
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

// Funkcija za ispis obojenih kvadrata
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
    default:
        cout << "[" << boja << "]";
        break;
    }
}

// Ispis kocke s bojama
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

// Rotacija lica u smjeru kazaljke na satu
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

// Rotacija lica suprotno od kazaljke na satu
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

// Izvršavanje poteza na osnovu stringa
void izvrsiPotez(const char *potez)
{
    if (strcmp(potez, "F") == 0)
    {
        rotirajF();
    }
    else if (strcmp(potez, "F'") == 0)
    {
        rotirajFPrim();
    }
    else if (strcmp(potez, "R") == 0)
    {
        rotirajR();
    }
    else if (strcmp(potez, "R'") == 0)
    {
        rotirajRPrim();
    }
    else if (strcmp(potez, "U") == 0)
    {
        rotirajU();
    }
    else if (strcmp(potez, "U'") == 0)
    {
        rotirajUPrim();
    }
    else if (strcmp(potez, "L") == 0)
    {
        rotirajL();
    }
    else if (strcmp(potez, "L'") == 0)
    {
        rotirajLPrim();
    }
    else if (strcmp(potez, "D") == 0)
    {
        rotirajD();
    }
    else if (strcmp(potez, "D'") == 0)
    {
        rotirajDPrim();
    }
    else if (strcmp(potez, "B") == 0)
    {
        rotirajB();
    }
    else if (strcmp(potez, "B'") == 0)
    {
        rotirajBPrim();
    }
    else
    {
        cout << "Nepoznat potez: " << potez << endl;
        return;
    }

    if (igra_aktivna)
    {
        ukupan_broj_poteza++;
    }
}

// Miješanje kocke
void promijesajKocku(int broj_poteza)
{
    cout << "Miješam kocku sa " << broj_poteza << " poteza..." << endl;

    const char *mogući_potezi[] = {"F", "F'", "R", "R'", "U", "U'", "L", "L'", "D", "D'", "B", "B'"};
    int broj_mogućih = 12;

    srand(time(NULL));

    // Privremeno isključi brojanje poteza
    bool temp_igra = igra_aktivna;
    igra_aktivna = false;

    for (int i = 0; i < broj_poteza; i++)
    {
        int index = rand() % broj_mogućih;
        const char *potez = mogući_potezi[index];
        izvrsiPotez(potez);

        cout << "Potez " << (i + 1) << ": " << potez << " ";
        if ((i + 1) % 10 == 0)
            cout << endl;
    }
    cout << endl
         << "Kocka je pomiješana!" << endl;

    // Vrati stanje igre
    igra_aktivna = temp_igra;
    ukupan_broj_poteza = 0; // Reset brojač poteza nakon miješanja
}

// Provjera da li je kocka riješena
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

// Spremanje stanja igre u binarnu datoteku
void spremiStanjeIgre()
{
    if (!igra_aktivna)
    {
        cout << "Nema aktivne igre za spremanje!" << endl;
        return;
    }

    char ime_datoteke[100];
    sprintf(ime_datoteke, "save_%s.dat", trenutni_igrac);

    ofstream file(ime_datoteke, ios::binary);
    if (file.is_open())
    {
        StanjeKocke stanje;

        // Kopiraj stanje kocke
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    stanje.kocka[i][j][k] = kocka[i][j][k];
                }
            }
        }

        strcpy(stanje.ime_igraca, trenutni_igrac);
        stanje.broj_poteza = ukupan_broj_poteza;
        stanje.proteklo_vrijeme = trenutno_vrijeme() - pocetno_vrijeme;

        file.write((char *)&stanje, sizeof(StanjeKocke));
        file.close();

        cout << "Igra spremljena u datoteku: " << ime_datoteke << endl;

        // Pauziranje igre
        igra_aktivna = false;
    }
    else
    {
        cout << "Greška pri spremanju igre!" << endl;
    }
}

// Učitavanje stanja igre iz binarne datoteke
bool ucitajStanjeIgre(const char *ime_igraca)
{
    char ime_datoteke[100];
    sprintf(ime_datoteke, "save_%s.dat", ime_igraca);

    ifstream file(ime_datoteke, ios::binary);
    if (file.is_open())
    {
        StanjeKocke stanje;
        file.read((char *)&stanje, sizeof(StanjeKocke));
        file.close();

        // Ucitaj stanje kocke
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    kocka[i][j][k] = stanje.kocka[i][j][k];
                }
            }
        }

        strcpy(trenutni_igrac, stanje.ime_igraca);
        ukupan_broj_poteza = stanje.broj_poteza;
        pocetno_vrijeme = trenutno_vrijeme() - stanje.proteklo_vrijeme;
        igra_aktivna = true;

        cout << "Igra uspješno učitana za igrača: " << trenutni_igrac << endl;
        cout << "Potezi: " << ukupan_broj_poteza << endl;
        printf("Proteklo vrijeme: %.0f sekundi\n", stanje.proteklo_vrijeme);

        // Obriši save datoteku
        remove(ime_datoteke);

        return true;
    }
    else
    {
        cout << "Nema spremljene igre za igrača: " << ime_igraca << endl;
        return false;
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

// Prikaz leaderboard-a
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

void nastaviOdZadnjegPokusaja()
{
    cout << "\n========================================" << endl;
    cout << "        NASTAVI OD ZADNJEG POKUŠAJA      " << endl;
    cout << "========================================" << endl;
    cout << "Unesite ime igrača: ";
    char ime_za_ucitavanje[50];
    cin.ignore();
    cin.getline(ime_za_ucitavanje, sizeof(ime_za_ucitavanje));

    if (strlen(ime_za_ucitavanje) == 0)
    {
        cout << "Ime igrača ne može biti prazno!" << endl;
        return;
    }

    if (ucitajStanjeIgre(ime_za_ucitavanje))
    {
        cout << "Igra je uspješno učitana i nastavljena!" << endl;
    }
}

void ispisiNaslov()
{
    cout << "\n";
    cout << "========================================" << endl;
    cout << "         RUBIKOVA KOCKA 3D             " << endl;
    cout << "        Natjecanje igrača              " << endl;
    cout << "========================================" << endl;
}

void ispisiPravila()
{
    cout << "\n=== PRAVILA IGRE ===" << endl;
    cout << "1. Cilj je riješiti Rubikovu kocku - svaka strana mora biti jedne boje" << endl;
    cout << "2. Kocka ima 6 strana sa bojama:" << endl;
    cout << "   W = Bijela (Gornja)    O = Narančasta (Lijeva)" << endl;
    cout << "   G = Zelena (Prednja)   R = Crvena (Desna)" << endl;
    cout << "   B = Plava (Stražnja)   Y = Žuta (Donja)" << endl;
    cout << "3. Potezi rotiraju lica kocke:" << endl;
    cout << "   F/F' = Prednja strana  R/R' = Desna strana" << endl;
    cout << "   L/L' = Lijeva strana   U/U' = Gornja strana" << endl;
    cout << "   D/D' = Donja strana    B/B' = Stražnja strana" << endl;
    cout << "   (Znak ' = rotacija u suprotnom smjeru)" << endl;
    cout << "4. Riješite kocku u najmanjem vremenu s najmanjim brojem poteza!" << endl;
    cout << "5. Rezultati se automatski spremaju u leaderboard!" << endl;
    cout << "6. Na početku svake igre unesete ime igrača!" << endl;
    cout << "7. Možete spremiti igru i nastaviti kasnije pomoću opcije 'Nastavi od zadnjeg pokušaja'" << endl;
}

void ispisiStatistike()
{
    cout << "\n=== TRENUTNA IGRA ===" << endl;
    if (igra_aktivna)
    {
        cout << "Igrač: " << trenutni_igrac << endl;
        cout << "Status: AKTIVNA" << endl;
        double trenutno = trenutno_vrijeme() - pocetno_vrijeme;
        printf("Vrijeme: %.0f sekundi\n", trenutno);
        cout << "Potezi: " << ukupan_broj_poteza << endl;
        cout << "Riješeno: " << (jeLiRiješeno() ? "DA" : "NE") << endl;
    }
    else
    {
        if (strlen(trenutni_igrac) > 0)
        {
            cout << "Zadnji igrač: " << trenutni_igrac << endl;
        }
        cout << "Status: NEAKTIVNA" << endl;
        if (ukupan_broj_poteza > 0)
        {
            cout << "Zadnji rezultat - Potezi: " << ukupan_broj_poteza << endl;
        }
    }
}

int main()
{
    int izbor;
    char unos[10];

    // Inicijalizacija
    inicijalizirajKocku();

    while (true)
    {
        // Čišćenje ekrana
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        ispisiNaslov();
        ispisiStatistike();

        cout << "\n=== GLAVNI MENI ===" << endl;
        cout << "1. Pravila igre" << endl;
        cout << "2. Nova igra (unesi ime i promiješaj kocku)" << endl;
        cout << "3. Prikaži kocku" << endl;
        cout << "4. Izvrši potez" << endl;
        cout << "5. Prikaži leaderboard" << endl;
        cout << "6. Završi trenutnu igru" << endl;
        cout << "7. Spremi trenutnu igru" << endl;
        cout << "8. Nastavi od zadnjeg pokušaja" << endl;
        cout << "9. Izlaz" << endl;
        cout << "\nUnesite vaš izbor: ";
        cin >> izbor;

        switch (izbor)
        {
        case 1:
        {
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            ispisiPravila();
            cout << "\nPritisnite Enter za nastavak...";
            cin.ignore();
            cin.get();
            break;
        }

        case 2:
        {
            // Nova igra s unosom imena
            zapocniNovuIgru();

            cout << "\nPritisnite Enter za nastavak...";
            cin.ignore();
            cin.get();
            break;
        }

        case 3:
        {
            ispisiKocku();
            cout << "\nPritisnite Enter za nastavak...";
            cin.ignore();
            cin.get();
            break;
        }

        case 4:
        {
            if (!igra_aktivna)
            {
                cout << "Igra nije aktivna! Prvo započnite novu igru ili nastavite od zadnjeg pokušaja." << endl;
            }
            else
            {
                cout << "Unesite potez (npr. F, R', U): ";
                cin >> unos;

                izvrsiPotez(unos);
                cout << "Potez izvršen: " << unos << endl;
                cout << "Ukupno poteza: " << ukupan_broj_poteza << endl;
                double trenutno = trenutno_vrijeme() - pocetno_vrijeme;
                printf("Vrijeme: %.0fs\n", trenutno);

                if (jeLiRiješeno())
                {
                    cout << "\n****************************************" << endl;
                    cout << "    ČESTITAMO! KOCKA JE RIJEŠENA!      " << endl;
                    cout << "****************************************" << endl;
                    cout << "Idite na opciju 6 da završite igru i spremite rezultat!" << endl;
                }
            }

            cout << "\nPritisnite Enter za nastavak...";
            cin.ignore();
            cin.get();
            break;
        }

        case 5:
        {
            prikaziLeaderboard();

            cout << "\nPritisnite Enter za nastavak...";
            cin.ignore();
            cin.get();
            break;
        }

        case 6:
        {
            if (!igra_aktivna)
            {
                cout << "Nema aktivne igre za završetak!" << endl;
            }
            else
            {
                zavrsiIgru();
            }

            cout << "\nPritisnite Enter za nastavak...";
            cin.ignore();
            cin.get();
            break;
        }

        case 7:
        {
            if (!igra_aktivna)
            {
                cout << "Nema aktivne igre za spremanje!" << endl;
            }
            else
            {
                spremiStanjeIgre();
            }

            cout << "\nPritisnite Enter za nastavak...";
            cin.ignore();
            cin.get();
            break;
        }

        case 8:
        {
            nastaviOdZadnjegPokusaja();

            cout << "\nPritisnite Enter za nastavak...";
            cin.ignore();
            cin.get();
            break;
        }

        case 9:
        {
            // Ako je igra aktivna, pitaj da li želi završiti
            if (igra_aktivna)
            {
                cout << "Imate aktivnu igru! Želite li je:" << endl;
                cout << "1. Završiti i spremiti rezultat" << endl;
                cout << "2. Spremiti za kasnije" << endl;
                cout << "3. Izaći bez spremanja" << endl;
                cout << "Vaš izbor (1-3): ";
                int opcija;
                cin >> opcija;

                if (opcija == 1)
                {
                    zavrsiIgru();
                }
                else if (opcija == 2)
                {
                    spremiStanjeIgre();
                }
            }

            cout << "Hvala na igranju! Doviđenja!" << endl;
            return 0;
        }

        default:
        {
            cout << "Neispravni izbor! Pokušajte ponovo." << endl;
            cout << "\nPritisnite Enter za nastavak...";
            cin.ignore();
            cin.get();
            break;
        }
        }
    }

    return 0;
}
