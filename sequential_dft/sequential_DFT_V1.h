#ifndef SEQUENTIAL_DFT_V1_H
#define SEQUENTIAL_DFT_V1_H

#include <complex>
#include <vector>

void DFT_V1(std::vector<std::complex<double>>& data, int N);
void invDFT_V1(std::vector<std::complex<double>>& data);

#endif
