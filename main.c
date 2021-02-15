/*********************************************************************
 * Tic-Tac-Toe Game  
 *********************************************************************
 * NAMA FILE    : main.c
 * AUTHOR       : Ihsan Fauzan Hanif & Syelvie Ira Ratna Mulia
 * COMPILER     : MinGW GCC
 * DESKRIPSI    : Aplikasi konsol permainan Tic-Tac-Toe
 * 2021
 ********************************************************************/

/**
 * Header File
 */
#include <conio.h>
#include <ctype.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <windows.h>

/**
 * Makro
 */
#define DEBUG
#define GAME_TIMER 10
#define KEYBOARD_ENTER 13
#define KEYBOARD_ESC 27
#define SIMBOL_P1 X
#define SIMBOL_P2 O

#ifdef DEBUG
    #define PATH_GAME "D:/Arsip Kuliah/Coding/TicTacToe-Game/debug/game/save"
    #define PATH_CARA "D:/Arsip Kuliah/Coding/TicTacToe-Game/debug/CaraBermain.txt"
#else
    #define PATH_GAME "/game/save"
    #define PATH_CARA "caraBermain.txt"
#endif

/**
 * Enumerasi
 */
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

/**
 * Struktur
 */
typedef struct
{
	bool isKomputer;
	char nama[50];
	char simbol;
	int input;
	long long int skor;
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

/**
 * Variable Global
 */
HANDLE hConsole;
WORD consoleAttr;
pthread_t thTimer, thGame;
bool inputValid = true;
bool jeda = false;
Papan papan;

/**
 * Prototipe Modul
 */
void bagianCara();
void bagianMain();
void bagianKeluar();
void bagianSkor();
bool cekDiagonal(char);
bool cekHorizontal(char);
bool cekVertikal(char);
bool cekPapan(char);
void clearConsole();
int eval();
char* getKesulitanStr(int);
void getInput(char*);
int getInputKomputer();
char getIsi(int);
int getTotalKosong();
int imporPermainan(int, Papan*);
bool isBisaDiisi();
int isiAcak();
int isiKritis();
void initComponent();
void jedaPermainan();
void menuGameOver(int);
void menuKalah(int);
void menuMenang(int);
void menuMultipemain(int);
void menuSeri(int);
void menuSimpan();
void menuSimpanSiap(int);
void menuSimpanSelesai();
void menuUtama();
int minimax(bool);
void mulaiPermainan();
int panjangAngka();
int parsePosisi();
void pesanInvalid(char*);
void reverse(char*, int);
void saveSkor();
void setKomputer();
void setMultipemain();
void setModePermainan();
void setPermainan();
void setPermainanLama();
void setIsi(int, char);
void setUkuran();
int simpanPermainan(int);
void showPermainanLama();
void showSkor();
void showPapan(bool);
void *threadPermainan();
void *threadTimer();
void upperCase(char*);

/**
 * Entry Point
 */
int main(int argc, char *argv[])
{
    initComponent();
	menuUtama();
	return 0;
}

/**
 * Implementasi Modul
 */
void initComponent()
{
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    memset(&papan, 0, sizeof(papan));
    inputValid = true;
    jeda = false;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    consoleAttr = consoleInfo.wAttributes;
}

void clearConsole()
{
    system("cls");
}

void getInput(char *input)
{
    char temp = '\000';
    fflush(stdin);
    scanf(" %c", &temp);
    *input = toupper(temp);
}

void pesanInvalid(char *pesan)
{
    if (!inputValid)
    {
        printf("%s", pesan);
        inputValid = true;
    }
}

char* getKesulitanStr(int kesulitan)
{
    switch (kesulitan)
    {
    case 1: return "mudah";
    case 2: return "sedang";
    case 3: return "sulit";
    default: return "";
    }
}

void menuUtama()
{
    char input = '\000';
    clearConsole();
    printf("======================================================");
    printf("\nMenu Utama");
    printf("\n======================================================");
    printf("\n");
	printf("\nBerikut merupakan pilihan yang dapat anda pilih.");
    printf("\n(Pilihlah opsi berikut dalam bentuk angka)");
	printf("\n[1.] Mulai Permainan");
	printf("\n[2.] Daftar 10 Pemain dengan Skor Tertinggi");
	printf("\n[3.] Cara Bermain");
	printf("\n[4.] Keluar Permainan");
    printf("\n");
	pesanInvalid("\nPilihan anda tidak valid");
	printf("\nMasukan pilihan: ");
    getInput(&input);
    
    int opsi = atoi(&input);
    switch (opsi)
    {
    case 1:
        bagianMain();
        break;
    case 2:
        //bagianSkor();
        break;
    case 3:
        bagianCara();
        break;
    case 4:
        bagianKeluar();
        break;
    default:
		inputValid = false;
		menuUtama();
		break;
    }
}

void bagianMain()
{
    char input = '\000';
    clearConsole();
    printf("======================================================");
    printf("\nMulai Permainan");
    printf("\n======================================================");
    printf("\n");
    printf("\nPilihlah jenis permainan yang akan anda mainkan");
    printf("\n[1.] Permainan Baru");
    printf("\n[2.] Lanjutkan Permainan Sebelumnya");
    printf("\nQ: Kembali");
    printf("\n");
	pesanInvalid("\nPilihan anda tidak valid");
    printf("\nMasukkan pilihan: ");
    getInput(&input);
    if (input == 'Q') return menuUtama();

    int opsi = atoi(&input);
    switch (opsi)
    {
    case 1: // permainan baru
        setUkuran();
        setModePermainan();
        if (!papan.pemain[0].isKomputer && papan.pemain[1].isKomputer) setKomputer();
        else setMultipemain();
        break;
    case 2: // permainan lama
        setPermainanLama();
        break;
    default:
        inputValid = false;
        bagianMain();
        break;
    }
    setPermainan();
}

void setUkuran()
{
    char input = '\000';
    clearConsole();
    printf("======================================================");
    printf("\nPermainan Baru");
    printf("\n======================================================");
    printf("\n");
    printf("\nPilihlah ukuran dari papan permainan");
    printf("\n[1.] 3 x 3");
    printf("\n[2.] 5 x 5");
    printf("\n[3.] 7 x 7");
    printf("\nQ: Kembali");
    printf("\n");
    pesanInvalid("\nPilihan anda tidak valid");
    printf("\nMasukkan pilihan: ");
    getInput(&input);
    if (input == 'Q') return bagianMain();
    
    int pilihanInt = atoi(&input);
    switch (pilihanInt)
    {
    case 1: // 3 x 3
        papan.ukuran = 3;
        break;
    case 2: // 5 x 5
        papan.ukuran = 5;
        break;
    case 3: // 7 x 7
        papan.ukuran = 7;
        break;
    default:
        inputValid = false;
        setUkuran();
        break;
    }
}

void setModePermainan()
{
    char input = '\000';
    clearConsole();
    printf("======================================================");
    printf("\nPermainan Baru");
    printf("\n======================================================");
    printf("\n");
    printf("\nPilihlah mode permainan yang akan dimainkan");
    printf("\n[1.] Komputer");
    printf("\n[2.] Multipemain");
    printf("\nQ: Kembali");
    printf("\n");
    pesanInvalid("\nPilihan anda tidak valid");
    printf("\nMasukkan pilihan: ");
    getInput(&input);
    if (input == 'Q') return bagianMain();

    int pilihanInt = atoi(&input);
    switch (pilihanInt)
    {
    case 1: // komputer
        papan.pemain[0].isKomputer = false;
        papan.pemain[1].isKomputer = true;
        break;
    case 2: // multipemain
        papan.pemain[0].isKomputer = false;
        papan.pemain[1].isKomputer = false;
        break;
    default:
        inputValid = false;
        setModePermainan();
        break;
    }
}

void setKomputer()
{
    char input = '\000';
    clearConsole();
    printf("======================================================");
    printf("\nPermainan Baru");
    printf("\n======================================================");
    printf("\n");
	printf("\nPilihlah tingkat kesulitan");
	printf("\n[1.] Mudah");
	printf("\n[2.] Sedang");
	printf("\n[3.] Sulit");
    printf("\nQ: Kembali");
    printf("\n");
	pesanInvalid("\nPilihan anda tidak valid");
	printf("\nMasukan pilihan: ");
    getInput(&input);
    if (input == 'Q') return bagianMain();
    memset(papan.pemain[0].nama, '\000', sizeof(papan.pemain[0].nama));
	memset(papan.pemain[1].nama, '\000', sizeof(papan.pemain[1].nama));
	memcpy(papan.pemain[0].nama, "Anda", 4);
	memcpy(papan.pemain[1].nama, "Komputer", 9);

    int pilihanInt = atoi(&input);
    switch (pilihanInt)
	{
	case 1: // mudah
        papan.kesulitan = 1;
		break;
	case 2: // sedang
        papan.kesulitan = 2;
		break;
	case 3: // sulit
        papan.kesulitan = 3;
		break;
	default:
		inputValid = false;
		setKomputer();
		break;
	}
}

void setMultipemain()
{
    char input = '\000';
    char input1[25] = {'\000'};
    char input2[25] = {'\000'};
    clearConsole();
    printf("======================================================");
    printf("\nPermainan Baru");
    printf("\n======================================================");
    printf("\n");
	printf("\nHarap masukkan nama pemain");
	printf("\nNama pemain pertama: ");
    fflush(stdin);
	scanf("%[^\n]s", input1);
	printf("Nama pemain kedua: ");
    fflush(stdin);
	scanf("%[^\n]s", input2);
    memset(papan.pemain[0].nama, '\000', sizeof(papan.pemain[0].nama));
	memset(papan.pemain[1].nama, '\000', sizeof(papan.pemain[1].nama));
    memcpy(papan.pemain[0].nama, input1, strlen(input1));
    memcpy(papan.pemain[1].nama, input2, strlen(input2));

    showSet:
    clearConsole();
    printf("======================================================");
    printf("\nPermainan Baru");
    printf("\n======================================================");
    printf("\n");
    printf("\nPastikan bahwa data ini sesuai");
    printf("\nNama pemain pertama : %s", papan.pemain[0].nama);
	printf("\nNama pemain kedua   : %s", papan.pemain[1].nama);
    printf("\nQ: Kembali | S: Sesuai");
    printf("\n");
    pesanInvalid("\nPilihan anda tidak valid");
    printf("\nMasukkan pilihan: ");
    getInput(&input);
    switch (input)
	{
	case 'Q': // kembali
		setMultipemain();
		break;
	case 'S': // mulai permainan
		break;
	default:
		inputValid = false;
		goto showSet;
		break;
	}
}

void setPermainanLama()
{
    char input = '\000';
    clearConsole();
    printf("======================================================");
    printf("\nPermainan Lama");
    printf("\n======================================================");
    printf("\n");
    printf("\nPilihlah permainan yang ingin anda lanjutkan");
    showPermainanLama();
    printf("\n");
    printf("\nQ: Kembali");
    pesanInvalid("\nPilihan data permainan tidak valid");
    printf("\nMasukkan pilihan: ");
    getInput(&input);
    if (input == 'Q') return bagianMain();
    
    int pilihanInt = atoi(&input);
    if (pilihanInt > 0 && pilihanInt <= 5)
    {
        if (imporPermainan(pilihanInt, &papan) == 0) return;
    }
    inputValid = false;
    return setPermainanLama();
}

void showPermainanLama()
{
    int i = 0;
    for (i = 0; i < 5; i++)
    {
        Papan temp;
        printf("\n%d. ", i + 1);
        if (imporPermainan(i + 1, &temp) == 0) printf("%s", temp.nama);
        else printf("[ Kosong ]");
    }
}

int imporPermainan(int pos, Papan* pPapan)
{
    int pPapanSize = sizeof(*pPapan);
    char fileData[255] = {'\000'};
    char fileLokasi[255] = PATH_GAME;
    char* dataPapan = malloc(pPapanSize);
    memset(dataPapan, 0, pPapanSize);
    memset(pPapan, 0, pPapanSize);
    memcpy(fileData, fileLokasi, strlen(fileLokasi));
    itoa(pos, fileData + strlen(fileData), 10);
    memcpy(fileData + strlen(fileData), ".dat", 4);
    
    FILE* pFile = fopen(fileData, "rb");
    if (pFile == NULL) return 1;
    if (fread(dataPapan, pPapanSize, 1, pFile) != 1) return 1;
    reverse(dataPapan, pPapanSize);
    memcpy(pPapan, dataPapan, pPapanSize - 1);
    free(dataPapan);
    fclose(pFile);
    return 0;
}

int simpanPermainan(int pos)
{
    int papanSize = sizeof(papan);
    char fileData[255] = {'\000'};
    char fileLokasi[255] = PATH_GAME;
    char *dataPapan = malloc(papanSize);
    memcpy(fileData, fileLokasi, strlen(fileLokasi));
    itoa(pos, fileData + strlen(fileData), 10);
    memcpy(fileData + strlen(fileData), ".dat", 4);
    
    FILE* pFile = fopen(fileData, "wb");
    if (pFile == NULL) return 1;
    memset(dataPapan, 0, papanSize);
    memcpy(dataPapan, &papan, papanSize - 1);
    reverse(dataPapan, papanSize);
    if (fwrite(dataPapan, papanSize, 1, pFile) != 1) return 1;
    fclose(pFile);
    free(dataPapan);
    return 0;
}

void reverse(char* data, int length)
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

void setPermainan()
{
    char input = '\000';
    char kesulitan[10] = {'\000'};
    memcpy(kesulitan, getKesulitanStr(papan.kesulitan), strlen(getKesulitanStr(papan.kesulitan)));
    clearConsole();
    printf("======================================================");
    printf("\nMulai Permainan");
    printf("\n======================================================");
    printf("\n");
    printf("\nBerikut merupakan data permainan yang akan dimulai.");
    printf("\nPemain 1          : %s", papan.pemain[0].nama);
    printf("\nPemain 2          : %s", papan.pemain[1].nama);
    printf("\nUkuran Papan      : %d x %d", papan.ukuran, papan.ukuran);
    if (papan.pemain[1].isKomputer)
    {
        printf("\nTingkat Kesulitan : %s", kesulitan);
        printf("\nSkor Saat ini     : %d", papan.pemain[0].skor);
    }
    else printf("\n[Mode Multipemain]");
    printf("\n");
    printf("\nTekan Enter untuk memulai permainan atau 'Esc' untuk ke menu utama");

    while (true)
    {
        fflush(stdin);
        char input = getch();
        if ((int)input == KEYBOARD_ENTER) return mulaiPermainan();
        else if ((int)input == KEYBOARD_ESC) return bagianMain();
    }
}

void mulaiPermainan()
{
    bool sesuai = true;
    papan.giliran = 1;
    papan.isBermain = true;
    papan.pemain[0].simbol = 'X';
    papan.pemain[1].simbol = 'O';
    memset(papan.isi, '\000', sizeof(papan.isi));
    do
    {
        jeda = false;
        papan.waktu = GAME_TIMER;
        papan.giliran = (papan.giliran == 1)? 0 : 1;
        pthread_create(&thGame, NULL, threadPermainan, NULL);
        pthread_create(&thTimer, NULL, threadTimer, NULL);
        pthread_join(thGame, NULL);
        pthread_join(thTimer, NULL);

        int pos = 0;
        char simbol = papan.pemain[papan.giliran].simbol;
        switch (papan.waktu)
        {
        case TERLAMBAT:
            pos = isiAcak();
            break;
        case KELUAR:
            papan.waktu = 0;
            return bagianMain();
            break;
        case SELESAI:
            pos = papan.pemain[papan.giliran].input;
            break;
        }
        setIsi(pos, simbol);
        sesuai = cekPapan(simbol);
    }
    while (sesuai);
    
    int status;
    if (papan.giliran == 2) status = 2;
    else if (!cekPapan('X')) status = 0;
    else status = 1;
    menuGameOver(status);
}

void* threadTimer()
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

void* threadPermainan()
{
    char input[5] = {'\000'};
    do
    {
        if (papan.waktu == -2) break;
        clearConsole();
        printf("======================================================");
        printf("\nPermainan %d x %d (%s vs %s)", papan.ukuran, papan.ukuran, papan.pemain[0].nama, papan.pemain[1].nama);
        printf("\n======================================================");
        printf("\n");
        if (jeda) jedaPermainan();
        else
        {
            int posPapan = 0;
            showPapan(true);
            if (papan.pemain[1].isKomputer) printf("\nSkor: %d", papan.pemain[0].skor);
            printf("\nQ: Ke menu utama");
            printf("\n");
            printf("\n[Giliran %s]", papan.pemain[papan.giliran].nama);
            pesanInvalid("\nPosisi petak tidak valid");
            if (papan.pemain[papan.giliran].isKomputer)
            {
                printf("\nMenunggu giliran %s ...", papan.pemain[papan.giliran].nama);
                posPapan = getInputKomputer();
                papan.waktu = SELESAI;
                sleep(1);
                setIsi(posPapan, papan.pemain[papan.giliran].simbol);
                pthread_cancel(thTimer);
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
                    pthread_cancel(thTimer);
                    papan.pemain[papan.giliran].input = posPapan;
                    if (papan.waktu > -1) papan.waktu = SELESAI;
                    break;
                }
                else inputValid = false;
            }
        }
    } while (true);
    return NULL;
}

