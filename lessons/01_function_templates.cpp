// Урок 1. Шаблоны функций в бэкенде.
//
// В веб-сервисе одни и те же операции применяются к разным типам: ограничить
// пагинацию, посчитать метрику, собрать ответ. Один шаблон функции заменяет
// десяток перегрузок под каждый тип.

#include <cstddef>
#include <iostream>
#include <string>
#include <utility>

#include "backend.hpp"

namespace {

// Ограничение пагинации: работает и для int, и для size_t.
// Тип T выводится из аргументов, поэтому перегрузки не нужны.
template <typename T>
constexpr T clamp(T value, T lo, T hi) {
    if (value < lo) return lo;
    if (value > hi) return hi;
    return value;
}

// Среднее время ответа. Типы счётчика и суммы независимы, а общий тип
// результата компилятор выводит сам (int / int -> double).
template <typename Hits, typename Milliseconds>
auto average_response(Hits hits, Milliseconds total_ms) {
    return total_ms / hits;
}

// Параметры можно указывать вручную: конфиг хранит строки, а нужен конкретный
// тип.
template <typename To, typename From>
constexpr To config_cast(From value) {
    return static_cast<To>(value);
}

// Сборка ответа: тело принимается по универсальной ссылке, поэтому подходит и
// std::string, и строковый литерал.
template <typename Body>
backend::Response json_response(backend::HttpStatus status, Body&& body) {
    return {status, "application/json", std::forward<Body>(body), {}};
}

void run() {
    using backend::HttpStatus;

    const std::size_t raw_limit = 1000;
    const std::size_t limit = clamp<std::size_t>(raw_limit, 1, 100);

    std::cout << "limit после clamp()      = " << limit << '\n';
    std::cout << "среднее время ответа     = "
              << average_response(50, 1234.5) << " мс\n";
    std::cout << "config_cast<int>(3.9)    = " << config_cast<int>(3.9) << '\n';

    const backend::Response res =
        json_response(HttpStatus::Ok, std::string{"{\"ok\":true}"});
    std::cout << "response: " << res.content_type << " " << res.body << '\n';

    static_assert(clamp(150, 1, 100) == 100);
    static_assert(clamp(-5, 1, 100) == 1);
    static_assert(config_cast<int>(3.9) == 3);
}

}  // namespace

int main() { run(); }
