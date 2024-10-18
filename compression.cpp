#include <algorithm>
#include <complex>
#include <chrono>
#include <iostream>
#include <vector>

#include "parallel_dft/parallel_DFT_V0.h"
#include "parallel_dft/parallel_DFT_V1.h"
#include "parallel_dft/parallel_DFT_V2.h"
#include "sequential_dft/sequential_DFT_V1.h"
#include "sequential_dft/sequential_DFT_V2.h"

#include "compression.h"

#define COLOR_RESET "\033[0m"
#define COLOR_ORANGE "\033[38;5;214m"
#define COLOR_GREEN "\033[32m"
#define COLOR_BLUE "\033[34m"


/**
 * @brief Compares the magnitudes of two complex numbers.
 * 
 * @param a First complex number.
 * @param b Second complex number.
 * @return True if the magnitude of a is greater than the magnitude of b, false otherwise.
 */
bool compare( std::complex<double>& a,  std::complex<double>& b) {
    return std::abs(a) > std::abs(b);
}


/**
 * @brief Prepares a boolean mask for compressing a signal by keeping the k largest terms.
 * 
 * @param x Input vector of complex numbers.
 * @param k Number of largest terms to keep.
 * @return A vector of booleans indicating which terms to keep.
 */
std::vector<bool> precompress(std::vector<std::complex<double>>& x, int k) {
    std::vector<std::complex<double>> sorted_x = x;
    std::vector<bool> list_index( x.size());
    std::sort(sorted_x.begin(), sorted_x.end(), compare);

    double kth_largest = std::abs(sorted_x[k - 1]);

    
    for (int i=0; i< x.size(); i++) {
        if (std::abs(x[i]) >= kth_largest) {
            list_index[i]= true; 
        }
        else{
            list_index[i]= false; 
        }
    }
    return list_index;
}


/**
 * @brief Computes the absolute error between two vectors of complex numbers.
 * 
 * @param vec1 First input vector.
 * @param vec2 Second input vector.
 * @return The absolute error.
 */
double computeCompressionAbsoluteError(const std::vector<std::complex<double>>& vec1, const std::vector<std::complex<double>>& vec2) {
    double results = 0.0;
    for (size_t i = 0; i < vec1.size(); ++i) {
        std::complex<double> error = vec1[i] - vec2[i];
        results += std::abs(error);
    }
    return results/vec1.size();
}

/*
 * Computes the Nth roots of unity for a given integer N.
 * @param N The number of roots of unity to compute.
 * @return A dynamically allocated array of std::complex<double> containing the roots of unity.
 */
std::vector<std::complex<double>> root_of_unity(int N) {
    std::vector<std::complex<double>> ROOTS(N);
    for (int k = 0; k < N; ++k) {
        std::complex<double> rootk(cos(k * M_PI * 2 / N), sin(k * M_PI * 2 / N));
        ROOTS[k] = rootk;
    }
    return ROOTS;
}



/**
 * @brief Approximates a signal using the DFT, keeping only the k largest terms.
 * 
 * @param data Input vector of complex numbers.
 * @param k Number of largest terms to keep.
 * @param parallel Boolean indicating whether to use parallel computation.
 * @param version Version of the DFT algorithm to use.
 */
void approximation(std::vector<std::complex<double>>& data, int k, bool parallel, int version){
    
    if (!parallel) {

        std::vector<std::complex<double>> original_data = data;
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<std::complex<double>> roots = root_of_unity(data.size());

        switch (version) {
            case 1:
                DFT_V1(data, data.size());
                break;
            case 2:
                DFT_V2(data, data.size());
                break;
        }

        std::vector<bool> to_remove = precompress(data, k);

        switch (version) {
            case 1:
                invDFT_V1(data);
                break;
            case 2:
                invDFT_V2(data);
                break;
        }

        for (int i = 0; i < data.size(); i++) {
            if (to_remove[i]) {
                data[i] = std::complex<double>(0, 0);
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        double error = computeCompressionAbsoluteError(original_data, data);

        std::cout << COLOR_BLUE << "[Sequential V" << version << "] " << COLOR_RESET << "Time taken for the sequential compression process: " << COLOR_GREEN << duration.count() << " seconds" << COLOR_RESET << std::endl;
        std::cout << COLOR_BLUE << "[Sequential V" << version << "] " << COLOR_RESET << "Error for the sequential compression process (DFT + invDFT + removal): " << COLOR_ORANGE << error << COLOR_RESET << std::endl;

    } else {

        std::vector<std::complex<double>> original_data = data;
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<std::complex<double>> roots = root_of_unity(data.size());

        switch (version) {
            case 0:
                DFT_parallel_V0(data);
                break;
            case 1:
                DFT_parallel_V1(data);
                break;
            case 2:
                DFT_parallel_V2(data);
                break;
        }

        std::vector<bool> to_remove = precompress(data, k);

        switch (version) {
            case 0:
                invDFT_parallel_V0(data);
                break;
            case 1:
                invDFT_parallel_V1(data);
                break;
            case 2:
                invDFT_parallel_V2(data);
                break;
        }
        
        for (int i = 0; i < data.size(); i++) {
            if (to_remove[i]) {
                data[i] = std::complex<double>(0, 0);
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        double error = computeCompressionAbsoluteError(original_data, data);

        std::cout << COLOR_BLUE << "[Parallel V" << version << "] " << COLOR_RESET << "Time taken for the parallel compression process: " << COLOR_GREEN << duration.count() << " seconds" << COLOR_RESET << std::endl;
        std::cout << COLOR_BLUE << "[Parallel V" << version << "] " << COLOR_RESET << "Error for the parallel compression process (DFT + invDFT + removal): " << COLOR_ORANGE << error << COLOR_RESET << std::endl;

    }
}
