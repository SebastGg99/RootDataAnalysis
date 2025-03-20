#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <TCanvas.h>
#include <TH1F.h>
#include <TGraph.h>
#include <TApplication.h>
#include <TFile.h>

using namespace std;

// Estructura para almacenar los datos de cada viaje
struct TaxiTrip {
    int passenger_count;
    float trip_distance;
    float fare_amount;
    float tip_amount;
    float total_amount;
    int payment_type;
};

// Función para leer el CSV
vector<TaxiTrip> readCSV(const string &filename) {
    vector<TaxiTrip> trips;
    ifstream file(filename);
    string line;
    
    // Ignorar la primera línea (encabezados)
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string value;
        TaxiTrip trip;

        getline(ss, value, ',');  // VendorID (no lo usamos)
        getline(ss, value, ',');  // pickup_datetime (no lo usamos)
        getline(ss, value, ',');  // dropoff_datetime (no lo usamos)

        getline(ss, value, ','); trip.passenger_count = stoi(value);
        getline(ss, value, ','); trip.trip_distance = stof(value);
        getline(ss, value, ',');  // RatecodeID (no lo usamos)
        getline(ss, value, ',');  // store_and_fwd_flag (no lo usamos)
        getline(ss, value, ',');  // PULocationID (no lo usamos)
        getline(ss, value, ',');  // DOLocationID (no lo usamos)

        getline(ss, value, ','); trip.payment_type = stoi(value);
        getline(ss, value, ','); trip.fare_amount = stof(value);
        getline(ss, value, ',');  // extra (no lo usamos)
        getline(ss, value, ',');  // mta_tax (no lo usamos)
        getline(ss, value, ','); trip.tip_amount = stof(value);
        getline(ss, value, ',');  // tolls_amount (no lo usamos)
        getline(ss, value, ',');  // improvement_surcharge (no lo usamos)
        getline(ss, value, ','); trip.total_amount = stof(value);
        getline(ss, value, ',');  // congestion_surcharge (no lo usamos)

        trips.push_back(trip);
    }

    file.close();
    return trips;
}

int main(int argc, char **argv) {
    vector<TaxiTrip> trips = readCSV("primerasFilas.csv");

    // Crear un canvas para los histogramas
    TCanvas *c1 = new TCanvas("c1", "Histograms & Scatter Plots", 1200, 800);
    c1->Divide(2, 2);

    // 1️⃣ Histograma de la cantidad de pasajeros
    TH1F *hPassenger = new TH1F("hPassenger", "Distribución del número de pasajeros; Número de pasajeros; Frecuencia", 6, 0, 6);
    
    // 2️⃣ Histograma de distancias de viaje
    TH1F *hDistance = new TH1F("hDistance", "Distribución de distancia de viaje; Distancia (millas); Frecuencia", 50, 0, 20);
    
    // 3️⃣ Histograma de propinas
    TH1F *hTips = new TH1F("hTips", "Distribución de propinas; Propina (USD); Frecuencia", 50, 0, 10);

    // Gráficos de dispersión para correlaciones
    TGraph *gFareVsDistance = new TGraph();
    TGraph *gTipVsFare = new TGraph();
    TGraph *gTotalVsDistance = new TGraph();

    int index = 0;
    for (const auto &trip : trips) {
        hPassenger->Fill(trip.passenger_count);
        hDistance->Fill(trip.trip_distance);
        hTips->Fill(trip.tip_amount);

        gFareVsDistance->SetPoint(index, trip.trip_distance, trip.fare_amount);
        gTipVsFare->SetPoint(index, trip.fare_amount, trip.tip_amount);
        gTotalVsDistance->SetPoint(index, trip.trip_distance, trip.total_amount);
        index++;
    }

    // Abrir un archivo PDF para guardar todas las gráficas
    c1->Print("resultados.pdf("); // Abre el PDF en modo multipágina

    // Dibujar y guardar histogramas
    c1->cd(1);
    hPassenger->Draw();
    c1->Print("resultados.pdf"); 

    c1->cd(2);
    hDistance->Draw();
    c1->Print("resultados.pdf"); 

    c1->cd(3);
    hTips->Draw();
    c1->Print("resultados.pdf"); 

    // Crear un nuevo canvas para los gráficos de dispersión
    TCanvas *c2 = new TCanvas("c2", "Correlations", 1200, 800);
    c2->Divide(2, 2);

    c2->cd(1);
    gFareVsDistance->SetTitle("Tarifa vs Distancia; Distancia (millas); Tarifa (USD)");
    gFareVsDistance->SetMarkerStyle(7);
    gFareVsDistance->Draw("AP");
    c2->Print("resultados.pdf"); 

    c2->cd(2);
    gTipVsFare->SetTitle("Propina vs Tarifa; Tarifa (USD); Propina (USD)");
    gTipVsFare->SetMarkerStyle(7);
    gTipVsFare->Draw("AP");
    c2->Print("resultados.pdf"); 

    c2->cd(3);
    gTotalVsDistance->SetTitle("Total vs Distancia; Distancia (millas); Total (USD)");
    gTotalVsDistance->SetMarkerStyle(7);
    gTotalVsDistance->Draw("AP");
    c2->Print("resultados.pdf)"); // Cierra el PDF

    return 0;
}
