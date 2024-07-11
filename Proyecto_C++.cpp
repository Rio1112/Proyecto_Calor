#include <iostream>
#include <vector>
#include <fstream>
#include <omp.h>
#include <sys/time.h>

double seconds()
{ 
  struct timeval tmp;
  double sec;
  gettimeofday( &tmp, (struct timezone *)0 );
  sec = tmp.tv_sec + ((double)tmp.tv_usec)/1000000.0;
     
  return sec;
}


void ftcs_calor(double Th, double Tc, double kappa, double L, double a, int M, int N, double h, double t_fin, std::vector<std::vector<double> >& T) {
    // Condiciones iniciales
    T.assign(M + 1, std::vector<double> (N + 1, Tc));
    for (int i = 24; i <= 26; ++i) {
        for (int j = 24; j <= 26; ++j) {
            T[i][j] = Th;
        }
    }

    // Otros parámetros
    double t = 0.0;
    double dy = a / (N - 1);  // Particiones del ancho
    double dx = L / (M - 1);  // Particiones del largo
    int num_procs;
    double time_1 = seconds();

    // Implementación del FTCS
    while (t < t_fin) {
        num_procs = omp_get_max_threads();
        std::vector<std::vector<double> > Tk = T;

        #pragma omp parallel for
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
    }
    double time_2 = seconds();
    
    std::cout << "Number of Threads: " << num_procs << std::endl;
    std::cout << "Time to complete loop: " << time_2 - time_1 << std::endl;
}

void guardar_datos(const std::vector<std::vector<double> >& T, const std::string& filename) {
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

int main() {
    std::vector<std::vector<double> > T;

    ftcs_calor(300.0, 20.0, 98.8e-6, 0.3, 0.3, 50, 50, 1e-4, 60.0, T);
    guardar_datos(T, "datos_ftcs.txt");

    ftcs_calor(200.0, 20.0, 98.8e-6, 0.3, 0.3, 50, 50, 1e-4, 60.0, T);
    guardar_datos(T, "datos_ftcs_2.txt");

    return 0;
}
