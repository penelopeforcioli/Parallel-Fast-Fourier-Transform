#include <complex>
#include <vector>
#include <cmath>

#include "sequential_DFT_V1.h"


// Discrete Fourier Transform using Cooley Tukey algorithm
// This first version computes the "twiddle factors / Nth roots of unity" inside the function


void DFT_V2(std::vector<std::complex<double>>& data, int N) {
    if (N <= 1) {
		return;}

	std::vector<std::complex<double>> odd(N / 2);
    std::vector<std::complex<double>> even(N / 2);

	for (int i = 0; i < N / 2; i++) {
		even[i] = data[i*2];
		odd[i] = data[i*2+1];
	}

	DFT_V2(even, N/2);
	DFT_V2(odd, N/2);

	for (int k = 0; k < N / 2; k++) {
		std::complex<double> t = std::polar(1.0,  - 2 * M_PI * k / N)  * odd[k];
		data[k] = even[k] + t;
		data[N / 2 + k] = even[k] - t;
	}
}


// data_copy never used

void invDFT_V2(std::vector<std::complex<double>>& data) {
    int N = data.size() ;

    for (int i=0; i < N; i++){
        data[i]= std::conj(data[i]);
    }

    DFT_V2(data, N);
   
    for (int i = 0; i < N; ++i) {
     
        data[i] = std::complex<double>(1.0/N)* data[i];

    }
}

