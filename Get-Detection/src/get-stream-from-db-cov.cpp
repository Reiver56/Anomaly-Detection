#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include  <pqxx/pqxx>



using namespace std;

vector<vector<string>> getStreamCovariance(){
    vector<vector<string>> temps;
    
    try {
        // Connessione al database
        pqxx::connection C("dbname = anomaly-detection user = postgres password = matteus67 hostaddr = 127.0.0.1 port = 5432");
        if (C.is_open()) {
            std::cout << "Connessione al database riuscita: " << C.dbname() << std::endl;
        } else {
            std::cout << "Non è possibile connettersi al database." << std::endl;
            return temps;
        }
        // prendo temps e tempe dal database e le metto in un vettore
        pqxx::work W(C);
        pqxx::result R = W.exec("SELECT wind1,wind2 FROM covariances");
        for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
            vector<string> temp;
            temp.push_back(c[0].as<string>());
            temp.push_back(c[1].as<string>());
            temps.push_back(temp);
        }
        
        W.commit();
        C.disconnect();
        return temps;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return temps;
    }
    
}