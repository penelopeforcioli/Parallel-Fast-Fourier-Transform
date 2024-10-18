#include <vector>
#include <complex>
#include <thread>
#include <cmath>
#include <mutex>

#include "parallel_DFT_V0.h"
#include "../sequential_dft/sequential_DFT_V1.h"


using complex = std::complex<double>;
extern int p; // number of processors


/**
 * @brief Helper function to compute segments of the DFT in parallel.
 * 
 * @param array Input vector of complex numbers.
 * @param output Output vector to store the results.
 * @param start Starting index for the segment.
 * @param num_threads Number of threads to use.
 * @param N Size of the input vector.
 * @param mutex Mutex for thread synchronization.
 */
void DFT_segments(const std::vector<complex>& array, std::vector<complex>& output, size_t start, size_t num_threads, int N, std::mutex& mutex,std::vector<std::complex<double>>  twiddle) {
    for (size_t k = start; k < N; k += num_threads) {
        complex sum = 0;
        for (size_t j = 0; j < N; ++j) {
            sum += array[j] * twiddle[(j*k)%N];
        }
        std::lock_guard<std::mutex> lock(mutex);
        output[k] = sum;
    }
}


/**
 * @brief Performs the Discrete Fourier Transform (DFT) on a given input vector using parallel processing.
 * 
 * @param array Input vector of complex numbers.
 * @return A vector of complex numbers representing the transformed data.
 */
void DFT_parallel_V0(std::vector<complex>& array) {
    int N = array.size();
    size_t num_threads = p;
    if (num_threads == 1) {
        DFT_V1(array, N);
        return;
    }

    int block_size = N / num_threads;

    // COMPUTATION IN PARALLEL OF THE TWIDDLE FACTORS
    std::vector<std::thread> workers_twiddle(num_threads);
    std::vector<std::complex<double>> twiddle(N);

    int begin_twiddle = 0;
    int end_twiddle = block_size;
    
    for (int i = 0; i < num_threads; i++){
        workers_twiddle[i] = std::thread([begin_twiddle, end_twiddle, &twiddle, N]() {
            for (int j = begin_twiddle; j < end_twiddle; ++j) {
               twiddle[j]= std::polar(1.0, - 2 * M_PI * j / N);
             }});

        begin_twiddle += block_size;
        end_twiddle += block_size;}

    for (size_t i = 0; i < num_threads; ++i) {
        workers_twiddle[i].join();
    }
    
    std::vector<std::thread> threads;
    threads.reserve(num_threads - 1);

    std::vector<complex> output(N);
    std::mutex mutex;

    for (size_t i = 1; i < num_threads; ++i) {
        threads.emplace_back(DFT_segments, std::cref(array), std::ref(output), i, num_threads, N, std::ref(mutex), twiddle);
    }
    DFT_segments(array, output, 0, num_threads, N, mutex,twiddle);

    for (auto& thread : threads) {
        thread.join();
    }

    array = output;
}


/**
 * @brief Helper function to compute segments of the DFT in parallel.
 * 
 * @param array Input vector of complex numbers.
 * @param output Output vector to store the results.
 * @param start Starting index for the segment.
 * @param num_threads Number of threads to use.
 * @param N Size of the input vector.
 * @param mutex Mutex for thread synchronization.
 */
void invDFT_segments(const std::vector<complex>& array, std::vector<complex>& output, size_t start, size_t num_threads, int N, std::mutex& mutex, std::vector<std::complex<double>> twiddle) {
    for (size_t k = start; k < N; k += num_threads) {
        complex sum = 0;
        for (size_t j = 0; j < N; ++j) {
            sum += std::conj(array[j]) * twiddle[(j*k)%N];
        }
        std::lock_guard<std::mutex> lock(mutex);
        output[k] = sum / (1.0*N);
    }
}


/**
 * @brief Performs the Inverse Discrete Fourier Transform (IDFT) on a given input vector using parallel processing.
 * 
 * @param data Input vector of complex numbers.
 * @return A vector of complex numbers representing the inverse transformed data.
 */

void invDFT_parallel_V0(std::vector<complex>& array) {
    int N = array.size();
    size_t num_threads = p;
    if (num_threads == 1) {
        invDFT_V1(array);
        return;
    }

    int block_size = N / num_threads;

    // COMPUTATION IN PARALLEL OF THE TWIDDLE FACTORS
    std::vector<std::thread> workers_twiddle(num_threads);
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

    std::vector<std::thread> threads;
    threads.reserve(num_threads - 1);

    std::vector<complex> output(N);
    std::mutex mutex;

    for (size_t i = 1; i < num_threads; ++i) {
        threads.emplace_back(invDFT_segments, std::cref(array), std::ref(output), i, num_threads, N, std::ref(mutex),twiddle);
    }
    invDFT_segments(array, output, 0, num_threads, N, mutex,twiddle);

    for (auto& thread : threads) {
        thread.join();
    }

    array = output;
}
