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

#include "full_dft.h"

#define COLOR_RESET "\033[0m"
#define COLOR_ORANGE "\033[38;5;214m"
#define COLOR_GREEN "\033[32m"
#define COLOR_BLUE "\033[34m"

/**
 * @brief Computes the absolute error between two vectors of complex numbers.
 * 
 * @param vec1 First input vector.
 * @param vec2 Second input vector.
 * @return The absolute error.
 */
double computeAbsoluteError(const std::vector<std::complex<double>>& vec1, const std::vector<std::complex<double>>& vec2) {
    double results = 0.0;
    for (size_t i = 0; i < vec1.size(); ++i) {
        std::complex<double> error = vec1[i] - vec2[i];
        results += std::abs(error);
    }
    return results/vec1.size();
}


/**
 * @brief Performs the full DFT computation on the input data, either in parallel or non-parallel mode.
 * 
 * @param data Input vector of complex numbers.
 * @param parallel Boolean indicating whether to use parallel computation.
 * @param version Version of the DFT algorithm to use.
 */
void full_dft(std::vector<std::complex<double>>& data,  bool parallel, int version){
    
    if (!parallel) {

        if (version == 1) {
            std::vector<std::complex<double>> original_data = data;
            auto start1 = std::chrono::high_resolution_clock::now();
            DFT_V1(data, data.size());
            auto end1 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration1 = end1 - start1;
            std::cout << COLOR_BLUE << "[Sequential V1]" << COLOR_RESET << "Time taken for the sequential DFT: " << COLOR_GREEN << duration1.count() << " seconds" << COLOR_RESET << std::endl;

            auto start2 = std::chrono::high_resolution_clock::now();
            invDFT_V1(data);
            auto end2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration2 = end2 - start2;
            std::cout << COLOR_BLUE << "[Sequential V1]" << COLOR_RESET << "Time taken for the sequential inverse DFT: " << COLOR_GREEN << duration2.count() << " seconds" << COLOR_RESET << std::endl;

            double error = computeAbsoluteError(original_data, data);
            std::cout << COLOR_BLUE << "[Sequential V1]" << COLOR_RESET << "Error for the sequential DFT + inverse DFT transformation: " << COLOR_ORANGE << error << COLOR_RESET << std::endl;
        }

        if (version == 2) {
            std::vector<std::complex<double>> original_data = data;
            auto start1 = std::chrono::high_resolution_clock::now();
            DFT_V2(data, data.size());
            auto end1 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration1 = end1 - start1;
            std::cout << COLOR_BLUE << "[Sequential V2]" << COLOR_RESET << "Time taken for the sequential DFT: " << COLOR_GREEN << duration1.count() << " seconds" << COLOR_RESET << std::endl;

            auto start2 = std::chrono::high_resolution_clock::now();
            invDFT_V2(data);
            auto end2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration2 = end2 - start2;
            std::cout << COLOR_BLUE << "[Sequential V2]" << COLOR_RESET << "Time taken for the sequential inverse DFT: " << COLOR_GREEN << duration2.count() << " seconds" << COLOR_RESET << std::endl;

            double error = computeAbsoluteError(original_data, data);
            std::cout << COLOR_BLUE << "[Sequential V2]" << COLOR_RESET << "Error for the sequential DFT + inverse DFT transformation: " << COLOR_ORANGE << error << COLOR_RESET << std::endl;
        }

    } else {

        if (version == 0) {
            std::vector<std::complex<double>> original_data = data;
            auto start1 = std::chrono::high_resolution_clock::now();
            DFT_parallel_V0(data);
            auto end1 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration1 = end1 - start1;
            std::cout << COLOR_BLUE << "[Parallel V0] " << COLOR_RESET << "Time taken for the parallel DFT: " << COLOR_GREEN << duration1.count() << " seconds" << COLOR_RESET << std::endl;
        
            auto start2 = std::chrono::high_resolution_clock::now();
            invDFT_parallel_V0(data);
            auto end2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration2 = end2 - start2;
            std::cout << COLOR_BLUE << "[Parallel V0] " << COLOR_RESET << "Time taken for the parallel inverse DFT: " << COLOR_GREEN << duration2.count() << " seconds" << COLOR_RESET << std::endl;

            double error = computeAbsoluteError(original_data, data);
            std::cout << COLOR_BLUE << "[Parallel V0] " << COLOR_RESET << "Error for the parallel DFT + inverse DFT transformation: " << COLOR_ORANGE << error << COLOR_RESET << std::endl;
        }

        if (version == 1) {
            std::vector<std::complex<double>> original_data = data;
            auto start1 = std::chrono::high_resolution_clock::now();
            DFT_parallel_V1(data);
            auto end1 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration1 = end1 - start1;
            std::cout << COLOR_BLUE << "[Parallel V1] " << COLOR_RESET << "Time taken for the parallel DFT: " << COLOR_GREEN << duration1.count() << " seconds" << COLOR_RESET << std::endl;
        
            auto start2 = std::chrono::high_resolution_clock::now();
            invDFT_parallel_V1(data);
            auto end2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration2 = end2 - start2;
            std::cout << COLOR_BLUE << "[Parallel V1] " << COLOR_RESET << "Time taken for the parallel inverse DFT: " << COLOR_GREEN << duration2.count() << " seconds" << COLOR_RESET << std::endl;

            double error = computeAbsoluteError(original_data, data);
            std::cout << COLOR_BLUE << "[Parallel V1] " << COLOR_RESET << "Error for the parallel DFT + inverse DFT transformation: " << COLOR_ORANGE << error << COLOR_RESET << std::endl;
        }

        if (version == 2) {
            std::vector<std::complex<double>> original_data = data;
            auto start1 = std::chrono::high_resolution_clock::now();
            DFT_parallel_V2(data);
            auto end1 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration1 = end1 - start1;
            std::cout << COLOR_BLUE << "[Parallel V2] " << COLOR_RESET << "Time taken for the parallel DFT: " << COLOR_GREEN << duration1.count() << " seconds" << COLOR_RESET << std::endl;

            auto start2 = std::chrono::high_resolution_clock::now();
            invDFT_parallel_V2(data);
            auto end2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration2 = end2 - start2;
            std::cout << COLOR_BLUE << "[Parallel V2] " << COLOR_RESET << "Time taken for the parallel inverse DFT: " << COLOR_GREEN << duration2.count() << " seconds" << COLOR_RESET << std::endl;

            double error = computeAbsoluteError(original_data, data);
            std::cout << COLOR_BLUE << "[Parallel V2] " << COLOR_RESET << "Error for the parallel DFT + inverse DFT transformation: " << COLOR_ORANGE << error << COLOR_RESET << std::endl;
        }
    
    }
}
