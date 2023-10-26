#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10
#define SIZE_OF 8 // for double precision
#define SEED 86456

void matrix_multiply_block(double a[N][N], double b[N][N], double c[N][N], int block_size) {
    int i, j, k, ii, jj, kk;

    for (i = 0; i < N; i += block_size) {
        for (j = 0; j < N; j += block_size) {
            for (k = 0; k < N; k += block_size) {
                for (ii = i; ii < i + block_size; ii++) {
                    for (jj = j; jj < j + block_size; jj++) {
                        for (kk = k; kk < k + block_size; kk++) {
                            c[ii][jj] += a[ii][kk] * b[kk][jj];
                        }
                    }
                }
            }
        }
    }
}

int main() {
    double x[N][N], y[N][N], z[N][N];
    int i, j;
    int block_sizes[] = {1, 2, 5, 10, 20};
    int num_block_sizes = sizeof(block_sizes) / sizeof(block_sizes[0]);

    srand(SEED);

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            x[i][j] = (double)rand() + 1.0;
            y[i][j] = (double)rand() + 1.0;
            z[i][j] = 0.0;
        }
    }

    FILE *fp; // Déclaration du pointeur de fichier
    fp = fopen("block_O2.csv", "w"); // Ouvre le fichier en mode écritur

    fprintf(fp, "Block Size, time (msec), rate (MB/s)\n");

    for (int bs = 0; bs < num_block_sizes; bs++) {
        int block_size = block_sizes[bs];

        clock_t start = clock();

        matrix_multiply_block(x, y, z, block_size);
        clock_t end = clock();

        double msec = 1000.0 * (double)(end - start) / CLOCKS_PER_SEC;
        double rate = (double)(SIZE_OF * N * N * N) / (msec * 1024.0 * 1024.0);

        fprintf(fp, "%d, %lf, %lf\n", block_size, msec, rate);
    }

    fclose(fp);
    return 0;
}