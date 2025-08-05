#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <mutex>
#include <vector>

std::mutex cout_mutex;

void progress_bar(int thread_num, int length)
{
    auto start = std::chrono::steady_clock::now();
    std::string bar;
    std::default_random_engine gen(std::random_device{}());
    std::uniform_int_distribution<int> err_chance(1, 10);

    for (int i = 0; i < length; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        bool is_error = (err_chance(gen) == 1);

        {
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << "Поток " << thread_num << ", ID: " << std::this_thread::get_id() << ": ";
            if (is_error)
            {
                bar += "\033[31m!\033[0m"; // красный
            }
            else
            {
                bar += "\033[32m#\033[0m"; // зелёный
            }
            std::cout << bar << std::endl;
        }
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Поток " << thread_num << " завершён. Время: " << duration << " мс" << std::endl;
    }
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");

    const int thread_count = 4;
    const int bar_length = 20;
    std::vector<std::thread> threads;
    for (int i = 0; i < thread_count; ++i)
    {
        threads.emplace_back(progress_bar, i + 1, bar_length);
    }
    for (auto& t : threads)
    {
        t.join();
    }
    return 0;
}
