#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int rank, size, N;
    int *local_values;
    int local_sum = 0, total_sum = 0;
    float promedio = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // ID del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Cantidad de procesos

    // 1. Proceso raíz solicita N
    if (rank == 0) {
        printf("Ingrese la cantidad de valores por proceso (N): ");
        fflush(stdout);
        scanf("%d", &N);
    }

    // 2. Broadcast del valor de N a todos los procesos
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // 3. Generación de valores aleatorios locales
    local_values = (int *)malloc(N * sizeof(int));
    srand(time(NULL) + rank); // Semilla distinta por proceso

    for (int i = 0; i < N; i++) {
        local_values[i] = rand() % 100;  // valor entre 0 y 99
        local_sum += local_values[i];
    }

    // 4. Reducción: suma total de todos los procesos
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // 5. Cálculo del promedio y Broadcast del resultado
    if (rank == 0) {
        promedio = (float)total_sum / (N * size);
    }
    MPI_Bcast(&promedio, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // 6. Cada proceso imprime el promedio
    printf("Proceso %d recibió el promedio: %.2f\n", rank, promedio);

    free(local_values);
    MPI_Finalize();
    return 0;
}
