#include <iostream>
#include <fstream>
#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TGraph.h>

void read_taxi_data() {
    // Crear un TTree
    TTree *tree = new TTree("taxi_data", "NYC Taxi Data");

    // Definir variables que almacenarán los datos
    Float_t trip_distance, fare_amount, total_amount, trip_duration;
    Int_t passenger_count, payment_type;

    // Leer el archivo CSV (se debe eliminar la primera línea del CSV manualmente si contiene encabezados)
    tree->ReadFile("yellow_tripdata_2018-01_sample.csv",
                   "trip_distance/F:fare_amount/F:total_amount/F:trip_duration/F:passenger_count/I:payment_type/I",
                   ',');

    // Asignar ramas a variables
    tree->SetBranchAddress("trip_distance", &trip_distance);
    tree->SetBranchAddress("fare_amount", &fare_amount);
    tree->SetBranchAddress("total_amount", &total_amount);
    tree->SetBranchAddress("trip_duration", &trip_duration);
    tree->SetBranchAddress("passenger_count", &passenger_count);
    tree->SetBranchAddress("payment_type", &payment_type);
}

tree->Print();  // Muestra la estructura del árbol
tree->Scan();   // Muestra algunas filas de datos

void plot_trip_duration() {
    TCanvas *c1 = new TCanvas("c1", "Distribución de Duración de Viajes", 800, 600);
    TH1F *h1 = new TH1F("h1", "Distribución de la Duración de Viajes;Duración (minutos);Frecuencia", 50, 0, 120);

    Float_t trip_duration;
    tree->SetBranchAddress("trip_duration", &trip_duration);

    Long64_t nentries = tree->GetEntries();
    for (Long64_t i = 0; i < nentries; i++) {
        tree->GetEntry(i);
        if (trip_duration > 0 && trip_duration <= 120) {
            h1->Fill(trip_duration);
        }
    }

    h1->Draw();
    c1->SaveAs("trip_duration_hist.pdf");
}

void plot_trip_distance() {
    TCanvas *c2 = new TCanvas("c2", "Distribución de Distancia de Viajes", 800, 600);
    TH1F *h2 = new TH1F("h2", "Distribución de la Distancia de Viajes;Distancia (millas);Frecuencia", 50, 0, 50);

    Float_t trip_distance;
    tree->SetBranchAddress("trip_distance", &trip_distance);

    Long64_t nentries = tree->GetEntries();
    for (Long64_t i = 0; i < nentries; i++) {
        tree->GetEntry(i);
        if (trip_distance > 0 && trip_distance <= 50) {
            h2->Fill(trip_distance);
        }
    }

    h2->Draw();
    c2->SaveAs("trip_distance_hist.pdf");
}

void plot_distance_vs_duration() {
    TCanvas *c3 = new TCanvas("c3", "Distancia vs Duración", 800, 600);
    TGraph *graph = new TGraph();

    Float_t trip_distance, trip_duration;
    tree->SetBranchAddress("trip_distance", &trip_distance);
    tree->SetBranchAddress("trip_duration", &trip_duration);

    Long64_t nentries = tree->GetEntries();
    int index = 0;
    for (Long64_t i = 0; i < nentries; i++) {
        tree->GetEntry(i);
        if (trip_distance > 0 && trip_duration > 0) {
            graph->SetPoint(index++, trip_distance, trip_duration);
        }
    }

    graph->SetTitle("Distancia vs Duración de Viajes;Distancia (millas);Duración (minutos)");
    graph->Draw("AP");
    c3->SaveAs("distance_vs_duration.pdf");
}
void plot_fare_by_payment() {
    TCanvas *c4 = new TCanvas("c4", "Tarifas por Tipo de Pago", 800, 600);
    TH1F *h3 = new TH1F("h3", "Distribución de Tarifas por Tipo de Pago;Tipo de Pago;Frecuencia", 6, 0, 6);

    Int_t payment_type;
    Float_t total_amount;
    tree->SetBranchAddress("payment_type", &payment_type);
    tree->SetBranchAddress("total_amount", &total_amount);

    Long64_t nentries = tree->GetEntries();
    for (Long64_t i = 0; i < nentries; i++) {
        tree->GetEntry(i);
        if (total_amount > 0) {
            h3->Fill(payment_type, total_amount);
        }
    }

    h3->Draw();
    c4->SaveAs("fare_by_payment.pdf");
}
