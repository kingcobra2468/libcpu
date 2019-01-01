#include <cstdlib>
#include <cstdio>
#include <string>
#include <fstream>
#include <regex>
#include <vector>
#include <iostream>
#include <numeric>
#include <chrono>
#include <thread>
#include <utility>
#include "cpuLib.hpp"

cpuUsage::cpuUsage(){
    numOfCpus();
    populateUsage();
}

std::vector<float> cpuUsage::usage(int secDelay){
    std::vector<float> cpuUse;
    float difTotal, difIdle;
    for(int j = 0; j < 3; j++){ //run twise to get usage;
    getCpuValues();
    for(int i = 0; i < cores; i++){
        usageNew[i] = std::make_pair(std::accumulate(rawData[i].begin(),rawData[i].begin()+8, 0), rawData[i][3]); 
        difTotal = usageNew[i].first - usageOld[i].first;
        difIdle = usageNew[i].second - usageOld[i].second;
        if(j+1 == 2){
        cpuUse.push_back(100 * (1-difIdle/ difTotal)) ;
        }
    } 
    usageOld = usageNew;
    sleep(secDelay);
    }
    //std::cout << cpuUse.size();
    return cpuUse;
}

void cpuUsage::usageVisualLoop(int secDelay){
    std::vector<float> cpuUse;
    float difTotal, difIdle;
    while(true){
        getCpuValues();
        for(int i = 0; i < cores; i++){
            usageNew[i] = std::make_pair(std::accumulate(rawData[i].begin(),rawData[i].begin()+8, 0), rawData[i][3]); 
            difTotal = usageNew[i].first - usageOld[i].first;
            difIdle = usageNew[i].second - usageOld[i].second;
            printf("USAGE %s: %f\n", ((i==0) ? "CPU " : "CPU" + std::to_string(i-1)).c_str(), 100 * (1-difIdle/difTotal)); 
        }
        sleep(2);
        usageOld = usageNew;
        system("clear"); 
    }
} 

void cpuUsage::sleep(int seconds){
    std::this_thread::sleep_for(std::chrono::milliseconds(seconds*1000));
}

void cpuUsage::numOfCpus(){
    std::string line;
    std::ifstream readFile("/proc/stat");
    std::getline(readFile, line);
    while (std::regex_search(line, static_cast<std::regex>("^(cpu)"))){
        cores++;
        std::getline(readFile, line);
    };
};          

void cpuUsage::getCpuValues(){
    std::string line;
    std::ifstream readFile("/proc/stat");
    std::vector<int> temp;
    for(int i = 0; i < cores; i++){
        std::getline(readFile, line);
        std::string result = std::regex_replace(line, static_cast<std::regex>("[cpu]+[0-9]*\\s"), "");
        std::istringstream iss(result);
        int tmp;
        while(iss >> tmp) {
            temp.push_back(tmp);
        };
        rawData.emplace(rawData.begin()+i, temp);
        temp.erase(temp.begin(), temp.end());
    }
};
void cpuUsage::populateUsage(){
    usageNew.reserve(cores);
    usageOld.reserve(cores);
    for(int i =0; i < cores; i++) {
    usageOld.push_back(std::make_pair(0,0));
    usageNew.push_back(std::make_pair(0,0));
    };
};
