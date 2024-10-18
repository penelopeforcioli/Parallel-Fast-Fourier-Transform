#ifndef SEQUENTIAL_DFT_V2_H
#define SEQUENTIAL_DFT_V2_H

#include <complex>
#include <vector>

void DFT_V2(std::vector<std::complex<double>>& data, int N);
void invDFT_V2(std::vector<std::complex<double>>& data);

#endif
