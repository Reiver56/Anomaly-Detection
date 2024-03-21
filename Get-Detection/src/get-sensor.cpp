#include <pqxx/pqxx>
#include <iostream>
#include <fstream>
#include <vector>
#include "../../include/database-methods.hpp"

inline std::vector<int> getSensorFromAverages(pqxx::connection& C){
    std::vector<int> sensors;
        selectSensorFromAverages(C,sensors);
        return sensors;
}

inline std::vector<std::vector<int>> getSensorsFromCovariances(pqxx::connection& C){
    std::vector<std::vector<int>> sensors;
        selectSensorsFromCovariances(C,sensors);
        return sensors;
}