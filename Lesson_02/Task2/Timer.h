#pragma once
#include <chrono>
#include <iostream>
#include <iomanip>
#include <Windows.h>

class consol_parameter
{
public:
    static void SetColor(int text, int background)
    {
        SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
    }

    static void SetPosition(int x, int y)
    {
        COORD point;
        point.X = x;
        point.Y = y;
        SetConsoleCursorPosition(hStdOut, point);
    }

private:
    static HANDLE hStdOut;
};

HANDLE consol_parameter::hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

class Timer
{
private:
    using clock_t = std::chrono::high_resolution_clock;
    using second_t = std::chrono::duration<double, std::ratio<1>>;

    std::chrono::time_point<clock_t> m_beg;

public:
    Timer() : m_beg(clock_t::now()) {}

    void reset()
    {
        m_beg = clock_t::now();
    }

    double elapsed_ms() const
    {
        return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count() * 1000;
    }
};
