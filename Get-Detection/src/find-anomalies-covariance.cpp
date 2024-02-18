#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include  <pqxx/pqxx>
#include "get-global2.cpp"



using namespace std;
void findAnomaliesCov(vector<double> values,vector<vector<string>> streamName,vector<vector<int>> ranges1,vector<vector<int>> ranges2, double k) {
    double averange = getGlobal1(values);
    double standardDeviation = calculateStandardDeviation1(values, averange);
    cout << "Media: " << averange << endl;
    for (int i = 0; i < values.size(); i++){
        if (values[i] > averange + k * standardDeviation || values[i] < averange - k * standardDeviation){
            cout << "Anomalia rilevata nella stream: \n  " << streamName[i][0] << " - " << streamName[i][1] << " con valore " << values[i] << " nel tempo " << ranges1[i][0] << " - " << ranges1[i][1] << " e " << ranges2[i][0] << " - " << ranges2[i][1] << endl;
        } 
    }
    

}

