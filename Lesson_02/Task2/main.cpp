#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <mutex>
#include <vector>
#include <iomanip>
#include "Timer.h"

std::mutex m;

void drawRowProgress(int row, int length)
{
    Timer t;
    std::default_random_engine gen(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 10);
    std::thread::id id = std::this_thread::get_id();

    {
        std::lock_guard<std::mutex> lock(m);
        consol_parameter::SetPosition(0, row + 1);
        std::cout << std::left << std::setw(3) << row << std::setw(7) << id << " ";
        std::cout.flush();
    }

    int start_x = 3 + 3 +3;

    for (int i = 0; i < length; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        bool is_error = dist(gen) == 1;

        {
            std::lock_guard<std::mutex> lock(m);
            consol_parameter::SetPosition(start_x + i, row + 1);
            if (is_error)
            {
                consol_parameter::SetColor(FOREGROUND_RED, 0);
                std::cout << "|";
            }
            else
            {
                consol_parameter::SetColor(FOREGROUND_GREEN, 0);
                std::cout << "■";
            }
            consol_parameter::SetColor(7, 0);
            std::cout.flush();
        }
    }

    {
        std::lock_guard<std::mutex> lock(m);
        consol_parameter::SetPosition(start_x + length + 5, row + 1);
        std::cout << std::fixed << std::setprecision(2) << t.elapsed_ms() << "s";
        std::cout.flush();
    }
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");

    const int thread_count = 4;
    const int bar_length = 20;
    std::vector<std::thread> threads;

    consol_parameter::SetPosition(0, 0);
    std::cout << "#     id        ProgressBar" << std::setw(7) << " " << "Time" << std::endl;

    for (int i = 0; i < thread_count; ++i)
    {
        threads.emplace_back(drawRowProgress, i, bar_length);
    }

    for (auto& t : threads)
    {
        t.join();
    }

    consol_parameter::SetPosition(0, thread_count + 2);
}
