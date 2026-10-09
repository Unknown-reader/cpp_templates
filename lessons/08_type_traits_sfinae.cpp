// Урок 8. Type traits, SFINAE и if constexpr.
//
// Type traits спрашивают у компилятора свойства типов. SFINAE позволяет
// включать/выключать перегрузки по этим свойствам, а if constexpr — выбирать
// ветку кода уже внутри одной функции.

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

// Перегрузка существует только для целочисленных типов.
template <typename T>
std::enable_if_t<std::is_integral_v<T>, std::string> describe(T) {
    return "целое число";
}

// А эта — только для чисел с плавающей точкой.
template <typename T>
std::enable_if_t<std::is_floating_point_v<T>, std::string> describe(T) {
    return "число с плавающей точкой";
}

// if constexpr отбрасывает невыбранные ветки до их инстанциации.
template <typename T>
std::string kind(const T& value) {
    if constexpr (std::is_pointer_v<T>) {
        return value ? "указатель на данные" : "нулевой указатель";
    } else if constexpr (std::is_same_v<T, std::string>) {
        return "строка длины " + std::to_string(value.size());
    } else {
        return "значение";
    }
}

// Detection idiom: есть ли у типа метод size()?
template <typename, typename = void>
struct has_size : std::false_type {};

template <typename T>
struct has_size<T, std::void_t<decltype(std::declval<T>().size())>>
    : std::true_type {};

void run() {
    std::cout << describe(42) << '\n';
    std::cout << describe(3.14) << '\n';

    int x = 0;
    std::cout << kind(x) << '\n';
    std::cout << kind(&x) << '\n';
    std::cout << kind(static_cast<int*>(nullptr)) << '\n';
    std::cout << kind(std::string{"hello"}) << '\n';

    std::cout << std::boolalpha;
    std::cout << "has_size<std::vector<int>> = "
              << has_size<std::vector<int>>::value << '\n';
    std::cout << "has_size<int>              = "
              << has_size<int>::value << '\n';

    static_assert(has_size<std::vector<int>>::value);
    static_assert(!has_size<int>::value);
}

}  // namespace

int main() { run(); }
