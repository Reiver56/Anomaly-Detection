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

inline vector<double> getAverangeFromDB(vector<int> times, string user, string password){
    vector<double> averanges;
    
    try {
        // Connessione al database
        pqxx::connection C("dbname = anomaly_detection user = " + user+ " password = " + password + " hostaddr = 127.0.0.1 port = 5432");
        if (C.is_open()) {
            //std::cout << "Connessione al database riuscita: " << C.dbname() << std::endl;
        } else {
            std::cout << "Non è possibile connettersi al database." << std::endl;
            return averanges;
        }
        // prendo le medie dal database e le metto in un vettore
        selectAverages(times[0],times[1],C,averanges);
        C.disconnect();
        return averanges;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return averanges;
    }
    
}
