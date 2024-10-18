#include "select_dataset.h"
#include <iostream>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

std::string data_file;

void loadSelectedDataset() {
    std::ifstream infile("selected_dataset.txt");
    if (infile.is_open()) {
        std::getline(infile, data_file);
        infile.close();
    }
}

void saveSelectedDataset(const std::string& dataset) {
    std::ofstream outfile("selected_dataset.txt");
    if (outfile.is_open()) {
        outfile << dataset;
        outfile.close();
    }
}

void listDatasets() {
    int index = 1;
    for (const auto& entry : fs::directory_iterator("data")) {
        if (entry.is_regular_file() && entry.path().extension() == ".csv") {
            std::cout << "[" << index++ << "] " << entry.path().stem().string() << std::endl;
        }
    }
}

void selectDataset(int num) {
    int index = 1;
    for (const auto& entry : fs::directory_iterator("data")) {
        if (entry.is_regular_file() && entry.path().extension() == ".csv") {
            if (index == num) {
                data_file = entry.path().string();
                saveSelectedDataset(data_file);
                std::cout << "Selected dataset: " << data_file << std::endl;
                return;
            }
            index++;
        }
    }
    std::cerr << "Invalid dataset number: " << num << std::endl;
}
