#ifndef MULTIPLICATION_H
#define MULTIPLICATION_H

#include <vector>
#include <complex>

std::vector<std::complex<double>> multiply(std::vector<std::complex<double>> P, std::vector<std::complex<double>> Q, int version);
std::vector<std::complex<double>> multiply_naive(std::vector<std::complex<double>>& P, std::vector<std::complex<double>>& Q);
std::vector<std::complex<double>> generateRandomPolynomial(int length);

#endif
