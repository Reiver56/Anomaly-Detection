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
#include "get-sensor.cpp"
#include  <pqxx/pqxx>
#include <unistd.h>
#include <hiredis/hiredis.h>
#include "../../include/database-methods.hpp"
#include "../../get-credentials.cpp"

using namespace std;
 
int main(){

    tuple<string, string> credentials = getCredentials("../../DB_credentials.csv");
    string user = get<0>(credentials); string password = get<1>(credentials);
    
    
    while(true){
        redisContext *c = redisConnect("127.0.0.1", 6379);
        if (c == NULL || c->err) {
            if (c) {
                cout << "Errore di connessione: " << c->errstr << endl;
                return 1;
            } else {
            cout << "Can't allocate redis context" << endl;
            return 1;
            }
        exit(1);
        }
        // faccio un TRUNCATE della tabella anomalies_averages e anomalies_covariances, perché le voglio aggiornare ad ogni ciclo
        try {
            pqxx::connection C("dbname = anomaly_detection user = " + user+ " password = " + password + " hostaddr = 127.0.0.1 port = 5432");
            pqxx::work W(C);
            W.exec("TRUNCATE anomalies_averages, anomalies_covariances");
            W.commit();
            sleep(1);
            //-----------------------------------------------------------------------
            // Anomaly detection per le medie
            //-----------------------------------------------------------------------
            double k = 2; // Soglia di deviazione standard
            vector<vector<int>> temps = getTempsFromDB(C); // array delle temperature
            vector<double> avaranges = getAverange(C); // array delle medie 
            vector<int> sensors = getSensorFromAverages(C); // array dei sensori
            findAnomalies(avaranges, temps, sensors,k,C);

            //-----------------------------------------------------------------------
            // Anomaly detection per le covarianze
            //-----------------------------------------------------------------------
            vector<double> covariances = getCovariance(C); // array delle covarianze
            vector<vector<int>> Temps = getStreamCovariance(C); // array delle stringhe
            vector<vector<int>> sensors1 = getSensorsFromCovariances(C); // array delle range
            findAnomaliesCov(covariances, Temps, sensors1 , k,C); // cerca le anomalie
            cout << "attendo se vuoi chiudere e vedere le anomalie(10 sec)" << endl;
            sleep(10);


            C.disconnect();
        }catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }

       
    
    }
}