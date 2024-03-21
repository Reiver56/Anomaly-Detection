#include "../../include/database-methods.hpp"
#include "../../include/database-classes.hpp"
#include "../../get-credentials.cpp"


int main() {
   // prendo le credenziali del database
    tuple<string, string> credentials = getCredentials("../DB_credentials.csv");
    string user = get<0>(credentials); string password = get<1>(credentials);
    cout << "User: " << user << " Password: " << password << endl;

    // inizializza il db di redis -DA FARE-
    try {
        // Connessione al database di default per creare un nuovo database
        // Sostituisci le seguenti stringhe con le tue credenziali
        pqxx::connection C("host=localhost user="+user+" password="+password+ " dbname=postgres");
        if (C.is_open()) {
            //std::cout << "Connesso a " << C.dbname() << std::endl;
        } else {
            std::cout << "Non posso connettermi al database." << std::endl;
            return 1;
        }
        // Creazione del database
        if (!databaseExists(C)){
            createDatabase(C);
            std::cout << "Database creato con successo" << std::endl;
        }
        else{
            std::cout << "Database già esistente" << std::endl;
        }

        pqxx::connection C1("dbname = anomaly_detection user = " + user+ " password = " + password + " hostaddr = 127.0.0.1 port = 5432");

        createTables(C1); // Crea le tabelle del database
        C.disconnect(); // Chiudi la connessione
        C1.disconnect();
        
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

    
