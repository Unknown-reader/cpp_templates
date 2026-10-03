#include <iostream>
#include <string_view>

template <typename T>
constexpr const T& max(const T& a, const T& b) { return a > b ? a : b; }

static_assert(max(1, 23) == 23);

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

struct FastTimeout { static int ms() { return 100;  } };
struct SlowTimeout { static int ms() { return 5000; } };

template <typename Policy>
struct Client {
    void call() { std::cout << "timeout=" << Policy::ms() << "ms\n"; }
};

int main()
{
    std::cout << max(1, 23) << "\n";
    std::cout << max(3.14, 2.27) << "\n";
    std::cout << max('a', 'z') << "\n";

    std::cout << to_string(Status::NotFound) << "\n"; // NotFound

    Client<FastTimeout>{}.call(); // 100ms
    Client<SlowTimeout>{}.call(); // 5000ms
}