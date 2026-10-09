// Урок 3. Полная специализация в бэкенде.
//
// Разные типы сериализуются в JSON по-разному, а у каждого HTTP-статуса есть
// текстовое имя. Полная специализация даёт каждому типу собственную реализацию,
// не ломая общий интерфейс Serializer<T>.

#include <iostream>
#include <string>
#include <string_view>

#include "backend.hpp"

namespace {

// Общий случай намеренно не определён: каждый поддерживаемый тип должен задать
// свою специализацию.
template <typename T>
struct JsonSerializer;

template <>
struct JsonSerializer<int> {
    static std::string serialize(int value) { return std::to_string(value); }
};

template <>
struct JsonSerializer<bool> {
    static std::string serialize(bool value) { return value ? "true" : "false"; }
};

template <>
struct JsonSerializer<std::string> {
    static std::string serialize(const std::string& value) {
        return "\"" + value + "\"";
    }
};

// Специализация для доменной сущности — собирает объект из полей.
template <>
struct JsonSerializer<backend::User> {
    static std::string serialize(const backend::User& user) {
        return "{\"id\":" + JsonSerializer<int>::serialize(user.id) +
               ",\"name\":" + JsonSerializer<std::string>::serialize(user.name) +
               ",\"email\":" + JsonSerializer<std::string>::serialize(user.email) +
               "}";
    }
};

// Специализация функции на нетиповом параметре: имя HTTP-статуса.
template <backend::HttpStatus Status>
constexpr std::string_view status_text();

template <>
constexpr std::string_view status_text<backend::HttpStatus::Ok>() { return "OK"; }
template <>
constexpr std::string_view status_text<backend::HttpStatus::Created>() { return "Created"; }
template <>
constexpr std::string_view status_text<backend::HttpStatus::NotFound>() { return "Not Found"; }
template <>
constexpr std::string_view status_text<backend::HttpStatus::InternalError>() {
    return "Internal Server Error";
}

void run() {
    using backend::HttpStatus;

    std::cout << "id:    " << JsonSerializer<int>::serialize(42) << '\n';
    std::cout << "flag:  " << JsonSerializer<bool>::serialize(true) << '\n';
    std::cout << "name:  " << JsonSerializer<std::string>::serialize("Алиса") << '\n';
    std::cout << "user:  "
              << JsonSerializer<backend::User>::serialize(
                     backend::User{7, "Алиса", "alice@example.com"})
              << '\n';

    std::cout << "404:   " << status_text<HttpStatus::NotFound>() << '\n';
    std::cout << "500:   " << status_text<HttpStatus::InternalError>() << '\n';

    static_assert(status_text<HttpStatus::Ok>() == "OK");
    static_assert(status_text<HttpStatus::Created>() == "Created");
}

}  // namespace

int main() { run(); }
