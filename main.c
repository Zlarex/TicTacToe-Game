/*********************************************************************
 * Tic-Tac-Toe Game  
 *********************************************************************
 * NAMA FILE    : main.c
 * AUTHOR       : Ihsan Fauzan Hanif & Syelvie Ira Ratna Mulia
 * COMPILER     : MinGW GCC
 * DESKRIPSI    : Aplikasi konsol permainan Tic-Tac-Toe
 * 2021
 ********************************************************************/

/* HEADER */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include <windows.h>

/* MAKRO */
#define KEY_ENTER 13
#define KEY_ESCAPE 2

/* STRUKTUR */
typedef struct
{
	char nama[50];
	char simbol;
	bool isKomputer;
	long long int skor;
	int langkah[50];
	int input;
} Pemain;

typedef struct 
{
    char nama[25];
	int ukuran;
	int waktu;
	int kesulitan;
	int giliran;
	bool isBermain;
	char isi[10][10];
	Pemain pemain[2];
} Papan;

/* VARIABEL GLOBAL */
HANDLE hConsole;
WORD consoleAttr;
pthread_t timerId, permainanId;
Papan papan;
bool validInput = true;
bool pause = false;

/* DEKLARASI MODUL */
void menuPanel(char*, char*);
void menuOpsi(char*);
void menuPilihan(char*);
void pesanInvalid();
void pilihan(char*);
void aturUkuran(int*);
void aturModePermainan(bool*);
void aturNamaPemain(char*, char*);
void permainanLama();
void bagianPermainan();
void bagianCaraMain();
void bagianKeluar();
void menuUtama();
void mulaiPermainan();

/* IMPLEMENTASI MODUL */
void biarkan() {}

void keluarProgram() { exit(0); }

void menuPanel(char* judul, char* pesan)
{
    system("cls");
    printf("%s\n============================================\n%s", judul, pesan);
}

int MAX(int a, int b) { return (a > b) ? a : b; }
int MIN(int a, int b) { return (a < b) ? a : b; }

void menuOpsi(char* opsi)
{
    printf("\n%s\n", opsi);
}

void menuPilihan(char* perintahInput)
{
    printf("\n%s: ", perintahInput);
}

void pesanInvalid(char* pesan)
{
    if (!validInput) 
    {
        printf("\n%s", pesan);
        validInput = true;
    }
}

void pilihan(char* input)
{
    char temp;
    fflush(stdin);
    scanf(" %c", &temp);
    *input = toupper(temp);
}

void pilihanYaTidak(void (*pilih_ya)(void), void (*pilih_tidak)(void))
{
    while (true)
    {
        fflush(stdin);

        char input = getch();
        if ((int)input == KEY_ENTER) return (*pilih_ya)();
        else if ((int) input == KEY_ESCAPE) return (*pilih_tidak)();
    }
}

void aturPermainan()
{
    char input = '\000';
    menuPanel("Mulai Permainan", "Tekan Enter untuk memulai permainan atau 'Esc' untuk kembali\n");
    pilihanYaTidak(biarkan, bagianPermainan);
}

void dataReverse(char* data, int length)
{
    int i = 0;
    int j;
    int len = j = length - 1;
    while (i < j)
    {
        char temp = *(data + i);
        *(data + i) = *(data + len - i);
        *(data + len - i) = temp;
        i++;
        j--;
    }
}

void upperCase(char *data)
{
    int i = 0;
    for (i = 0; i < strlen(data); i++)
    {
        *(data + i) = toupper(*(data + i));
    }
}

int simpanPermainan(int pos)
{
    int papanSize = sizeof(papan);
    char fileData[255] = {'\000'};
    char fileLokasi[100] = "D:/Arsip Kuliah/Coding/TicTacToe-Game/debug/game/save";
    char *dataPapan = malloc(papanSize);
    memcpy(fileData, fileLokasi, strlen(fileLokasi));
    itoa(pos, fileData + strlen(fileData), 10);
    memcpy(fileData + strlen(fileData), ".dat", 4);
    
    FILE* pFile = fopen(fileData, "wb");
    if (pFile == NULL) return 1;
    memset(dataPapan, 0, papanSize);
    memcpy(dataPapan, &papan, papanSize - 1);
    dataReverse(dataPapan, papanSize);
    if (fwrite(dataPapan, papanSize, 1, pFile) != 1) return 1;
    fclose(pFile);
    free(dataPapan);
    return 0;
}

