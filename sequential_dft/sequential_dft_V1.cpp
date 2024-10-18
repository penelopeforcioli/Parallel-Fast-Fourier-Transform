#include <complex>
#include <vector>
#include <cmath>

#include "sequential_DFT_V1.h"


/**
 * @brief Performs the Discrete Fourier Transform (DFT) on a given input vector using the Cooley-Tukey algorithm.
 * 
 * This first version computes the DFT using the basic definition.
 * 
 * @param data Input vector of complex numbers. The transformed data will be stored back in this vector.
 * @param N Size of the input vector.
 */
void DFT_V1(std::vector<std::complex<double>>& data, int N) {
	std::vector<std::complex<double>> output(N);
	for (int i = 0; i < N; ++i) {
    output[i] = std::complex<double>(0, 0);
			}

	 for (int k = 0; k < N; ++k) {
        for (int n = 0; n < N; ++n) {
        
            output[k] += data[n] * std::polar(1.0,  - 2 * M_PI * k*n / N) ;
        }
    }
	
	data =  output;
}


/**
 * @brief Performs the Inverse Discrete Fourier Transform (IDFT) on a given input vector using the Cooley-Tukey algorithm.
 * 
 * This first version computes the IDFT using the basic definition.
 * 
 * @param data Input vector of complex numbers. The transformed data will be stored back in this vector.
 */
void invDFT_V1(std::vector<std::complex<double>>& data ) {
	int N= data.size();
	std::vector<std::complex<double>> output(N);
		for (int i = 0; i < N; ++i) {
    	output[i] = std::complex<double>(0, 0);
			}

	 for (int k = 0; k < N; ++k) {
        for (int n = 0; n < N; ++n) {
            output[k] += data[n] * std::polar(1.0,   2 * M_PI * k*n / N) ;
        }
		output[k] = output[k] /(1.0 * N);
    }

	data = output;  
}
