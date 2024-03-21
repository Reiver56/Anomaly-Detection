#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include  "get-from-redis-1.cpp"
#include  "get-stream-from-redis-2.cpp"
#include  "get-average.cpp"
#include  "get-sensor-from-redis.cpp"
#include "truncate-averages.cpp"
#include  <pqxx/pqxx>
#include <unistd.h> // permette di mettere in sleep il programma
#include "../../get-credentials.cpp"

using namespace std;

int main(){
    vector<string> streams;
    vector<int> sensors ;
    vector<int> times;
    vector<vector<int>> windowSlot;

    // prendo le credenziali del database
    tuple<string, string> credentials = getCredentials("../../DB_credentials.csv");
    string user = get<0>(credentials); string password = get<1>(credentials);
    
    // -----------------------------------

    // comincio a connettermi a redis
    redisContext *c = redisConnect("127.0.0.1", 6379);
    if (c == NULL || c->err) {
        if (c) {
            cout << "Errore di connessione: " << c->errstr << endl;
        } else {
            cout << "Can't allocate redis context" << endl;
        }
        exit(1);
    }
    
    truncateAverages(user, password); // funzione che inizializza il database quando viene avviato un nuovo test-generator
    
    while (true){
        vector<vector<double>> averages;
        while(isReady_(c) != "yes"){
        }
        isReady(c);
        //cout << "Nuova media" << endl;
        int window; int windowStart = 0;  // prendo la finestra da redis
        //cout << window << endl;
        streams = getStreamFromRedis(c); // prendo i nomi di tutte le stream
        sensors = getSensor(c); // prendo i sensori
        times = getTimes(c);
    

        for (int i = 0; i < streams.size(); i++){
            tuple<double, int> result = getAvarage(streams[i], window, c);
            // voglio prendere la media e la lunghezza del vettore
            double average = get<0>(result);
            int length = get<1>(result);
            if (length == 0 && average == 0){
                // pusho un valore arbitrario per segnalare che la finestra è vuota
                averages.push_back({0,1});
                continue;  
            }
            else{averages.push_back({average,0});}
            windowSlot.push_back({windowStart, window});
        }

    // qui si potrebbe mettere un metodo che viene iterato 
        try {
            // Connessione al database
            pqxx::connection C("dbname = anomaly_detection user = " + user+ " password = " + password + " hostaddr = 127.0.0.1 port = 5432");
            if (C.is_open()) {
                //std::cout << "Connessione al database riuscita: " << C.dbname() << std::endl;
            } else {
                std::cout << "Non è possibile connettersi al database." << std::endl;
                return 1;
            }
            // mando al DB le medie che ho in averages con tempS che corrisponde a windowSlot[i][0] e tempE a windowSlot[i][1], mentre la primary key (my_ stream) si trova scorrendo l'array streams
            for (int i = 0; i < streams.size(); i++){
                bool null_window = false;
                if (averages[i][1] == 1){
                    null_window = true;
                }
                // inserisco la media nel database
                Average average(times[0],times[1], sensors[i] ,streams[i],averages[i][0],null_window);
                //Average average(windowSlot[i][0], windowSlot[i][1], sensors[i], i, averages[i][0], null_window);
                insertAverage(average, C);
                }
                //una volta inserita la media nel database, posso inviare in un'altra stream redis 
                //che comunica a Get-Covariance che può fare il calcolo della covarianza su quella determinata finestra temporale
                isReady2(c, "yes");
        C.disconnect (); // Chiudi la connessione
        //cout << "Connessione al database chiusa" << endl;
        
        
        } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
        }
    }
   
}