int imporPermainan(int pos, Papan* pPapan)
{
    int pPapanSize = sizeof(*pPapan);
    char fileData[255] = {'\000'};
    char fileLokasi[100] = "D:/Arsip Kuliah/Coding/TicTacToe-Game/debug/game/save";
    char* dataPapan = malloc(pPapanSize);
    memcpy(fileData, fileLokasi, strlen(fileLokasi));
    itoa(pos, fileData + strlen(fileData), 10);
    memcpy(fileData + strlen(fileData), ".dat", 4);
    
    FILE* pFile = fopen(fileData, "rb");
    if (pFile == NULL) return 1;
    memset(dataPapan, 0, pPapanSize);
    memset(pPapan, 0, pPapanSize);
    if (fread(dataPapan, pPapanSize, 1, pFile) != 1) return 1;
    fclose(pFile);
    dataReverse(dataPapan, pPapanSize);
    memcpy(pPapan, dataPapan, pPapanSize - 1);
    free(dataPapan);
    return 0;
}

void aturPermainanLama()
{
    char input = '\000';
    menuPanel("Mulai Permainan", "Pilihlah permainan yang ingin anda lanjutkan");
    pesanInvalid("Pilihan data permainan tidak valid");

    int i = 0;
    for (i = 0; i < 5; i++)
    {
        Papan temp;
        printf("\n%d. ", i + 1);
        if (imporPermainan(i + 1, &temp) == 0) printf("%s", temp.nama);
        else printf("[Kosong]");
    }
    menuOpsi("Q: Kembali");
    menuPilihan("Masukkan pilihan anda");
    pilihan(&input);
    if (input == 'Q') return bagianPermainan();
    
    int pilihanInt = atoi(&input);
    if (pilihanInt > 0 && pilihanInt <= 5)
    {
        if (imporPermainan(pilihanInt, &papan) == 0) return;
    }
    validInput = false;
    return aturPermainanLama();
}

int panjangAngka(int angka)
{
    int len = 0;
    int i = 0;
    while (angka != 0)
    {
        angka /= 10;
        len++;
    }
    return len;
}

char getIsiPapan(int pos)
{
    pos--;
    return papan.isi[pos/papan.ukuran][pos%papan.ukuran];
}

void setIsiPapan(int pos, char nilai)
{
    pos--;
    papan.isi[pos/papan.ukuran][pos%papan.ukuran] = nilai;
}

void tampilPapan(bool tampilPosisi)
{
    int isi = 1;
    int maxIsi = papan.ukuran * papan.ukuran;
    int maxVertikal = papan.ukuran * 3;
    int isiLen = panjangAngka(maxIsi);
    int i, j;
    int posIsi = 2;
    for (i = 0; i < maxVertikal + 1; i++)
    {
        bool bisaDiisi = false;
        char cTulisAwal = '|';

        if (i == posIsi)
        {
            bisaDiisi = true;
            posIsi += 3;
        }
        if (i % 3 == 0)
        {
            if (i == 0) cTulisAwal = ' ';
            else cTulisAwal = '|';
        }
        for (j = 0; j < papan.ukuran; j++)
        {
            int maxLenIsi = isiLen + 3;
            if (j == 0) maxLenIsi++;
            char cTulis = ' ';
            char cIsi = '_';
            if (j == papan.ukuran - 1) cTulis = cTulisAwal;
            if (i % 3 != 0)
            {
                cTulis = '|';
                cIsi = ' ';
            }
            else if (i % 3 == 0 && i != 0) cTulis = '|';
            char* cGaris = malloc(maxLenIsi + 1);
            memset(cGaris, '\000', maxLenIsi + 1);
            memset(cGaris, cIsi, maxLenIsi - 1);
            memcpy(cGaris + maxLenIsi - 1, &cTulis, 1);
            if (j == 0) memcpy(cGaris, &cTulisAwal, 1);
            if (bisaDiisi)
            {
                int allocSize = isiLen + 1;
                char* isiChar = malloc(allocSize);
                char isiPapan = getIsiPapan(isi);
                int pos = 1;
                if (j == 0) pos++;
                memset(isiChar, '\000', allocSize);
                if (isiPapan == 'X' || isiPapan == 'O') memcpy(isiChar, &isiPapan, 1);
                else if (tampilPosisi) itoa(isi, isiChar, 10);
                memcpy(cGaris + pos, isiChar, strlen(isiChar));
                free(isiChar);
                isi++;
            }
            int k = 0;
            for (k = 0; k < strlen(cGaris); k++)
            {
                if (cGaris[k] == 'X')
                {
                    SetConsoleTextAttribute(hConsole, 12);
                    printf("%c", cGaris[k]);
                    SetConsoleTextAttribute(hConsole, consoleAttr);
                }
                else if (cGaris[k] == 'O')
                {
                    SetConsoleTextAttribute(hConsole, 10);
                    printf("%c", cGaris[k]);
                    SetConsoleTextAttribute(hConsole, consoleAttr);
                }
                else printf("%c", cGaris[k]);
            }
            free(cGaris);
        }
        printf("\n");
    }
}

