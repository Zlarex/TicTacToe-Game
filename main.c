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
}Pemain;

typedef struct 
{
	int ukuran;
	int waktu;
	int kesulitan;
	int giliran;
	bool isBermain;
	char isi[10][10];
	Pemain pemain[2];
}Papan;

Papan papan;
bool validInput = true;
void menuUtama();
void mulaiPermainan();
void modeMultiPemain();
void modeKomputer();
void bagianSkor();
void bagianCaraBermain();
void bagianKeluar();
void simpanSkor();
void bagianPermainan();
void pesanInvalid(char* pesan);
void menuPanel();
void menuOpsi();
void menuPilihan();
void pilihan();
void aturPermainan();
void pilihanYaTidak();
void tampilSkor();
void urutSkor();
void menuGameOver();
bool cekHorizontal();
bool cekVertikal();

void biarkan(){}

bool cekHorizontal(char simbol)
{
	int cek = cekMinimal(papan.ukuran);
	int i,j,count=0;
	
	for (i=0; i<papan.ukuran; i++) {
		count = 0;
            for (j=0; j<papan.ukuran; j++) {
            if (papan.isi[i][j] == simbol){
                count++;
			}else{
            	count = 0;
			}
				if(count >= cek) 
				return true;
			}
		}
	return false;
}

bool cekVertikal(char simbol)
{
	int cek = cekMinimal(papan.ukuran);
	int i,j,count=0;
	
	for (i=0; i<papan.ukuran; i++) {
		count = 0;
            for (j=0; j<papan.ukuran; j++) {
            if (papan.isi[j][i] == simbol){
                count++;
			}else{
            	count = 0;
			}
				if(count >= cek) 
				return true;
			}
		}
	return false;
}

void menuGameOver(int status)
{
	char pilih;
	if (papan.pemain[1].isKomputer){
	switch (status)
	{
	case 0:
		system("cls");
        fflush(stdout);
        printf("Permainan %d x %d (%s vs %s)", papan.ukuran, papan.ukuran, papan.pemain[0].nama, papan.pemain[1].nama);
        printf("\n============================================\n");
        tampilPapan(true);
		printf("Anda telah memenangkan permainan!\n");
		printf("Skor: %d\n", papan.pemain[0].skor);
        menuOpsi("Q: Ke menu utama || R: Ulang Permainan || S: Simpan permainan\n");
		printf("Masukan pilihan Anda: ");
		scanf("%d", &pilih);
		pilih = toupper(pilih);
		switch (pilih){
			case 'Q':
			menuUtama();
			break;
			case 'R':
			mulaiPermainan();
			break;
			case 'S':
			simpanPermainan();
			break;
			default:
			validInput = false;
			menuGameOver(status);
		}
		break;
	case 1:
		//if (papan.pemain[0].skor > tmpl) ini teh klo yg skor lebih besaar dri yg di HS masuk nama klo ngga ya ga bsia wkw
		system("cls");
        fflush(stdout);
        printf("Permainan %d x %d (%s vs %s)", papan.ukuran, papan.ukuran, papan.pemain[0].nama, papan.pemain[1].nama);
        printf("\n============================================\n");
        tampilPapan(true);
		printf("Anda kalah dalam permainan!\n");
		printf("Skor: %d\n", papan.pemain[0].skor);
        menuOpsi("Q: Ke menu utama || S: Simpan Skor\n");
		pesanInvalid("Pilihan anda tidak valid\n");
		printf("Masukan pilihan Anda: ");
		scanf("%d", &pilih);
		pilih = toupper(pilih);
		switch (pilih){
			case 'Q':
			menuUtama();
			break;
			case 'S':
			simpanSkor(papan.pemain[0].skor);
			break;
			default :
			validInput = false;
			menuGameOver(status);
		}
		break;
	case 2:
		system("cls");
        fflush(stdout);
        printf("Permainan %d x %d (%s vs %s)", papan.ukuran, papan.ukuran, papan.pemain[0].nama, papan.pemain[1].nama);
        printf("\n============================================\n");
        tampilPapan(true);
		printf("Permainan Seri!\n");
		printf("Skor: %d\n", papan.pemain[0].skor);
        menuOpsi("Q: Ke menu utama || R: Ulangi Permainan\n");
		pesanInvalid("Pilihan anda tidak valid\n");
		printf("Masukan pilihan Anda: ");
		scanf("%d", &pilih);
		pilih = toupper(pilih);
		switch (pilih){
			case 'Q':
			menuUtama();
			break;
			case 'R':
			mulaiPermainan();
			break;
			default :
			validInput = false;
			menuGameOver(status);
			break;
		}
	}
}
	else {
	char pilih;
	switch (status)
	{
	case 0:
		system("cls");
        fflush(stdout);
        printf("Permainan %d x %d (%s vs %s)", papan.ukuran, papan.ukuran, papan.pemain[0].nama, papan.pemain[1].nama);
        printf("\n============================================\n");
        tampilPapan(true);
		printf("%s telah mengalahkan %s", papan.pemain[0].nama, papan.pemain[1].nama);
        menuOpsi("Q: Ke menu utama || R: Ulang Permainan");
		printf("Masukan pilihan Anda: ");
		scanf("%d", &pilih);
		pilih = toupper(pilih);
		switch (pilih){
			case 'Q':
			menuUtama();
			break;
			case 'R':
			mulaiPermainan();
			break;
			default:
			validInput = false;
			menuGameOver(status);
		}
		break;
	case 1:
		system("cls");
        fflush(stdout);
        printf("Permainan %d x %d (%s vs %s)", papan.ukuran, papan.ukuran, papan.pemain[0].nama, papan.pemain[1].nama);
        printf("\n============================================\n");
        tampilPapan(true);
		printf("%s telah mengalahkan %s", papan.pemain[1].nama, papan.pemain[0].nama);
        menuOpsi("Q: Ke menu utama || R: Ulangi Permainan\n");
		pesanInvalid("Pilihan anda tidak valid\n");
		printf("Masukan pilihan Anda: ");
		scanf("%d", &pilih);
		pilih = toupper(pilih);
		switch (pilih){
			case 'Q':
			menuUtama();
			break;
			case 'R':
			mulaiPermainan();
			break;
			default:
			validInput = false;
			menuGameOver(status);
		}
		break;
	case 2:
		system("cls");
        fflush(stdout);
        printf("Permainan %d x %d (%s vs %s)", papan.ukuran, papan.ukuran, papan.pemain[0].nama, papan.pemain[1].nama);
        printf("\n============================================\n");
        tampilPapan(true);
		printf("Permainan Seri!\n");
        menuOpsi("Q: Ke menu utama || R: Ulangi Permainan\n");
		pesanInvalid("Pilihan anda tidak valid\n");
		printf("Masukan pilihan Anda: ");
		scanf("%d", &pilih);
		pilih = toupper(pilih);
		switch (pilih){
			case 'Q':
			menuUtama();
			break;
			case 'R':
			mulaiPermainan();
			break;
			default:
			validInput = false;
			menuGameOver(status);
		}
		break;
		}
	}
}

