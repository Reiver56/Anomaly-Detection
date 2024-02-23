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
#include "get-temp-from-db.cpp"
#include  <pqxx/pqxx>
#include <chrono>



using namespace std::chrono;
using namespace std;

int main(){
    auto start = high_resolution_clock::now();
     redisContext *c = redisConnect("127.0.0.1", 6379);
    if (c == NULL || c->err) {
        if (c) {
            cout << "Errore di connessione: " << c->errstr << endl;
        } else {
            cout << "Can't allocate redis context" << endl;
        }
        exit(1);
    }
    vector<string> streams = getStreamFromRedis(c); 
    vector<double> averanges; vector<vector<int>> ranges;
    averanges = getAverangeFromDB(); ranges = getTempsFromDB();
    // metto nel database le covarianze
    try {
        // Connessione al database
        pqxx::connection C("dbname = anomaly-detection user = postgres password = REPLACE_PW hostaddr = 127.0.0.1 port = 5432");
        if (C.is_open()) {
            std::cout << "Connessione al database riuscita: " << C.dbname() << std::endl;
        } else {
            std::cout << "Non è possibile connettersi al database." << std::endl;
            return 1;
        }
        // voglio fare trucate della tabella covariances
        pqxx::work W(C);
        std::string sql = "TRUNCATE covariances;";
        W.exec(sql);
        W.commit();
        // mando al DB le covarianze
        for (int i = 0; i < streams.size()-1; i=i+2){
            pqxx::work W(C);
    
            std::string sql = "INSERT INTO covariances (id, wind1, wind2, temp11, temp12, temp21, temp22, covariance) VALUES ($1, $2, $3, $4, $5, $6, $7, $8);";
    
            // Eseguo il comando SQL con i valori delle variabili come parametri
            W.exec_params(sql, i, streams[i], streams[i+1],ranges[i][0],ranges[i][1], ranges[i+1][0], ranges[i+1][1], getCovariance(streams[i], streams[i+1], averanges[i], averanges[i+1], c));
            W.commit();
            //std::cout << "Records created successfully" << std::endl;
        }
        
        

        C.disconnect (); // Chiudi la connessione
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    // fine connessione a redis
    redisFree(c);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    cout << "Tempo di esecuzione: " << duration.count() << " secondi" << endl;
    return 0;
}
