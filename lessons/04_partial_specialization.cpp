// Урок 4. Частичная специализация.
//
// Полная специализация фиксирует все аргументы, а частичная — только их форму
// (например, «указатель» или «массив»), оставляя тип элемента свободным.

#include <cstddef>
#include <iostream>

namespace {

template <typename T>
struct ScalarTraits {
    static constexpr bool is_pointer = false;
    static constexpr bool is_array   = false;
    static constexpr int  dimensions = 0;
};

// Частичная специализация для любого T*.
template <typename T>
struct ScalarTraits<T*> {
    static constexpr bool is_pointer = true;
    static constexpr bool is_array   = false;
    static constexpr int  dimensions = 0;
};

// Частичная специализация для любого массива T[N]; размерность рекурсивно
// «снимает» вложенные массивы: int[3][4] -> 2 измерения.
template <typename T, std::size_t N>
struct ScalarTraits<T[N]> {
    static constexpr bool is_pointer = false;
    static constexpr bool is_array   = true;
    static constexpr int  dimensions = 1 + ScalarTraits<T>::dimensions;
};

void run() {
    using Int = ScalarTraits<int>;
    using Ptr = ScalarTraits<int*>;
    using Arr = ScalarTraits<int[3][4]>;

    std::cout << std::boolalpha;
    std::cout << "int       pointer=" << Int::is_pointer
              << " array=" << Int::is_array
              << " dims=" << Int::dimensions << '\n';
    std::cout << "int*      pointer=" << Ptr::is_pointer
              << " array=" << Ptr::is_array
              << " dims=" << Ptr::dimensions << '\n';
    std::cout << "int[3][4] pointer=" << Arr::is_pointer
              << " array=" << Arr::is_array
              << " dims=" << Arr::dimensions << '\n';

    static_assert(Int::is_pointer == false);
    static_assert(Ptr::is_pointer == true);
    static_assert(Arr::dimensions == 2);
}

}  // namespace

int main() { run(); }
