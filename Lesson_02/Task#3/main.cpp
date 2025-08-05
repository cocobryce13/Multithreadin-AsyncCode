#include <iostream>
#include <mutex>
#include <thread>
#include <utility>

class Data
{
public:
    int value;
    std::mutex m;

    Data(int val) : value(val) {}
};

void swap_with_lock(Data& a, Data& b)
{
    std::lock(a.m, b.m);
    std::lock_guard<std::mutex> lg1(a.m, std::adopt_lock);
    std::lock_guard<std::mutex> lg2(b.m, std::adopt_lock);
    std::swap(a.value, b.value);
}

void swap_with_scoped_lock(Data& a, Data& b)
{
    std::scoped_lock lock(a.m, b.m);
    std::swap(a.value, b.value);
}

void swap_with_unique_lock(Data& a, Data& b)
{
    std::unique_lock<std::mutex> lk1(a.m, std::defer_lock);
    std::unique_lock<std::mutex> lk2(b.m, std::defer_lock);
    std::lock(lk1, lk2);
    std::swap(a.value, b.value);
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");

    Data d1(10);
    Data d2(20);

    swap_with_lock(d1, d2);
    std::cout << "После swap_with_lock: d1=" << d1.value << ", d2=" << d2.value << std::endl;

    swap_with_scoped_lock(d1, d2);
    std::cout << "После swap_with_scoped_lock: d1=" << d1.value << ", d2=" << d2.value << std::endl;

    swap_with_unique_lock(d1, d2);
    std::cout << "После swap_with_unique_lock: d1=" << d1.value << ", d2=" << d2.value << std::endl;

}
