#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ELEMENTS 20  // Número de elementos no arquivo de entrada

int main() {
    FILE *file = fopen("input.dat", "wb");
    srand(time(NULL));

    for (int i = 0; i < NUM_ELEMENTS; i++) {
        int num = rand() % 100;  // Números aleatórios entre 0 e 99
        fwrite(&num, sizeof(int), 1, file);
    }
    fclose(file);

    printf("Arquivo input.dat gerado!\n");
    return 0;
}