void urutSkor()
{

}
void tampilSkor()
{
	FILE *in;
	char tampil[6000];
	
	if((in=fopen("D:\\Kuliah\\Semester 1\\Dasar Dasar Pemograman\\Tugas Besar\\TicTacToe-Game\\Daftar10SkorTertinggi1.txt", "r")) == NULL){
		printf("Error Opening File"); //tutup program karena file ga ada
		fclose(in);
	}
		//baca isi file dengan gets lalu simpan ke tampil
	while(fgets(tampil, sizeof(tampil), in)){
		printf("%s", &tampil);
	}
		fclose(in);
}

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

void bagianPermainan()
{
    char input = '\000';
	
    menuPanel("Mulai Permainan", "Pilihlah jenis permainan yang akan anda mainkan");
    menuOpsi("1. Permainan Baru\n2. Lanjutkan Permainan Sebelumnya\nQ: Kembali");
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
        //aturPermainanLama();
        break;
    default:
        validInput = false;
		pesanInvalid("Pilihan anda tidak valid");
		getch();
        bagianPermainan();
        break;
    }
    aturPermainan();
    mulaiPermainan();
}
void mulaiPermainan()
{

}

void modeMultiPemain()
{
	char input [50], input2 [50], pilihan;
	system("CLS");

	papan.pemain[1].isKomputer = false;

	printf("Multipemain");
	printf("Harap masukkan nama pemain pertama.");
	printf("Nama pemain pertama: ");
	scanf("%^[\n]s", &input);
	printf("Nama pemain kedua: ");
	scanf("%^[\n]s", &input2);
	shownama:
	system("cls");
	printf("Nama pemain pertama : %s\n", papan.pemain[0].nama);
	printf("Nama pemain kedua   : %s\n", papan.pemain[1].nama);
	printf("Q: Kembali | S: Mulai permainan\n");
	pesanInvalid("Pilihan anda tidak valid\n");
	printf("Masukan pilihan anda: ");
	
	scanf("%c", &pilihan);

	pilihan = toupper(pilihan);

	switch (pilihan)
	{
	case 'Q':
		bagianPermainan();
		break;
	case 'S':
		mulaiPermainan();
		break;
	default:
		validInput = false;
		goto shownama;
		break;
	}
}

