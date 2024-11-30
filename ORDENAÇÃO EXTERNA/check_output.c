#include <stdio.h>

int main() {
    int num;
    FILE *file = fopen("output.dat", "rb");

    while (fread(&num, sizeof(int), 1, file)) {
        printf("%d ", num);
    }
    printf("\n");
    fclose(file);

    return 0;
}
