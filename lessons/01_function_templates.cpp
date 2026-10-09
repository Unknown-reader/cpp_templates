// Урок 1. Шаблоны функций — самый простой случай.
//
// Шаблон — это «чертёж» функции. Компилятор сам выводит типы из аргументов
// вызова и создаёт (инстанцирует) конкретную функцию под каждый набор типов.

#include <iostream>
#include <string>

namespace {

// T выводится из аргументов. const T& позволяет сравнивать тяжёлые типы
// без копирования, а constexpr делает функцию пригодной для static_assert.
template <typename T>
constexpr const T& max_value(const T& a, const T& b) {
    return (a > b) ? a : b;
}

// Два независимых параметра шаблона; тип результата выводится компилятором.
template <typename T, typename U>
constexpr auto add(T a, U b) {
    return a + b;
}

// Параметры можно указывать вручную, а не выводить.
template <typename To, typename From>
constexpr To narrow_cast(From value) {
    return static_cast<To>(value);
}

void run() {
    static_assert(max_value(1, 2) == 2);
    static_assert(add(2, 3) == 5);
    static_assert(narrow_cast<int>(3.9) == 3);

    std::cout << "max_value(1, 23)          = " << max_value(1, 23) << '\n';
    std::cout << "max_value(3.14, 2.27)     = " << max_value(3.14, 2.27) << '\n';
    std::cout << "max_value('a', 'z')       = " << max_value('a', 'z') << '\n';
    std::cout << "max_value(std::string)    = "
              << max_value(std::string{"apple"}, std::string{"pear"}) << '\n';

    // add<int, double> выводит общий тип через обычные правила арифметики.
    std::cout << "add(1, 2.5)         = " << add(1, 2.5) << '\n';
    std::cout << "narrow_cast<int>(3.9) = " << narrow_cast<int>(3.9) << '\n';
}

}  // namespace

int main() { run(); }
