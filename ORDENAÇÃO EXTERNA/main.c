#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 5  // Tamanho máximo do bloco na memória
#define TEMP_FILES 4   // Número de arquivos temporários

// Função de comparação para qsort
int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// Função para criar e ordenar runs
void createSortedRuns(const char *inputFile) {
    FILE *in = fopen(inputFile, "rb");
    int buffer[MEMORY_SIZE];
    int count, fileIndex = 0;

    while ((count = fread(buffer, sizeof(int), MEMORY_SIZE, in)) > 0) {
        qsort(buffer, count, sizeof(int), compare);
        char fileName[20];
        sprintf(fileName, "temp%d.dat", fileIndex++);
        FILE *out = fopen(fileName, "wb");
        fwrite(buffer, sizeof(int), count, out);
        fclose(out);
    }
    fclose(in);
}

// Função para fazer o merge dos runs ordenados
void mergeRuns(const char *outputFile) {
    FILE *out = fopen(outputFile, "wb");
    FILE *inFiles[TEMP_FILES];
    int values[TEMP_FILES];
    int active[TEMP_FILES];

    // Abrir arquivos temporários
    for (int i = 0; i < TEMP_FILES; i++) {
        char fileName[20];
        sprintf(fileName, "temp%d.dat", i);
        inFiles[i] = fopen(fileName, "rb");
        active[i] = (fread(&values[i], sizeof(int), 1, inFiles[i]) > 0);
    }

    // Merge
    while (1) {
        int minIndex = -1, minValue = __INT_MAX__;
        for (int i = 0; i < TEMP_FILES; i++) {
            if (active[i] && values[i] < minValue) {
                minValue = values[i];
                minIndex = i;
            }
        }
        if (minIndex == -1) break;  // Nenhum valor restante

        fwrite(&minValue, sizeof(int), 1, out);

        if (fread(&values[minIndex], sizeof(int), 1, inFiles[minIndex]) == 0) {
            active[minIndex] = 0;  // Fim desse arquivo
        }
    }

    // Fechar arquivos
    for (int i = 0; i < TEMP_FILES; i++) fclose(inFiles[i]);
    fclose(out);
}

int main() {
    const char *inputFile = "input.dat";
    const char *outputFile = "output.dat";

    createSortedRuns(inputFile);
    mergeRuns(outputFile);

    printf("Ordenação concluída! Resultado em %s\n", outputFile);
    return 0;
}
