#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
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

        getline(ss, value, ',');  // payment_type (no lo usamos)
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

    // Crear un canvas para los histogramas 1D
    TCanvas *c1 = new TCanvas("c1", "Histogramas 1D", 1200, 800);
    c1->Divide(3, 1);

    // Histogramas 1D
    TH1F *hPassenger = new TH1F("hPassenger", "Distribucion de pasajeros; Pasajeros; Frecuencia", 6, 0, 6);
    TH1F *hDistance = new TH1F("hDistance", "Distancia de viaje; Distancia (millas); Frecuencia", 50, 0, 20);
    TH1F *hTips = new TH1F("hTips", "Distribucion de propinas; Propina (USD); Frecuencia", 50, 0, 10);

    // Histogramas 2D
    TH2F *hFareVsDistance = new TH2F("hFareVsDistance", "Tarifa vs Distancia; Distancia (millas); Tarifa (USD)", 50, 0, 20, 50, 0, 100);
    TH2F *hTipVsFare = new TH2F("hTipVsFare", "Propina vs Tarifa; Tarifa (USD); Propina (USD)", 50, 0, 100, 50, 0, 20);
    TH2F *hTotalVsDistance = new TH2F("hTotalVsDistance", "Total vs Distancia; Distancia (millas); Total (USD)", 50, 0, 20, 50, 0, 150);

    for (const auto &trip : trips) {
        hPassenger->Fill(trip.passenger_count);
        hDistance->Fill(trip.trip_distance);
        hTips->Fill(trip.tip_amount);

        hFareVsDistance->Fill(trip.trip_distance, trip.fare_amount);
        hTipVsFare->Fill(trip.fare_amount, trip.tip_amount);
        hTotalVsDistance->Fill(trip.trip_distance, trip.total_amount);
    }

    // Guardar histogramas 1D en la primera página
    c1->cd(1);
    hPassenger->Draw();
    c1->cd(2);
    hDistance->Draw();
    c1->cd(3);
    hTips->Draw();

    c1->Print("resultados.pdf("); // Abre el PDF y guarda la primera página

    // Crear un segundo canvas para histogramas 2D
    TCanvas *c2 = new TCanvas("c2", "Histogramas 2D", 1200, 800);
    c2->Divide(3, 1);

    c2->cd(1);
    hFareVsDistance->Draw("COLZ");
    c2->cd(2);
    hTipVsFare->Draw("COLZ");
    c2->cd(3);
    hTotalVsDistance->Draw("COLZ");

    c2->Print("resultados.pdf)"); // Guarda la segunda página y cierra el PDF

    return 0;
}
