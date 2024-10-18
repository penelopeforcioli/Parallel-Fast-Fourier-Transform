#ifndef SELECT_DATASET_H
#define SELECT_DATASET_H

#include <string>

extern std::string data_file;

void loadSelectedDataset();
void saveSelectedDataset(const std::string& dataset);
void listDatasets();
void selectDataset(int num);

#endif
