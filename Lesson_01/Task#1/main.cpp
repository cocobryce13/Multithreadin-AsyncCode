#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

std::atomic<int> clientCount(0);

void clientThread(int maxClients)
{
    for (int i = 0; i < maxClients; ++i)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ++clientCount;
        std::cout << "[Клиент] Пришел клиент. Всего в очереди: " << clientCount << std::endl;
    }
}

void operatorThread()
{
    while (true)
    {
        if (clientCount > 0)
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            --clientCount;
            std::cout << "[Оператор] Обслужил клиента. Осталось в очереди: " << clientCount << std::endl;
        }
        else if (clientCount == 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    int maxClients = 5;
    std::thread t1(clientThread, maxClients);
    std::thread t2(operatorThread);

    t1.join();
    while (clientCount > 0)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "Все клиенты обслужены. Завершение программы." << std::endl;
    std::exit(0);
}
