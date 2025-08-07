#include <iostream>
#include <vector>
#include <future>

int findMinIndex(const std::vector<int>& vec, int start)
{
    int minIndex = start;
    for (int i = start + 1; i < vec.size(); ++i)
    {
        if (vec[i] < vec[minIndex])
        {
            minIndex = i;
        }
    }
    return minIndex;
}

void selectionSort(std::vector<int>& vec)
{
    for (int i = 0; i < vec.size(); ++i)
    {
        std::promise<int> prom;
        std::future<int> fut = prom.get_future();

        std::thread t([&vec, &prom, i]()
            {
                int minIndex = findMinIndex(vec, i);
                prom.set_value(minIndex);
            });

        int minIndex = fut.get();
        t.join();

        std::swap(vec[i], vec[minIndex]);
    }
}

int main()
{
    std::vector<int> data = { 5, 3, 8, 1, 4 };

    selectionSort(data);

    for (int num : data)
    {
        std::cout << num << " ";
    }
    std::cout << std::endl;

}
