#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include  <pqxx/pqxx>
#include "get-global2.cpp"
#include "../../include/database-methods.hpp"


using namespace std;
inline void findAnomaliesCov(vector<double> values ,vector<vector<int>> ranges, vector<vector<int>> sensors ,double k,pqxx::connection &C) {
    double averange = getGlobal1(values);
    double standardDeviation = calculateStandardDeviation1(values, averange);
    std::cout << "verifico anomalie" << std::endl;
    for (int i = 0; i < values.size(); i++) {
        if (values[i] > averange + k * standardDeviation || values[i] < averange - k * standardDeviation) {
            std::cout << "Anomaly found in range: " << ranges[i][0] << " - " << ranges[i][1] << " with sensors: " << sensors[i][0] << " - " << sensors[i][1] << std::endl;
            Covariance_anomaly anomaly(ranges[i][0], ranges[i][1], sensors[i][0], sensors[i][1]);
            insertAnomalyCovariance(anomaly, C);
            }
        }

}

