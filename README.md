# CSE305-FFT-Project

## Overview

This project aims to implement and parallelize the computation of the Discrete Fourier Transform (DFT). The DFT is a fundamental tool in digital signal processing, and its efficient computation is crucial for a wide range of applications. This project explores both sequential and parallel implementations of DFT and evaluates their performance.

## Features

- **Sequential and Parallel DFT**: Implementations of DFT in both sequential and parallel forms.
- **Polynomial Multiplication**: Use of DFT for polynomial multiplication.
- **Data Compression**: Compression of data using DFT.
- **Visualization**: Plotting of original and transformed data using ROOT.
- **Dataset Selection**: Ability to select and change datasets dynamically.

## Directory Structure

```bash
├── data/                      # Directory containing data files
│   ├── import_data.cpp        # Method to import our weather data into a C++ array
│   ├── select_dataset.cpp     # Method to list and select datasets
│   └── Marseille_average_temps_for_2048_days.csv
├── parallel_dft/              # Directory containing parallel DFT implementation
│   ├── p_transpose.cpp
│   ├── parallel_DFT_V0.cpp
│   ├── parallel_DFT_V1.cpp
│   └── parallel_DFT_V2.cpp
├── sequential_dft/            # Directory containing sequential DFT implementation
│   ├── sequential_DFT_V1.cpp
│   └── sequential_DFT_V2.cpp
├── compression.cpp            # Data compression using DFT
├── full_dft.cpp               # Full DFT computation
├── import_data.cpp            # Data import functionality
├── main.cpp                   # Main program
├── multiplication.cpp         # Polynomial multiplication using DFT
├── plot.cpp                   # Plotting functionality
├── Makefile                   # Makefile for building the project
└── README.md                  # This README file
```


## Requirements

- **ROOT**: The project uses ROOT for plotting. Make sure you have ROOT installed. You can install ROOT from [here](https://root.cern/install/).

## Building the Project

1. **Clone the Repository**:
    ```bash
    git clone https://github.com/victor-lasocki/CSE305-FFT-Project.git
    cd CSE305-FFT-Project
    ```

2. **Build the Project**:
    ```bash
    make
    ```

## Running the Project

The executable `compute` can be run with the following syntax:

```bash
./compute <command> [<args>]
```

### Dataset Selection

To see the available datasets, simply call `./compute dataset` and a list of all datasets will be provided.
Then, to select one, enter `./comptue dataset {num}` with the number of the dataset you want to use.
Be sure to run this command at least once when starting the project.

For example, to use the 5th dataset in the list, run `./comptue dataset 5`.

### Computations and DFT

Once a dataset is selected, you can run a specified computation on the selected dataset using the following syntax:

```bash
./compute <mode> <computation> <version>
```

- **mode**: sequential or parallel
- **computation**: dft, compression, or multiplication
- **version**: an integer specifying the version of the implementation (0, 1 or 2)

NB: The sequential implementation has two versions (V1 and V2), whereas the parallel version has three (V0, V1 and V2).

### Examples

1. **Sequential DFT Computation (Version 1)**:
    **bash**
    ./compute sequential dft 1
    **end**

2. **Parallel DFT Computation (Version 0)**:
    **bash**
    ./compute parallel dft 0
    **end**

3. **Sequential Compression (Version 2)**:
    **bash**
    ./compute sequential compression 2
    **end**

