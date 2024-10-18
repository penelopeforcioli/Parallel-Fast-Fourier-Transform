#include "compression.h"
#include "multiplication.h"
#include "full_dft.h"
#include "plot.h"
#include "data/import_data.h"
#include "data/select_dataset.h"

#include <iostream>
#include <string>
#include <vector>
#include <complex>
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;

int p = 4;

void runComputation(const std::string& computation, int version, bool parallel, const std::string& mode) {
    loadSelectedDataset();

    if (data_file.empty()) {
        std::cerr << "No dataset selected. Please run './compute dataset' to select a dataset." << std::endl;
        return;
    }

    std::vector<std::complex<double>> data;
    std::vector<std::complex<double>> original_data;

    if (computation == "compression") {
        data = readWeatherData(data_file);
        original_data = data;
        int k = 1500; // Example value for k
        approximation(data, k, parallel, version);
        plot_DFT(original_data, data, mode, computation, version);

    } else if (computation == "full_dft") {
        data = readWeatherData(data_file);
        original_data = data;
        full_dft(data, parallel, version);
        plot_DFT(original_data, data, mode, computation, version);

    } else if (computation == "multiplication") {
        std::vector<std::complex<double>> P = generateRandomPolynomial(2*2); 
        std::vector<std::complex<double>> Q = generateRandomPolynomial(2*2);
        std::vector<std::complex<double>> result1;
        std::vector<std::complex<double>> result2;
    
        result1= multiply(P, Q, version);
        result2 = multiply_naive(P, Q); 
      
    } else {
        std::cerr << "Invalid computation: " << computation << std::endl;
        return;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <command> [<args>]" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "dataset") {
        if (argc == 2) {
            listDatasets();
        } else if (argc == 3) {
            int dataset_num = std::stoi(argv[2]);
            selectDataset(dataset_num);
        } else {
            std::cerr << "Usage: " << argv[0] << " dataset [<num>]" << std::endl;
        }

    } else {

        if (argc != 4) {
            std::cerr << "Usage: " << argv[0] << " <sequential|parallel> <computation> <version>" << std::endl;
            return 1;
        }

        std::string mode = argv[1];
        std::string computation = argv[2];
        int version = std::stoi(argv[3]);
        bool parallel;

        if (mode == "sequential") {
            parallel = false;
            if (version != 1 && version != 2) {
                std::cerr << "Invalid version for sequential computation: version " << version << " does not exist, select from {1, 2}." << std::endl;
                return 1;
            }

        } else if (mode == "parallel") {
            parallel = true;
            if (version != 0 && version != 1 && version != 2) {
                std::cerr << "Invalid version for parallel computation: version " << version << " does not exist, select from {0, 1, 2}." << std::endl;
                return 1;
            }

        } else {
            std::cerr << "Invalid mode: " << mode << std::endl;
            std::cerr << "Usage: " << argv[0] << " <sequential|parallel> <computation> <version>" << std::endl;
            return 1;
        }

        runComputation(computation, version, parallel, mode);
        return 0;
    }
}
