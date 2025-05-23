#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;

class RubikovaKocka
{
private:
    static const int VELICINA = 3;
    static const int LICA = 6;

    enum Lice
    {
        VRH = 0,
        LIJEVO = 1,
        PREDNJE = 2,
        DESNO = 3,
        STRAZNJE = 4,
        DNO = 5
    };

    char kocka[LICA][VELICINA][VELICINA];
    vector<string> historijaPoteza;
    chrono::steady_clock::time_point pocetnoVrijeme;
    bool igraPoceta;

    const string boje[6] = {
        "\x1b[47m \x1b[0m",       // Bijela (Vrh)
        "\x1b[48;5;202m \x1b[0m", // Narančasta (Lijevo)
        "\x1b[42m \x1b[0m",       // Zelena (Prednje)
        "\x1b[41m \x1b[0m",       // Crvena (Desno)
        "\x1b[44m \x1b[0m",       // Plava (Stražnje)
        "\x1b[43m \x1b[0m"        // Žuta (Dno)
    };

    const char znakovi[6] = {'B', 'N', 'Z', 'C', 'P', 'Z'};

public:
    struct UnosLjestvice
    {
        string imeIgraca;
        double vrijeme;
        int potezi;
        string stanjeKocke;
        string datumVrijeme;

        string uString() const
        {
            stringstream ss;
            ss << fixed << setprecision(2) << vrijeme;
            return imeIgraca + "," + ss.str() + "," + to_string(potezi) + "," + stanjeKocke + "," + datumVrijeme;
        }

        static UnosLjestvice izStringa(const string &linija)
        {
            UnosLjestvice unos;
            stringstream ss(linija);
            string stavka;
            getline(ss, unos.imeIgraca, ',');
            getline(ss, stavka, ',');
            unos.vrijeme = stod(stavka);
            getline(ss, stavka, ',');
            unos.potezi = stoi(stavka);
            getline(ss, unos.stanjeKocke, ',');
            getline(ss, unos.datumVrijeme);
            return unos;
        }
    };

    RubikovaKocka() : igraPoceta(false)
    {
        inicijalizirajKocku();
    }

    void inicijalizirajKocku()
    {
        for (int lice = 0; lice < LICA; lice++)
        {
            for (int i = 0; i < VELICINA; i++)
            {
                for (int j = 0; j < VELICINA; j++)
                {
                    kocka[lice][i][j] = znakovi[lice];
                }
            }
        }
        historijaPoteza.clear();
        igraPoceta = false;
    }

