#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>
#include <math.h>

#define KEY_ENTER 13
#define KEY_ESCAPE 27

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

Papan papan;
bool validInput = true;

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
        if (kbhit)
        {
            fflush(stdin);
            char input = getch();
            if ((int)input == KEY_ENTER) return (*pilih_ya)();
            else if ((int) input == KEY_ESCAPE) return (*pilih_tidak)();
        }
    }
}

void aturPermainan()
{
    char input = '\000';
    menuPanel("Mulai Permainan", "Tekan Enter untuk memulai permainan atau 'Esc' untuk kembali\n");
    pilihanYaTidak(biarkan, bagianPermainan);
}

void printall(char *data, int len)
{
    int i = 0;
    for (i = 0; i < len; i++)
    {
        printf("\n%d", *(data + i));
    }
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

int simpanPermainan(int posisi)
{
    char fileData[255] = {'\000'};
    char fileLokasi[100] = "D:/Arsip Kuliah/Coding/TicTacToe-Game/debug/game/save";
    memcpy(fileData, fileLokasi, strlen(fileLokasi));
    itoa(posisi, fileData + strlen(fileData), 10);
    memcpy(fileData + strlen(fileData), ".dat", 4);
    
    FILE* pFile = fopen(fileData, "wb");
    if (pFile == NULL) return 1;
    int papanSize = sizeof(papan);
    char dataPapan[696] = {'\000'};
    memset(dataPapan, 0, papanSize);
    memcpy(dataPapan, &papan, papanSize - 1);
    dataReverse(dataPapan, papanSize);
    if (fwrite(dataPapan, papanSize, 1, pFile) != 1) return 1;
    fclose(pFile);
    free(dataPapan);
    return 0;
}

int imporPermainan(int posisi, Papan* pPapan)
{
    char fileData[255] = {'\000'};
    char fileLokasi[100] = "D:/Arsip Kuliah/Coding/TicTacToe-Game/debug/game/save";
    memcpy(fileData, fileLokasi, strlen(fileLokasi));
    itoa(posisi, fileData + strlen(fileData), 10);
    memcpy(fileData + strlen(fileData), ".dat", 4);
    
    FILE* pFile = fopen(fileData, "rb");
    if (pFile == NULL) return 1;
    int pPapanSize = sizeof(*pPapan);
    char* dataPapan = malloc(pPapanSize);
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
    int inputInteger = 0;
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

void tampilPapan(int ukuran, bool tampilPosisi)
{
    int isi = 1;
    int maxIsi = ukuran * ukuran;
    int maxVertikal = ukuran * 3; // --- dan | | = 5 char untuk 1 kotak
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
            // if (i == 0) cTulisAwal = 'x';
            // else if (i == maxVertikal - 1) cTulisAwal = 'x';
            // else cTulisAwal = 'x';
        }
        for (j = 0; j < ukuran; j++)
        {
            int maxLenIsi = isiLen + 3; // 2 = space kanan kiri
            if (j == 0) maxLenIsi++;
            char cTulis = ' ';
            char cIsi = '_';
            if (j == ukuran - 1) cTulis = cTulisAwal;
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
                char* isiChar = malloc(isiLen + 1);
                if (tampilPosisi)
                {
                    int pos = 1;
                    if (j == 0) pos++;
                    itoa(isi, isiChar, 10);
                    memcpy(cGaris + pos, isiChar, strlen(isiChar));
                }
                
                free(isiChar);
                isi++;
            }
            printf("%s", cGaris);
            free(cGaris);
        }
        printf("\n");
    }
}

void mulaiPermainan()
{
    printf("%s", papan.nama);
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

int main(int argc, char* argv[])
{
    
	return 0;
}