#include <iostream>
#include <fstream>
#include <vector>

// Función ftcs para resolver el problema de transferencia de calor
std::vector<std::vector<double> > ftcs(double Ti, double Td, double Tar, double Tab, double Tp, double kappa, int L, int a, int M, int N, double h, double t_fin) {
    // Matrices para almacenar temperaturas
    std::vector<std::vector<double> > T(M + 1, std::vector<double>(N + 1, 0.0));
    std::vector<std::vector<double> > Tk(M + 1, std::vector<double>(N + 1, 0.0));

    // Condiciones iniciales
    for (int i = 0; i <= M; ++i) {
        T[i][0] = Ti;
        T[i][N] = Td;
        Tk[i][0] = Ti;
        Tk[i][N] = Td;
    }
    for (int j = 0; j <= N; ++j) {
        T[0][j] = Tar;
        T[M][j] = Tab;
        Tk[0][j] = Tar;
        Tk[M][j] = Tab;
    }

    for (int i = 1; i < M; ++i) {
        for (int j = 1; j < N; ++j) {
            T[i][j] = Tp;
            Tk[i][j] = Tp;
        }
    }

    // Otros parámetros necesarios para el método
    double t = 0.0;
    double dy = static_cast<double>(a) / N;
    double dx = static_cast<double>(L) / M;
    double d1 = dy / dx;
    double d2 = dx / dy;
    double Fo = (kappa * t_fin) / (dy * dx);

    // Implementación del método FTCS
    while (t < t_fin) {
        for (int j = 1; j < N; ++j) {
            for (int i = 1; i < M; ++i) {
                Tk[i][j] = Fo * (d1 * (T[i - 1][j] + T[i + 1][j] - 2 * T[i][j]) + d2 * (T[i][j + 1] + T[i][j - 1] - 2 * T[i][j])) + T[i][j];
            }
        }
        T = Tk;  // Actualización de la matriz T con los nuevos valores calculados
        t += h;
    }

    return T;
}

int main() {
    // Ejemplo de uso de la función ftcs para obtener los valores
    std::vector<std::vector<double> > valores = ftcs(100.0, 0.0, 0.0, 120.0, 10.0, 22.8e-6, 10, 8, 50, 40, 1e-4, 30.0);

    // Guardar los datos en un archivo externo
    std::ofstream archivo("resultados.txt");
    if (archivo.is_open()) {
        for (int i = 0; i <= 50; ++i) {
            for (int j = 0; j <= 40; ++j) {
                archivo << valores[i][j] << " ";
            }
            archivo << "\n"; // Nueva línea al final de cada fila
        }
        archivo.close();
        std::cout << "Datos guardados correctamente en 'resultados.txt'" << std::endl;
    } else {
        std::cerr << "Error al abrir el archivo 'resultados.txt' para escritura" << std::endl;
    }

    return 0;
}
