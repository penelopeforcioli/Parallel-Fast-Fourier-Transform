#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <complex>
#include <vector>

bool compare(std::complex<double>& a, std::complex<double>& b);
std::vector<bool> precompress(std::vector<std::complex<double>>& x, int k);
double computeAbsoluteError(const std::vector<std::complex<double>>& vec1, const std::vector<std::complex<double>>& vec2);
void approximation(std::vector<std::complex<double>>& data, int k, bool parallel, int version);

#endif
