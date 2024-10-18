#ifndef P_TRANSPOSE_H
#define P_TRANSPOSE_H

#include <vector>
#include <complex>

std::vector<int> chooseSequence(int p);
std::vector<std::complex<double>> iterative_bit_reversal(std::vector<std::complex<double>>& input_array, int p, int N);

#endif // P_TRANSPOSE_H
