
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <tuple>
#include <error.h>

using namespace std;

// contiene una funzione che preso in input il file txt DB_credentials.csv ritorna una tupla con i dati del database 
// (username e password) per la connessione
// file strutturato in questo modo:
// DB_USER=nome_utente
// DB_PASSWORD=password

inline tuple<string, string> getCredentials(string filename){
    string line;
    ifstream file;
    file.open(filename);
    string user;
    string password;
    if (file.is_open()){
        while (getline(file, line)){
            if (line.find("DB_USER=") != string::npos){
                user = line.substr(8);
            }
            if (line.find("DB_PASSWORD=") != string::npos){
                password = line.substr(12);
            }
        }
    }
    else {
        // stampa un messaggio di errore se non riesce ad aprire il file
        error(1, 0, "Errore nell'apertura del file");
    }
    file.close();
    return make_tuple(user, password);
}
