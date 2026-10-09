// Урок 5. Нетиповые (non-type) параметры шаблона.
//
// Параметром шаблона может быть не только тип, но и значение: целое число,
// указатель, ссылка и т.п. Тогда значение известно уже на этапе компиляции.

#include <cstddef>
#include <iostream>
#include <stdexcept>

namespace {

// N — нетиповой параметр: размер хранится в самом типе массива.
template <typename T, std::size_t N>
class FixedArray {
public:
    T& operator[](std::size_t i) {
        if (i >= N) throw std::out_of_range("FixedArray: выход за границы");
        return data_[i];
    }

    const T& operator[](std::size_t i) const { return data_[i]; }

    static constexpr std::size_t size() { return N; }

    T* begin() { return data_; }
    T* end() { return data_ + N; }

private:
    T data_[N]{};  // размер N встроен в тип, аллокации нет
};

// Нетиповой параметр-значение: результат доступен в static_assert.
template <int N>
constexpr int square() {
    return N * N;
}

void run() {
    FixedArray<int, 5> values;
    for (std::size_t i = 0; i < values.size(); ++i) {
        values[i] = static_cast<int>(i * i);
    }

    std::cout << "FixedArray<int, 5>: ";
    for (int v : values) std::cout << v << ' ';
    std::cout << '\n';

    static_assert(square<5>() == 25);
    static_assert(FixedArray<double, 3>::size() == 3);

    std::cout << "square<5>() = " << square<5>() << '\n';
}

}  // namespace

int main() { run(); }
