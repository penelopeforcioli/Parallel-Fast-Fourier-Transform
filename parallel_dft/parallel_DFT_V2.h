#ifndef PARALLEL_DFT_V2_H
#define PARALLEL_DFT_V2_H

#include <complex>
#include <vector>

void DFT_parallel_V2(std::vector<std::complex<double>>& dft);
void invDFT_parallel_V2(std::vector<std::complex<double>>& data);

#endif
