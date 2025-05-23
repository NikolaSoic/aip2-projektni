#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <ctime>

using namespace std;
using namespace chrono;

class RubikovaKocka
{
private:
    static const int VELICINA = 3;
    static const int LICA = 6;

    enum Lice
    {
        VRH,
        LIJEVO,
        PREDNJE,
        DESNO,
        STRAZNJE,
        DNO
    };
    char kocka[LICA][VELICINA][VELICINA];

    // Vremensko mjerenje
    time_point<steady_clock> pocetak;
    bool aktivna_igra;

    // Historija poteza
    vector<string> potezi;
    int ukupan_broj_poteza;

    // Boje za prikaz
    const string ANSI_BOJE[6] = {
        "\x1b[47m \x1b[0m", // Bijela (VRH)
        "\x1b[41m \x1b[0m", // Crvena (LIJEVO)
        "\x1b[42m \x1b[0m", // Zelena (PREDNJE)
        "\x1b[44m \x1b[0m", // Plava (DESNO)
        "\x1b[43m \x1b[0m", // Žuta (STRAZNJE)
        "\x1b[45m \x1b[0m"  // Magenta (DNO)
    };

    const char IMENA_BOJA[6] = {'W', 'R', 'G', 'B', 'Y', 'M'};

public:
    struct Rezultat
    {
        string ime;
        double vrijeme;
        int broj_poteza;
        string stanje;
        string datum;
        bool rijeseno;

        string za_datoteku() const
        {
            stringstream ss;
            ss << fixed << setprecision(2) << vrijeme;
            return ime + "," + ss.str() + "," + to_string(broj_poteza) + "," +
                   (rijeseno ? "RIJESENO" : "NEDOVRSENO") + "," + datum + "," + stanje;
        }

        static Rezultat iz_linije(const string &linija)
        {
            Rezultat r;
            stringstream ss(linija);
            getline(ss, r.ime, ',');
            string vrijeme_str;
            getline(ss, vrijeme_str, ',');
            r.vrijeme = stod(vrijeme_str);
            string potezi_str;
            getline(ss, potezi_str, ',');
            r.broj_poteza = stoi(potezi_str);
            string status;
            getline(ss, status, ',');
            r.rijeseno = (status == "RIJESENO");
            getline(ss, r.datum, ',');
            getline(ss, r.stanje);
            return r;
        }
    };

    RubikovaKocka() : aktivna_igra(false), ukupan_broj_poteza(0)
    {
        inicijaliziraj();
        stvori_datoteke_ako_ne_postoje();
    }

    void inicijaliziraj()
    {
        for (int l = 0; l < LICA; l++)
            for (int i = 0; i < VELICINA; i++)
                for (int j = 0; j < VELICINA; j++)
                    kocka[l][i][j] = IMENA_BOJA[l];
    }

    void promijesaj(int broj_poteza = 25)
    {
        cout << "Miješam kocku sa " << broj_poteza << " poteza..." << endl;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(0, 11);
        const string svi_potezi[] = {"F", "F'", "B", "B'", "L", "L'", "R", "R'", "U", "U'", "D", "D'"};

        vector<string> korišteni_potezi;
        for (int i = 0; i < broj_poteza; i++)
        {
            int p = dist(gen);
            korišteni_potezi.push_back(svi_potezi[p]);
            izvrsi_potez_interno(svi_potezi[p]);
        }

        // Spremi početno stanje nakon miješanja
        spremi_raspored_polja("pocetno_stanje.txt", "POČETNO STANJE NAKON MIJEŠANJA", korišteni_potezi);
        aktivna_igra = false;
        ukupan_broj_poteza = 0;
        potezi.clear();
    }

    void zapocni_igru()
    {
        pocetak = steady_clock::now();
        aktivna_igra = true;
        potezi.clear();
        ukupan_broj_poteza = 0;
        cout << "Igra započeta! Vrijeme se mjeri..." << endl;
    }

    double vrijeme_igre() const
    {
        if (!aktivna_igra)
            return 0.0;
        return duration<double>(steady_clock::now() - pocetak).count();
    }

