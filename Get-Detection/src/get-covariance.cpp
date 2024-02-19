#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include  <pqxx/pqxx>



using namespace std;

vector<double> getCovariance(){
    vector<double> covariances;
    
    try {
        // Connessione al database
        pqxx::connection C("dbname = anomaly-detection user = postgres password = DA_INSERIRE hostaddr = 127.0.0.1 port = 5432");
        if (C.is_open()) {
            std::cout << "Connessione al database riuscita: " << C.dbname() << std::endl;
        } else {
            std::cout << "Non è possibile connettersi al database." << std::endl;
            return covariances;
        }
        // prendo le medie dal database e le metto in un vettore
        pqxx::work W(C);
        pqxx::result R = W.exec("SELECT covariance FROM covariances");
        for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
            covariances.push_back(c[0].as<double>());
        }
        W.commit();
        C.disconnect();
        return covariances;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return covariances;
    }
    
}
