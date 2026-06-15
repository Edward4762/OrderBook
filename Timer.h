#pragma once
#include<chrono>

class Timer
{
private:
    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;

public:
    void startTimer()
    {
        start = std::chrono::high_resolution_clock::now();
    }

    void endTimer()
    {
        end = std::chrono::high_resolution_clock::now();
    }

    int getTime()
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    }
};