    void izvrsi_potez(const string &potez)
    {
        if (!aktivna_igra)
        {
            cout << "Igra nije započeta! Započinjem novu igru..." << endl;
            zapocni_igru();
        }

        potezi.push_back(potez);
        ukupan_broj_poteza++;
        izvrsi_potez_interno(potez);

        cout << "Potez: " << potez << " (ukupno: " << ukupan_broj_poteza << ")" << endl;
        cout << "Vrijeme: " << fixed << setprecision(1) << vrijeme_igre() << "s" << endl;

        // Automatski spremi stanje nakon svakog poteza
        spremi_trenutno_stanje();

        if (je_rijesena())
        {
            aktivna_igra = false;
            cout << "\n🎉 ČESTITAMO! Riješili ste Rubikovu kocku! 🎉" << endl;
            cout << "Vrijeme: " << fixed << setprecision(2) << vrijeme_igre() << " sekundi" << endl;
            cout << "Broj poteza: " << ukupan_broj_poteza << endl;
        }
    }

private:
    void izvrsi_potez_interno(const string &potez)
    {
        // Implementacija rotacija za osnovne poteze
        if (potez == "F")
        {
            rotiraj_lice(PREDNJE);
            rotiraj_rubove_F();
        }
        else if (potez == "F'")
        {
            rotiraj_lice_obrnuto(PREDNJE);
            rotiraj_rubove_F_obrnuto();
        }
        else if (potez == "B")
        {
            rotiraj_lice(STRAZNJE);
            rotiraj_rubove_B();
        }
        else if (potez == "B'")
        {
            rotiraj_lice_obrnuto(STRAZNJE);
            rotiraj_rubove_B_obrnuto();
        }
        else if (potez == "L")
        {
            rotiraj_lice(LIJEVO);
            rotiraj_rubove_L();
        }
        else if (potez == "L'")
        {
            rotiraj_lice_obrnuto(LIJEVO);
            rotiraj_rubove_L_obrnuto();
        }
        else if (potez == "R")
        {
            rotiraj_lice(DESNO);
            rotiraj_rubove_R();
        }
        else if (potez == "R'")
        {
            rotiraj_lice_obrnuto(DESNO);
            rotiraj_rubove_R_obrnuto();
        }
        else if (potez == "U")
        {
            rotiraj_lice(VRH);
            rotiraj_rubove_U();
        }
        else if (potez == "U'")
        {
            rotiraj_lice_obrnuto(VRH);
            rotiraj_rubove_U_obrnuto();
        }
        else if (potez == "D")
        {
            rotiraj_lice(DNO);
            rotiraj_rubove_D();
        }
        else if (potez == "D'")
        {
            rotiraj_lice_obrnuto(DNO);
            rotiraj_rubove_D_obrnuto();
        }
    }

    void rotiraj_lice(int lice)
    {
        char temp[VELICINA][VELICINA];
        for (int i = 0; i < VELICINA; i++)
            for (int j = 0; j < VELICINA; j++)
                temp[j][VELICINA - 1 - i] = kocka[lice][i][j];

        for (int i = 0; i < VELICINA; i++)
            for (int j = 0; j < VELICINA; j++)
                kocka[lice][i][j] = temp[i][j];
    }

    void rotiraj_lice_obrnuto(int lice)
    {
        rotiraj_lice(lice);
        rotiraj_lice(lice);
        rotiraj_lice(lice);
    }

    // Implementacija rotacija rubova (pojednostavljena verzija)
    void rotiraj_rubove_F()
    {
        char temp[3];
        for (int i = 0; i < 3; i++)
            temp[i] = kocka[VRH][2][i];
        for (int i = 0; i < 3; i++)
            kocka[VRH][2][i] = kocka[LIJEVO][2 - i][2];
        for (int i = 0; i < 3; i++)
            kocka[LIJEVO][2 - i][2] = kocka[DNO][0][2 - i];
        for (int i = 0; i < 3; i++)
            kocka[DNO][0][2 - i] = kocka[DESNO][i][0];
        for (int i = 0; i < 3; i++)
            kocka[DESNO][i][0] = temp[i];
    }

    void rotiraj_rubove_F_obrnuto()
    {
        rotiraj_rubove_F();
        rotiraj_rubove_F();
        rotiraj_rubove_F();
    }

