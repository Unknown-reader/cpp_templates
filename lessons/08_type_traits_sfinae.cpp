// Урок 8. Type traits и SFINAE в бэкенде.
//
// Не всякую сущность можно отдать в JSON. Признак has_id отсекает неподходящие
// типы ещё при компиляции, а if constexpr выбирает формат для разных категорий
// значений внутри одной функции.

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

#include "backend.hpp"

namespace {

// Detection idiom: есть ли у типа поле id?
template <typename, typename = void>
struct has_id : std::false_type {};

template <typename T>
struct has_id<T, std::void_t<decltype(std::declval<T>().id)>> : std::true_type {};

// Перегрузка существует только для сущностей с полем id: иначе SFINAE уберёт её
// из набора кандидатов, и компилятор сообщит об отсутствии подходящей функции.
template <typename T>
std::enable_if_t<has_id<T>::value, std::string> to_json(const T& entity) {
    return "{\"id\":" + std::to_string(entity.id) + "}";
}

// Формат скалярных значений выбирается ветками if constexpr.
template <typename T>
std::string scalar_to_json(const T& value) {
    if constexpr (std::is_same_v<T, std::string>) {
        return "\"" + value + "\"";
    } else if constexpr (std::is_same_v<T, bool>) {
        return value ? "true" : "false";
    } else if constexpr (std::is_arithmetic_v<T>) {
        return std::to_string(value);
    } else {
        return "null";
    }
}

void run() {
    using backend::User;

    std::cout << "user:  " << to_json(User{1, "Алиса", "alice@example.com"}) << '\n';

    std::cout << "int:   " << scalar_to_json(42) << '\n';
    std::cout << "str:   " << scalar_to_json(std::string{"ok"}) << '\n';
    std::cout << "bool:  " << scalar_to_json(true) << '\n';

    std::cout << std::boolalpha
              << "has_id<User>:  " << has_id<User>::value << '\n'
              << "has_id<int>:   " << has_id<int>::value << '\n';

    static_assert(has_id<User>::value);
    static_assert(!has_id<std::string>::value);
}

}  // namespace

int main() { run(); }
