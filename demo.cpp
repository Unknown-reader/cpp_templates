#include <iostream>

template <typename T>
T max(T a, T b) { return a > b ? a : b; }

int main()
{
    std::cout << max(1, 23) << "\n";
    std::cout << max(3.14, 2.27) << "\n";
    std::cout << max('a', 'z') << "\n";
}