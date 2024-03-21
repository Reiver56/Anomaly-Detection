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

inline vector<vector<int>> getTempsFromDB(pqxx::connection &C){
    vector<vector<int>> temps;
        getTempsAverages2(C,temps);
        return temps;

    }
    

