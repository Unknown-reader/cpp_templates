// Урок 6. Вариадические шаблоны и fold-выражения.
//
// Пакет параметров (typename... Args) принимает любое число аргументов.
// Раскрывать пакет можно рекурсией, а с C++17 — коротко через fold-выражения.

#include <cstddef>
#include <iostream>
#include <string>
#include <type_traits>

namespace {

// База рекурсии: аргументов больше не осталось.
void print() { std::cout << '\n'; }

// Рекурсивно печатаем первый аргумент, остальные передаём дальше.
template <typename First, typename... Rest>
void print(const First& first, const Rest&... rest) {
    std::cout << first;
    if constexpr (sizeof...(rest) > 0) std::cout << ", ";
    print(rest...);
}

// Fold-выражение: (args + ... + 0) разворачивается в a1 + (a2 + (... + 0)).
template <typename... Args>
auto sum(Args... args) {
    return (args + ... + 0);
}

// Проверка, что все типы совпадают с первым.
template <typename T, typename... Rest>
bool all_same(const T&, const Rest&...) {
    return (std::is_same_v<T, Rest> && ...);
}

// sizeof... возвращает число элементов в пакете.
template <typename... Args>
constexpr std::size_t count() {
    return sizeof...(Args);
}

void run() {
    print(1, 2.5, "три", std::string{"четыре"}, '5');

    std::cout << "sum(1,2,3,4,5) = " << sum(1, 2, 3, 4, 5) << '\n';
    std::cout << "sum()          = " << sum() << '\n';

    std::cout << std::boolalpha;
    std::cout << "all_same(1,2,3) = " << all_same(1, 2, 3) << '\n';
    std::cout << "all_same(1,2.0) = " << all_same(1, 2.0) << '\n';

    std::cout << "count<int,double,char>() = "
              << count<int, double, char>() << '\n';
}

}  // namespace

int main() { run(); }
