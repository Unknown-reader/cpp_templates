// Урок 2. Шаблоны классов.
//
// Класс-шаблон параметризуется типом. Методы можно определять внутри класса,
// а внутри шаблонного класса можно объявлять свои шаблонные методы.

#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

template <typename T>
class Box {
public:
    explicit Box(T value) : value_(std::move(value)) {}

    const T& get() const { return value_; }
    void set(T value) { value_ = std::move(value); }

    // Шаблонный метод: принимает произвольный функтор и меняет тип содержимого.
    template <typename F>
    auto map(F f) const {
        return Box<decltype(f(value_))>(f(value_));
    }

    // Сравнение с Box любого другого типа.
    template <typename U>
    bool operator==(const Box<U>& other) const {
        return value_ == other.get();
    }

private:
    T value_;
};

// Классический пример шаблона класса — контейнер с хранением внутри.
template <typename T>
class Stack {
public:
    void push(T value) { data_.push_back(std::move(value)); }

    T pop() {
        if (data_.empty()) throw std::out_of_range("Stack<>::pop: пусто");
        T value = std::move(data_.back());
        data_.pop_back();
        return value;
    }

    bool empty() const { return data_.empty(); }

private:
    std::vector<T> data_;
};

void run() {
    Box<int> a{42};
    std::cout << "Box<int>: " << a.get() << '\n';

    // map меняет параметр шаблона: Box<int> -> Box<std::string>.
    auto b = a.map([](int v) { return std::to_string(v) + "!"; });
    std::cout << "после map: " << b.get() << '\n';

    Box<int> same{42};
    std::cout << std::boolalpha << "a == same: " << (a == same) << '\n';

    Stack<std::string> stack;
    stack.push("первый");
    stack.push("второй");
    while (!stack.empty()) std::cout << "pop: " << stack.pop() << '\n';
}

}  // namespace

int main() { run(); }
