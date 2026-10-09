// Урок 10. CRTP, policy-based design и вычисления на этапе компиляции.
//
// CRTP (Curiously Recurring Template Pattern) даёт статический полиморфизм без
// виртуальных вызовов, policy-based design подставляет поведение параметром,
// а рекурсия шаблонов считает значения прямо во время компиляции.

#include <iostream>
#include <string>

namespace {

// --- CRTP: базовый класс знает точный тип наследника через параметр шаблона.
template <typename Derived>
class Shape {
public:
    std::string describe() const {
        return "площадь = " + std::to_string(self().area());
    }

private:
    const Derived& self() const { return static_cast<const Derived&>(*this); }
};

class Circle : public Shape<Circle> {
public:
    explicit Circle(double r) : radius_(r) {}
    double area() const { return 3.141592653589793 * radius_ * radius_; }

private:
    double radius_;
};

class Square : public Shape<Square> {
public:
    explicit Square(double s) : side_(s) {}
    double area() const { return side_ * side_; }

private:
    double side_;
};

// --- Policy-based design: поведение задаётся шаблонным параметром.
struct FastTimeout { static int ms() { return 100; } };
struct SlowTimeout { static int ms() { return 5000; } };

template <typename TimeoutPolicy>
class Client {
public:
    void call() const {
        std::cout << "timeout = " << TimeoutPolicy::ms() << " ms\n";
    }
};

// --- Вычисления на этапе компиляции: факториал через рекурсию шаблонов.
template <unsigned N>
struct Factorial {
    static constexpr unsigned long long value = N * Factorial<N - 1>::value;
};

template <>
struct Factorial<0> {
    static constexpr unsigned long long value = 1;
};

void run() {
    Circle c{2.0};
    Square s{3.0};
    std::cout << "Circle: " << c.describe() << '\n';
    std::cout << "Square: " << s.describe() << '\n';

    Client<FastTimeout>{}.call();
    Client<SlowTimeout>{}.call();

    std::cout << "Factorial<10> = " << Factorial<10>::value << '\n';

    static_assert(Factorial<0>::value == 1);
    static_assert(Factorial<5>::value == 120);
    static_assert(Factorial<10>::value == 3628800);
}

}  // namespace

int main() { run(); }