    void rotiraj_rubove_B()
    {
        char temp[3];
        for (int i = 0; i < 3; i++)
            temp[i] = kocka[VRH][0][i];
        for (int i = 0; i < 3; i++)
            kocka[VRH][0][i] = kocka[DESNO][i][2];
        for (int i = 0; i < 3; i++)
            kocka[DESNO][i][2] = kocka[DNO][2][2 - i];
        for (int i = 0; i < 3; i++)
            kocka[DNO][2][2 - i] = kocka[LIJEVO][2 - i][0];
        for (int i = 0; i < 3; i++)
            kocka[LIJEVO][2 - i][0] = temp[i];
    }

    void rotiraj_rubove_B_obrnuto()
    {
        rotiraj_rubove_B();
        rotiraj_rubove_B();
        rotiraj_rubove_B();
    }

    void rotiraj_rubove_L()
    {
        char temp[3];
        for (int i = 0; i < 3; i++)
            temp[i] = kocka[VRH][i][0];
        for (int i = 0; i < 3; i++)
            kocka[VRH][i][0] = kocka[STRAZNJE][2 - i][2];
        for (int i = 0; i < 3; i++)
            kocka[STRAZNJE][2 - i][2] = kocka[DNO][i][0];
        for (int i = 0; i < 3; i++)
            kocka[DNO][i][0] = kocka[PREDNJE][i][0];
        for (int i = 0; i < 3; i++)
            kocka[PREDNJE][i][0] = temp[i];
    }

    void rotiraj_rubove_L_obrnuto()
    {
        rotiraj_rubove_L();
        rotiraj_rubove_L();
        rotiraj_rubove_L();
    }

    void rotiraj_rubove_R()
    {
        char temp[3];
        for (int i = 0; i < 3; i++)
            temp[i] = kocka[VRH][i][2];
        for (int i = 0; i < 3; i++)
            kocka[VRH][i][2] = kocka[PREDNJE][i][2];
        for (int i = 0; i < 3; i++)
            kocka[PREDNJE][i][2] = kocka[DNO][i][2];
        for (int i = 0; i < 3; i++)
            kocka[DNO][i][2] = kocka[STRAZNJE][2 - i][0];
        for (int i = 0; i < 3; i++)
            kocka[STRAZNJE][2 - i][0] = temp[i];
    }

    void rotiraj_rubove_R_obrnuto()
    {
        rotiraj_rubove_R();
        rotiraj_rubove_R();
        rotiraj_rubove_R();
    }

    void rotiraj_rubove_U()
    {
        char temp[3];
        for (int i = 0; i < 3; i++)
            temp[i] = kocka[PREDNJE][0][i];
        for (int i = 0; i < 3; i++)
            kocka[PREDNJE][0][i] = kocka[DESNO][0][i];
        for (int i = 0; i < 3; i++)
            kocka[DESNO][0][i] = kocka[STRAZNJE][0][i];
        for (int i = 0; i < 3; i++)
            kocka[STRAZNJE][0][i] = kocka[LIJEVO][0][i];
        for (int i = 0; i < 3; i++)
            kocka[LIJEVO][0][i] = temp[i];
    }

    void rotiraj_rubove_U_obrnuto()
    {
        rotiraj_rubove_U();
        rotiraj_rubove_U();
        rotiraj_rubove_U();
    }

    void rotiraj_rubove_D()
    {
        char temp[3];
        for (int i = 0; i < 3; i++)
            temp[i] = kocka[PREDNJE][2][i];
        for (int i = 0; i < 3; i++)
            kocka[PREDNJE][2][i] = kocka[LIJEVO][2][i];
        for (int i = 0; i < 3; i++)
            kocka[LIJEVO][2][i] = kocka[STRAZNJE][2][i];
        for (int i = 0; i < 3; i++)
            kocka[STRAZNJE][2][i] = kocka[DESNO][2][i];
        for (int i = 0; i < 3; i++)
            kocka[DESNO][2][i] = temp[i];
    }

    void rotiraj_rubove_D_obrnuto()
    {
        rotiraj_rubove_D();
        rotiraj_rubove_D();
        rotiraj_rubove_D();
    }

public:
    bool je_rijesena() const
    {
        for (int l = 0; l < LICA; l++)
        {
            char boja = kocka[l][0][0];
            for (int i = 0; i < VELICINA; i++)
                for (int j = 0; j < VELICINA; j++)
                    if (kocka[l][i][j] != boja)
                        return false;
        }
        return true;
    }

