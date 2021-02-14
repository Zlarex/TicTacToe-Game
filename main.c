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
#define KEY_ESCAPE 27
#define GAME_TIMER 10
#define SIMBOL_P1 X
#define SIMBOL_P2 O
#define LOKASI_SIMPAN_PERMAINAN "D:/Arsip Kuliah/Coding/TicTacToe-Game/debug/game/save"

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

/* ENUMERASI */
typedef enum
{
    KALAH = -1,
    SERI = 0,
    MENANG = 1,
} Kondisi;

typedef enum
{
    TERLAMBAT = -1,
    KELUAR = -2,
    SELESAI = -3
} InputPapan;

/* VARIABEL GLOBAL */
HANDLE hConsole;
WORD consoleAttr;
pthread_t timerId, permainanId;
Papan papan;
bool validInput = true;
bool jeda = false;

/* DEKLARASI MODUL */
bool cekPapan(char);
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
    menuPanel("Mulai Permainan", "");
    printf("\nPengaturan permainan.");
    printf("\nPemain 1\t\t: %s", papan.pemain[0].nama);
    printf("\nPemain 2\t\t: %s", papan.pemain[1].nama);
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
    char fileLokasi[100] = LOKASI_SIMPAN_PERMAINAN;
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
    char fileLokasi[100] = LOKASI_SIMPAN_PERMAINAN;
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

int parsePosisi(int x, int y)
{
    int hasil = 0;
    for (int i = 0; i < papan.ukuran; i++)
    {
        for (int j = 0; j < papan.ukuran; j++)
        {
            if (i == x && j == y) return hasil + 1;
            hasil++;
        }
    }
    return 0;
}

char getIsi(int pos)
{
    pos--;
    return papan.isi[pos/papan.ukuran][pos%papan.ukuran];
}

void setIsi(int pos, char nilai)
{
    pos--;
    papan.isi[pos/papan.ukuran][pos%papan.ukuran] = nilai;
}

int getTotalKosong()
{
    int counter = 0;
    int i;
    for (i = 1; i < papan.ukuran * papan.ukuran + 1; i++)
    {
        if (getIsi(i) == '\000') counter++;
    }
    return counter;
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
                char isiPapan = getIsi(isi);
                int pos = 1;
                if (j == 0) pos++;
                memset(isiChar, '\000', allocSize);
                if (isiPapan != '\000') memcpy(isiChar, &isiPapan, 1);
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

bool cekHorizontal(char simbol)
{
    int i;
    for (i = 0; i < 3; i++)
    {
        if (papan.isi[i][0] == simbol && papan.isi[i][1] == simbol && papan.isi[i][2] == simbol) return true;
    }
    return false;
}

bool cekVertikal(char simbol)
{
    int i;
    for (i = 0; i < 3; i++)
    {
        if (papan.isi[0][i] == simbol && papan.isi[1][i] == simbol && papan.isi[2][i] == simbol) return true;
    }
    return false;
}

bool cekPapan(char simbol)
{
    if (cekDiagonal(simbol)) return false;
    else if (cekHorizontal(simbol)) return false;
    else if (cekVertikal(simbol)) return false;
    
    if (getTotalKosong() == 0) 
    {
        papan.giliran = 2;
        return false;
    }
    else return true;
}

void jedaPermainan()
{
    printf("\nApakah anda ingin keluar dari permainan?");
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
            papan.waktu = KELUAR;
            jeda = false;
            break;
        }
        if ((int)pilihan == KEY_ESCAPE)
        {
            jeda = false;
            break;
        }
    }
}

typedef struct
{
    int x;
    int y;
} AI;

bool isBisaDiisi()
{
    int i, j;
    for (i = 0; i < papan.ukuran; i++)
    {
        for (j = 0; j < papan.ukuran; j++)
        {
            if (papan.isi[i][j] == '\000') return true;
        }
    }
    return false;
}

int eval()
{
    char simbolPemain = papan.pemain[papan.giliran].simbol;
    int giliran = papan.giliran;

    char cek = 'X';
    bool status = cekPapan(cek);
    if (status)
    {
        cek = 'O';
        status = cekPapan(cek);
    }
    if (!status)
    {
        if (papan.giliran == 2)
        {
            papan.giliran = giliran;
            return 0;
        }
        if (cek == simbolPemain) return 10;
        else return -10;
    }
    return 0;
}

int minimax(bool isMax)
{
    int giliranLawan = (papan.giliran == 1)? 0 : 1;
    int skor = eval();
    if (skor == 10 || skor == -10) return skor;
    if (!isBisaDiisi()) return 0;

    if (isMax)
    {
        int skorX = -1000;
        int i, j;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
            {
                if (papan.isi[i][j]=='\000')
                {
                    papan.isi[i][j] = papan.pemain[papan.giliran].simbol;
                    skorX = max(skorX, minimax(!isMax));
                    papan.isi[i][j] = '\000';
                }
            }
        }
        return skorX;
    }
    else
    {
        int skorX = 1000;
        int i, j;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
            {
                if (papan.isi[i][j]=='\000')
                {
                    papan.isi[i][j] = papan.pemain[giliranLawan].simbol;
                    skorX = min(skorX, minimax(!isMax));
                    papan.isi[i][j] = '\000';
                }
            }
        }
        return skorX;
    }
}

