#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>

void sumArrays(const std::vector<int>& a, const std::vector<int>& b,
    std::vector<int>& result, int numThreads)
{
    int size = static_cast<int>(a.size());
    int chunk = size / numThreads;

    auto worker = [&](int start, int end)
        {
            for (int i = start; i < end; ++i)
            {
                result[i] = a[i] + b[i];
            }
        };

    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; ++i)
    {
        int start = i * chunk;
        int end = (i == numThreads - 1) ? size : start + chunk;
        threads.emplace_back(worker, start, end);
    }

    for (auto& t : threads)
    {
        t.join();
    }
}

void runBenchmark(int size)
{
    std::vector<int> a(size, 1);
    std::vector<int> b(size, 2);
    std::vector<int> result(size);

    std::cout << "Размер массива: " << size << std::endl << std::endl;
    std::cout << std::setw(10) << "Потоки" << " | " << "Время (сек)" << std::endl;
    std::cout << "---------------------------" << std::endl;

    for (int threads : {1, 2, 4, 8, 16})
    {
        auto start = std::chrono::steady_clock::now();

        sumArrays(a, b, result, threads);

        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        std::cout << std::setw(10) << threads << " | " << elapsed.count() << std::endl;
    }

    std::cout << std::endl;
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");

    runBenchmark(1'000'000);
    runBenchmark(10'000'000);
    runBenchmark(100'000'000);

}
