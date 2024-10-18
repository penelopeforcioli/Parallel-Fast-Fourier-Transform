#include <vector>
#include <iostream>
#include <complex>

#include "p_transpose.h"


/**
 * @brief Chooses the bit-reversal sequence based on the number of processors.
 * 
 * @param p Number of processors.
 * @return A vector of integers representing the bit-reversal sequence.
 */
std::vector<int> chooseSequence(int p) {
    std::vector<int> sequence;

    switch (p) {
        case 2:
            sequence = {0, 1};
            break;
        case 4:
            sequence = {0, 2, 1, 3};
            break;
        case 8:
            sequence = {0, 4, 2, 6, 1, 5, 3, 7};
            break;
        case 16:
            sequence = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};
            break;
        case 32:
            sequence = {0, 16, 8, 24, 4, 20, 12, 28, 2, 18, 10, 26, 6, 22, 14, 30, 
                        1, 17, 9, 25, 5, 21, 13, 29, 3, 19, 11, 27, 7, 23, 15, 31};
            break;
        case 64:
            sequence = {0, 32, 16, 48, 8, 40, 24, 56, 4, 36, 20, 52, 12, 44, 28, 60, 
                        2, 34, 18, 50, 10, 42, 26, 58, 6, 38, 22, 54, 14, 46, 30, 62, 
                        1, 33, 17, 49, 9, 41, 25, 57, 5, 37, 21, 53, 13, 45, 29, 61, 
                        3, 35, 19, 51, 11, 43, 27, 59, 7, 39, 23, 55, 15, 47, 31, 63};
            break;
        default:
            std::cerr << "Unsupported value of p: " << p << std::endl;
            break;
    }

    return sequence;
}


/**
 * @brief Performs iterative bit-reversal on the input array.
 * 
 * @param input_array Input vector of complex numbers.
 * @param p Number of processors.
 * @param N Size of the input vector.
 * @return A vector of complex numbers representing the bit-reversed array.
 */
std::vector<std::complex<double>> iterative_bit_reversal(std::vector<std::complex<double>>& input_array, int p, int N) {
    std::vector<int> sequence = chooseSequence(p);
    std::vector<std::complex<double>> output_array(N);

    for (int i = 0; i < p; ++i) {
        int start_index = sequence[i] * (N / p);
        for (int j = 0; j < N / p; ++j) {
            int index = i + j * p;
            int input = start_index + j;
            output_array[input] = input_array[index];
        }
        
       
    }
    
    return output_array;
}