    void promijesajKocku(int potezi = 20)
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> rasporedPoteza(0, 11);
        string nazivi[] = {"D", "D'", "L", "L'", "V", "V'", "N", "N'", "P", "P'", "S", "S'"};
        cout << "Miješanje kocke potezima: ";
        for (int i = 0; i < potezi; i++)
        {
            int indeksPoteza = rasporedPoteza(gen);
            string potez = nazivi[indeksPoteza];
            cout << potez << " ";
            izvrsiPotez(potez, false);
        }
        cout << endl;
        historijaPoteza.clear();
        igraPoceta = false;
    }

    void pocniIgru()
    {
        pocetnoVrijeme = chrono::steady_clock::now();
        igraPoceta = true;
        cout << "Igra počinje! Štoperica radi..." << endl;
    }

    double dohvatiProtekloVrijeme() const
    {
        if (!igraPoceta)
            return 0.0;
        auto sada = chrono::steady_clock::now();
        auto trajanje = chrono::duration_cast<chrono::milliseconds>(sada - pocetnoVrijeme);
        return trajanje.count() / 1000.0;
    }

    void rotirajUSmjeruSata(int lice)
    {
        char temp[VELICINA][VELICINA];
        for (int i = 0; i < VELICINA; i++)
            for (int j = 0; j < VELICINA; j++)
                temp[i][j] = kocka[lice][i][j];
        for (int i = 0; i < VELICINA; i++)
            for (int j = 0; j < VELICINA; j++)
                kocka[lice][j][VELICINA - 1 - i] = temp[i][j];
    }

    void rotirajProtivoSmjeraSata(int lice)
    {
        rotirajUSmjeruSata(lice);
        rotirajUSmjeruSata(lice);
        rotirajUSmjeruSata(lice);
    }

    void izvrsiPotez(const string &potez, bool zapisiPotez = true)
    {
        if (!igraPoceta && zapisiPotez)
            pocniIgru();
        if (zapisiPotez)
            historijaPoteza.push_back(potez);

        if (potez == "D")
        {
            rotirajUSmjeruSata(DESNO);
            char temp[VELICINA];
            for (int i = 0; i < VELICINA; i++)
                temp[i] = kocka[VRH][i][VELICINA - 1];
            for (int i = 0; i < VELICINA; i++)
                kocka[VRH][i][VELICINA - 1] = kocka[PREDNJE][i][VELICINA - 1];
            for (int i = 0; i < VELICINA; i++)
                kocka[PREDNJE][i][VELICINA - 1] = kocka[DNO][i][VELICINA - 1];
            for (int i = 0; i < VELICINA; i++)
                kocka[DNO][i][VELICINA - 1] = kocka[STRAZNJE][VELICINA - 1 - i][0];
            for (int i = 0; i < VELICINA; i++)
                kocka[STRAZNJE][VELICINA - 1 - i][0] = temp[i];
        }
        else if (potez == "D'")
        {
            rotirajProtivoSmjeraSata(DESNO);
            char temp[VELICINA];
            for (int i = 0; i < VELICINA; i++)
                temp[i] = kocka[VRH][i][VELICINA - 1];
            for (int i = 0; i < VELICINA; i++)
                kocka[VRH][i][VELICINA - 1] = kocka[STRAZNJE][VELICINA - 1 - i][0];
            for (int i = 0; i < VELICINA; i++)
                kocka[STRAZNJE][VELICINA - 1 - i][0] = kocka[DNO][i][VELICINA - 1];
            for (int i = 0; i < VELICINA; i++)
                kocka[DNO][i][VELICINA - 1] = kocka[PREDNJE][i][VELICINA - 1];
            for (int i = 0; i < VELICINA; i++)
                kocka[PREDNJE][i][VELICINA - 1] = temp[i];
        }
        // ... (dodaj ostale poteze prema uzorku iz izvora)
    }

    bool jeRijesena() const
    {
        for (int lice = 0; lice < LICA; lice++)
        {
            char bojaLica = kocka[lice][0][0];
            for (int i = 0; i < VELICINA; i++)
                for (int j = 0; j < VELICINA; j++)
                    if (kocka[lice][i][j] != bojaLica)
                        return false;
        }
        return true;
    }

    void prikaziKocku() const
    {
        // Prikaži gornje lice
        cout << "    ";
        for (int i = 0; i < VELICINA; i++)
        {
            for (int j = 0; j < VELICINA; j++)
            {
                cout << boje[0];
            }
            cout << endl
                 << "    ";
        }
        cout << endl;
        // Prikaži srednja lica (Lijevo, Prednje, Desno, Stražnje)
        for (int i = 0; i < VELICINA; i++)
        {
            for (int lice = 1; lice <= 4; lice++)
            {
                for (int j = 0; j < VELICINA; j++)
                {
                    cout << boje[lice];
                }
                cout << " ";
            }
            cout << endl;
        }
        cout << endl;
        // Prikaži donje lice
        cout << "    ";
        for (int i = 0; i < VELICINA; i++)
        {
            for (int j = 0; j < VELICINA; j++)
            {
                cout << boje[5];
            }
            cout << endl
                 << "    ";
        }
        cout << endl;
        if (igraPoceta)
        {
            cout << "Vrijeme: " << fixed << setprecision(2) << dohvatiProtekloVrijeme() << "s | ";
            cout << "Potezi: " << historijaPoteza.size() << endl;
        }
    }

    string dohvatiStanjeKocke() const
    {
        string stanje = "";
        for (int lice = 0; lice < LICA; lice++)
            for (int i = 0; i < VELICINA; i++)
                for (int j = 0; j < VELICINA; j++)
                    stanje += kocka[lice][i][j];
        return stanje;
    }

    void spremiKockuUTekst(const string &imeDatoteke) const
    {
        ofstream datoteka(imeDatoteke);
        if (!datoteka)
        {
            cout << "Greška pri otvaranju datoteke za pisanje!" << endl;
            return;
        }
        for (int lice = 0; lice < LICA; lice++)
        {
            for (int i = 0; i < VELICINA; i++)
            {
                for (int j = 0; j < VELICINA; j++)
                {
                    datoteka << kocka[lice][i][j] << " ";
                }
            }
            datoteka << endl;
        }
        datoteka.close();
        cout << "Kocka spremljena u " << imeDatoteke << endl;
    }

    void ucitajKockuIzTeksta(const string &imeDatoteke)
    {
        ifstream datoteka(imeDatoteke);
        if (!datoteka)
        {
            cout << "Datoteka ne postoji!" << endl;
            return;
        }
        for (int lice = 0; lice < LICA; lice++)
            for (int i = 0; i < VELICINA; i++)
                for (int j = 0; j < VELICINA; j++)
                    datoteka >> kocka[lice][i][j];
        datoteka.close();
        historijaPoteza.clear();
        igraPoceta = false;
        cout << "Kocka učitana iz " << imeDatoteke << endl;
    }

    void spremiKockuUBinarno(const string &imeDatoteke) const
    {
        ofstream datoteka(imeDatoteke, ios::binary);
        if (!datoteka)
        {
            cout << "Greška pri otvaranju datoteke za pisanje!" << endl;
            return;
        }
        datoteka.write(reinterpret_cast<const char *>(kocka), sizeof(kocka));
        datoteka.close();
        cout << "Kocka spremljena u " << imeDatoteke << endl;
    }

    void ucitajKockuIzBinarnog(const string &imeDatoteke)
    {
        ifstream datoteka(imeDatoteke, ios::binary);
        if (!datoteka)
        {
            cout << "Datoteka ne postoji!" << endl;
            return;
        }
        datoteka.read(reinterpret_cast<char *>(kocka), sizeof(kocka));
        datoteka.close();
        historijaPoteza.clear();
        igraPoceta = false;
        cout << "Kocka učitana iz " << imeDatoteke << endl;
    }

    void spremiULjestvicu(const string &imeIgraca)
    {
        if (!igraPoceta || !jeRijesena())
        {
            cout << "Igra nije završena ili kocka nije riješena!" << endl;
            return;
        }
        UnosLjestvice unos;
        unos.imeIgraca = imeIgraca;
        unos.vrijeme = dohvatiProtekloVrijeme();
        unos.potezi = historijaPoteza.size();
        unos.stanjeKocke = dohvatiStanjeKocke();
        auto sada = chrono::system_clock::now();
        auto vrijeme_t = chrono::system_clock::to_time_t(sada);
        stringstream ss;
        ss << put_time(localtime(&vrijeme_t), "%Y-%m-%d %H:%M:%S");
        unos.datumVrijeme = ss.str();
        ofstream datoteka("ljestvica.txt", ios::app);
        datoteka << unos.uString() << endl;
        datoteka.close();
        cout << "Rezultat spremljen u ljestvicu!" << endl;
    }

    void prikaziLjestvicu() const
    {
        ifstream datoteka("ljestvica.txt");
        if (!datoteka)
        {
            cout << "Ljestvica nije pronađena!" << endl;
            return;
        }
        vector<UnosLjestvice> unosi;
        string linija;
        while (getline(datoteka, linija))
        {
            if (!linija.empty())
                unosi.push_back(UnosLjestvice::izStringa(linija));
        }
        datoteka.close();
        sort(unosi.begin(), unosi.end(),
             [](const UnosLjestvice &a, const UnosLjestvice &b)
             {
                 return a.vrijeme < b.vrijeme;
             });
        cout << "\n=== LJESTVICA ===" << endl;
        cout << setw(4) << "Rank" << setw(15) << "Ime"
             << setw(12) << "Vrijeme(s)" << setw(8) << "Potezi"
             << setw(20) << "Datum" << endl;
        cout << string(59, '-') << endl;
        for (size_t i = 0; i < min(unosi.size(), size_t(10)); i++)
        {
            cout << setw(4) << (i + 1) << setw(15) << unosi[i].imeIgraca
                 << setw(12) << fixed << setprecision(2) << unosi[i].vrijeme
                 << setw(8) << unosi[i].potezi
                 << setw(20) << unosi[i].datumVrijeme << endl;
        }
        cout << endl;
    }

    void prikaziHistorijuPoteza() const
    {
        cout << "\nHistorija poteza (" << historijaPoteza.size() << " poteza): ";
        for (const auto &potez : historijaPoteza)
        {
            cout << potez << " ";
        }
        cout << endl;
    }

    void prikaziPomoc() const
    {
        cout << "\n=== POTEZI RUBIKOVE KOCKE ===" << endl;
        cout << "D  - Desno lice u smjeru sata" << endl;
        cout << "D' - Desno lice protiv smjera sata" << endl;
        cout << "L  - Lijevo lice u smjeru sata" << endl;
        cout << "L' - Lijevo lice protiv smjera sata" << endl;
        cout << "V  - Vrh lice u smjeru sata" << endl;
        cout << "V' - Vrh lice protiv smjera sata" << endl;
        cout << "N  - Donje lice u smjeru sata" << endl;
        cout << "N' - Donje lice protiv smjera sata" << endl;
        cout << "P  - Prednje lice u smjeru sata" << endl;
        cout << "P' - Prednje lice protiv smjera sata" << endl;
        cout << "S  - Stražnje lice u smjeru sata" << endl;
        cout << "S' - Stražnje lice protiv smjera sata" << endl;
        cout << "===============================" << endl;
    }
};

