#ifndef FULL_DFT_H
#define FULL_DFT_H

#include <complex>
#include <vector>

double computeAbsoluteError(const std::vector<std::complex<double>>& vec1, const std::vector<std::complex<double>>& vec2);
void full_dft(std::vector<std::complex<double>>& data, bool parallel, int version);

#endif
