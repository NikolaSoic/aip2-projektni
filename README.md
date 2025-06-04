# Rubikova Kocka 3D

Napredna implementacija Rubikove kocke u C++ sa automatskim spremanjem rezultata i leaderboard funkcionalnostima.

## Funkcionalnosti

- 🎮 **Kompletna Rubikova kocka** - Sve rotacije (F, R, U, L, D, B) sa obrnutim varijantama
- ⏱️ **Mjerenje vremena** - Automatsko praćenje vremena rješavanja
- 🏆 **Leaderboard** - Top 10 rezultata sa vremenima i brojem poteza
- 💾 **Spremanje stanja** - Tekstualne i binarne datoteke
- 🔄 **Pravo miješanje** - Kocka se miješa stvarnim potezima
- 📊 **Statistike** - Praćenje napretka i rezultata
- 🎨 **Vizualni prikaz** - ANSI boje za realističan prikaz kocke

## Korištene biblioteke

Projekt koristi samo standardne C++ biblioteke:
- `<iostream>` - Za input/output operacije
- `<fstream>` - Za rad sa datotekama
- `<algorithm>` - Za algoritme sortiranja
- `<string.h>` - Za rad sa stringovima
- `<numeric>` - Za numeričke operacije
- `<cmath>` - Za matematičke funkcije

## Kako pokrenuti

### Preduvjeti
- C++ kompajler (g++, clang++, ili MSVC)
- Operativni sistem koji podržava ANSI escape kodove za boje

### Kompajliranje

```bash
# Linux/macOS
g++ -o rubik main.cpp

# Windows (MinGW)
g++ -o rubik.exe main.cpp

# Windows (MSVC)
cl main.cpp /Fe:rubik.exe
```

### Pokretanje

```bash
# Linux/macOS
./rubik

# Windows
rubik.exe
```

## Kako koristiti

1. **Pokretanje nove igre**: Odaberite opciju 2 u glavnom meniju
2. **Miješanje kocke**: Unesite broj poteza za miješanje (preporučeno 15-25)
3. **Izvršavanje poteza**: Koristite standardnu notaciju (F, R, U, L, D, B sa ' za obrnut smjer)
4. **Praćenje napretka**: Vrijeme i potezi se automatski prate
5. **Spremanje rezultata**: Kada riješite kocku, možete spremiti rezultat u leaderboard

## Notacija poteza

- **F/F'** - Prednja strana (Front)
- **R/R'** - Desna strana (Right)  
- **U/U'** - Gornja strana (Up)
- **L/L'** - Lijeva strana (Left)
- **D/D'** - Donja strana (Down)
- **B/B'** - Stražnja strana (Back)

*Napomena: ' označava rotaciju u suprotnom smjeru od kazaljke*

## Boje kocke

- **W** - Bijela (Gornja strana)
- **O** - Narančasta (Lijeva strana)
- **G** - Zelena (Prednja strana)
- **R** - Crvena (Desna strana)
- **B** - Plava (Stražnja strana)
- **Y** - Žuta (Donja strana)

## Datoteke koje se kreiraju

- `leaderboard.txt` - Tekstualna datoteka sa najboljim rezultatima
- `rezultati.bin` - Binarna datoteka sa svim rezultatima
- `stanje_*.txt` - Datoteke sa spremljenim stanjem kocke
- `finalno_stanje_*.txt` - Finalna stanja riješenih kocki

## Primjer igre

```
╔══════════════════════════════════════╗
║          RUBIKOVA KOCKA 3D           ║
║     Sa automatskim spremanjem        ║
╚══════════════════════════════════════╝

=== TRENUTNA IGRA ===
Status: AKTIVNA
Vrijeme: 45.2 sekundi
Potezi: 23
Riješeno: NE

=== GLAVNI MENI ===
1. Pravila igre
2. Nova igra (promiješaj kocku)
3. Prikaži kocku
4. Izvrši potez
5. Spremi trenutno stanje
6. Učitaj stanje iz datoteke
7. Prikaži leaderboard
8. Spremi rezultat
9. Izlaz
```

## Razvoj

Projekt je razvijen sa fokusom na:
- Čist i čitljiv kod
- Efikasne algoritme za rotacije
- Robusno rukovanje datotekama
- Korisno sučelje

## Licence

Ovaj projekt je open source i slobodan za korištenje i modificiranje.