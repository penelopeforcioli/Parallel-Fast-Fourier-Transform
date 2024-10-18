#ifndef PLOT_H
#define PLOT_H

#include <vector>
#include <complex>
#include <string>

void plot_DFT(const std::vector<std::complex<double>>& original_data, const std::vector<std::complex<double>>& modified_data, const std::string& mode, const std::string& computation, int version);
void plot_multiplication(const std::vector<std::complex<double>>& original_data1, const std::vector<std::complex<double>>& original_data2, const std::vector<std::complex<double>>& modified_data, const std::string& mode, const std::string& computation, int version);

#endif
