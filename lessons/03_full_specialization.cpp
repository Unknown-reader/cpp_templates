// Урок 3. Полная (явная) специализация.
//
// Иногда общий код для типа не подходит и нужна отдельная реализация.
// Явная специализация задаёт особое поведение для конкретного набора аргументов.

#include <iostream>
#include <string>
#include <string_view>

namespace {

// Общий шаблон: используется, если специализации нет.
template <typename T>
struct TypeName {
    static std::string get() { return "неизвестный тип"; }
};

// Явные специализации для конкретных типов.
template <>
struct TypeName<int> {
    static std::string get() { return "int"; }
};

template <>
struct TypeName<double> {
    static std::string get() { return "double"; }
};

template <>
struct TypeName<std::string> {
    static std::string get() { return "std::string"; }
};

// Специализировать можно и функции, но для них это делают реже.
template <typename E>
constexpr std::string_view to_string(E);  // общее объявление без определения

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

void run() {
    std::cout << "TypeName<int>         = " << TypeName<int>::get() << '\n';
    std::cout << "TypeName<double>      = " << TypeName<double>::get() << '\n';
    std::cout << "TypeName<std::string> = " << TypeName<std::string>::get() << '\n';
    std::cout << "TypeName<char>        = " << TypeName<char>::get() << '\n';

    std::cout << "to_string(Status::NotFound) = "
              << to_string(Status::NotFound) << '\n';
}

}  // namespace

int main() { run(); }