void modeKomputer()
{
	int pilihan;

	system("CLS");
	
	papan.pemain[1].isKomputer = true;
	printf ("Mode Komputer");
	printf("Memilih Tingkat Kesulitan\n");
	printf("Pilihlah tingkat kesulitan\n");
	printf("[1] Mudah\n");
	printf("[2] Sedang\n");
	printf("[3] Sulit\n");
	pesanInvalid("Pilihan anda tidak valid\n");
	printf("Masukan pilihan anda:\n");
	
	scanf("&d", &pilihan);

	memset(papan.pemain[0].nama, '\000', 50);
	memset(papan.pemain[1].nama, '\000', 50);
	memcpy(papan.pemain[0].nama, "Anda", 4);
	memcpy(papan.pemain[1].nama, "Komputer", 9);
	
	switch (pilihan)
	{
	case 1 :
		return;
	case 2 :
		return;
	case 3 :
		return;
	default:
		validInput = false;
		modeKomputer();
		break;
	}
}

void simpanSkor(int skor){

	FILE *in;
	int pilihan;
	
	in = fopen("Daftar10SkorTertinggi.txt", "ab");
	printf("Skor kamu : %d", skor);
	//papan.pemain[0].skor = skor;

	printf("Masukkan nama anda : ");
	scanf("%[^\n]s", papan.pemain[0].nama);
	
	fprintf(in, "%s - %d\n", papan.pemain[0].nama, papan.pemain[0].skor);
	fclose(in);	
}

void bagianSkor()
{
	system("CLS");
	char input;
	printf ("\t\t\t\t\tDaftar 10 Besar Skor\n");
	tampilSkor();
	
	printf("\n");
	printf("Q = Menu Utama || D = Hapus Data Skor Tertinggi\n");
	pesanInvalid("Pilihan anda tidak valid\n");
	printf("Masukan Pilihan Anda: ");
	scanf("%c", &input);
	
	input = tolower(input);

	switch (input)
	{
	case 'q' :
		menuUtama();
		break;
	
	case 'd' :

		break;

	default:
		validInput = false;
		bagianSkor();
		break;
	}
}

void bagianCaraMain()
{
	char menu;
	system("CLS");
	FILE *in;
	char tampil[6000];
	
	if((in=fopen("D:\\Kuliah\\Semester 1\\Dasar Dasar Pemograman\\Tugas Besar\\TicTacToe-Game\\CaraBermain.txt", "r")) == NULL){
		printf("Error Opening File"); //tutup program karena file ga ada
		fclose(in);
	}
		//baca isi file dengan gets lalu simpan ke tampil
	while(fgets(tampil, sizeof(tampil), in)){
		printf("%s", &tampil);
	}
		fclose(in);
		printf("\n");
		printf("\nTekan tombol apapun untuk kembali ke Menu Utama");
		while (true){
			fflush(stdin);
			char input = getch();
			if((int)input) return menuUtama();
		}
}

void bagianKeluar()
{
	exit(1);
}

void menuUtama()
{
	int pilihan;

	system("CLS");
		
	printf("Menu Utama\n");
	printf("Berikut merupakan pilihan yang dapat anda pilih.(Pilihlah pilihan dalam bentuk angka)\n");
	printf("[1.]Mulai Permainan\n");
	printf("[2.]Daftar 10 Pemain dengan Skor Tertinggi\n");
	printf("[3.]Cara Bermain\n");
	printf("[4.]Keluar Permainan\n");
	pesanInvalid("Pilihan anda tidak valid\n");
	printf("Masukan Pilihan Anda: ");
	
    scanf ("%d", &pilihan);
	getchar();
    
	switch (pilihan)
    { 
    case 1 :
        bagianPermainan();
        break;
    case 2 :
        bagianSkor();
        break;
    case 3 :
        bagianCaraMain();
        break;
    case 4 :
        bagianKeluar ();
        break;
    default :
		validInput = false;
		menuUtama();
		break;
	}
}

int main()
{
	menuUtama();
	return 0;
}