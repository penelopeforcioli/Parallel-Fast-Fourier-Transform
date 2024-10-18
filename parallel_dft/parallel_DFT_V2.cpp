#include <complex>
#include <vector>
#include <thread>
#include <cmath>

#include "parallel_DFT_V2.h"
#include "../sequential_dft/sequential_DFT_V1.h"
#include "p_transpose.h"

extern int p; // number of processors


void workerDFT(int begin, int end,std::vector<std::complex<double>>& result, std::vector<std::complex<double>>&  data){
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





void mainDFT(std::vector<std::complex<double>>& array, std::vector<std::complex<double>>& results,int N, int num_threads) {
    int block_size = N / num_threads;

    
    // COMPUTATION IN PARALLEL OF THE TWIDDLE FACTORS
    std::vector<std::thread> workers_twiddle(num_threads - 1);
    std::vector<std::complex<double>>  twiddle(N);

    int begin_twiddle =0;
    int end_twiddle= block_size;
    
    for (int i = 0; i < num_threads; i++){
        workers_twiddle[i] = std::thread([begin_twiddle, end_twiddle, &twiddle,N]() {
            for (int j = begin_twiddle; j < end_twiddle; ++j) {
               twiddle[j]= std::polar(1.0,  - 2 * M_PI * j / N);
             }});

        begin_twiddle += block_size;
        end_twiddle += block_size;}
    

    for (size_t i = 0; i < num_threads ; ++i) {
        workers_twiddle[i].join();
    }

    

    // COMPUTATION IN PARALLEL OF DFT CHUNKS
    std::vector<std::thread> workers(num_threads);

    int begin =0;
    int end = block_size;

    for (int i = 0; i < num_threads; i++){
        workers[i] = std::thread(&workerDFT, begin, end, std::ref(results), std::ref(array));
        begin += block_size;
        end += block_size;
    }

    for (size_t i = 0; i < num_threads ; ++i) {
        workers[i].join();
    }




    // ASSEMBLING EVERYTHING
    int block_size2 = N/num_threads*2;
    // pas sûre ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    for (int thread=0; thread < log2(num_threads); thread++){
        int begin,end;
        begin = 0;
        end = block_size2;
        while (end <= N){

            std::vector<std::complex<double>>  G_N(block_size2 /2), H_N(block_size2 /2);


            for (int i = 0; i< block_size2 /2; i++){
                G_N[i] = results[begin+ i];
                H_N[i] = results[begin +i + block_size2/2];

                results[begin + i] = G_N[i] +  twiddle[i] *H_N[i];
                results[begin + i + block_size2/2] = G_N[i] - twiddle[i] *H_N[i];
            }


            begin += block_size2;
            end += block_size2;
        }
        block_size2 *=2;
        

    } 

    results = iterative_bit_reversal(results, num_threads, N);


   //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

}

void DFT_parallel_V2(std::vector<std::complex<double>>& dft){
    int N = dft.size();
    mainDFT(dft, dft , N, p);
}


void invDFT_parallel_V2(std::vector<std::complex<double>>& data){
    int N = data.size() ;

    for (int i=0; i < N; i++){
        data[i]= std::conj(data[i]);
    }

    mainDFT(data, data , N, p);

    for (int i=0; i < N; i++){
        data[i]= data[i] / (1.0 *N);
    }

}
