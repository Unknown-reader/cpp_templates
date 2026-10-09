// Урок 6. Вариадические шаблоны в бэкенде.
//
// Логгер принимает любое число полей, а сборщик ответа — любое число заголовков.
// Пакет параметров избавляет от ручных перегрузок под 1, 2, 3, ... аргумента.

#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include "backend.hpp"

namespace {

// Склеиваем любое число полей в строку через разделитель (fold-выражение).
template <typename... Fields>
std::string join(const std::string& separator, Fields&&... fields) {
    std::ostringstream out;
    bool first = true;
    auto append = [&](const auto& field) {
        if (!first) out << separator;
        first = false;
        out << field;
    };
    (append(std::forward<Fields>(fields)), ...);
    return out.str();
}

// Структурный лог: уровень + произвольные поля.
template <typename... Fields>
void log_line(const std::string& level, Fields&&... fields) {
    std::cout << "[" << level << "] "
              << join(" ", std::forward<Fields>(fields)...) << '\n';
}

// Добавить любое число заголовков в ответ: каждый аргумент — пара key/value.
template <typename... Headers>
void set_headers(backend::Response& response, Headers&&... headers) {
    (response.headers.emplace(std::forward<Headers>(headers).first,
                              std::forward<Headers>(headers).second),
     ...);
}

// Собрать ответ из статуса и произвольного набора заголовков.
template <typename... Headers>
backend::Response make_response(backend::HttpStatus status, Headers&&... headers) {
    backend::Response response;
    response.status = status;
    set_headers(response, std::forward<Headers>(headers)...);
    return response;
}

void run() {
    using backend::HttpStatus;

    log_line("INFO", "user", 42, "created");
    log_line("WARN", "cache miss", "key=profile:42");
    std::cout << "join: " << join(", ", 1, 2.5, "three") << '\n';

    const backend::Response response =
        make_response(HttpStatus::Created,
                      std::pair{"Content-Type", "application/json"},
                      std::pair{"X-Request-Id", "abc123"});

    std::cout << "response headers:\n";
    for (const auto& [key, value] : response.headers) {
        std::cout << "  " << key << ": " << value << '\n';
    }
}

}  // namespace

int main() { run(); }
