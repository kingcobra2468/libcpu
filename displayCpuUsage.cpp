#include <string>
#include <deque>
#include <iostream>
#include <iomanip>
#include <utility>
#include "cpuLib.hpp"

std::pair<std::string, float> giveTicks(float percentage){
    std::pair <std::string, float> obj;
    obj = std::make_pair("", percentage);
    for(int i = 0; i < percentage/2;i++){
        obj.first+="#";
    }
    return obj;
}

void printUsage(std::deque<std::pair<std::string, float>> * usage){
    for(int i = 0; i < usage->size(); i++){
        std::cout << usage->at(i).first << std::setw(50-usage->at(i).first.length()) << "]" << std::setprecision(2) << usage->at(i).second << "%\n";
        std::cout.flush();
    } 
}
int main(){
    cpuUsage mylib;
    std::deque<std::pair<std::string, float>> usageTicks(10,std::make_pair("",0));
    float cpuUseNum;
    while(true){
        float percent = mylib.usage(1.5).at(0);
        usageTicks.push_back(giveTicks(percent));
        system("clear");
        printUsage(&usageTicks);
        usageTicks.pop_front();
    }    

}
