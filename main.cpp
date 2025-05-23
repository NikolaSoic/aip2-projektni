#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define VELICINA 3

#define CRVENA 'R'
#define ZELENA 'G'
#define PLAVA 'B'
#define ZUTA 'Y'
#define NARANCASTA 'O'
#define BIJELA 'W'

char kocka[6][VELICINA * VELICINA];

void inicijalizirajKocku()
{
    char boje[] = {BIJELA, NARANCASTA, ZELENA, CRVENA, PLAVA, ZUTA};
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < VELICINA * VELICINA; j++)
        {
            kocka[i][j] = boje[i];
        }
    }
}

void promijesajKocku()
{
    char sve_boje[6 * VELICINA * VELICINA];
    int index = 0;
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < VELICINA * VELICINA; j++)
        {
            sve_boje[index++] = kocka[i][j];
        }
    }
    for (int i = 6 * VELICINA * VELICINA - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        char temp = sve_boje[i];
        sve_boje[i] = sve_boje[j];
        sve_boje[j] = temp;
    }
    index = 0;
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < VELICINA * VELICINA; j++)
        {
            kocka[i][j] = sve_boje[index++];
        }
    }
}

void ispisiKvadratBoje(char boja)
{
    switch (boja)
    {
    case CRVENA:
        printf("[\x1b[41m \x1b[0m]");
        break;
    case ZELENA:
        printf("[\x1b[42m \x1b[0m]");
        break;
    case PLAVA:
        printf("[\x1b[44m \x1b[0m]");
        break;
    case ZUTA:
        printf("[\x1b[43m \x1b[0m]");
        break;
    case NARANCASTA:
        printf("[\x1b[48;5;202m \x1b[0m]");
        break;
    case BIJELA:
        printf("[\x1b[47m \x1b[0m]");
        break;
    }
}

void ispisiKocku()
{
    printf(" ");
    for (int j = 0; j < VELICINA; j++)
        ispisiKvadratBoje(kocka[0][j]);
    printf("\n ");
    for (int j = VELICINA; j < 2 * VELICINA; j++)
        ispisiKvadratBoje(kocka[0][j]);
    printf("\n ");
    for (int j = 2 * VELICINA; j < 3 * VELICINA; j++)
        ispisiKvadratBoje(kocka[0][j]);
    printf("\n");

    for (int i = 0; i < VELICINA; i++)
    {
        for (int lice = 1; lice <= 4; lice++)
        {
            for (int j = 0; j < VELICINA; j++)
            {
                ispisiKvadratBoje(kocka[lice][i * VELICINA + j]);
            }
            printf(" ");
        }
        printf("\n");
    }

    printf(" ");
    for (int j = 0; j < VELICINA; j++)
        ispisiKvadratBoje(kocka[5][j]);
    printf("\n ");
    for (int j = VELICINA; j < 2 * VELICINA; j++)
        ispisiKvadratBoje(kocka[5][j]);
    printf("\n ");
    for (int j = 2 * VELICINA; j < 3 * VELICINA; j++)
        ispisiKvadratBoje(kocka[5][j]);
    printf("\n");
}

void spremiKockuUTekst()
{
    FILE *f = fopen("rubik.txt", "w");
    if (!f)
    {
        printf("Greška pri otvaranju datoteke!\n");
        return;
    }
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < VELICINA * VELICINA; j++)
        {
            fprintf(f, "%c ", kocka[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

void ucitajKockuIzTeksta()
{
    FILE *f = fopen("rubik.txt", "r");
    if (!f)
    {
        printf("Datoteka ne postoji!\n");
        return;
    }
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < VELICINA * VELICINA; j++)
        {
            fscanf(f, " %c", &kocka[i][j]);
        }
    }
    fclose(f);
}

void spremiKockuUBinarno()
{
    FILE *f = fopen("rubik.bin", "wb");
    if (!f)
    {
        printf("Greška pri otvaranju datoteke!\n");
        return;
    }
    fwrite(kocka, sizeof(char), 6 * VELICINA * VELICINA, f);
    fclose(f);
}

void ucitajKockuIzBinarnog()
{
    FILE *f = fopen("rubik.bin", "rb");
    if (!f)
    {
        printf("Datoteka ne postoji!\n");
        return;
    }
    fread(kocka, sizeof(char), 6 * VELICINA * VELICINA, f);
    fclose(f);
}

int main()
{
    int izbor;

    while (1)
    {
        printf("1. Igraj\n");
        printf("2. Spremi u tekstualnu datoteku\n");
        printf("3. Učitaj iz tekstualne datoteke\n");
        printf("4. Spremi u binarnu datoteku\n");
        printf("5. Učitaj iz binarne datoteke\n");
        printf("6. Izlaz\n");

        printf("Unesite vaš izbor: ");
        scanf("%d", &izbor);

        switch (izbor)
        {
        case 1:
            inicijalizirajKocku();
            promijesajKocku();
            ispisiKocku();
            break;
        case 2:
            spremiKockuUTekst();
            break;
        case 3:
            ucitajKockuIzTeksta();
            ispisiKocku();
            break;
        case 4:
            spremiKockuUBinarno();
            break;
        case 5:
            ucitajKockuIzBinarnog();
            ispisiKocku();
            break;
        case 6:
            return 0;
        default:
            printf("Neispravan unos!\n");
        }
    }
}
