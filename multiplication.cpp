#include <vector>
#include <ctime>
#include <sstream>
#include <complex>
#include <cmath> 
#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <random>

#include "parallel_dft/parallel_DFT_V0.h"
#include "parallel_dft/parallel_DFT_V1.h"
#include "parallel_dft/parallel_DFT_V2.h"
#include "sequential_dft/sequential_DFT_V1.h"
#include "sequential_dft/sequential_DFT_V2.h"

#include "multiplication.h"

#define COLOR_RESET "\033[0m"
#define COLOR_GREEN "\033[32m"
#define COLOR_PURPLE "\033[35m"
#define COLOR_BLUE "\033[34m"


// Input: Polynomials P, Q as a list of their coefficients
// Ouput: P*Q as a list of its coeff

/**
 * @brief Multiplies two polynomials using the Discrete Fourier Transform (DFT).
 * 
 * @param P First polynomial represented as a vector of complex numbers.
 * @param Q Second polynomial represented as a vector of complex numbers.
 * @return Resultant polynomial coefficients after multiplication.
 */
std::vector<std::complex<double>> multiply(std::vector<std::complex<double>> P,  std::vector<std::complex<double>> Q, int version) {
    /*std::cout << "Starting polynomials: " << std::endl;
    std::cout << "  * " << COLOR_PURPLE << "P = {";
    for (const auto& coeff : P) {std::cout << coeff << ", ";}
    std::cout << "}" << COLOR_RESET << std::endl;
    std::cout << "  * " << COLOR_PURPLE << "Q = {";
    for (const auto& coeff : Q) {std::cout << coeff << ", ";}
    std::cout << "}" << COLOR_RESET << std::endl;*/

    auto start_time = std::chrono::steady_clock::now();

    int len_P = P.size();
    int len_Q = Q.size();
    int len_PQ= 2*(len_P + len_Q);


    // Ensure len_PQ is a power of 2
    while (std::fmod(std::log2(len_PQ), 1) != 0) {
        len_PQ += 1;
    }

    // Pad the two lists with 0s
    if (P.size() < len_PQ) {
        P.resize(len_PQ, 0);}
    if (Q.size() < len_PQ) {
        Q.resize(len_PQ, 0); }

    std::vector<std::complex<double>> DFT_P = P;
    std::vector<std::complex<double>> DFT_Q = Q ;

    switch (version) {
        case 0:
            DFT_parallel_V0(DFT_P);
            DFT_parallel_V0(DFT_Q);
            break;
        case 1:
            DFT_parallel_V1(DFT_P);
            DFT_parallel_V1(DFT_Q);
            break;
        case 2:
            DFT_parallel_V2(DFT_P);
            DFT_parallel_V2(DFT_Q);
            break;
    }
    
    std::vector<std::complex<double>> DFT_PQ(len_PQ); 
    for (int i=0; i<len_PQ; i++ ){
        DFT_PQ[i]= DFT_P[i] * DFT_Q[i];
    }

    switch (version) {
        case 0:
            invDFT_parallel_V0(DFT_PQ);
            break;
        case 1:
            invDFT_parallel_V1(DFT_PQ);
            break;
        case 2:
            invDFT_parallel_V2(DFT_PQ);
            break;
    }
    DFT_PQ.resize(len_P + len_Q - 1);


    std::cout << "Lenght of resulting polynom PQ: " << len_PQ << std::endl;

    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_time = end_time - start_time;

    // We don't consider this in the total time because in a final version with the computations inside a ring of integers
    // This woud not be considered
    for (int i=0; i< len_P + len_Q-1 ; i++ ){
        DFT_PQ[i]= DFT_PQ[i].real();
    }

    /*std::cout << COLOR_BLUE << "[Parallel V" << version << "] " << COLOR_RESET << "Resulting polynomial coefficients after multiplication:" << std::endl << " * " << COLOR_PURPLE << "PQ = {";
    for (const auto& coeff : DFT_PQ) {
        std::cout << coeff << ", ";
    }
    std::cout << "}" << COLOR_RESET << std::endl;
*/
    std::cout << COLOR_BLUE << "[Parallel V" << version << "] " << COLOR_RESET << "Time taken for the multiplication: " << COLOR_GREEN << elapsed_time.count() << " seconds" << COLOR_RESET << std::endl;
  

    return DFT_PQ;
}


/**
 * @brief Multiplies two polynomials using the naive approach.
 * 
 * @param P First polynomial represented as a vector of complex numbers.
 * @param Q Second polynomial represented as a vector of complex numbers.
 * @return Resultant polynomial coefficients after multiplication.
 */
std::vector<std::complex<double>> multiply_naive(std::vector<std::complex<double>>& P, std::vector<std::complex<double>>& Q) {
    /*std::cout << "Starting polynomials: " << std::endl;
    std::cout << "  * " << COLOR_PURPLE << "P = {";
    for (const auto& coeff : P) {std::cout << coeff << ", ";}
    std::cout << "}" << COLOR_RESET << std::endl;
    std::cout << "  * " << COLOR_PURPLE << "Q = {";
    for (const auto& coeff : Q) {std::cout << coeff << ", ";}
    std::cout << "}" << COLOR_RESET << std::endl;*/

    auto start_time = std::chrono::steady_clock::now();

    int resultSize = P.size() + Q.size() - 1;
    std::vector<std::complex<double>> result(resultSize, 0);

    for (size_t i = 0; i < P.size(); ++i) {
        for (size_t j = 0; j < Q.size(); ++j) {
            result[i + j] += P[i] * Q[j];
        }
    }

    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_time = end_time - start_time;
    
    /*std::cout << COLOR_BLUE << "[Sequential] " << COLOR_RESET << "Resulting polynomial coefficients for sequential multiplication" << std::endl << " * " << COLOR_PURPLE << "PQ = {";
    for (const auto& coeff : result) {
        std::cout << coeff << ", ";
    }
    std::cout << "}" << COLOR_RESET << std::endl;
*/
    std::cout << COLOR_BLUE << "[Naive version] " << COLOR_RESET << "Time taken for the naive multiplication: " << COLOR_GREEN << elapsed_time.count() << " seconds" << COLOR_RESET << std::endl;
   
    return result;
}


std::vector<std::complex<double>> generateRandomPolynomial(int length) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 20);

    std::vector<std::complex<double>> polynomial(length);
    for (int i = 0; i < length; ++i) {
        polynomial[i] = {static_cast<double>(dis(gen)), 0.0};
    }
    return polynomial;
}
