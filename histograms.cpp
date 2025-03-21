// Incluimos las librerías necesarias
#include <iostream>   // Para entrada y salida estándar
#include <fstream>    // Para trabajar con archivos
#include <sstream>    // Para manipulación de cadenas
#include <vector>     // Para utilizar vectores dinámicos
#include <TCanvas.h>  // Para crear lienzos y organizar gráficos en ROOT
#include <TH1F.h>     // Para histogramas 1D en ROOT
#include <TH2F.h>     // Para histogramas 2D en ROOT

using namespace std;

// Estructura para almacenar los datos que nos interesan de cada viaje en taxi
struct TaxiTrip {
    int passenger_count;   // Número de pasajeros en el viaje
    float trip_distance;   // Distancia total recorrida (en millas)
    float fare_amount;     // Tarifa cobrada por el viaje (en dólares)
    float tip_amount;      // Propina dada por el usuario (en dólares)
    float total_amount;    // Monto total pagado por el viaje (incluyendo tarifa y propina)
};

// Función para leer el archivo CSV y extraer los datos relevantes
vector<TaxiTrip> readCSV(const string &filename) {
    vector<TaxiTrip> trips;  // Vector de variables tipo TaxiTrip donde almacenamos los datos
    ifstream file(filename); // Abrimos el archivo en modo lectura
    string line;             // Variable para almacenar cada línea del archivo
    
    getline(file, line);  // Leemos y descartamos la primera línea (nombres de los campos)

    // Bucle para leer línea por línea el archivo CSV
    while (getline(file, line)) {
        stringstream ss(line); // Convertimos la línea en un flujo de datos
        TaxiTrip trip;         // Estructura donde almacenamos los datos de un viaje
        string temp;           // Variable temporal para almacenar valores descartados

        // Ignoramos los primeros tres valores del CSV (columnas no necesarias para mi análisis)
        for (int i = 0; i < 3; i++) getline(ss, temp, ',');

        // Extraemos los valores importantes y los convertimos a su tipo correspondiente
        getline(ss, temp, ','); trip.passenger_count = stoi(temp); // Número de pasajeros
        getline(ss, temp, ','); trip.trip_distance = stof(temp);   // Distancia del viaje

        // Ignoramos cinco valores intermedios 
        // (RatecodeID, store_and_fwd_flag, PULocationID, DOLocationID, payment_type)
        for (int i = 0; i < 5; ++i) getline(ss, temp, ',');

        // Extraemos la tarifa del viaje
        getline(ss, temp, ','); trip.fare_amount = stof(temp);

        // Ignoramos dos valores intermedios (extra, mta_tax)
        for (int i = 0; i < 2; ++i) getline(ss, temp, ',');

        // Extraemos la propina dada por el usuario
        getline(ss, temp, ','); trip.tip_amount = stof(temp);

        // Ignoramos dos valores intermedios (tolls_amount, improvement_surcharge)
        for (int i = 0; i < 2; ++i) getline(ss, temp, ',');

        // Extraemos el monto total pagado por el viaje
        getline(ss, temp, ','); trip.total_amount = stof(temp);

        // Ignoramos congestion_surcharge (si está presente)
        getline(ss, temp, ',');

        // Agregamos el viaje al vector de datos
        trips.push_back(trip);
    }

    file.close(); // Cerramos el archivo después de leerlo
    return trips; // Retornamos el vector con los datos de los viajes
}

// Función para generar gráficos a partir de los datos procesados
void grapher(vector<TaxiTrip> trips) {
    // Creación de histogramas 1D (frecuencia de valores individuales)
    TH1F *numberPassenger = new TH1F("numberPassenger", "Distribucion de numero de pasajeros; Pasajeros; Frecuencia", 6, 0, 6);
    TH1F *tripDistance = new TH1F("tripDistance", "Distancia del viaje; Distancia (millas); Frecuencia", 50, 0, 20);
    TH1F *totalAmount = new TH1F("totalAmount", "Distribucion del monto total; Total (USD); Frecuencia", 50, 0, 150);

    // Creación de histogramas 2D (relaciones entre variables)
    TH2F *FareVsDistance = new TH2F("FareVsDistance", "Tarifa vs Distancia; Distancia (millas); Tarifa (USD)", 50, 0, 20, 50, 0, 100);
    TH2F *TipVsPassenger = new TH2F("TipVsPassenger", "Propina vs Numero de Pasajeros; Pasajeros; Propina (USD)", 6, 0, 6, 50, 0, 20);
    TH2F *AmountVsTip = new TH2F("AmountVsTip", "Monto total vs Propina; Propina (USD); Total (USD)", 50, 0, 20, 50, 0, 150);

    // Llenado de histogramas con los datos de los viajes
    for (const auto &trip : trips) {
        numberPassenger->Fill(trip.passenger_count);
        tripDistance->Fill(trip.trip_distance);
        totalAmount->Fill(trip.total_amount);
        FareVsDistance->Fill(trip.trip_distance, trip.fare_amount);
        TipVsPassenger->Fill(trip.passenger_count, trip.tip_amount);
        AmountVsTip->Fill(trip.tip_amount, trip.total_amount);
    }

    // Creación de un lienzo  para los histogramas 1D
    TCanvas *c1 = new TCanvas("c1", "Histogramas 1D", 1200, 800);
    c1->Divide(3, 1); // Divide el canvas en 3 columnas y 1 fila
    
    // Dibujamos cada histograma en su respectiva sección del lienzo
    c1->cd(1);
    numberPassenger->Draw("HIST");
    c1->cd(2);
    tripDistance->Draw("HIST");
    c1->cd(3);
    totalAmount->Draw("HIST");

    // Guardamos la primera página del archivo PDF con los histogramas 1D
    c1->Print("resultados.pdf("); // Abre el PDF y guarda la primera página

    // Creación de un segundo lienzo para los histogramas 2D
    TCanvas *c2 = new TCanvas("c2", "Histogramas 2D", 1200, 800);
    c2->Divide(3, 1);
    
    // Dibujamos los histogramas 2D en el nuevo lienzo
    c2->cd(1);
    FareVsDistance->Draw("COLZ");
    c2->cd(2);
    TipVsPassenger->Draw("COLZ");
    c2->cd(3);
    AmountVsTip->Draw("COLZ");

    // Guardamos la segunda página del archivo PDF y cerramos el documento
    c2->Print("resultados.pdf)");
}

// Función principal del programa
int main() {
    
    vector<TaxiTrip> trips = readCSV("primerasFilas.csv"); // Cargamos los datos desde el archivo CSV
    grapher(trips); // Generamos los gráficos con los datos procesados
    
    return 0;
}