int cekMinimal(int ukuran)
{
    switch (ukuran)
    {
    case 3: return 3;
    case 5: return 4;
    case 7: return 5;
    }
    return 0;
}

bool cekDiagonal(char simbol)
{
    int cek = cekMinimal(papan.ukuran);
    int counter, i, j, temp;

    // Diagonal [/] dari (0, 0) sampai (papan.ukuran-1, 0)
    for (i = 0; i < papan.ukuran; i++)
    {
        counter = 0;
        temp = i;
        j = 0;
        while(temp >= 0)
        {
            if (papan.isi[temp][j] == simbol) counter++;
            else counter = 0;
            temp--;
            j++;
        }
        if (counter >= cek) return true;
    }

    // Diagonal [/] dari (0, papan.ukuran-1) sampai (papan.ukuran-1, papan.ukuran-1)
    for (i = 0; i < papan.ukuran; i++)
    {
        counter = 0;
        temp = i;
        j = papan.ukuran - 1;
        while (temp <= papan.ukuran - 1)
        {
            if (papan.isi[temp][j] == simbol) counter++;
            else counter = 0;
            temp++;
            j--;
        }
        if (counter >= cek) return true;
    }

    // Diagonal [\] dari (0, papapn.ukuran-1) sampai (papan.ukuran-1, papan.ukuran-1)
    for (i = 0; i < papan.ukuran; i++)
    {
        counter = 0;
        temp = i;
        j = papan.ukuran - 1;
        while (temp >= 0)
        {
            if (papan.isi[temp][j] == simbol) counter++;
            else counter = 0;
            temp--;
            j--;
        }
        if (counter >= cek) return true;
    }

    // Diagonal [\] dari (0,0) sampai (papan.ukuran-1, 0)
    for (i = 0; i < papan.ukuran; i++)
    {
        counter = 0;
        temp = i;
        j = 0;
        while (temp < papan.ukuran)
        {
            if (papan.isi[temp][j] == simbol) counter++;
            else counter = 0;
            temp++;
            j++;
        }
        if (counter >= cek) return true;
    }

    return false;
}

void* timerThread()
{
    while (papan.waktu > -1)
    {
        if (pause) continue;
        sleep(1);
        papan.waktu--;
    }
    pthread_exit(NULL);
}

