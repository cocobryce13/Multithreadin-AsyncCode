#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
#include <string>

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

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");

    std::vector<int> sizes = { 1000, 10000, 100000, 1000000 };
    std::vector<int> threadCounts = { 1, 2, 4, 8, 16 };

    int cores = std::thread::hardware_concurrency();
    std::cout << "Количество аппаратных ядер - " << cores << std::endl << std::endl;

    std::cout << std::setw(10) << " "
        << std::setw(5) << "1000"
        << std::setw(16) << "10000"
        << std::setw(16) << "100000"
        << std::setw(16) << "1000000" << std::endl;

    for (int threads : threadCounts)
    {
        std::cout << threads << " потоков ";

        for (int size : sizes)
        {
            std::vector<int> a(size, 1);
            std::vector<int> b(size, 2);
            std::vector<int> result(size);

            auto start = std::chrono::steady_clock::now();
            sumArrays(a, b, result, threads);
            auto end = std::chrono::steady_clock::now();

            std::chrono::duration<double> elapsed = end - start;

            std::cout << std::fixed << std::setprecision(6) << elapsed.count() << "s\t";
        }

        std::cout << std::endl;
    }

}
