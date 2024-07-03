#include<iostream>
#include<vector>

std::vector<std::<vector<double>>ftcs(double Ti, double Td, double Tar, double Tab, double Tp, double kappa, int L, int a, int M, int N, double h, double t_fin { 
		std::vector<std::vector<double> > T(M+1.0, std::vector<double>(N+1, 0.00));
		std::vector<std::vector<double> > Tk(M+1.0, std::vector<double>(N+1, 0.00));

		//condiciones iniciales
		//
		for (int i = 0; i<= M; ++i){
			T[i][0] = Ti;
			T[i][N] = Td; 
			Tk[i][0] = Ti;
			Tk[i][N] Td; 
		}
		for (int j =0; j<= N; ++j){
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

		// Otros parámetros
    		double t = 0.0;
    		double dy = static_cast<double>(a) / N;
  	        double dx = static_cast<double>(L) / M;
	        double d1 = dy / dx;
 	   	double d2 = dx / dy;
    		double Fo = (kappa * t_fin) / (dy * dx);

		// Implementación del FCTS
   		 while (t < t_fin) {
       			 for (int j = 1; j < N; ++j) {
            			for (int i = 1; i < M; ++i) {
                			Tk[i][j] = Fo * (d1 * (T[i - 1][j] + T[i + 1][j] - 2 * T[i][j]) + d2 * (T[i][j + 1] + T[i][j - 1] - 2 * T[i][j])) + T[i][j];
				}
			 }
			 T = Tk;
			 t+= h;
		 }
		 return T; 

}

int main() {
    // Ejemplo de uso de la función fcts_calor
    std::vector<std::vector<double> > valores = ftcs_calor(100.0, 0.0, 0.0, 120.0, 10.0, 22.8e-6, 10, 8, 50, 40, 1e-4, 30.0);

    // Imprimir los valores resultantes (puedes modificar esta parte según tus necesidades)
    for (int i = 0; i <= 50; ++i) {
        for (int j = 0; j <= 40; ++j) {
            std::cout << valores[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0; 

}


            

