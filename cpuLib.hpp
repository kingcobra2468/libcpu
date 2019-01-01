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

class cpuUsage{
    public:
        cpuUsage();
        std::vector<float> usage(int secDelay);
        void usageVisualLoop(int secDelay);
        void sleep(int seconds);

    private:
        size_t cores = 0;
        std::vector<std::vector<int>> rawData;
        std::vector<std::pair <int,int>> usageNew, usageOld;
        void numOfCpus();     
        void getCpuValues();
        void populateUsage();
};