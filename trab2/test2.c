#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function prototype
void doitgen(int R, int Q, int S, int P, int *A, int *x, int *Aout);

int main() {
    // Set the dimensions of the input arrays
    int R = 2, Q = 2, S = 2, P = 2;

    // Allocate memory for the input and output arrays
    int *A = (int *) malloc(R * Q * S * sizeof(int));
    int *x = (int *) malloc(P * S * sizeof(int));
    int *Aout = (int *) calloc(R * Q * P, sizeof(int));

    // Initialize the input arrays with random values
    srand(time(NULL));
    printf("Matrix A:\n");
    for (int i = 0; i < R * Q * S; i++) {
        A[i] = rand() % 10;
        printf("%d\t", A[i]);
        if ((i + 1) % S == 0) {
            printf("\n");
        }
    }

    printf("\nMatrix x:\n");
    for (int i = 0; i < P * S; i++) {
        x[i] = rand() % 10;
        printf("%d\t", x[i]);
        if ((i + 1) % S == 0) {
            printf("\n");
        }
    }

    // Compute the output array using the doitgen function
    doitgen(R, Q, S, P, A, x, Aout);

    // Print the calculated and expected output arrays
    printf("\nMatrix Aout (Calculated):\n");
    for (int i = 0; i < R * Q * P; i++) {
        printf("%d\t", Aout[i]);
        if ((i + 1) % P == 0) {
            printf("\n");
        }
    }

    // Compute the expected output array
    int *Aout_expected = (int *) calloc(R * Q * P, sizeof(int));
    for (int r = 0; r < R; r++) {
        for (int q = 0; q < Q; q++) {
            for (int p = 0; p < P; p++) {
                for (int s = 0; s < S; s++) {
                    Aout_expected[r * Q * P + q * P + p] += A[r * Q * S + q * S + s] * x[p * S + s];
                }
            }
        }
    }

    printf("\nMatrix Aout (Expected):\n");
    for (int i = 0; i < R * Q * P; i++) {
        printf("%d\t", Aout_expected[i]);
        if ((i + 1) % P == 0) {
            printf("\n");
        }
    }

    // Compare the output array with the expected output array
    int success = 1;
    for (int i = 0; i < R * Q * P; i++) {
        if (Aout[i] != Aout_expected[i]) {
            success = 0;
            break;
        }
    }

    // Print the result of the test
    if (success) {
        printf("\nTest passed!\n");
    } else {
        printf("\nTest failed!\n");
    }

    // Free the memory allocated for the arrays
    free(A);
    free(x);
    free(Aout);
    free(Aout_expected);

    return 0;
}

// Function to compute the output array using the given equation
void doitgen(int R, int Q, int S, int P, int *A, int *x, int *Aout) {
    // Iterate over the elements of the output array
    for (int r = 0; r < R; r++) {
        for (int q = 0; q < Q; q++) {
            for (int p = 0; p < P; p++) {
                // Iterate over the elements of the input arrays
                for (int s = 0; s < S; s++) {
                    // Add the product of the corresponding elements
                    Aout[r * Q * P + q * P + p] += A[r * Q * S + q * S + s] * x[p * S + s];
                }
            }
        }
    }
}