int main()
{
    RubikovaKocka kocka;
    int izbor;
    string unos;

    cout << "=== Napredni Simulator Rubikove Kocke ===" << endl;

    while (true)
    {
        cout << "\n1. Nova igra (Promiješaj kocku)" << endl;
        cout << "2. Napravi potez" << endl;
        cout << "3. Prikaži kocku" << endl;
        cout << "4. Resetiraj kocku" << endl;
        cout << "5. Spremi u tekstualnu datoteku" << endl;
        cout << "6. Učitaj iz tekstualne datoteke" << endl;
        cout << "7. Spremi u binarnu datoteku" << endl;
        cout << "8. Učitaj iz binarne datoteke" << endl;
        cout << "9. Prikaži ljestvicu" << endl;
        cout << "10. Prikaži historiju poteza" << endl;
        cout << "11. Prikaži pomoć" << endl;
        cout << "12. Izlaz" << endl;
        cout << "\nUnesite vaš izbor: ";

        cin >> izbor;

        switch (izbor)
        {
        case 1:
            kocka.inicijalizirajKocku();
            kocka.promijesajKocku();
            break;
        case 2:
        {
            cout << "Unesite potez (npr. D, D', L, L', V, V', N, N', P, P', S, S'): ";
            cin >> unos;
            kocka.izvrsiPotez(unos);
            if (kocka.jeRijesena())
            {
                cout << "Čestitamo! Riješili ste kocku!" << endl;
                cout << "Unesite vaše ime za ljestvicu: ";
                cin >> unos;
                kocka.spremiULjestvicu(unos);
            }
            break;
        }
        case 3:
            kocka.prikaziKocku();
            break;
        case 4:
            kocka.inicijalizirajKocku();
            cout << "Kocka resetirana." << endl;
            break;
        case 5:
            kocka.spremiKockuUTekst("stanje.txt");
            break;
        case 6:
            kocka.ucitajKockuIzTeksta("stanje.txt");
            break;
        case 7:
            kocka.spremiKockuUBinarno("stanje.bin");
            break;
        case 8:
            kocka.ucitajKockuIzBinarnog("stanje.bin");
            break;
        case 9:
            kocka.prikaziLjestvicu();
            break;
        case 10:
            kocka.prikaziHistorijuPoteza();
            break;
        case 11:
            kocka.prikaziPomoc();
            break;
        case 12:
            cout << "Izlaz iz programa." << endl;
            return 0;
        default:
            cout << "Nevažeći izbor!" << endl;
        }
    }
    return 0;
}
