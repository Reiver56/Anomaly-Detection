#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include  <pqxx/pqxx>
#include <cmath> // per sqrt e pow



using namespace std;

inline double getGlobal(std::vector<double> values){
    double sum = 0;
    for (int i = 0; i < values.size(); i++){
        sum += values[i];
    }
    return sum/values.size();
}
inline double calculateStandardDeviation(std::vector<double> values, double avarange){
    double sum = 0;
    for (int i = 0; i < values.size(); i++){
        sum += pow(values[i] - avarange, 2);
    }
    return sqrt(sum/values.size());
}
