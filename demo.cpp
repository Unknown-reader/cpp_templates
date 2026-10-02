#include <iostream>
#include <string_view>

template <typename T>
T max(T a, T b) { return a > b ? a : b; }

template <typename E>
constexpr std::string_view to_string(E e);   // объявление

enum class Status { Ok, NotFound, Error };

template <>
constexpr std::string_view to_string(Status s) {
    switch (s) {
        case Status::Ok:       return "Ok";
        case Status::NotFound: return "NotFound";
        case Status::Error:    return "Error";
    }
    return "?";
}

int main()
{
    std::cout << max(1, 23) << "\n";
    std::cout << max(3.14, 2.27) << "\n";
    std::cout << max('a', 'z') << "\n";

    std::cout << to_string(Status::NotFound) << "\n"; // NotFound
}