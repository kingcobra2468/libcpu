#include <cstdlib>
#include <cstdio>
#include <string>
#include <fstream>
#include <regex>
#include <vector>
#include <array>
#include <iostream>
#include <numeric>
#include <chrono>
#include <thread>

int numOfCpus(){
    int cpus = 0;
    std::string line;
    std::ifstream readFile("/proc/stat");
    std::getline(readFile, line);
    while (std::regex_search(line, static_cast<std::regex>("^(cpu)"))){
        cpus++;
        std::getline(readFile, line);
    };
    return cpus;
};

std::vector<std::vector<int>> getCpuValues(size_t cores){
    std::vector<std::vector<int>> cpuUse;
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
        cpuUse.push_back(temp);
        temp.erase(temp.begin(), temp.end());
    }
    return cpuUse;
}

//create vector with important values to store as old

int main(){
    size_t cores = numOfCpus();
    std::vector<std::vector<int>> rawData;
    std::array<std::array<int, 3>, 9> usageNew, usageOld;
    for(int i =0; i < usageOld.size(); i++) usageOld[i].fill(0);
    float difTotal, difIdle;
    while(true){
        rawData=getCpuValues(cores);
        for(int i = 0; i < cores; i++){
            usageNew.at(i).at(0) = std::accumulate(rawData[i].begin(),rawData[i].begin()+8, 0);
            usageNew.at(i).at(1) = rawData.at(i).at(3); //+ rawData.at(i).at(4); 
            usageNew.at(i).at(2) = usageNew.at(i).at(0) + usageNew.at(i).at(1);
            difTotal = usageNew.at(i).at(0) - usageOld.at(i).at(0);
            difIdle = usageNew.at(i).at(1) - usageOld.at(i).at(1);
            std::cout << difIdle;
            //std::cout <<usageOld.at(i).at(2) << " " << usageNew.at(i).at(1) << std::endl; verbose
            printf("USAGE %s: %f\n", ((i==0) ? "CPU " : "CPU" + std::to_string(i-1)).c_str(), 100 * (1-difIdle/ difTotal)); //(1000*(difTotal-difIdle)/difTotal+5)/10); //
        } 
        usageOld = usageNew;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        system("clear");
        //std::cout << "\033[2J\033[1;1H";
        }
}