#ifndef PARALLEL_DFT_V0_H
#define PARALLEL_DFT_V0_H

#include <vector>
#include <complex>

void DFT_parallel_V0(std::vector<std::complex<double>>& P);
void invDFT_parallel_V0(std::vector<std::complex<double>>& data);

#endif
