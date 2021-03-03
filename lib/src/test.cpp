#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
using json = nlohmann::json;

int main(){
    std::ifstream input_json_file("sample.pt.json");    
    json j;
    input_json_file >> j;
    auto x = j["PointsToInfo"];
    for(auto el : x){
        std::cout << el["Procedure"]["FunctionName"] << std::endl;
    }
    return 0;
}