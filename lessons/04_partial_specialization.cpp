// Урок 4. Частичная специализация в бэкенде.
//
// В каждом API есть списки и необязательные поля. Частичная специализация
// сериализует любой vector<T> и любой optional<T>, не зная заранее конкретный T,
// и сама вызывает правильную специализацию для элемента.

#include <cstddef>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "backend.hpp"

namespace {

// Общий случай: реализацию задают специализации.
template <typename T>
struct JsonSerializer;

// Частичная специализация: любой список -> JSON-массив.
template <typename T>
struct JsonSerializer<std::vector<T>> {
    static std::string serialize(const std::vector<T>& items) {
        std::string out = "[";
        for (std::size_t i = 0; i < items.size(); ++i) {
            if (i != 0) out += ",";
            out += JsonSerializer<T>::serialize(items[i]);
        }
        return out + "]";
    }
};

// Частичная специализация: необязательное поле -> значение или null.
template <typename T>
struct JsonSerializer<std::optional<T>> {
    static std::string serialize(const std::optional<T>& value) {
        return value ? JsonSerializer<T>::serialize(*value) : "null";
    }
};

// Специализации для конкретных типов, замыкающие рекурсию выше.
template <>
struct JsonSerializer<int> {
    static std::string serialize(int value) { return std::to_string(value); }
};

template <>
struct JsonSerializer<std::string> {
    static std::string serialize(const std::string& value) {
        return "\"" + value + "\"";
    }
};

void run() {
    const std::vector<int> ids{1, 2, 3};
    std::cout << "ids:   " << JsonSerializer<std::vector<int>>::serialize(ids) << '\n';

    const std::vector<std::string> tags{"cpp", "backend"};
    std::cout << "tags:  "
              << JsonSerializer<std::vector<std::string>>::serialize(tags) << '\n';

    const std::optional<std::string> nickname{"neo"};
    const std::optional<std::string> empty;
    std::cout << "nick:  "
              << JsonSerializer<std::optional<std::string>>::serialize(nickname)
              << '\n';
    std::cout << "empty: "
              << JsonSerializer<std::optional<std::string>>::serialize(empty)
              << '\n';

    // Комбинация обеих частичных специализаций: массив nullable-полей.
    const std::vector<std::optional<int>> ratings{1, std::nullopt, 3};
    std::cout << "mix:   "
              << JsonSerializer<std::vector<std::optional<int>>>::serialize(ratings)
              << '\n';
}

}  // namespace

int main() { run(); }
