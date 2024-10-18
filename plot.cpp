#include <TCanvas.h>
#include <TGraph.h>
#include <TAxis.h>
#include <TMultiGraph.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <complex>
#include <string>
#include <sstream>

#include "plot.h"

void plot_DFT(const std::vector<std::complex<double>>& original_data, const std::vector<std::complex<double>>& modified_data, const std::string& mode, const std::string& computation, int version) {
    int n = original_data.size();

    std::vector<double> x(n), real_original(n), real_modified(n);
    for (int i = 0; i < n; i++) {
        x[i] = i + 1;
        real_original[i] = original_data[i].real();
        real_modified[i] = modified_data[i].real();
    }

    std::ofstream dataFile("Plots/data_values.csv"); // save data to csv file if we want to check it
    dataFile << "Index, original_data, real_original, DFT_data, real_DFT\n";
    for (int i = 0; i < n; i++) {
        dataFile << x[i] << "; "
                 << original_data[i] << "; " << real_original[i] << "; "
                 << modified_data[i] << "; " << real_modified[i] << "\n";
    }
    dataFile.close();

    TGraph *gr1 = new TGraph();
    TGraph *gr2 = new TGraph();
    
    for (int i = 0; i < n; i++) {
        gr1->SetPoint(i, x[i], real_original[i]);
        if (real_modified[i] != 0) {
            gr2->SetPoint(gr2->GetN(), x[i], real_modified[i]); // Add non-zero points only to adjust for compression
        }
    }

    gr1->SetLineColor(kBlue);
    gr1->SetMarkerColor(kBlue);
    gr1->SetMarkerStyle(kFullCircle);

    gr2->SetLineColor(kRed);
    gr2->SetMarkerColor(kRed);
    gr2->SetMarkerStyle(kFullSquare);

    TMultiGraph *mg = new TMultiGraph();
    mg->Add(gr1, "P");
    mg->Add(gr2, "P");

    TCanvas *c = new TCanvas("c", "Visualisation", 800, 600);
    mg->Draw("A");
    
    std::ostringstream title;
    title << mode << " " << computation << " using V" << version;
    mg->SetTitle(title.str().c_str());
    
    mg->GetXaxis()->SetTitle("Days");
    mg->GetYaxis()->SetTitle("Measured Temperature");

    std::ostringstream oss;
    oss << "plots/" << mode << "_" << computation << "_V" << version << ".png";
    std::string filename = oss.str();

    c->SaveAs(filename.c_str());
}

void plot_multiplication(const std::vector<std::complex<double>>& original_data1, const std::vector<std::complex<double>>& original_data2, const std::vector<std::complex<double>>& modified_data, const std::string& mode, const std::string& computation, int version) {
    int n1 = original_data1.size();
    int n2 = original_data2.size();
    int m = modified_data.size();

    std::vector<double> x_original1(n1), real_original1(n1);
    std::vector<double> x_original2(n2), real_original2(n2);
    std::vector<double> x_modified(m), real_modified(m);

    for (int i = 0; i < n1; i++) {
        x_original1[i] = i + 1;
        real_original1[i] = original_data1[i].real();
    }
    for (int i = 0; i < n2; i++) {
        x_original2[i] = i + 1;
        real_original2[i] = original_data2[i].real();
    }
    for (int i = 0; i < m; i++) {
        x_modified[i] = i + 1;
        real_modified[i] = modified_data[i].real();
    }

    TGraph *gr1 = new TGraph(n1, x_original1.data(), real_original1.data());
    TGraph *gr2 = new TGraph(n2, x_original2.data(), real_original2.data());
    TGraph *gr3 = new TGraph(m, x_modified.data(), real_modified.data());

    gr1->SetLineColor(kBlue);
    gr1->SetMarkerColor(kBlue);
    gr1->SetMarkerStyle(kFullCircle);

    gr2->SetLineColor(kGreen);
    gr2->SetMarkerColor(kGreen);
    gr2->SetMarkerStyle(kFullCircle);

    gr3->SetLineColor(kRed);
    gr3->SetMarkerColor(kRed);
    gr3->SetMarkerStyle(kFullSquare);

    TCanvas *c = new TCanvas("c", "Visualisation", 1600, 600);
    c->Divide(2, 1);

    c->cd(1);
    gr1->Draw("AL");
    gr2->Draw("AL");
    gr1->SetTitle("Original Polynomial Coefficients");
    gr1->GetXaxis()->SetTitle("Index");
    gr1->GetYaxis()->SetTitle("Value");

    c->cd(2);
    gr3->Draw("AL");
    gr3->SetTitle("Resultant Polynomial Coefficients");
    gr3->GetXaxis()->SetTitle("Index");
    gr3->GetYaxis()->SetTitle("Value");

    std::ostringstream oss;
    oss << "plots/" << mode << "_" << computation << "_V" << version << ".png";
    std::string filename = oss.str();

    c->SaveAs(filename.c_str());
}
