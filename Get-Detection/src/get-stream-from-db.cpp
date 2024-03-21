#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include  <pqxx/pqxx>
#include "../../include/database-methods.hpp"




using namespace std;

inline vector<int> getStreamsFromDB(string user, string password){
    vector<int> temps;
    // prendo le credenziali del database
    
    try {
        // Connessione al database
        pqxx::connection C("dbname = anomaly_detection user = " + user+ " password = " + password + " hostaddr = 127.0.0.1 port = 5432");
        // prendo temps e tempe dal database e le metto in un vettore
        selectSensorFromAverages(C,temps);

        C.disconnect();
        return temps;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return temps;
    }
    
}
