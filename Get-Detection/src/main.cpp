#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include "get-covariance.cpp"
#include "get-averange.cpp"
#include "get-temp-from-db.cpp"
#include "get-stream-from-db.cpp"
#include "get-stream-from-db-cov.cpp"
#include "find-anomalies-averange.cpp"
#include "find-anomalies-covariance.cpp"
#include "get-temp1.cpp"
#include "get-temp2.cpp"
#include  <pqxx/pqxx>
#include <chrono>



using namespace std::chrono;
using namespace std;
 
int main(){
    auto start = high_resolution_clock::now();
    //-----------------------------------------------------------------------
    // Anomaly detection per le medie
    //-----------------------------------------------------------------------
    // metto in diversi array le informazioni che mi servono
    double k = 3; // Soglia di deviazione standard
    cout << "Anomaly detection per le medie" << endl;
    vector<string> streams = getStreamsFromDB(); // array delle stringhe
    vector<vector<int>> temps = getTempsFromDB(); // array delle temperature
    vector<double> avaranges = getAverange(); // array delle medie
    
    
    findAnomalies(avaranges, streams, temps, k);
    //-----------------------------------------------------------------------
    // Anomaly detection per le covarianze
    //-----------------------------------------------------------------------
    // metto in diversi array le informazioni che mi servono
    cout << "Anomaly detection per le covarianze" << endl;
    vector<double> covariances = getCovariance(); // array delle covarianze
    vector<vector<string>> StreamCovariances = getStreamCovariance(); // array delle stringhe
    vector<vector<int>> tempsCovariances1 = getTemp1(); // array dei tempi 1
    vector<vector<int>> tempsCovariances2 = getTemp2(); // array dei tempi 2
    findAnomaliesCov(covariances, StreamCovariances, tempsCovariances1, tempsCovariances2, k); // cerca le anomalie
    //-------------------------------------------------------------------------
     auto stop = high_resolution_clock::now();
     auto duration = duration_cast<seconds>(stop - start);
     cout << "Tempo di esecuzione: " << duration.count() << " secondi" << endl;
    //-------------------------------------------------------------------------
    
}