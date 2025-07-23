#include <iostream>
#include <vector>
#include <numeric>
#include <execution>
#include <chrono>
#include <thread>

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");

    const int size = 1'000'000;
    std::vector<int> a(size);
    std::vector<int> b(size);
    std::vector<int> result(size);

    std::iota(a.begin(), a.end(), 0);
    std::iota(b.begin(), b.end(), 0);

    std::cout << "Доступно потоков: " << std::thread::hardware_concurrency() << std::endl;

    auto start = std::chrono::steady_clock::now();

    // подсчёт
    std::transform(std::execution::par, a.begin(), a.end(), b.begin(), result.begin(),
        [](int x, int y) { return x + y; });

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;

    // Проверка
    std::cout << "Сумма рассчитана. Пример: result[0] = " << result[0]
        << ", result[" << size - 1 << "] = " << result[size - 1] << std::endl;

    std::cout << "Затраченное время: " << elapsed_seconds.count() << " секунд" << std::endl;

    return 0;
}
