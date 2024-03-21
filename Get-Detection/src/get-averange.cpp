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

inline vector<double> getAverange(pqxx::connection &C){
    vector<double> averanges;
        selectValidAverages(C,averanges);
        return averanges;
}
