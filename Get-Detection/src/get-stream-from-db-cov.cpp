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

inline vector<vector<int>> getStreamCovariance(pqxx::connection &C){
    vector<vector<int>> temps;
        pqxx::work W(C);
        pqxx::result R = W.exec("SELECT temp_start, temp_end FROM covariances");
        for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
            vector<int> temp;
            temp.push_back(c[0].as<int>());
            temp.push_back(c[1].as<int>());
            temps.push_back(temp);
        }
        
        W.commit();
        return temps;
    
}
