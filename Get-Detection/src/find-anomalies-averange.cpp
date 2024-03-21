#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include  <pqxx/pqxx>
#include "get-global.cpp"
#include "../../include/database-methods.hpp"



using namespace std;
inline void findAnomalies(vector<double> values,vector<vector<int>> ranges, vector<int> sensors ,double k,pqxx::connection &C) {
    double averange = getGlobal(values);
    double standardDeviation = calculateStandardDeviation(values, averange);
    for (int i = 0; i < values.size(); i++) {
        if (values[i] > averange + k * standardDeviation || values[i] < averange - k * standardDeviation) {
            std::cout << "Anomaly found in range: " << ranges[i][0] << " - " << ranges[i][1] << " with sensor: " << sensors[i] << std::endl;
            Average_anomaly anomaly(ranges[i][0], ranges[i][1], sensors[i]);
            insertAnomalyAverage(anomaly, C);

            }
            
        }

}

