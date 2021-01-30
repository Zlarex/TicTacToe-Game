#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

void bagianCaraMain()
{
    printf("Test");
}

int main(int argc, char *argv[])
{
	printf("Hello world.");
	return 0;
}