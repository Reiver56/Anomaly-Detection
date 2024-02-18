#include <iostream>
#include <pqxx/pqxx>

int deleteData() {
    try {
        // Connessione al database
        pqxx::connection C("dbname = anomaly-detection user = postgres password = matteus67 hostaddr = 127.0.0.1 port = 5432");
        if (C.is_open()) {
            std::cout << "Connessione al database riuscita: " << C.dbname() << std::endl;
        } else {
            std::cout << "Non è possibile connettersi al database." << std::endl;
            return 1;
        }

        // Crea un oggetto di transazione
        pqxx::work W(C);

        // Esegue il comando TRUNCATE
        W.exec("TRUNCATE averanges RESTART IDENTITY CASCADE;");

        // Conferma la transazione
        W.commit();

        std::cout << "Table troncata con successo" << std::endl;

        C.disconnect (); // Chiudi la connessione
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
