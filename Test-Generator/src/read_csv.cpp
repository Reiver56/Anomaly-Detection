// questa funzione legge un file csv e restituisce una matrice n x 3
// dove n è il numero di righe del file csv e 3 è il numero di colonne

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;
// funzione per eliminare gli spazi bianchi
string deleteWhiteSpace(string str) {
    str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
    return str;
}
// funzione per leggere un file csv
vector<vector<string>> read_csv(string filename) {
    vector<vector<string>> data;
    ifstream file(filename
    );
    if (!file.is_open()) throw runtime_error("Could not open file");
    string line;
    
    while (getline(file, line)) {
        vector<string> row;
        stringstream ss(line);
        string cell;
        while (getline(ss, cell, ',')) {
            cell = deleteWhiteSpace(cell);
            row.push_back(cell);
            
        }
        data.push_back(row);
        
    }
    file.close();
    return data;
}