    void prikazi_kocku() const
    {
        cout << "\n=== TRENUTNO STANJE KOCKE ===" << endl;

        // Prikaz vrha
        cout << "      VRH (W)" << endl;
        for (int i = 0; i < VELICINA; i++)
        {
            cout << "    ";
            for (int j = 0; j < VELICINA; j++)
            {
                cout << ANSI_BOJE[VRH] << " ";
            }
            cout << endl;
        }

        // Prikaz sredine (L F R B)
        cout << "\nL(R) F(G) R(B) B(Y)" << endl;
        for (int i = 0; i < VELICINA; i++)
        {
            for (int lice : {LIJEVO, PREDNJE, DESNO, STRAZNJE})
            {
                for (int j = 0; j < VELICINA; j++)
                {
                    cout << ANSI_BOJE[lice] << " ";
                }
                cout << " ";
            }
            cout << endl;
        }

        // Prikaz dna
        cout << "\n      DNO (M)" << endl;
        for (int i = 0; i < VELICINA; i++)
        {
            cout << "    ";
            for (int j = 0; j < VELICINA; j++)
            {
                cout << ANSI_BOJE[DNO] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    void spremi_trenutno_stanje()
    {
        string filename = "stanje_potez_" + to_string(ukupan_broj_poteza) + ".txt";
        spremi_raspored_polja(filename, "STANJE NAKON " + to_string(ukupan_broj_poteza) + " POTEZA", potezi);
    }

    void spremi_raspored_polja(const string &datoteka, const string &naslov = "", const vector<string> &korišteni_potezi = {}) const
    {
        ofstream f(datoteka);
        f << "=== " << naslov << " ===" << endl;
        f << "Datum/Vrijeme: " << trenutni_datum() << endl;

        if (aktivna_igra)
        {
            f << "Vrijeme igre: " << fixed << setprecision(2) << vrijeme_igre() << " sekundi" << endl;
            f << "Broj poteza: " << ukupan_broj_poteza << endl;
        }

        f << "Status: " << (je_rijesena() ? "RIJEŠENO" : "NIJE RIJEŠENO") << endl;

        if (!korišteni_potezi.empty())
        {
            f << "Korišteni potezi: ";
            for (size_t i = 0; i < korišteni_potezi.size(); i++)
            {
                f << korišteni_potezi[i];
                if (i < korišteni_potezi.size() - 1)
                    f << ", ";
            }
            f << endl;
        }

        f << "\n=== RASPORED POLJA ===" << endl;
        const string imena_lica[] = {"VRH(W)", "LIJEVO(R)", "PREDNJE(G)", "DESNO(B)", "STRAZNJE(Y)", "DNO(M)"};

        for (int l = 0; l < LICA; l++)
        {
            f << "\n"
              << imena_lica[l] << ":" << endl;
            for (int i = 0; i < VELICINA; i++)
            {
                f << "  ";
                for (int j = 0; j < VELICINA; j++)
                {
                    f << kocka[l][i][j] << " ";
                }
                f << endl;
            }
        }

        f << "\n=== KOMPAKTAN FORMAT ===" << endl;
        f << "Stanje: " << trenutno_stanje() << endl;
        f.close();

        if (naslov.find("POČETNO") == string::npos)
        {
            cout << "Stanje spremljeno u: " << datoteka << endl;
        }
    }

    void ucitaj_stanje(const string &datoteka)
    {
        ifstream f(datoteka);
        if (!f.is_open())
        {
            cout << "Greška: Ne mogu otvoriti datoteku " << datoteka << endl;
            return;
        }

        string linija;
        bool citam_stanje = false;
        int trenutno_lice = -1;
        int trenutni_red = 0;

        while (getline(f, linija))
        {
            if (linija.find("KOMPAKTAN FORMAT") != string::npos)
            {
                getline(f, linija); // Preskoči prazan red
                getline(f, linija); // Uzmi liniju sa stanjem
                if (linija.find("Stanje: ") != string::npos)
                {
                    string stanje = linija.substr(8);
                    if (stanje.length() == LICA * VELICINA * VELICINA)
                    {
                        int index = 0;
                        for (int l = 0; l < LICA; l++)
                        {
                            for (int i = 0; i < VELICINA; i++)
                            {
                                for (int j = 0; j < VELICINA; j++)
                                {
                                    kocka[l][i][j] = stanje[index++];
                                }
                            }
                        }
                        cout << "Stanje uspješno učitano iz: " << datoteka << endl;
                        return;
                    }
                }
            }
        }

        cout << "Greška pri učitavanju stanja iz datoteke." << endl;
    }

    void spremi_rezultat(const string &ime)
    {
        Rezultat r;
        r.ime = ime;
        r.vrijeme = vrijeme_igre();
        r.broj_poteza = ukupan_broj_poteza;
        r.datum = trenutni_datum();
        r.stanje = trenutno_stanje();
        r.rijeseno = je_rijesena();

        ofstream f("leaderboard.txt", ios::app);
        f << r.za_datoteku() << endl;
        f.close();

        cout << "Rezultat spremljen!" << endl;

        // Spremi i finalno stanje
        spremi_raspored_polja("finalno_stanje_" + ime + ".txt",
                              "FINALNO STANJE - " + ime, potezi);
    }

    void prikazi_ljestvicu() const
    {
        ifstream f("leaderboard.txt");
        if (!f.is_open())
        {
            cout << "Leaderboard datoteka ne postoji ili je prazna." << endl;
            return;
        }

        vector<Rezultat> svi_rezultati;
        vector<Rezultat> rijeseni;
        string linija;

        while (getline(f, linija))
        {
            if (!linija.empty())
            {
                Rezultat r = Rezultat::iz_linije(linija);
                svi_rezultati.push_back(r);
                if (r.rijeseno)
                {
                    rijeseni.push_back(r);
                }
            }
        }
        f.close();

        // Sortiraj riješene po vremenu
        sort(rijeseni.begin(), rijeseni.end(),
             [](const Rezultat &a, const Rezultat &b)
             {
                 return a.vrijeme < b.vrijeme;
             });

        cout << "\n=== LEADERBOARD - RIJEŠENI REZULTATI ===" << endl;
        cout << left << setw(20) << "Ime" << setw(12) << "Vrijeme(s)"
             << setw(10) << "Potezi" << "Datum" << endl;
        cout << string(70, '-') << endl;

        for (size_t i = 0; i < min(rijeseni.size(), size_t(10)); i++)
        {
            const auto &r = rijeseni[i];
            cout << left << setw(20) << r.ime
                 << setw(12) << fixed << setprecision(2) << r.vrijeme
                 << setw(10) << r.broj_poteza
                 << r.datum << endl;
        }

        cout << "\n=== STATISTIKE ===" << endl;
        cout << "Ukupno pokušaja: " << svi_rezultati.size() << endl;
        cout << "Riješeno: " << rijeseni.size() << endl;
        cout << "Postotak uspjeha: " << fixed << setprecision(1)
             << (svi_rezultati.empty() ? 0.0 : (double(rijeseni.size()) / svi_rezultati.size() * 100))
             << "%" << endl;

        if (!rijeseni.empty())
        {
            cout << "Najbolje vrijeme: " << fixed << setprecision(2)
                 << rijeseni[0].vrijeme << "s (" << rijeseni[0].ime << ")" << endl;
        }
    }

    void prikazi_statistike() const
    {
        cout << "\n=== TRENUTNA IGRA ===" << endl;
        if (aktivna_igra)
        {
            cout << "Status: AKTIVNA" << endl;
            cout << "Vrijeme: " << fixed << setprecision(1) << vrijeme_igre() << "s" << endl;
            cout << "Potezi: " << ukupan_broj_poteza << endl;
            cout << "Riješeno: " << (je_rijesena() ? "DA" : "NE") << endl;
        }
        else
        {
            cout << "Status: NEAKTIVNA" << endl;
        }
    }

private:
    void stvori_datoteke_ako_ne_postoje()
    {
        // Stvori prazan leaderboard ako ne postoji
        ifstream test("leaderboard.txt");
        if (!test.is_open())
        {
            ofstream f("leaderboard.txt");
            f << "# Leaderboard - Format: ime,vrijeme,potezi,status,datum,stanje" << endl;
            f.close();
        }
        test.close();
    }

    string trenutni_datum() const
    {
        auto now = system_clock::now();
        time_t t = system_clock::to_time_t(now);
        char buf[80];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&t));
        return string(buf);
    }

    string trenutno_stanje() const
    {
        string stanje;
        for (int l = 0; l < LICA; l++)
            for (int i = 0; i < VELICINA; i++)
                for (int j = 0; j < VELICINA; j++)
                    stanje += kocka[l][i][j];
        return stanje;
    }
};

void ispisi_naslov()
{
    cout << "\n";
    cout << "╔══════════════════════════════════════╗" << endl;
    cout << "║          RUBIKOVA KOCKA 3D           ║" << endl;
    cout << "║     Sa automatskim spremanjem        ║" << endl;
    cout << "╚══════════════════════════════════════╝" << endl;
}

void ispisi_upute()
{
    cout << "\n=== UPUTE ZA KORIŠTENJE ===" << endl;
    cout << "Potezi: F, B, L, R, U, D (sa ' za obrnut smjer)" << endl;
    cout << "F/F' = Prednje lice, B/B' = Stražnje lice" << endl;
    cout << "L/L' = Lijevo lice, R/R' = Desno lice" << endl;
    cout << "U/U' = Gornje lice, D/D' = Donje lice" << endl;
    cout << "\nBoje: W=Bijela, R=Crvena, G=Zelena, B=Plava, Y=Žuta, M=Magenta" << endl;
    cout << "Svi potezi i vremena se automatski spremaju!" << endl;
}

int main()
{
    RubikovaKocka kocka;
    int izbor;
    string unos;

    while (true)
    {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        ispisi_naslov();
        kocka.prikazi_statistike();

        cout << "\n=== GLAVNI MENI ===" << endl;
        cout << "1. Nova igra (promiješaj kocku)" << endl;
        cout << "2. Izvrši potez" << endl;
        cout << "3. Prikaži kocku" << endl;
        cout << "4. Spremi trenutno stanje" << endl;
        cout << "5. Učitaj stanje iz datoteke" << endl;
        cout << "6. Prikaži leaderboard" << endl;
        cout << "7. Spremi rezultat" << endl;
        cout << "8. Upute za korištenje" << endl;
        cout << "9. Izlaz" << endl;
        cout << "\nIzbor: ";
        cin >> izbor;

        switch (izbor)
        {
        case 1:
        {
            cout << "Broj poteza za miješanje (preporučeno 20-30): ";
            int br_poteza;
            cin >> br_poteza;
            if (br_poteza < 1 || br_poteza > 100)
                br_poteza = 25;
            kocka.promijesaj(br_poteza);
            kocka.zapocni_igru();
            cout << "\nNova igra započeta! Pritisnite Enter za nastavak...";
            cin.ignore();
            cin.get();
            break;
        }
        case 2:
        {
            cout << "Unesite potez (npr. F, R', U2): ";
            cin >> unos;
            kocka.izvrsi_potez(unos);

            if (kocka.je_rijesena())
            {
                cout << "\nŽelite li spremiti rezultat? (d/n): ";
                char odgovor;
                cin >> odgovor;
                if (odgovor == 'd' || odgovor == 'D')
                {
                    cout << "Unesite svoje ime: ";
                    string ime;
                    cin.ignore();
                    getline(cin, ime);
                    kocka.spremi_rezultat(ime);
                }
            }

            cout << "\nPritisnite Enter za nastavak...";
            cin.ignore();
            if (cin.peek() != '\n')
                cin.ignore();
            cin.get();
            break;
        }
        case 3:
        {
            kocka.prikazi_kocku();
            cout << "\nPritisnite Enter za nastavak...";
            cin.ignore();
            cin.get();
            break;
        }
        case 4:
        {
            kocka.spremi_trenutno_stanje();
            cout << "\nPritisnite Enter za nastavak...";
            cin.ignore();
            cin.get();
            break;
        }
        case 5:
        {
            cout << "Unesite naziv datoteke: ";
            string datoteka;
            cin >> datoteka;
            kocka.ucitaj_stanje(datoteka);
            cout << "\nPritisnite Enter za nastavak...";
            cin.ignore();
            cin.get();
            break;
        }
        case 6:
        {
            kocka.prikazi_ljestvicu();
            cout << "\nPritisnite Enter za nastavak...";
            cin.ignore();
            cin.get();
            break;
        }
        case 7:
        {
            cout << "Unesite svoje ime: ";
            string ime;
            cin.ignore();
            getline(cin, ime);
            kocka.spremi_rezultat(ime);
            cout << "\nPritisnite Enter za nastavak...";
            cin.get();
            break;
        }
        case 8:
        {
            ispisi_upute();
            cout << "\nPritisnite Enter za nastavak...";
            cin.ignore();
            cin.get();
            break;
        }
        case 9:
        {
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