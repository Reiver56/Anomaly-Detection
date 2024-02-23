#include <iostream>
#include <pqxx/pqxx>
#include <stdlib.h>


int main() {
    int init = system("sudo service postgresql start"); 
    try {
        // Connessione al database
        pqxx::connection C("dbname = anomaly-detection user = postgres password = REPLACE_PW hostaddr = 127.0.0.1 port = 5432");
        if (C.is_open()) {
            std::cout << "Connessione al database riuscita: " << C.dbname() << std::endl;
        } else {
            std::cout << "Non è possibile connettersi al database." << std::endl;
            return 1;
        }

        // Crea un oggetto transazione
        pqxx::work W(C);
        
        // Creo la tabella se non esiste già di averanges
        std::string sql = "CREATE TABLE IF NOT EXISTS averanges ("
                          "MYSTREAM VARCHAR PRIMARY KEY     NOT NULL,"
                          "TEMPS          INT    NOT NULL,"
                          "TEMPE          INT     NOT NULL,"
                          "AVERANGE       REAL    NOT NULL);";
       
        W.exec(sql);
        W.commit(); // Conferma la transazione
        std::cout << "Tabella averanges creata con successo" << std::endl;
        pqxx::work W2(C);
        
        // Creo la tabella se non esiste già di averanges
        std::string sql2 = "CREATE TABLE IF NOT EXISTS covariances ("
                          "ID             INT PRIMARY KEY     NOT NULL,"
                          "WIND1          VARCHAR     NOT NULL,"
                          "WIND2          VARCHAR     NOT NULL,"
                          "TEMP11         INTEGER     NOT NULL,"
                          "TEMP12         INTEGER     NOT NULL,"
                          "TEMP21         INTEGER     NOT NULL,"
                          "TEMP22         INTEGER     NOT NULL,"
                          "COVARIANCE     DOUBLE PRECISION    NOT NULL);";
        
        W2.exec(sql2);
        W2.commit(); // Conferma la transazione
        std::cout << "Tabella covariances creata con successo" << std::endl;
        

        C.disconnect (); // Chiudi la connessione
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    
    return init;
}

    