void jedaPermainan()
{
    clearConsole();
    printf("======================================================");
    printf("\nPermainan %d x %d (%s vs %s)", papan.ukuran, papan.ukuran, papan.pemain[0].nama, papan.pemain[1].nama);
    printf("\n======================================================");
    printf("\n");
    printf("\nKeluar dari permainan ini?");
    if (papan.waktu >= 0) printf("\nWaktu tersisa: %d detik", papan.waktu);
    else printf("\nWaktu habis. Posisi akan diisi secara acak");
    printf("\nTekan Enter untuk keluar atau 'Esc' untuk melanjutkan permainan\n");
    while (true)
    {
        fflush(stdin);

        char input = getch();
        if ((int)input == KEYBOARD_ENTER)
        {
            pthread_cancel(thTimer);
            papan.waktu = KELUAR;
            jeda = false;
            break;
        }
        if ((int)input == KEYBOARD_ESC)
        {
            jeda = false;
            break;
        }
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

bool cekPapan(char simbol)
{
    if (cekDiagonal(simbol) || cekHorizontal(simbol) || cekVertikal(simbol)) return false;
    if (getTotalKosong() == 0) 
    {
        papan.giliran = 2;
        return false;
    }
    else return true;
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
	int cek = cekMinimal(papan.ukuran);
	int i, j, counter = 0;
	
	for (i = 0; i<papan.ukuran; i++)
    {
		counter = 0;
        for (j = 0; j<papan.ukuran; j++)
        {
            if (papan.isi[i][j] == simbol) counter++;
            else counter = 0;		
		}
        if (counter >= cek) return true;
    }
	return false;
}

bool cekVertikal(char simbol)
{
	int cek = cekMinimal(papan.ukuran);
	int i, j, counter = 0;
	
	for (i = 0; i<papan.ukuran; i++)
    {
		counter = 0;
        for (j = 0; j<papan.ukuran; j++)
        {
            if (papan.isi[j][i] == simbol) counter++;
            else counter = 0;
        }
        if(counter >= cek) return true;
    }
	return false;
}

void showPapan(bool tampilPosisi)
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

void upperCase(char *data)
{
    int i = 0;
    for (i = 0; i < strlen(data); i++)
    {
        *(data + i) = toupper(*(data + i));
    }
}

int getInputKomputer()
{
    int input = 0;
    switch (papan.kesulitan)
    {
    case 1: // mudah
        input = isiAcak();
        break;
    case 2: // sedang
        if (getTotalKosong() % papan.ukuran == 0) input = isiKritis();
        else input = isiAcak();
        break;
    case 3: // sulit
        input = isiKritis();
        break;
    }
    return input;
}

int isiAcak()
{
    srand(time(NULL));
    int max = papan.ukuran * papan.ukuran;
    int min = 1;
    int pos = 0;
    do { pos = (rand() % (max - min)) + min; }
    while (getIsi(pos) != '\000');
    return pos;
}

int isiKritis()
{
    if (getTotalKosong() == papan.ukuran * papan.ukuran - 1) return isiAcak();
    
    int skorX = -10000;
    int posX = -1;
    int posY = -1;
    int i, j;
    for (int i = 0; i < papan.ukuran; i++)
    {
        for (int j = 0; j < papan.ukuran; j++)
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

int minimax(bool isMax)
{
    int giliranLawan = (papan.giliran == 1)? 0 : 1;
    int skor = eval();
    if (skor == 10 || skor == -10) return skor;
    if (!isBisaDiisi()) return 0;
    if (isMax)
    {
        int skorX = -10000;
        int i, j;
        for (i = 0; i < papan.ukuran; i++)
        {
            for (j = 0; j < papan.ukuran; j++)
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
        int skorX = 10000;
        int i, j;
        for (i = 0; i < papan.ukuran; i++)
        {
            for (j = 0; j < papan.ukuran; j++)
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

void menuMenang(int status)
{
    char input = '\000';
    papan.pemain[0].skor += 100;
    printf("\n%s telah memenangkan permainan", papan.pemain[0].nama);
    printf("\nSkor Baru: %d (+100)", papan.pemain[0].skor);
    printf("\nQ: Ke menu utama | R: Lanjutkan Permainan | S: Simpan Permainan");
    printf("\n");
    pesanInvalid("\nPilihan anda tidak valid");
    printf("\nMasukkan pilihan: ");
    getInput(&input);
    switch (input)
    {
    case 'Q':
        return menuUtama();
        break;
    case 'R':
        return setPermainan();
        break;
    case 'S':
        return menuSimpan();
        break;
    default:
        inputValid = false;
        menuGameOver(status);
        break;
    }
    menuMenang(status);
}

void menuKalah(int status)
{
    char input = '\000';
    printf("\n%s kalah dalam permainan", papan.pemain[0].nama);
    printf("\nSkor: %d", papan.pemain[0].skor);
    printf("\nQ: Ke menu utama | R: Ulangi permainan (skor akan hilang)");
    printf("\n");
    pesanInvalid("\nPilihan anda tidak valid");
    printf("\nMasukkan pilihan: ");
    getInput(&input);
    switch (input)
    {
    case 'Q':
        return menuUtama();
        break;
    case 'R':
        papan.pemain[0].skor = 0;
        return setPermainan();
        break;
    default:
        inputValid = false;
        menuGameOver(status);
        break;
    }
}

void menuSeri(int status)
{
    char input = '\000';
    printf("\nPermainan seri", papan.pemain[0].nama);
    printf("\nSkor: %d", papan.pemain[0].skor);
    printf("\nQ: Ke menu utama | R: Ulangi Permainan");
    printf("\n");
    pesanInvalid("\nPilihan anda tidak valid");
    printf("\nMasukkan pilihan: ");
    getInput(&input);
    switch (input)
    {
    case 'Q':
        return menuUtama();
        break;
    case 'R':
        return setPermainan();
        break;
    default:
        inputValid = false;
        menuGameOver(status);
        break;
    }
}

void menuSimpan()
{
    char input = '\000';
    clearConsole();
    printf("======================================================");
    printf("\nSimpan Permainan");
    printf("\n======================================================");
    printf("\n");
    printf("\nPilihlah lokasi di mana anda akan menyimpan permainan");
    showPermainanLama();
    printf("\nQ: Batalkan");
    printf("\n");
    pesanInvalid("\nPilihan anda tidak valid");
    printf("\nMasukkan pilihan: ");
    getInput(&input);
    if (input == 'Q') menuGameOver(1);
    
    int opsi = atoi(&input);
    if (opsi > 0 && opsi < 4) menuSimpanSiap(opsi);
    else
    {
        inputValid = false;
        return menuSimpan();
    }
}

void menuSimpanSiap(int posisi)
{
    Papan temp;
    char input[25] = {'\000'};
    char kesulitan[10] = {'\000'};
    clearConsole();
    memcpy(kesulitan, getKesulitanStr(papan.kesulitan), strlen(getKesulitanStr(papan.kesulitan)));
    printf("======================================================");
    printf("\nSimpan Permainan");
    printf("\n======================================================");
    printf("\n");
    printf("\nBerikut merupakan data permainan yang akan anda simpan");
    printf("\nTingkat Kesulitan : %s", kesulitan);
    printf("\nUkuran Papan      : %d x %d", papan.ukuran, papan.ukuran);
    printf("\nQ: Kembali");
    if (imporPermainan(posisi, &temp) == 0) printf("\n[Peringatan! Anda akan menimpa data permainan ini]");
    printf("\n");
    printf("\nMasukkan label penyimpanan: ");
    fflush(stdin);
    scanf("%[^\n]s", input);
    memset(papan.nama, '\000', sizeof(papan.nama));
    memcpy(papan.nama, input, strlen(input));

    int status = simpanPermainan(posisi);
    if (status == 0) return menuSimpanSelesai();
}

void menuSimpanSelesai()
{
    char input = '\000';
    char kesulitan[10] = {'\000'};
    clearConsole();
    memcpy(kesulitan, getKesulitanStr(papan.kesulitan), strlen(getKesulitanStr(papan.kesulitan)));
    printf("======================================================");
    printf("\nSimpan Permainan");
    printf("\n======================================================");
    printf("\n");
    printf("\nBerikut merupakan data permainan yang telah disimpan");
    printf("\nLabel Papan       : %s", papan.nama);
    printf("\nTingkat Kesulitan : %s", kesulitan);
    printf("\nPermainan berhasil disimpan");
    printf("\nQ: Ke menu utama | R: Lanjutkan Permainan");
    printf("\n");
    pesanInvalid("\nPilihan anda tidak valid");
    printf("\nMasukkan pilihan: ");
    getInput(&input);
    switch (input)
    {
    case 'Q':
        return menuUtama();
        break;
    case 'R':
        return setPermainan();
    default:
        inputValid = false;
        menuSimpanSelesai();
        break;
    }
}

void menuMultipemain(int status)
{
    char input = '\000';
    if (status == 1 || status == 0)
    {
        int pemenang = status;
        int kalah = (pemenang == 0)? 1 : 0;
        printf("\n%s telah mengalahkan %s", papan.pemain[pemenang].nama, papan.pemain[kalah].nama);
    }
    else printf("\nPermainan seri", papan.pemain[0].nama);
    printf("\nQ: Ke menu utama | R: Ulangi Permainan");
    printf("\n");
    pesanInvalid("Pilihan anda tidak valid");
    printf("\nMasukkan pilihan: ");
    getInput(&input);
    switch (input)
    {
    case 'Q':
        return menuUtama();
        break;
    case 'R':
        return setPermainan();
        break;
    case 'S':
        return menuSimpan();
        break;
    default:
        inputValid = false;
        menuGameOver(status);
        break;
    }
}

void menuGameOver(int status)
{
    clearConsole();
    printf("======================================================");
    printf("\nPermainan %d x %d (%s vs %s)", papan.ukuran, papan.ukuran, papan.pemain[0].nama, papan.pemain[1].nama);
    printf("\n======================================================");
    printf("\n");
    showPapan(false);
    if (papan.pemain[1].isKomputer)
    {
        switch (status)
        {
        case 0: // menang lawan komputer
            menuMenang(status);
            break;
        case 1: // kalah lawan komputer
            menuKalah(status);
        default: // seri
            menuSeri(status);
            break;
        }
    }
    else menuMultipemain(status);
}

void bagianCara()
{
    char input = '\000';
    char tampil[6000] = {'\000'};
    FILE *in;
    clearConsole();
	if((in=fopen(PATH_CARA, "r")) == NULL)
    {
        printf("Error Opening File"); //tutup program karena file ga ada
		fclose(in);
	}
    //baca isi file dengan gets lalu simpan ke tampil
	while(fgets(tampil, sizeof(tampil), in)) printf("%s", &tampil);
    fclose(in);
    printf("\n");
    printf("\nTekan tombol apapun untuk kembali ke Menu Utama");
    while (true)
    {
        fflush(stdin);

        char input = getch();
        if((int)input) return menuUtama();
    }
}

void bagianKeluar()
{
    char input = '\000';
    clearConsole();
    printf("======================================================");
    printf("\nKeluar Permainan");
    printf("\n======================================================");
    printf("\n");
    printf("\nApakah anda yakin ingin keluar dari permainan?");
    printf("\nTekan 'Enter' untuk melanjutkan atau 'Esc' untuk kembali");
    while (true)
    {
        fflush(stdin);

        char input = getch();
        if ((int)input == KEYBOARD_ENTER) exit(0);
        if ((int)input == KEYBOARD_ESC) return menuUtama();
    }
}

int eval()
{
    int giliran = papan.giliran;
    char cek = 'X';
    char simbolPemain = papan.pemain[papan.giliran].simbol;
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
