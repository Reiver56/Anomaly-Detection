#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include  "get-from-redis-1.cpp"
#include  "get-stream-from-redis-2.cpp"
#include  "get-avarange.cpp"
#include  <pqxx/pqxx>
#include <chrono>



using namespace std::chrono;
using namespace std;
const int windowSize = getInfoFromRedis();
int main(){
    auto start = high_resolution_clock::now();
    int window = getInfoFromRedis(); int windowStart = 0;  // prendo la finestra da redis
    cout << window << endl;
    vector<string> streams = getStreamFromRedis(); // prendo i nomi di tutte le stream
    vector<double> avaranges; // questo vettore conterrà tutte le medie
    vector<vector<int>> windowSlot; // questo array conterrà dei vettori di interi che rappresentano le finestre da che punto a che punto stanno
    for (int i = 0; i < streams.size(); i++){
        double avarange = getAvarange(streams[i], windowSize);
        avaranges.push_back(avarange);
        windowSlot.push_back({windowStart, window});
        window++; windowStart++; 
        //cout << "Avarange of " << streams[i] << " is: " << avarange << endl;
        //cout << "Window: " << windowSlot[i][0] << " - " << windowSlot[i][1] << endl;
    }
    // mi connetto al database
    // cout << streams.size() << endl;
    
     try {
        // Connessione al database
        pqxx::connection C("dbname = anomaly-detection user = postgres password = matteus67 hostaddr = 127.0.0.1 port = 5432");
        if (C.is_open()) {
            std::cout << "Connessione al database riuscita: " << C.dbname() << std::endl;
        } else {
            std::cout << "Non è possibile connettersi al database." << std::endl;
            return 1;
        }
        // faccio un Trucate della tabella averanges
        pqxx::work W(C);
        std::string sql = "TRUNCATE averanges;";
        W.exec(sql);
        W.commit();
        // mando al DB le medie che ho in avaranges con tempS che corrisponde a windowSlot[i][0] e tempE a windowSlot[i][1], mentre la primary key (my_ stream) si trova scorrendo l'array streams
        for (int i = 0; i < streams.size(); i++){
            pqxx::work W(C);
    
            std::string sql = "INSERT INTO averanges (mystream, temps, tempe, averange) VALUES ($1, $2, $3, $4);";
    
            // Eseguo il comando SQL con i valori delle variabili come parametri
            W.exec_params(sql, streams[i], windowSlot[i][0], windowSlot[i][1], avaranges[i]);
            W.commit();
            //std::cout << "Records created successfully" << std::endl;
        }
        
        

        C.disconnect (); // Chiudi la connessione
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    cout << "Tempo di esecuzione: " << duration.count() << " secondi" << endl;
    return 0;
   
}
