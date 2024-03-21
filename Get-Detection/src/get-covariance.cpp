#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include  <pqxx/pqxx>
#include "../../include/database-methods.hpp"



using namespace std;

inline vector<double> getCovariance(pqxx::connection& C){
    vector<double> covariances;
        selectValidCovariances(C, covariances);
        return covariances;  
}