int isiKritis()
{
    int skorX = -1000;
    int posX = -1;
    int posY = -1;

    int i, j;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (papan.isi[i][j] == '\000')
            {
                papan.isi[i][j] = papan.pemain[papan.giliran].simbol;
                int skor = minimax(false);
                papan.isi[i][j] = '\000';
                if (skor > skorX)
                {
                    posX = i;
                    posY = j;
                    skorX = skor;
                }
            }
        }
    }
    return parsePosisi(posX, posY);
}

int isiAcak()
{
    srand(time(NULL));
    int max = papan.ukuran * papan.ukuran;
    int min = 1;
    int pos = 0;
    do
    {
        pos = (rand() % (max - min)) + min;
    } while (getIsi(pos) != '\000');
    return pos;
}

int getInputKomputer()
{
    int input = 0;
    switch (papan.kesulitan)
    {
    case 1:
        // mudah
        input = isiAcak();
        break;
    case 2:
        // sedang
        if (getTotalKosong() % papan.ukuran == 0) input = isiKritis();
        else input = isiAcak();
        break;
    case 3:
        // sulit
        input = isiKritis();
        break;
    }
    return input;
}

void menuGameOver(int status)
{
    switch (status)
    {
    case 0:
        // pemain 1 menang
        break;
    case 1:
        // pemain 2 menang
        break;
    default:
        // seri
        break;
    }
}

void* timerThread()
{
    papan.waktu++;
    while (papan.waktu > -1)
    {
        if (jeda) continue;
        papan.waktu--;
        sleep(1);
    }
    return NULL;
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
        if (jeda) jedaPermainan();
        else
        {
            int posPapan = 0;
            tampilPapan(true);
            if (!papan.pemain[1].isKomputer) printf("Skor: %d", papan.pemain[papan.giliran].skor);
            menuOpsi("Q: Ke menu utama");
            printf("\n[Giliran %s]", papan.pemain[papan.giliran].nama);
            pesanInvalid("Posisi petak tidak valid");

            if (papan.pemain[papan.giliran].isKomputer)
            {
                printf("\nMenunggu giliran %s ...", papan.pemain[papan.giliran].nama);
                posPapan = getInputKomputer();
                papan.waktu = SELESAI;
                sleep(1);
                setIsi(posPapan, papan.pemain[papan.giliran].simbol);
                pthread_cancel(timerId);
                papan.waktu = SELESAI;
                break;
            }
            else
            {
                if (papan.waktu >= 0) printf("\nMasukkan nilai petak dalam waktu %d detik: ", papan.waktu);
                else printf("\nWaktu habis, masukkan posisi petak manapun untuk melanjutkan: ");
                fgets(input, 4, stdin);
                int newLine = strlen(input) - 1;
                
                if (input[newLine] == '\n') input[newLine] = '\000';
                upperCase(input);
                posPapan = atoi(input);

                if (strcmp(input, "Q") == 0)
                {
                    jeda = true;
                    continue;
                }
                else if (
                    (posPapan > 0 && posPapan <= papan.ukuran * papan.ukuran) &&
                    ((getIsi(posPapan) == '\000') || papan.waktu == TERLAMBAT)
                )
                {
                    pthread_cancel(timerId);
                    papan.pemain[papan.giliran].input = posPapan;
                    if (papan.waktu > -1) papan.waktu = SELESAI;
                    break;
                }
                else validInput = false;
            }
        }
    } while (true);
    return NULL;
}

void mulaiPermainan()
{
    memset(papan.isi, '\000', sizeof(papan.isi));
    papan.pemain[0].simbol = 'X';
    papan.pemain[1].simbol = 'O';
    papan.isBermain = true;
    papan.giliran = 1;
    bool sesuai = true;
    do
    {
        jeda = false;
        papan.waktu = GAME_TIMER;
        papan.giliran = (papan.giliran == 1)? 0 : 1;
        
        pthread_create(&permainanId, NULL, permainanThread, NULL);
        pthread_create(&timerId, NULL, timerThread, NULL);
        pthread_join(permainanId, NULL);
        pthread_join(timerId, NULL);
        
        int pos = 0;
        char simbol = papan.pemain[papan.giliran].simbol;
        switch (papan.waktu)
        {
        case TERLAMBAT:
            pos = isiAcak();
            break;
        case KELUAR:
            printf("keluar");
            papan.waktu = 0;
            return bagianPermainan();
            break;
        case SELESAI:
            pos = papan.pemain[papan.giliran].input;
            break;
        }
        setIsi(pos, simbol);
        sesuai = cekPapan(simbol);
    }
    while (sesuai);
    menuGameOver(papan.giliran);
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
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    consoleAttr = consoleInfo.wAttributes;
}

/* MODUL UTAMA */
int main(int argc, char* argv[])
{
    initColor();
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    memset(&papan, '\000', sizeof(papan));
    memcpy(papan.pemain[0].nama, "Anda", 4);
    memcpy(papan.pemain[1].nama, "Komputer", 8);
    papan.pemain[0].isKomputer = false;
    papan.pemain[1].isKomputer = true;
    papan.pemain[0].simbol = 'X';
    papan.pemain[1].simbol = 'O';
    papan.ukuran = 3;
    papan.kesulitan = 3;
    mulaiPermainan();
    // setIsi(48, 'O');
    // tampilPapan(true);
    // char* res = cekDiagonal('O') ? "ok" : "no";
    //printf("%s", res);
	return 0;
}