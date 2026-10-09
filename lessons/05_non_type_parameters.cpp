// Урок 5. Нетиповые параметры в бэкенде.
//
// Размер окна метрик и буфера чтения известны на этапе компиляции. Это позволяет
// обойтись без динамической памяти в горячем пути обработки запроса, а размер
// становится частью типа.

#include <cstddef>
#include <iostream>

namespace {

// Кольцевой буфер фиксированного размера: окно последних N замеров времени.
// T — тип замера, N — нетиповой параметр.
template <typename T, std::size_t N>
class RingBuffer {
public:
    void push(T value) {
        data_[next_] = value;
        next_ = (next_ + 1) % N;
        if (size_ < N) ++size_;
    }

    double average() const {
        if (size_ == 0) return 0.0;
        double sum = 0;
        for (std::size_t i = 0; i < size_; ++i) sum += data_[i];
        return sum / static_cast<double>(size_);
    }

    std::size_t size() const { return size_; }
    static constexpr std::size_t capacity() { return N; }

private:
    T data_[N]{};
    std::size_t next_ = 0;
    std::size_t size_ = 0;
};

// Буфер чтения: Capacity — параметр-значение, тело лежит на стеке.
template <std::size_t Capacity>
class ReadBuffer {
public:
    void append(const char* data, std::size_t length) {
        for (std::size_t i = 0; i < length && used_ < Capacity; ++i) {
            data_[used_++] = data[i];
        }
    }

    std::size_t size() const { return used_; }
    std::size_t capacity() const { return Capacity; }

private:
    char data_[Capacity]{};
    std::size_t used_ = 0;
};

void run() {
    RingBuffer<double, 4> latency;
    for (double ms : {12.0, 18.0, 25.0, 15.0, 20.0, 30.0}) latency.push(ms);

    std::cout << "замеров в окне: " << latency.size()
              << ", среднее: " << latency.average() << " мс\n";

    ReadBuffer<16> request_line;
    request_line.append("GET /users HTTP/1.1", 19);
    std::cout << "прочитано байт: " << request_line.size()
              << " из " << request_line.capacity() << '\n';

    static_assert(RingBuffer<int, 8>::capacity() == 8);
    static_assert(sizeof(ReadBuffer<64>) >= 64);
}

}  // namespace

int main() { run(); }
