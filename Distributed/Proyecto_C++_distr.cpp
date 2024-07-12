#include <iostream>
#include <vector>
#include <fstream>
#include <mpi.h>

double time_1, time_2;

void ftcs_calor(double Th, double Tc, double kappa, double L, double a, int M, int N, double h, double t_fin, std::vector<std::vector<double>>& T) {
    MPI_Init(NULL, NULL);

    // Condiciones iniciales
    T.assign(M + 1, std::vector<double>(N + 1, Tc));
    for (int i = 24; i <= 26; ++i) {
        for (int j = 24; j <= 26; ++j) {
            T[i][j] = Th;
        }
    }

    int rank, size;

    time_1 = MPI_Wtime();

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int fila_local = M / size;
    int rest = M % size;

    if(rest && (rank < rest)) fila_local++;

    int start_fila = rank * fila_local;
    if(rest && (rank >= rest)) start_fila += rest;

    int end = start_fila + fila_local;
      
    // Otros parámetros
    double t = 0.0;
    double dy = a / (N - 1);  // Particiones del ancho
    double dx = L / (M - 1);  // Particiones del largo  

    // Implementación del FTCS
    while (t < t_fin) {
        time_1 = MPI_Wtime();
        std::vector<std::vector<double>> Tk = T;
        for (int i = 1; i < M; ++i) {  // Evitar los bordes
            for (int j = 1; j < N; ++j) {  // Evitar los bordes
                if (i < 24 || i > 26 || j < 24 || j > 26) {
                    Tk[i][j] = (((kappa * h) / (dy * dx)) * ((dy / dx) * (T[i - 1][j] + T[i + 1][j] - 2 * T[i][j])) +
                                ((kappa * h) / (dy * dx)) * ((dx / dy) * (T[i][j + 1] + T[i][j - 1] - 2 * T[i][j])) +
                                T[i][j]);
                }
            }
        }
        T = Tk;
        t += h;
        std::vector<std::vector<double>> global_mat(T);

        MPI_Gather(&T[0][0], M*N, MPI_DOUBLE, &global_mat[0][0], M*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        time_2 = MPI_Wtime();
    }
    MPI_Finalize();
}

void guardar_datos(const std::vector<std::vector<double>>& T, const std::string& filename) {
    std::ofstream archivo(filename);
    if (archivo.is_open()) {
        for (const auto& fila : T) {
            for (const auto& valor : fila) {
                archivo << valor << " ";
            }
            archivo << "\n";
        }
        archivo.close();
        std::cout << "Datos guardados exitosamente en " << filename << std::endl;
    } else {
        std::cerr << "No se pudo abrir el archivo para escribir." << std::endl;
    }
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    std::vector<std::vector<double>> T;

    ftcs_calor(300.0, 20.0, 98.8e-6, 0.3, 0.3, 50, 50, 1e-2, 60.0, T, argc, argv);    
    guardar_datos(T, "datos_ftcs.txt");

    ftcs_calor(200.0, 20.0, 98.8e-6, 0.3, 0.3, 50, 50, 1e-2, 60.0, T, argc, argv);
    guardar_datos(T, "datos_ftcs_2.txt");

    MPI_Finalize();
    return 0;
}
