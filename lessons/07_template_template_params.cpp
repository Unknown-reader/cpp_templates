// Урок 7. Параметры-шаблоны в бэкенде.
//
// Хранилище репозитория можно менять, не переписывая код: vector сохраняет
// порядок вставки, deque удобен как очередь, а завтра добавится любая другая
// коллекция. Параметром шаблона выступает сам контейнер-шаблон.

#include <cstddef>
#include <deque>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "backend.hpp"

namespace {

// Storage — контейнер-шаблон (vector, deque, ...), Entity — тип сущности.
template <template <typename...> class Storage, typename Entity>
class InMemoryRepository {
public:
    void add(const Entity& entity) { items_.push_back(entity); }

    template <typename Predicate>
    std::optional<Entity> find_if(Predicate predicate) const {
        for (const auto& item : items_) {
            if (predicate(item)) return item;
        }
        return std::nullopt;
    }

    std::size_t size() const { return items_.size(); }

private:
    Storage<Entity> items_;
};

void run() {
    using backend::User;

    InMemoryRepository<std::vector, User> users;
    users.add(User{1, "Алиса", "alice@example.com"});
    users.add(User{2, "Боб", "bob@example.com"});

    InMemoryRepository<std::deque, User> queue;
    queue.add(User{3, "Кэрол", "carol@example.com"});

    if (auto user = users.find_if([](const User& u) { return u.id == 2; })) {
        std::cout << "найден: " << user->name << '\n';
    }

    std::cout << "vector: " << users.size()
              << ", deque: " << queue.size() << '\n';

    std::cout << std::boolalpha
              << "id=99 найден: "
              << users.find_if([](const User& u) { return u.id == 99; }).has_value()
              << '\n';
}

}  // namespace

int main() { run(); }
