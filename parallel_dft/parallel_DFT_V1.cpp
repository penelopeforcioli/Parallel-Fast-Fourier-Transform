#include <complex>
#include <vector>
#include <thread>
#include <cmath>
#include <mutex>

#include "parallel_DFT_V1.h"
#include "../sequential_dft/sequential_DFT_V1.h"
#include "p_transpose.h"


extern int p; // number of processors


/**
 * @brief Worker function for computing the DFT of a segment of data.
 * 
 * @param begin Starting index for the segment.
 * @param end Ending index for the segment.
 * @param result Output vector to store the results.
 * @param data Input vector of complex numbers.
 */
void workerDFT_V1(int begin, int end,std::vector<std::complex<double>>& result, std::vector<std::complex<double>>&  data){
    int n = end - begin;
    std::vector<std::complex<double>> temp(n);

    for (int i = 0; i < n; i++){
        temp[i] = data[i+begin];
    }

    DFT_V1(temp,n);

    for (int i = 0; i < n; i++){
        result[i+begin] = temp[i];
    }
}


/**
 * @brief Main function for performing the parallel DFT.
 * 
 * @param array Input vector of complex numbers.
 * @param results Output vector to store the results.
 * @param N Size of the input vector.
 * @param num_threads Number of threads to use.
 */
void mainDFT_V1(std::vector<std::complex<double>>& array, std::vector<std::complex<double>>& results, int N, int num_threads) {

    int block_size = N / num_threads;
    std::vector<std::complex<double>> array_rec(array);
    std::vector<std::complex<double>> array_ite(array);
    std::vector<std::complex<double>> output_rec(array);
    std::vector<std::complex<double>> output(N);

    std::vector<std::thread> workers(num_threads);
    int begin = 0;
    int end = block_size;

    for (int i = 0; i < num_threads; i++){
        workers[i] = std::thread(&workerDFT_V1, begin, end, std::ref(results), std::ref(array));
        begin += block_size;
        end += block_size;
    }

    for (size_t i = 0; i < num_threads ; ++i) {
        workers[i].join();
    }

    int block_size2 = N/num_threads*2;
    
    for (int thread=0; thread < log2(num_threads); thread++){
        int begin,end;
        begin = 0;
        end = block_size2;

      
        while (end <= N){

            std::vector<std::complex<double>>  G_N(block_size2 /2), H_N(block_size2 /2);
            std::complex<double>  twiddle;

            for (int i = 0; i< block_size2 /2; i++){
                twiddle = std::polar(1.0,  - 2 * M_PI * i / N);
                G_N[i] = results[begin+ i];
                H_N[i] = results[begin +i + block_size2/2];

                results[begin + i] = G_N[i] +  twiddle *H_N[i];
                results[begin + i + block_size2/2] = G_N[i] - twiddle*H_N[i];
            }



            begin += block_size2;
            end += block_size2;
        }

        block_size2 *=2;

    }
    results = iterative_bit_reversal(results, num_threads, N);

    }


/**
 * @brief Performs the Discrete Fourier Transform (DFT) on a given input vector using parallel processing (version 1).
 * 
 * @param dft Input vector of complex numbers.
 */
void DFT_parallel_V1(std::vector<std::complex<double>>& dft){
    int N = dft.size();
    std::vector<std::complex<double>> data_copy(N) ;
    for (int i=0; i < N; i++){
        data_copy[i]= dft[i];
    }
    mainDFT_V1(data_copy, dft , N, p);
}


/**
 * @brief Performs the Inverse Discrete Fourier Transform (IDFT) on a given input vector using parallel processing (version 1).
 * 
 * @param data Input vector of complex numbers.
 */
void invDFT_parallel_V1(std::vector<std::complex<double>>& data){
    int N = data.size() ;
    std::vector<std::complex<double>> data_copy(N) ;

    for (int i=0; i < N; i++){
        data_copy[i]= std::conj(data[i]);
    }

    mainDFT_V1(data_copy, data , N, p);

    for (int i=0; i < N; i++){
        data[i]= data[i] / (1.0 *N);
    }
}
