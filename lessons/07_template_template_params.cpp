// Урок 7. Параметры-шаблоны (template template parameters).
//
// Иногда параметром шаблона должен быть сам контейнер-шаблон, а не готовый тип.
// Тогда одну функцию можно применять к vector, deque, list и т.д.

#include <cstddef>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

namespace {

// Container — это сам шаблон (например, std::vector), а T — тип элемента.
template <template <typename, typename> class Container, typename T>
Container<T, std::allocator<T>> make_filled(const T& value, std::size_t n) {
    Container<T, std::allocator<T>> result;
    for (std::size_t i = 0; i < n; ++i) result.push_back(value);
    return result;
}

// Принимаем любой контейнер: variadic-версия template template parameter.
template <template <typename...> class Container, typename T>
void dump(const Container<T>& c) {
    for (const auto& v : c) std::cout << v << ' ';
    std::cout << '\n';
}

void run() {
    auto vec = make_filled<std::vector>(42, 4);
    auto deq = make_filled<std::deque>(7, 3);

    std::cout << "vector: ";
    dump(vec);

    std::cout << "deque:  ";
    dump(deq);

    auto names = make_filled<std::vector>(std::string{"cpp"}, 2);
    std::cout << "vector<string>: ";
    dump(names);
}

}  // namespace

int main() { run(); }
