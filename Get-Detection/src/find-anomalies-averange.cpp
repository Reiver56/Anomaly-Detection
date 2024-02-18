#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include  <pqxx/pqxx>
#include "get-global.cpp"



using namespace std;
void findAnomalies(vector<double> values,vector<string> streamName,vector<vector<int>> ranges, double k) {
    double averange = getGlobal(values);
    double standardDeviation = calculateStandardDeviation(values, averange);
    cout << "Media: " << averange << endl;
    for (int i = 0; i < values.size(); i++){
        if (values[i] > averange + k * standardDeviation || values[i] < averange - k * standardDeviation){
            cout << "Anomalia rilevata nella stream: \n  " << streamName[i] << " con valore " << values[i] << " nel tempo " << ranges[i][0] << " - " << ranges[i][1] << endl;
        }
        
    }

}

