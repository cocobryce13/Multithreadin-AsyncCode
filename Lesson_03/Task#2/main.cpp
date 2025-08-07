#include <iostream>
#include <vector>
#include <future>

template <typename Iterator, typename Func>
void parallelForEach(Iterator begin, Iterator end, Func func)
{
    const std::size_t size = std::distance(begin, end);
    if (size <= 2)
    {
        for (Iterator it = begin; it != end; ++it)
        {
            func(*it);
        }
    }
    else
    {
        Iterator mid = begin + size / 2;

        auto future = std::async(std::launch::async,
            parallelForEach<Iterator, Func>,
            begin, mid, func);

        parallelForEach(mid, end, func);
        future.get();
    }
}

int main()
{
    std::vector<int> data = { 1, 2, 3, 4, 5 };

    parallelForEach(data.begin(), data.end(), [](int& x)
        {
            x *= 2;
        });

    for (int x : data)
    {
        std::cout << x << " ";
    }
    std::cout << std::endl;

}
