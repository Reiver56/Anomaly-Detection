#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include "get-stream-from-redis-2.cpp"
#include "get-covariance.cpp"
#include "get-averange-from-db.cpp"
#include "truncate-averages.cpp"
#include  <pqxx/pqxx>
#include <unistd.h> // permette di mettere in sleep il programma
#include "../../include/database-methods.hpp"
#include "../../get-credentials.cpp"

using namespace std;

int main(){
    redisContext *c = redisConnect("127.0.0.1", 6379);
    cout << "Sto in Get-Covariance" << endl;
    if (c == NULL || c->err) {
        if (c) {
            cout << "Errore di connessione: " << c->errstr << endl;
        } else {
            cout << "Can't allocate redis context" << endl;
        }
        exit(1);
    }
    // prendo le credenziali del database
    tuple<string, string> credentials = getCredentials("../../DB_credentials.csv");
    string user = get<0>(credentials); string password = get<1>(credentials);

    truncateCovariance(user, password); // funzione che inizializza il database quando viene avviato un nuovo test-generator
    while(true){
        while (isReady2(c) != "yes"){ 
        }

        setIsReady2(c);
        //cout << "Nuova covarianza" << endl;
        vector<string> streams = getStreamFromRedis(c);
        vector<double> averages; 
        vector<int> times = getTimes(c);
        vector<int> sensor = getSensor(c);

        averages = getAverangeFromDB(times, user, password); // questo prende le medie dal database, dato un intervallo di tempo
        try {
        // Connessione al database
            pqxx::connection C("dbname = anomaly_detection user = " + user+ " password = " + password + " hostaddr = 127.0.0.1 port = 5432");
            if (C.is_open()) {
                //std::cout << "Connessione al database riuscita: " << C.dbname() << std::endl;
            } else {
                std::cout << "Non è possibile connettersi al database." << std::endl;
                return 1;
            }
            // mando al DB le covarianze
            tuple<double, int> cov;
            for (int i = 0; i < streams.size()-1; i=i+2){
    
            // Eseguo il comando SQL con i valori delle variabili come parametri
                //cout << streams[i] << " " << streams[i+1] << endl;
                //cout << averages[i] << " " << averages[i+1] << endl;

                cov = getCovariance(streams[i], streams[i+1], averages[i], averages[i+1],c);

                if (get<1>(cov) == 0){
                    Covariance covariance(times[0],times[1],sensor[i],sensor[i+1],get<0>(cov),false);
                    insertCovariance(covariance,C);
                }
                else{
                    Covariance covariance(times[0],times[1],sensor[i],sensor[i+1],0,true);
                    insertCovariance(covariance,C);
                    
                }
                            //std::cout << "Records created successfully" << std::endl;
            }
        
        

        C.disconnect (); // Chiudi la connessione
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    }
    // fine connessione a redis
    redisFree(c);
    return 0;
}
