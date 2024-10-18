#ifndef PARALLEL_DFT_V1_H
#define PARALLEL_DFT_V1_H

#include <complex>
#include <vector>

void DFT_parallel_V1(std::vector<std::complex<double>>& dft);
void invDFT_parallel_V1(std::vector<std::complex<double>>& data);

#endif
