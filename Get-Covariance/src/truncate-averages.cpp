#include <pqxx/pqxx>
#include <iostream>
#include "../../include/database-methods.hpp"



inline void truncateCovariance(std::string user, std::string password) {
    try {
        // Connessione al database
        pqxx::connection C("dbname = anomaly_detection user = " + user+ " password = " + password + " hostaddr = 127.0.0.1 port = 5432");
        if (C.is_open()) {
            //std::cout << "Connessione al database riuscita: " << C.dbname() << std::endl;
        } else {
            std::cout << "Non è possibile connettersi al database." << std::endl;
        }
        // TRUNCATE di averages
        truncateCovariances(C);
        //std::cout << "Covariances table truncated" << std::endl;
        C.disconnect (); // Chiudi la connessione
        //std::cout << "Connessione al database chiusa" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}