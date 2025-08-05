#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

std::atomic<int> client_counter{ 0 };
const int clients_total = 100;

void serve_clients()
{
    while (true)
    {
        int client_id = client_counter.fetch_add(1, std::memory_order_relaxed);
        if (client_id >= clients_total)
        {
            break;
        }
        std::cout << "Обслуживается клиент # " << client_id << std::endl;
    }
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");

    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i)
    {
        threads.emplace_back(serve_clients);
    }
    for (auto& t : threads)
    {
        t.join();
    }
    return 0;
}