void* permainanThread()
{
    char input[5] = {'\000'};
    do
    {
        if (papan.waktu == -2) break;
        system("cls");
        printf("Permainan %d x %d (%s vs %s)", papan.ukuran, papan.ukuran, papan.pemain[0].nama, papan.pemain[1].nama);
        printf("\n============================================");
        printf("\n");
        if (pause)
        {
            if (papan.waktu >= 0) printf("\nWaktu tersisa: %d detik", papan.waktu);
            else printf("\nWaktu habis. Posisi akan diisi secara acak");
            printf("\nTekan Enter untuk keluar atau 'Esc' untuk melanjutkan permainan\n");
            while (true)
            {
                fflush(stdin);
                char pilihan = getch();
                if ((int)pilihan == KEY_ENTER)
                {
                    pthread_cancel(timerId);
                    papan.waktu = -2;
                    pause = false;
                    break;
                }
                if ((int)pilihan == KEY_ESCAPE)
                {
                    pause = false;
                    break;
                }
            }
            continue;
        }
        else
        {
            tampilPapan(true);
            if (!papan.pemain[1].isKomputer) printf("Skor: %d", papan.pemain[papan.giliran].skor);
            menuOpsi("Q: Ke menu utama");
            printf("\n[Giliran %s]", papan.pemain[papan.giliran].nama);
            pesanInvalid("Posisi petak tidak valid");
            if (papan.waktu >= 0) printf("\nMasukkan nilai petak dalam waktu %d detik: ", papan.waktu);
            else
            {
                printf("\nWaktu habis");
                printf("\nMasukkan nilai apapun untuk melanjutkan: ");
            }
            fgets(input, 4, stdin);
            int newLine = strlen(input) - 1;
            if (input[newLine] == '\n') input[newLine] = '\000';
            upperCase(input);
            
            int posPapan = atoi(input);
            if (strcmp(input, "Q") == 0)
            {
                pause = true;
            }
            else if (posPapan > 0 && posPapan <= papan.ukuran * papan.ukuran)
            {
                if (papan.waktu > -1)
                {
                    if (getIsiPapan(posPapan) != 'X' && getIsiPapan(posPapan) != 'O') setIsiPapan(posPapan, papan.pemain[papan.giliran].simbol);
                    else
                    {
                        validInput = false;
                        continue;
                    }
                }
                else
                {
                    int posAcak = 0;
                }
                pthread_cancel(timerId);
                papan.waktu = -3;
                pause = false;
                break;
            }
            else
            {
                validInput = false;
                continue;
            }
            
        }
    } while (true);
    pthread_exit(NULL);
}

void mulaiPermainan()
{
    memset(papan.isi, '\000', sizeof(papan.isi));
    papan.pemain[0].simbol = 'X';
    papan.pemain[1].simbol = 'O';
    papan.isBermain = true;
    papan.giliran = 1;
    pause = false;
    do
    {
        papan.giliran = (papan.giliran == 1)? 0 : 1;
        papan.waktu = 10;
        pthread_create(&permainanId, NULL, permainanThread, NULL);
        pthread_create(&timerId, NULL, timerThread, NULL);
        pthread_join(permainanId, NULL);
        pthread_join(timerId, NULL);
        if (papan.waktu == -2)
        {
            papan.waktu = 0;
            return bagianPermainan();
        }
        else if (papan.waktu == -3) continue;
    }
    while (true);
}

void bagianPermainan()
{
    char input = '\000';
    menuPanel("Mulai Permainan", "Pilihlah jenis permainan yang akan anda mainkan");
    menuOpsi("1. Permainan Baru\n2. Lanjutkan Permainan Sebelumnya\nQ: Kembali");
    pesanInvalid("Pilihan anda tidak valid");
    menuPilihan("Masukkan Pilihan Anda");
    pilihan(&input);
    //if (input == 'Q')
    int pilihanInt = atoi(&input);
    switch (pilihanInt)
    {
    case 1:
        // permainan baru
        break;
    case 2:
        // permainan lama
        aturPermainanLama();
        break;
    default:
        validInput = false;
        bagianPermainan();
        break;
    }
    aturPermainan();
    mulaiPermainan();
}

void bagianCaraMain()
{
    printf("Test");
}

void bagianKeluar()
{
    char input = '\000';
    menuPanel("Keluar", "Apakah anda yakin ingin keluar dari permainan?");
    menuOpsi("Tekan 'Enter' untuk melanjutkan atau 'Esc' untuk kembali\n");
    pilihanYaTidak(keluarProgram, biarkan); // biarkan ---> menu utama
}

void initColor()
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    consoleAttr = consoleInfo.wAttributes;
}

/* MODUL UTAMA */
int main(int argc, char* argv[])
{
    initColor();
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    memset(&papan, '\000', sizeof(papan));
    memcpy(papan.pemain[0].nama, "Zan", 4);
    memcpy(papan.pemain[1].nama, "Z4x", 4);
    papan.ukuran = 7;
    // mulaiPermainan();
    setIsiPapan(16, 'O');
    setIsiPapan(24, 'O');
    setIsiPapan(32, 'O');
    setIsiPapan(40, 'O');
    setIsiPapan(48, 'O');
    tampilPapan(true);
    char* res = cekDiagonal('O') ? "ok" : "no";
    printf("%s", res);
	return 0;
}