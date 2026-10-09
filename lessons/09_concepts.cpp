// Урок 9. Концепты (C++20).
//
// Концепт — именованное требование к типу. Он даёт понятные ошибки компиляции
// и позволяет ограничивать параметры шаблона прямо в сигнатуре.

#include <concepts>
#include <iostream>
#include <string>
#include <type_traits>

namespace {

template <typename T>
concept Number = std::integral<T> || std::floating_point<T>;

template <typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
};

template <typename T>
concept Printable = requires(std::ostream& os, const T& value) {
    { os << value } -> std::same_as<std::ostream&>;
};

struct NoAdd {};

template <Number T>
T clamp(T value, T lo, T hi) {
    if (value < lo) return lo;
    if (value > hi) return hi;
    return value;
}

// Сокращённый синтаксис: Number auto вместо template <Number T>.
Number auto half(Number auto value) { return value / 2; }

template <Addable T>
T twice(T value) { return value + value; }

template <Printable T>
void show(const T& value) { std::cout << "show: " << value << '\n'; }

void run() {
    static_assert(Number<int>);
    static_assert(Number<double>);
    static_assert(!Number<std::string>);
    static_assert(Addable<int>);
    static_assert(!Addable<NoAdd>);

    std::cout << "clamp(15, 0, 10)     = " << clamp(15, 0, 10) << '\n';
    std::cout << "clamp(2.5, 0.0, 2.0) = " << clamp(2.5, 0.0, 2.0) << '\n';
    std::cout << "twice(21)            = " << twice(21) << '\n';
    std::cout << "half(10)             = " << half(10) << '\n';

    show(std::string{"концепты"});
    show(42);
}

}  // namespace

int main() { run(); }
