

#include <iostream>
#include <fstream>
#include <vector>
#include <complex>
#include <string>
#include <algorithm>

std::vector<std::complex<double>> readWeatherData(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::vector<std::complex<double>> data;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            if (!line.empty()) {
                double temp = std::stod(line);
                data.push_back(std::complex<double>(temp, 0.0));
            }
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
        exit(1);
    }

    return data;
}